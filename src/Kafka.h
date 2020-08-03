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

#include "RamTypes.h"
#include "SouffleInterface.h"

#include <librdkafka/rdkafkacpp.h>

#include <atomic>
#include <cassert>
#include <cctype>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <unordered_map>
#include <unordered_set>

#define KAFKA

// TODO (lh): this should be disabled for any performance related experiments
// this enables logging to the debug topic, undef it for performance
#define KAFKA_DEBUG

namespace souffle {
namespace kafka {

/** Simple Types */

using Byte = char;

using Ostream = std::ostream;

using SizeT = std::size_t;

using String = std::string;

using StringStream = std::stringstream;

/** Complex Types */

template <typename A, typename B>
using Map = std::unordered_map<A, B>;

template <typename A>
using Set = std::unordered_set<A>;

template <typename... A>
using Tuple = std::tuple<A...>;

template <typename A>
using Vec = std::vector<A>;

template <typename A>
class bit_length {
public:
    static constexpr SizeT value = 8 * sizeof(A);
};

}  // namespace kafka
}  // namespace souffle

namespace souffle {
namespace kafka {
namespace detail {

/** Into Type Declaration */

template <typename A, typename B>
class Into;

/** Into Implementations */

template <typename A>
class Into<A, A> {
public:
    static A into(const A&& a) {
        return a;
    }
};

template <>
class Into<SizeT, Vec<Byte>> {
public:
    static Vec<Byte> into(const SizeT&& a) {
        auto b = Vec<Byte>(sizeof(SizeT), 0);
        for (SizeT i = 0; i < sizeof(SizeT); ++i) {
            b[(sizeof(SizeT) - 1) - i] = (a >> (bit_length<Byte>::value * i)) & 0xFF;
        }
        return b;
    }
};

template <>
class Into<Vec<Byte>, SizeT> {
public:
    static SizeT into(const Vec<Byte>&& a) {
        SizeT b = 0;
        for (SizeT i = 0; i < sizeof(SizeT); ++i) {
            b = (b << bit_length<Byte>::value) | a[i];
        }
        return b;
    }
};

template <>
class Into<String, Vec<Byte>> {
public:
    static Vec<Byte> into(const String&& a) {
        auto b = Into<SizeT, Vec<Byte>>::into(a.size());
        b.insert(b.end(), a.begin(), a.end());
        return b;
    }
};

template <>
class Into<Vec<Byte>, String> {
public:
    static String into(const Vec<Byte>&& a) {
        return String(a.begin() + sizeof(SizeT), a.end());
    }
};

template <>
class Into<Vec<Byte>, Vec<String>> {
public:
    static Vec<String> into(const Vec<Byte>&& a) {
        auto b = Vec<String>();
        for (auto it = a.begin(); it < a.end();) {
            auto next = it + sizeof(SizeT);
            assert(next <= a.end());
            auto b1 = Vec<Byte>(it, next);
            auto s1 = Into<Vec<Byte>, SizeT>::into(std::move(b1));
            next = it + (sizeof(SizeT) + s1);
            assert(next <= a.end());
            auto b2 = Vec<Byte>(it, next);
            auto s2 = Into<Vec<Byte>, String>::into(std::move(b2));
            b.push_back(std::move(s2));
            it += sizeof(SizeT) + s1;
        }
        return b;
    }
};

template <>
class Into<Vec<String>, Vec<Byte>> {
public:
    static Vec<Byte> into(const Vec<String>&& a) {
        auto b = Vec<Byte>();
        for (const auto& s1 : a) {
            auto s2 = s1;  // make copy
            const auto c = Into<String, Vec<Byte>>::into(std::move(s2));
            b.insert(b.end(), c.begin(), c.end());
        }
        return b;
    }
};

/** Into Function Definition */

template <typename B, typename A>
B into(const A&& a) {
    return Into<A, B>::into(std::move(a));
}

template <typename C, typename B, typename A>
C into(const A&& a) {
    return into<C>(into<B>(std::move(a)));
}

}  // namespace detail
}  // namespace kafka
}  // namespace souffle

