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

#include "SouffleInterface.h"

#include <librdkafka/rdkafkacpp.h>

#include <atomic>
#include <cassert>
#include <chrono>
#include <csignal>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <unordered_map>

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
    static void consumeConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic, std::vector<T>& payload,
            const int timeout_ms = -1) {
        assert(consumer);
        assert(topic);
        const int32_t partition = 0;
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
                break;
            }
            default: {
                throwFatalException(message->errstr());
                break;
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


namespace souffle {
namespace kafka {
class Kafka {
    // TODO (lyndonhenry): make this thread safe, use mutex and lock_gaurd
private:
    RdKafka::Conf* globalConf_;
    RdKafka::Conf* topicConf_;
    detail::KafkaHelper::EventCb eventCb_;
    detail::KafkaHelper::DeliveryReportCb deliveryReportCb_;
    RdKafka::Producer* producer_;
    RdKafka::Consumer* consumer_;
    std::unordered_map<std::string, RdKafka::Topic*> producerTopics_;
    std::unordered_map<std::string, RdKafka::Topic*> consumerTopics_;
    std::unordered_map<std::string, std::string> customConf_ = {
        {"bootstrap-server", "localhost:9092"},
        {"create-topics", "true"},
        {"run-program", "true"},
        // @TODO (lh): choose if to delete topics by default, don't do here for speed and debugging
        {"delete-topics", "true"},
        {"unique-id", [](){ return std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count()) + "."; }()},
        {"use-kafkacat", "true"}
        };
    std::vector<std::string> topicNames_;
    std::string uniqueId_;

private:
    explicit Kafka() {}

public:
    ~Kafka() {}

public:
    static Kafka& getInstance() {
        // get instance for singleton design pattern
        static Kafka instance = Kafka();
        return instance;
    }
    // deleted constructor for singleton design pattern
    Kafka(Kafka const&) = delete;
    // deleted constructor for singleton design pattern
    void operator=(Kafka const&) = delete;

public:
    void beginClient(const std::unordered_map<std::string, std::string>& globalConf) {
        // @TODO (lh): get everything working with topic creation and deletion for testsuite
        detail::KafkaHelper::unthrowException();
        detail::KafkaHelper::setSigintAndSigterm();
        globalConf_ = detail::KafkaHelper::createGlobalConf();
        if (!globalConf.count("metadata.broker.list")) {
            detail::KafkaHelper::setConf(globalConf_, "metadata.broker.list", "localhost:9092");
        }
        for (auto it = globalConf.begin(); it != globalConf.end(); ++it) {
            const std::string prefix = "custom.";
            if (it->first.rfind(prefix, 0) == 0) {
                const auto suffix = std::string(it->first.begin() + prefix.size(), it->first.end());
                customConf_[suffix] = it->second;
            } else {
                detail::KafkaHelper::setConf(globalConf_, it->first, it->second);
            }
        }
        uniqueId_ = customConf_.at("unique-id");
        if (customConf_.at("create-topics") == "true") {
            for (const auto& topicName : topicNames_) {
                createTopic(topicName, customConf_.at("bootstrap-server"));
            }
        }
        topicConf_ = detail::KafkaHelper::createTopicConf();
        // TODO (lyndonhenry): remove callbacks for efficiency if possible, ensure polling is still correct
        detail::KafkaHelper::setEventCb(globalConf_, &eventCb_);
        detail::KafkaHelper::setDeliveryReportCb(globalConf_, &deliveryReportCb_);
        detail::KafkaHelper::setDefaultTopicConf(globalConf_, topicConf_);
        producer_ = detail::KafkaHelper::createProducer(globalConf_);
        consumer_ = detail::KafkaHelper::createConsumer(globalConf_);
        producerTopics_ = std::unordered_map<std::string, RdKafka::Topic*>();
        consumerTopics_ = std::unordered_map<std::string, RdKafka::Topic*>();
    }
    void endClient() {
        detail::KafkaHelper::pollProducerUntilEmpty(producer_);
        delete producer_;
        delete consumer_;
        detail::KafkaHelper::waitDestroyed();
        delete globalConf_;
        delete topicConf_;
        if (customConf_.at("delete-topics") == "true") {
            for (const auto& topicName : topicNames_) {
                // @TODO (lh): assumes there is only one broker, maybe only use the first or set by input
                // parameter
                deleteTopic(topicName, customConf_.at("bootstrap-server"));
            }
        }
    }
    bool hasProductionBegun(const std::string& topicName) const {
        return producerTopics_.find(topicName) != producerTopics_.end();
    }
    bool hasProductionEnded(const std::string& topicName) const {
        assert(hasProductionBegun(topicName));
        return producerTopics_.at(topicName) == nullptr;
    }
    void beginProduction(const std::string& topicName) {
        if (!hasProductionBegun(topicName) || hasProductionEnded(topicName)) {
            producerTopics_[topicName] = detail::KafkaHelper::createTopic(topicConf_, producer_, getTopicIdentifier(topicName));
        }
    }
    void endProduction(const std::string& topicName) {
        delete producerTopics_[topicName];
        producerTopics_[topicName] = nullptr;
    }
    bool hasConsumptionBegun(const std::string& topicName) const {
        return consumerTopics_.find(topicName) != consumerTopics_.end();
    }
    bool hasConsumptionEnded(const std::string& topicName) const {
        assert(hasConsumptionBegun(topicName));
        return consumerTopics_.at(topicName) == nullptr;
    }
    void beginConsumption(const std::string& topicName) {
        if (!hasConsumptionBegun(topicName) || hasConsumptionEnded(topicName)) {
            consumerTopics_[topicName] = detail::KafkaHelper::createTopic(topicConf_, consumer_, getTopicIdentifier(topicName));
            detail::KafkaHelper::startConsumer(consumer_, consumerTopics_.at(topicName));
        }
    }
    void endConsumption(const std::string& topicName) {
        detail::KafkaHelper::stopConsumer(consumer_, consumerTopics_.at(topicName));
        delete consumerTopics_[topicName];
        consumerTopics_[topicName] = nullptr;
    }
    template <typename T>
    void produce(const std::string& topicName, std::vector<T>& payload) {
        RdKafka::Topic* topic = producerTopics_.at(topicName);
        assert(topic);
        detail::KafkaHelper::produceProducer(producer_, topic, payload);
    }
    template <typename T>
    void consume(const std::string& topicName, std::vector<T>& payload, const int timeoutMs = -1) {
        RdKafka::Topic* topic = consumerTopics_.at(topicName);
        assert(topic);
        detail::KafkaHelper::consumeConsumer(consumer_, topic, payload, timeoutMs);
    }
    void pollProducer(const int timeoutMs = 1000) {
        detail::KafkaHelper::pollHandle(producer_, timeoutMs);
    }
    void pollConsumer(const int timeoutMs = 1000) {
        detail::KafkaHelper::pollHandle(consumer_, timeoutMs);
    }
    void pollProducerUntilEmpty() {
        detail::KafkaHelper::pollProducerUntilEmpty(producer_);
    }
    void withSouffleProgram(const SouffleProgram& souffleProgram) {
        for (const auto& relation : souffleProgram.getAllRelations()) {
            topicNames_.push_back(relation->getName());
        }
    }
    bool runSouffleProgram() {
        return customConf_.at("run-program") == "true";
    }

private:
    void createTopic(const std::string& topicName, const std::string& bootstrapServer) {
        std::stringstream stringstream;
        if (customConf_.at("use-kafkacat") == "true") {
            stringstream << "kafkacat -b \"" << bootstrapServer << "\" -t \"" << getTopicIdentifier(topicName) << "\" > /dev/null 2>&1";
        } else {
        stringstream << "kafka-topics.sh --create --bootstrap-server \"" << bootstrapServer << "\" --topic \"" << getTopicIdentifier(topicName) << "\" --replication-factor 1 --partitions 1 > /dev/null 2>&1";
        }
        system(stringstream.str().c_str());
    }
    void deleteTopic(const std::string& topicName, const std::string& bootstrapServer = "localhost") {
        std::stringstream stringstream;
        stringstream << "kafka-topics.sh --delete --bootstrap-server \"" << bootstrapServer << "\" --topic \"" <<  getTopicIdentifier(topicName) << "\" > /dev/null 2>&1";
        system(stringstream.str().c_str());
    }
    std::string getTopicIdentifier(const std::string& topicName) {
        std::stringstream stringstream;
        stringstream << uniqueId_;
        for (const auto& character : topicName) {
            if (std::isalnum(character) || character == '_') {
                stringstream << character;
            } else {
                stringstream << (int) character;
            }
        }
        return stringstream.str();
    }
};
}  // namespace kafka
}  // namespace souffle
