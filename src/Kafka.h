/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2020, The Souffle Developers. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file Kafka.h
 *
 ***********************************************************************/

#pragma once

#include <librdkafka/rdkafkacpp.h>

#include <atomic>
#include <cassert>
#include <csignal>

namespace souffle {
namespace kafka {
namespace detail {
static std::atomic<bool> exceptionThrown_;
class KafkaHelper {
public:
    static void unthrowException() {
        exceptionThrown_ = false;
    }
    static void throwException() {
        exceptionThrown_ = true;
    }
    static void throwException(const std::string& message) {
        std::cerr << message << std::endl;
        exceptionThrown_ = true;
    }
    static void throwFatalException() {
        throwException();
        assert(false);
    }
    static void throwFatalException(const std::string& message) {
        throwException(message);
        assert(false);
    }
    static bool exceptionThrown() {
        return exceptionThrown_;
    }
    static void sigintAndSigterm(int sig) {
        (void)sig;
        throwException();
    }
    static void setSigintAndSigterm() {
        signal(SIGINT, sigintAndSigterm);
        signal(SIGTERM, sigintAndSigterm);
    }
    static RdKafka::Conf* createConf(const RdKafka::Conf::ConfType confType) {
        RdKafka::Conf* conf = RdKafka::Conf::create(confType);
        if (!conf) {
            throwFatalException();
        }
        return conf;
    }
    static RdKafka::Conf* createGlobalConf() {
        return createConf(RdKafka::Conf::CONF_GLOBAL);
    }
    static RdKafka::Conf* createTopicConf() {
        return createConf(RdKafka::Conf::CONF_TOPIC);
    }
    template <typename T>
    static void setConf(RdKafka::Conf* conf, const std::string& key, T value) {
        assert(conf);
        std::string errstr;
        if (conf->set(key, value, errstr) != RdKafka::Conf::CONF_OK) {
            throwFatalException(errstr);
        }
    }
    static void eventCb(RdKafka::Event& event) {
        switch (event.type()) {
            case RdKafka::Event::EVENT_ERROR: {
                if (event.fatal()) {
                    const auto errstr = RdKafka::err2str(event.err());
                    throwFatalException(errstr);
                }
                break;
            }
            case RdKafka::Event::EVENT_STATS: {
                break;
            }
            case RdKafka::Event::EVENT_LOG: {
                break;
            }
            default: { break; }
        }
    }
    class EventCb : public RdKafka::EventCb {
    public:
        void event_cb(RdKafka::Event& event) {
            eventCb(event);
        }
    };
    static void setEventCb(RdKafka::Conf* conf, RdKafka::EventCb* eventCb) {
        setConf(conf, "event_cb", eventCb);
    }
    static void drCb(RdKafka::Message& message) {
        switch (message.status()) {
            case RdKafka::Message::MSG_STATUS_NOT_PERSISTED: {
                break;
            }
            case RdKafka::Message::MSG_STATUS_POSSIBLY_PERSISTED: {
                break;
            }
            case RdKafka::Message::MSG_STATUS_PERSISTED: {
                break;
            }
            default: { break; }
        }
    }
    class DeliveryReportCb : public RdKafka::DeliveryReportCb {
    public:
        void dr_cb(RdKafka::Message& message) {
            drCb(message);
        }
    };
    static void setDeliveryReportCb(RdKafka::Conf* conf, RdKafka::DeliveryReportCb* drCb) {
        setConf(conf, "dr_cb", drCb);
    }
    static void setDefaultTopicConf(RdKafka::Conf* conf, RdKafka::Conf* defaultTopicConf) {
        setConf(conf, "default_topic_conf", defaultTopicConf);
    }
    template <typename Handle>
    static Handle* createHandle(RdKafka::Conf* conf) {
        assert(conf);
        std::string errstr;
        Handle* handle = Handle::create(conf, errstr);
        if (!handle) {
            throwFatalException(errstr);
        }
        return handle;
    }
    static RdKafka::Producer* createProducer(RdKafka::Conf* conf) {
        return createHandle<RdKafka::Producer>(conf);
    }
    static RdKafka::Consumer* createConsumer(RdKafka::Conf* conf) {
        return createHandle<RdKafka::Consumer>(conf);
    }
    static RdKafka::Topic* createTopic(
            RdKafka::Conf* topicConf, RdKafka::Handle* base, const std::string& topicName) {
        assert(topicConf);
        assert(base);
        std::string errstr;
        RdKafka::Topic* topic = RdKafka::Topic::create(base, topicName, topicConf, errstr);
        if (!topic) {
            throwFatalException(errstr);
        }
        return topic;
    }
    static void pollHandle(RdKafka::Handle* handle, int timeout_ms) {
        assert(handle);
        handle->poll(timeout_ms);
    }
    template <typename T>
    static void produceProducer(RdKafka::Producer* producer, RdKafka::Topic* topic, std::vector<T>& payload) {
        assert(producer);
        assert(topic);
        const std::int32_t partition = RdKafka::Topic::PARTITION_UA;
        const int msgflags = RdKafka::Producer::MSG_COPY;
        const std::string* key = nullptr;
        void* msg_opaque = nullptr;
        RdKafka::ErrorCode errorCode = producer->produce(
                topic, partition, msgflags, payload.data(), sizeof(T) * payload.size(), key, msg_opaque);
        if (errorCode != RdKafka::ERR_NO_ERROR) {
            throwFatalException(RdKafka::err2str(errorCode));
        }
        pollHandle(producer, 0);
    }
    static void pollProducerUntilEmpty(RdKafka::Producer* producer) {
        assert(producer);
        // -1 will perform an indefinate blocking wait
        const int timeout_ms = -1;
        while (!exceptionThrown() && producer->outq_len() > 0) {
            pollHandle(producer, timeout_ms);
        }
    }
    static void startConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic) {
        assert(consumer);
        assert(topic);
        const int32_t partition = 0;
        const int64_t offset = RdKafka::Topic::OFFSET_BEGINNING;
        RdKafka::ErrorCode errorCode = consumer->start(topic, partition, offset);
        if (errorCode != RdKafka::ERR_NO_ERROR) {
            throwFatalException(RdKafka::err2str(errorCode));
        }
        pollHandle(consumer, 0);
    }
    static void stopConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic) {
        assert(consumer);
        assert(topic);
        const int32_t partition = RdKafka::Topic::PARTITION_UA;
        consumer->stop(topic, partition);
        pollHandle(consumer, 1000);
    }
    template <typename T>
    static void consumeConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic, std::vector<T>& payload) {
        assert(consumer);
        assert(topic);
        const int32_t partition = 0;
        // @@@TODO (lh): get the timeout right here, -1 just means it will wait forever
        const int timeout_ms = -1;
        while (true) {
            RdKafka::Message* message = consumer->consume(topic, partition, timeout_ms);
            pollHandle(consumer, 0);
            switch (message->err()) {
                case RdKafka::ERR__TIMED_OUT: {
                    delete message;
                    break;
                }
                case RdKafka::ERR__PARTITION_EOF: {
                    throwFatalException(message->errstr());
                    break;
                }
                case RdKafka::ERR__UNKNOWN_TOPIC: {
                    throwFatalException(message->errstr());
                    break;
                }
                case RdKafka::ERR__UNKNOWN_PARTITION: {
                    throwFatalException(message->errstr());
                    break;
                }
                case RdKafka::ERR_NO_ERROR: {
                    const T* rawPayload = static_cast<T*>(message->payload());
                    payload = std::vector<T>(rawPayload, rawPayload + (message->len() / sizeof(T)));
                    pollHandle(consumer, 0);
                    delete message;
                    return;
                }
                default: {
                    throwFatalException(message->errstr());
                    break;
                }
            }
        }
    }
    static void waitDestroyed() {
        // -1 will perform an indefinate blocking wait
        RdKafka::wait_destroyed(-1);
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

#define KAFKA_DEBUG

namespace souffle {
namespace kafka {
namespace detail {
class KafkaClient {
private:
    RdKafka::Conf* globalConf_;
    RdKafka::Conf* topicConf_;
    KafkaHelper::EventCb eventCb_;
    KafkaHelper::DeliveryReportCb deliveryReportCb_;
    RdKafka::Producer* producer_;
    RdKafka::Consumer* consumer_;
    std::unordered_map<std::string, RdKafka::Topic*> producerTopics_;
    std::unordered_map<std::string, RdKafka::Topic*> consumerTopics_;
#ifdef KAFKA_DEBUG
    RdKafka::Topic* debugTopic_;
#endif
private:
    explicit KafkaClient() {}

public:
    ~KafkaClient() {}

public:
    static KafkaClient& getInstance() {
        // get instance for singleton design pattern
        static KafkaClient instance = KafkaClient();
        return instance;
    }
    // deleted constructor for singleton design pattern
    KafkaClient(KafkaClient const&) = delete;
    // deleted constructor for singleton design pattern
    void operator=(KafkaClient const&) = delete;

public:
    void beginClient(const std::unordered_map<std::string, std::string>& globalConf) {
        KafkaHelper::unthrowException();
        KafkaHelper::setSigintAndSigterm();
        globalConf_ = KafkaHelper::createGlobalConf();
        topicConf_ = KafkaHelper::createTopicConf();
        for (auto it = globalConf.begin(); it != globalConf.end(); ++it) {
            KafkaHelper::setConf(globalConf_, it->first, it->second);
        }
        KafkaHelper::setEventCb(globalConf_, &eventCb_);
        // @@@TODO (lh): remove these if possible and get polling right, see https://docs.confluent.io/2.0.0/clients/librdkafka/classRdKafka_1_1Handle.html
        KafkaHelper::setDeliveryReportCb(globalConf_, &deliveryReportCb_);
        KafkaHelper::setDefaultTopicConf(globalConf_, topicConf_);
        producer_ = KafkaHelper::createProducer(globalConf_);
        consumer_ = KafkaHelper::createConsumer(globalConf_);
        producerTopics_ = std::unordered_map<std::string, RdKafka::Topic*>();
        consumerTopics_ = std::unordered_map<std::string, RdKafka::Topic*>();
#ifdef KAFKA_DEBUG
        debugTopic_ = KafkaHelper::createTopic(topicConf_, producer_, "_DEBUG_");
#endif
    }
    void endClient() {
        KafkaHelper::pollProducerUntilEmpty(producer_);
#ifdef KAFKA_DEBUG
        delete debugTopic_;
#endif
        delete producer_;
        delete consumer_;
        KafkaHelper::waitDestroyed();
        delete globalConf_;
        delete topicConf_;
    }
#ifdef KAFKA_DEBUG
    template <typename T>
    void debug(std::stringstream& stringstream, const std::vector<T>& payload) {
        stringstream << payload.size();
        for (const auto& element : payload) {
            stringstream << " " << element;
        }
        const std::string str = stringstream.str();
        const char* cstr = str.c_str();
        std::vector<char> debugPayload(cstr, cstr + str.size());
        KafkaHelper::produceProducer(producer_, debugTopic_, debugPayload);
    }
#endif
    void beginProduction(const std::string& topicName) {
        {
            auto iterator = producerTopics_.find(topicName);
            if (iterator != producerTopics_.end() && iterator->second != nullptr) {
                return;
            }
        }
        producerTopics_[topicName] = KafkaHelper::createTopic(topicConf_, producer_, topicName);
    }
    void endProduction(const std::string& topicName) {
        delete producerTopics_[topicName];
        producerTopics_[topicName] = nullptr;
    }
    void beginConsumption(const std::string& topicName) {
        {
            auto iterator = consumerTopics_.find(topicName);
            if (iterator != consumerTopics_.end() && iterator->second != nullptr) {
                return;
            }
        }
        consumerTopics_[topicName] = KafkaHelper::createTopic(topicConf_, consumer_, topicName);
        KafkaHelper::startConsumer(consumer_, consumerTopics_.at(topicName));
    }
    void endConsumption(const std::string& topicName) {
        KafkaHelper::stopConsumer(consumer_, consumerTopics_.at(topicName));
        delete consumerTopics_[topicName];
        consumerTopics_[topicName] = nullptr;
    }
    template <typename T>
    void produce(const std::string& topicName, std::vector<T>& payload) {
        RdKafka::Topic* topic = producerTopics_.at(topicName);
#ifdef KAFKA_DEBUG
        {
            std::stringstream stringstream;
            stringstream << producer_->name() << " " << topic->name() << " ";
            debug(stringstream, payload);
        }
#endif
        KafkaHelper::produceProducer(producer_, topic, payload);
    }
    template <typename T>
    void consume(const std::string& topicName, std::vector<T>& payload) {
        RdKafka::Topic* topic = consumerTopics_.at(topicName);
        KafkaHelper::consumeConsumer(consumer_, topic, payload);
#ifdef KAFKA_DEBUG
        {
            std::stringstream stringstream;
            stringstream << consumer_->name() << " " << topic->name() << " ";
            debug(stringstream, payload);
        }
#endif
    }
    void pollProducer(const int timeoutMs = 1000) {
        KafkaHelper::pollHandle(producer_, timeoutMs);
    }
    void pollConsumer(const int timeoutMs = 1000) {
        KafkaHelper::pollHandle(consumer_, timeoutMs);
    }
    void pollProducerUntilEmpty() {
        KafkaHelper::pollProducerUntilEmpty(producer_);
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

#include "SouffleInterface.h"
#include <fstream>

namespace souffle {
namespace kafka {
class Kafka {
    // @TODO (lh): document use of the kafka communication engine
public:
    explicit Kafka() {}
    void run(const std::unordered_map<std::string, std::string>& globalConf,
            const std::string& inputDirectory, const std::string& outputDirectory,
            souffle::SouffleProgram* souffleProgram, std::size_t stratumIndex,
            const std::string& metadataString) const {
        if (stratumIndex == static_cast<std::size_t>(-2)) {
            std::cout << metadataString << std::endl;
        } else {
            /**
             * Note that the `globalConf` parameter sets the global configuration properties for Kafka.
             * It is passed to the program with the -X option, using the format key=value.
             * For example, running with -Xmetadata.broker.list=localhost:9092 sets the Kafka broker.
             * See https://github.com/edenhill/librdkafka/blob/master/CONFIGURATION.md
             * for a full list of availble options.
             */
            auto& kafkaClient = detail::KafkaClient::getInstance();
            kafkaClient.beginClient(globalConf);
            souffleProgram->runAll(inputDirectory, outputDirectory, stratumIndex);
            kafkaClient.endClient();
        }
    }
};
}  // namespace kafka
}  // namespace souffle