namespace souffle {
namespace kafka {
namespace detail {

static std::atomic<bool> exceptionThrown_;

class KafkaHelperState {
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
};

class Poller {
public:
    static void pollHandle(RdKafka::Handle* handle, int timeout_ms) {
        assert(handle);
        handle->poll(timeout_ms);
    }
    static void pollProducerUntilEmpty(RdKafka::Producer* producer) {
        assert(producer);
        // -1 will perform an indefinate blocking wait
        const int timeout_ms = -1;
        while (!KafkaHelperState::exceptionThrown() && producer->outq_len() > 0) {
            pollHandle(producer, timeout_ms);
        }
    }
};

template <typename T>
class Producer {
public:
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
            KafkaHelperState::throwFatalException(RdKafka::err2str(errorCode));
        }
        Poller::pollHandle(producer, 0);
    }
};

template <>
class Producer<std::string> {
public:
    static void produceProducer(
            RdKafka::Producer* producer, RdKafka::Topic* topic, std::vector<std::string>& payload) {
        auto payloadCopy = payload;
        auto newPayload = into<std::vector<char>>(std::move(payloadCopy));
        Producer<char>::produceProducer(producer, topic, newPayload);
    }
};

template <typename T>
class Consumer {
public:
    static void consumeConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic, std::vector<T>& payload,
            const int timeout_ms = -1) {
        assert(consumer);
        assert(topic);
        const int32_t partition = 0;
        RdKafka::Message* message = consumer->consume(topic, partition, timeout_ms);
        Poller::pollHandle(consumer, 0);
        switch (message->err()) {
            case RdKafka::ERR__TIMED_OUT: {
                delete message;
                break;
            }
            case RdKafka::ERR__PARTITION_EOF: {
                KafkaHelperState::throwFatalException(message->errstr());
                break;
            }
            case RdKafka::ERR__UNKNOWN_TOPIC: {
                KafkaHelperState::throwFatalException(message->errstr());
                break;
            }
            case RdKafka::ERR__UNKNOWN_PARTITION: {
                KafkaHelperState::throwFatalException(message->errstr());
                break;
            }
            case RdKafka::ERR_NO_ERROR: {
                const T* rawPayload = static_cast<T*>(message->payload());
                payload = std::vector<T>(rawPayload, rawPayload + (message->len() / sizeof(T)));
                Poller::pollHandle(consumer, 0);
                delete message;
                break;
            }
            default: {
                KafkaHelperState::throwFatalException(message->errstr());
                break;
            }
        }
    }
};

template <>
class Consumer<std::string> {
public:
    static void consumeConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic,
            std::vector<std::string>& payload, const int timeout_ms = -1) {
        auto payloadCopy = std::vector<char>();
        Consumer<char>::consumeConsumer(consumer, topic, payloadCopy, timeout_ms);
        const auto newPayload = into<std::vector<std::string>>(std::move(payloadCopy));
        payload.insert(payload.end(), newPayload.begin(), newPayload.end());
    }
};

