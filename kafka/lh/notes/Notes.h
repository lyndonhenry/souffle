
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
    std::vector<RdKafka::Topic*> producerTopics_;
    std::vector<RdKafka::Topic*> consumerTopics_;
#ifdef DEBUG
    RdKafka::Topic* debugTopic_;
#endif
    void setUpKafka(const std::string& brokers) {
        KafkaHelper::unthrowException();
        KafkaHelper::setSigintAndSigterm();
        globalConf_ = KafkaHelper::createGlobalConf();
        topicConf_ = KafkaHelper::createTopicConf();
        KafkaHelper::setMetadataBrokerList(globalConf_, brokers);
        KafkaHelper::setEventCb(globalConf_, &eventCb_);
        KafkaHelper::setDeliveryReportCb(globalConf_, &deliveryReportCb_);
        KafkaHelper::setDefaultTopicConf(globalConf_, topicConf_);
        producer_ = KafkaHelper::createProducer(globalConf_);
        consumer_ = KafkaHelper::createConsumer(globalConf_);
        producerTopic_ = std::vector<RdKafka::Topic*>(0);
        consumerTopic_ = std::vector<RdKafka::Topic*>(0);
#ifdef DEBUG
        debugTopic_ = KafkaHelper::createTopic(topicConf_, producer_, "_DEBUG_");
#endif
    }
    void tearDownKafka() {
#ifdef DEBUG
        delete debugTopic_;
#endif
        delete producer_;
        delete consumer_;
        KafkaHelper::waitDestroyed();
        delete globalConf_;
        delete topicConf_;
    }

public:
    explicit KafkaClient(const std::string& brokers) {
        setUpKafka(brokers);
    }
    ~KafkaClient() {
        tearDownKafka();
    }
#ifdef DEBUG
    template <typename T>
    void debug(std::stringstream& stringstream, const std::vector<T>& payload) {
        other::write(stringstream, payload);
        stringstream << " " << __DATE__ << " " << __TIME__ << " ";
        const std::string str = stringstream.str();
        const char* cstr = str.c_str();
        std::vector<char> debugPayload(cstr, cstr + str.size());
        KafkaHelper::produceProducer(producer_, debugTopic_, debugPayload);
        KafkaHelper::pollProducerUntilEmpty(producer_, 1000);
    }
#endif
    template <typename T>
    void produce(RdKafka::Topic* topic, std::vector<T>& payload) {
#ifdef DEBUG
        {
            std::stringstream stringstream;
            stringstream << producer_->name() << " " << topic->name() << " ";
            debug(stringstream, payload);
        }
#endif
        KafkaHelper::produceProducer(producer_, topic, payload);
    }
    template <typename T>
    void consume(RdKafka::Topic* topic, std::vector<T>& payload) {
        KafkaHelper::consumeConsumer(consumer_, topic, payload);
#ifdef DEBUG
        {
            std::stringstream stringstream;
            stringstream << consumer_->name() << " " << topic->name() << " ";
            debug(stringstream, payload);
        }
#endif
    }
    void startConsumer(RdKafka::Topic* topic) {
        KafkaHelper::startConsumer(consumer_, topic);
    }
    void stopConsumer(RdKafka::Topic* topic) {
        KafkaHelper::stopConsumer(consumer_, topic);
    }
    void pollProducer(const int timeoutMs) {
        KafkaHelper::pollHandle(producer_, timeoutMs);
    }
    void pollConsumer(const int timeoutMs) {
        KafkaHelper::pollHandle(consumer_, timeoutMs);
    }
    void pollProducerUntilEmpty(int timeoutMs) {
        KafkaHelper::pollProducerUntilEmpty(producer_, timeoutMs);
    }
    RdKafka::Topic* createProducerTopic(const std::string& topicName) {
        return KafkaHelper::createTopic(topicConf_, producer_, topicName);
    }
    RdKafka::Topic* createConsumerTopic(const std::string& topicName) {
        return KafkaHelper::createTopic(topicConf_, consumer_, topicName);
    }
    void deleteTopic(RdKafka::Topic* topic) {
        delete topic;
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