class KafkaHelper {
public:
    static RdKafka::Conf* createConf(const RdKafka::Conf::ConfType confType) {
        RdKafka::Conf* conf = RdKafka::Conf::create(confType);
        if (!conf) {
            KafkaHelperState::throwFatalException();
        }
        return conf;
    }
    static RdKafka::Conf* createGlobalConf() {
        KafkaHelperState::unthrowException();
        KafkaHelperState::setSigintAndSigterm();
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
            KafkaHelperState::throwFatalException(errstr);
        }
    }
    static void eventCb(RdKafka::Event& event) {
        switch (event.type()) {
            case RdKafka::Event::EVENT_ERROR: {
                if (event.fatal()) {
                    const auto errstr = RdKafka::err2str(event.err());
                    KafkaHelperState::throwFatalException(errstr);
                }
                break;
            }
            case RdKafka::Event::EVENT_STATS: {
                break;
            }
            case RdKafka::Event::EVENT_LOG: {
                break;
            }
            default: {
                break;
            }
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
            default: {
                break;
            }
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
            KafkaHelperState::throwFatalException(errstr);
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
            KafkaHelperState::throwFatalException(errstr);
        }
        return topic;
    }
    template <typename T>
    static void produceProducer(RdKafka::Producer* producer, RdKafka::Topic* topic, std::vector<T>& payload) {
        Producer<T>::produceProducer(producer, topic, payload);
    }
    static void pollHandle(RdKafka::Handle* handle, int timeout_ms) {
        Poller::pollHandle(handle, timeout_ms);
    }
    static void pollProducerUntilEmpty(RdKafka::Producer* producer) {
        Poller::pollProducerUntilEmpty(producer);
    }
    static void startConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic) {
        assert(consumer);
        assert(topic);
        const int32_t partition = 0;
        const int64_t offset = RdKafka::Topic::OFFSET_BEGINNING;
        RdKafka::ErrorCode errorCode = consumer->start(topic, partition, offset);
        if (errorCode != RdKafka::ERR_NO_ERROR) {
            KafkaHelperState::throwFatalException(RdKafka::err2str(errorCode));
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
        Consumer<T>::consumeConsumer(consumer, topic, payload, timeout_ms);
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
class Metadata {
private:
    using ClientGraph = Map<SizeT, Vec<SizeT>>;
    using TopicAssignment = Map<SizeT, Vec<String>>;
    using MetadataType = Tuple<ClientGraph, TopicAssignment>;
    MetadataType value_;

public:
    using value_t = MetadataType;

public:
    void set(const value_t& value) {
        value_ = value;
    }

private:
    const ClientGraph& clientGraph() const {
        return std::get<0>(value_);
    }
    const TopicAssignment& topicAssignment() const {
        return std::get<1>(value_);
    };

private:
    mutable Set<SizeT> allClients_;

public:
    const Set<SizeT>& getAllClients() const {
        if (allClients_.empty()) {
            for (const auto& pair : clientGraph()) {
                allClients_.insert(pair.first);
                for (const auto client : pair.second) {
                    allClients_.insert(client);
                }
            }
        }
        return allClients_;
    }

private:
    mutable Map<String, SizeT> clientWithTopic_;

public:
    SizeT getClientWithTopic(const String& topic) const {
        if (clientWithTopic_.empty()) {
            for (const auto& pair : topicAssignment()) {
                for (const auto& topic : pair.second) {
                    clientWithTopic_.insert({{topic, pair.first}});
                }
            }
        }
        return clientWithTopic_.at(topic);
    }

public:
    void printClients(Ostream& os) const {
        bool flag = false;
        for (const auto& client : getAllClients()) {
            if (flag)
                os << " ";
            else
                flag = true;
            os << client;
        }
        os << std::endl;
    }

private:
    mutable Map<SizeT, Vec<RamDomain>> perClientIndex_;

public:
    RamDomain getRemoteIndexSize(const SizeT client) const {
        // note that this cast is unchecked
        return static_cast<RamDomain>(perClientIndex_[client].size());
    }
    RamDomain getLocalIndex(const SizeT client, const RamDomain remoteIndex) const {
        return perClientIndex_[client].at(remoteIndex);
    }
    void addLocalIndex(const SizeT client, const RamDomain localIndex) {
        perClientIndex_[client].push_back(localIndex);
    }

private:
    Map<SizeT, SizeT> kv_;

public:
    SizeT getOrElse(SizeT key, SizeT other) {
        return (kv_.count(key)) ? kv_.at(key) : other;
    }
    void set(SizeT key, SizeT value) {
        kv_[key] = value;
    }
};

class Kafka {
private:
    RdKafka::Conf* globalConf_;
    RdKafka::Conf* topicConf_;
    detail::KafkaHelper::EventCb eventCb_;
    detail::KafkaHelper::DeliveryReportCb deliveryReportCb_;
    RdKafka::Producer* producer_;
    RdKafka::Consumer* consumer_;
    Map<String, RdKafka::Topic*> producerTopics_;
    Map<String, RdKafka::Topic*> consumerTopics_;
    Map<String, String> customConf_ = {{"create-topics", "true"},
            {"disable-stdout", "true"}, {"disable-stderr", "true"}, {"run-program", "true"},
            {"delete-topics", "true"},
            {"unique-id", std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(
                                  std::chrono::high_resolution_clock::now().time_since_epoch())
                                                 .count())},
            {"use-kafkacat", "false"}, {"print-metadata", "false"}
#ifdef KAFKA_DEBUG
            // we can disable debugging regardless of compilation options by overriding this option
            , {"debug", "true"}
            , {"debug-topic", "souffle"}
#endif
    };
    Vec<String> topicNames_;
    Metadata metadata_;
#ifdef KAFKA_DEBUG
        bool debugState_ = false;
        RdKafka::Topic* debugTopic_;
        Vec<String> debugMessageQueue_;
#endif

private:
    explicit Kafka() {}

private:
    inline void beginDebug() {
        assert(debugState_ = false);
#ifdef KAFKA_DEBUG
        debugTopic_ = detail::KafkaHelper::createTopic(topicConf_, producer_, customConf_.at("debug-topic"));
#endif
        debugState_ = true;
    }

    inline void endDebug() {
        assert(debugState_ = true);
#ifdef KAFKA_DEBUG
        delete debugTopic_;
#endif
        debugState_ = false;
    }
public:
    inline void debug(const String& message) {
        (void) message;
#ifdef KAFKA_DEBUG
        if (!debugState_) {
            debugMessageQueue_.push_back(message);
        } else {
            auto produceDebugMessage = [&](const String& msg) -> void {
                auto payload = Vec<char>(msg.begin(), msg.end());
                detail::KafkaHelper::produceProducer(producer_, debugTopic_, payload);
            }
            if (!debugMessageQueue_.empty()) {
                for (auto& msg : debugMessageQueue_) {
                    produceDebugMessage(msg);
                }
                debugMessageQueue_.clear();
            }
            assert(debugMessageQueue_.empty());
            produceDebugMessage(message);
        }
#endif
    }
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
    void beginClient(const Map<String, String>& globalConf) {
        debug("Begin function Kafka::beginClient()");
        if (globalConf.count("custom.print-metadata")) {
            metadata_.printClients(std::cout);
            exit(0);
        }
        globalConf_ = detail::KafkaHelper::createGlobalConf();
        customConf_["metadata.broker.list"] = (globalConf.count("metadata.broker.list")) ? globalConf.at("metadata.broker.list") : "localhost:9092";

        for (auto it = globalConf.begin(); it != globalConf.end(); ++it) {
            const String prefix = "custom.";
            if (it->first.rfind(prefix, 0) == 0) {
                const auto suffix = String(it->first.begin() + prefix.size(), it->first.end());
                customConf_[suffix] = it->second;
            } else {
                detail::KafkaHelper::setConf(globalConf_, it->first, it->second);
            }
        }
        if (customConf_.at("create-topics") == "true") {
            for (const auto& topicName : topicNames_) {
                createTopic(topicName);
            }
        }
        topicConf_ = detail::KafkaHelper::createTopicConf();
        detail::KafkaHelper::setEventCb(globalConf_, &eventCb_);
        detail::KafkaHelper::setDeliveryReportCb(globalConf_, &deliveryReportCb_);
        detail::KafkaHelper::setDefaultTopicConf(globalConf_, topicConf_);
        producer_ = detail::KafkaHelper::createProducer(globalConf_);
        consumer_ = detail::KafkaHelper::createConsumer(globalConf_);
        producerTopics_ = Map<String, RdKafka::Topic*>();
        consumerTopics_ = Map<String, RdKafka::Topic*>();
        beginDebug();
        debug("End function Kafka::beginClient()");
    }
    void endClient() {
        debug("Begin function Kafka::endClient()");
        endDebug();
        detail::KafkaHelper::pollProducerUntilEmpty(producer_);
        delete producer_;
        delete consumer_;
        detail::KafkaHelper::waitDestroyed();
        delete globalConf_;
        delete topicConf_;
        if (customConf_.at("delete-topics") == "true") {
            for (const auto& topicName : topicNames_) {
                deleteTopic(topicName);
            }
        }
        debug("End function Kafka::endClient()");
    }
    bool hasProductionBegun(const String& topicName) const {
        debug("Begin function Kafka::hasProductionBegun()");
        const auto result = producerTopics_.find(topicName) != producerTopics_.end();
        debug("End function Kafka::hasProductionBegun()");
        return result;
    }
    bool hasProductionEnded(const String& topicName) const {
        debug("Begin function Kafka::hasProductionEnded()");
        assert(hasProductionBegun(topicName));
        const auto result = producerTopics_.at(topicName) == nullptr;
        debug("End function Kafka::hasProductionEnded()");
        return result;

    }
    void beginProduction(const String& topicName) {
        debug("Begin function Kafka::beginProduction(" + topicName + ")");
        if (!hasProductionBegun(topicName) || hasProductionEnded(topicName)) {
            producerTopics_[topicName] =
                    detail::KafkaHelper::createTopic(topicConf_, producer_, getTopicIdentifier(topicName));
        }
        debug("End function Kafka::beginProduction(" + topicName + ")");
    }
    void endProduction(const String& topicName) {
        debug("Begin function Kafka::endProduction(" + topicName + ")");
        delete producerTopics_[topicName];
        producerTopics_[topicName] = nullptr;
        debug("End function Kafka::endProduction(" + topicName + ")");
    }
    bool hasConsumptionBegun(const String& topicName) const {
        debug("Begin function Kafka::hasConsumptionBegun(" + topicName + ")");
        const auto result = consumerTopics_.find(topicName) != consumerTopics_.end();
        debug("End function Kafka::hasConsumptionBegun(" + topicName + ")");
        return result;
    }
    bool hasConsumptionEnded(const String& topicName) const {
        debug("Begin function Kafka::hasConsumptionEnded(" + topicName + ")");
        assert(hasConsumptionBegun(topicName));
        const auto result = consumerTopics_.at(topicName) == nullptr;
        debug("End function Kafka::hasConsumptionEnded(" + topicName + ")");
        return result;
    }
    void beginConsumption(const String& topicName) {
        debug("Begin function Kafka::beginConsumption(" + topicName + ")");
        if (!hasConsumptionBegun(topicName) || hasConsumptionEnded(topicName)) {
            consumerTopics_[topicName] =
                    detail::KafkaHelper::createTopic(topicConf_, consumer_, getTopicIdentifier(topicName));
            detail::KafkaHelper::startConsumer(consumer_, consumerTopics_.at(topicName));
        }
        debug("End function Kafka::beginConsumption(" + topicName + ")");
    }
    void endConsumption(const String& topicName) {
        debug("Begin function Kafka::endConsumption(" + topicName + ")");
        detail::KafkaHelper::stopConsumer(consumer_, consumerTopics_.at(topicName));
        delete consumerTopics_[topicName];
        consumerTopics_[topicName] = nullptr;
        debug("End function Kafka::endConsumption(" + topicName + ")");
    }
    template <typename T>
    void produce(const String& topicName, Vec<T>& payload) {
        debug("Begin function Kafka::produce(" + topicName + ", ...)");
        RdKafka::Topic* topic = producerTopics_.at(topicName);
        assert(topic);
        detail::KafkaHelper::produceProducer(producer_, topic, payload);
        debug("End function Kafka::produce(" + topicName + ", ...)");
    }
    template <typename T>
    void consume(const String& topicName, Vec<T>& payload, const int timeoutMs = -1) {
        debug("Begin function Kafka::consume(" + topicName + ", ..., " + timeoutMs + ")");
        RdKafka::Topic* topic = consumerTopics_.at(topicName);
        assert(topic);
        detail::KafkaHelper::consumeConsumer(consumer_, topic, payload, timeoutMs);
        debug("End function Kafka::consume(" + topicName + ", ..., " + timeoutMs + ")");
    }
    void pollProducer(const int timeoutMs = 1000) {
        debug("Begin function Kafka::pollProducer(" + timeoutMs + ")");
        detail::KafkaHelper::pollHandle(producer_, timeoutMs);
        debug("End function Kafka::pollProducer(" + timeoutMs + ")");
    }
    void pollConsumer(const int timeoutMs = 1000) {
        debug("Begin function Kafka::pollConsumer(" + timeoutMs + ")");
        detail::KafkaHelper::pollHandle(consumer_, timeoutMs);
        debug("End function Kafka::pollConsumer(" + timeoutMs + ")");
    }
    void pollProducerUntilEmpty() {
        debug("Begin function Kafka::pollProducerUntilEmpty()");
        detail::KafkaHelper::pollProducerUntilEmpty(producer_);
        debug("End function Kafka::pollProducerUntilEmpty()");
    }
    void withSouffleProgram(const SouffleProgram& souffleProgram) {
        debug("Begin function Kafka::withSouffleProgram(...)");
        for (const auto& relation : souffleProgram.getAllRelations()) {
            topicNames_.push_back(relation->getName());
        }
        debug("End function Kafka::withSouffleProgram(...)");
    }
    bool runSouffleProgram() {
        debug("Begin function Kafka::runSouffleProgram(...)");
        const auto result = customConf_.at("run-program") == "true";
        debug("End function Kafka::runSouffleProgram(...)");
        return result;
    }
    void withMetadata(const Metadata::value_t& metadata) {
        debug("Begin function Kafka::withMetadata(...)");
        metadata_.set(metadata);
        for (const auto& client : metadata_.getAllClients()) {
            topicNames_.push_back(std::to_string(client));
        }
        debug("End function Kafka::withMetadata(...)");
    }
    Metadata& getMetadata() {
        debug("Begin function Kafka::getMetadata(...)");
        debug("End function Kafka::getMetadata(...)");
        return metadata_;
    }

private:
    void createTopic(const String& topicName) {
        StringStream stringstream;
        if (customConf_.at("use-kafkacat") == "true") {
            stringstream << "kafkacat -b \"" << customConf_.at("metadata.broker.list") << "\" -t \"" << getTopicIdentifier(topicName)
                         << "\"";
        } else {
            stringstream << "kafka-topics.sh --create --bootstrap-server \"" << customConf_.at("metadata.broker.list")
                         << "\" --topic \"" << getTopicIdentifier(topicName)
                         << "\" --replication-factor 1 --partitions 1";
        }
        addIoRedirect(stringstream);
        system(stringstream.str().c_str());
    }
    void deleteTopic(const String& topicName) {
        StringStream stringstream;
        stringstream << "kafka-topics.sh --delete --bootstrap-server \"" << customConf_.at("metadata.broker.list") << "\" --topic \""
                     << getTopicIdentifier(topicName) << "\"";
        addIoRedirect(stringstream);
        system(stringstream.str().c_str());
    }

private:
    String getTopicIdentifier(const String& topicName) const {
        StringStream stringstream;
        stringstream << customConf_.at("unique-id") << "_";
        for (const auto& character : topicName) {
            if (std::isalnum(character) || character == '_') {
                stringstream << character;
            } else {
                stringstream << (int)character;
            }
        }
        return stringstream.str();
    }
    void addIoRedirect(StringStream& stringstream) const {
        if (customConf_.at("disable-stdout") == "true") {
            stringstream << " > /dev/null";
            if (customConf_.at("disable-stderr") == "true") {
                stringstream << " 2>&1";
            }
        }
    }
};
}  // namespace kafka
}  // namespace souffle
