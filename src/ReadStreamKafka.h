/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2020, The Souffle Developers. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file ReadStreamKafka.h
 *
 ***********************************************************************/

#pragma once

#include "IODirectives.h"
#include "Kafka.h"
#include "RamTypes.h"
#include "ReadStream.h"
#include "SymbolTable.h"
#include "Util.h"

#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>

namespace souffle {
namespace kafka {
class ReadStreamKafka : public ReadStream {
private:
    const std::string relationName_;
    kafka::Kafka& kafka_;
    std::vector<RamDomain> payload_;
    std::size_t index_;
    // id of client of relation, will be remote for reader, self for writer
    const std::size_t clientId_;

public:
    ReadStreamKafka(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : ReadStream(symbolMask, symbolTable, auxiliaryArity),
              relationName_(ioDirectives.getRelationNameSuffix()), kafka_(kafka::Kafka::getInstance()),
              index_(0), clientId_(kafka_.getMetadata().getClientWithTopic(relationName_)) {}
    virtual ~ReadStreamKafka() = default;

protected:
    virtual void beginReader() = 0;
    virtual void endReader() = 0;

private:
    void beforeReadAll() override {
        beginReader();
    }
    void afterReadAll() override {
        endReader();
    }

protected:
    void beginConsumption() {
        kafka_.beginConsumption(relationName_);
        kafka_.beginConsumption(std::to_string(clientId_));
    }
    void endConsumption() {
        kafka_.endConsumption(std::to_string(clientId_));
        kafka_.endConsumption(relationName_);
    }
    bool consumePayload(const int timeoutMs = -1) {
        std::vector<RamDomain> nextPayload;
        kafka_.consume(relationName_, nextPayload, timeoutMs);
        if (nextPayload.empty()) {
            return true;
        }
        std::vector<RamDomain> nullPayload(arity ? arity : 1, std::numeric_limits<RamDomain>::max());
        if (nextPayload == nullPayload) {
            return false;
        } else if (arity == 0) {
            assert(!nextPayload.empty());
            payload_.insert(payload_.end(), nextPayload.begin(), nextPayload.end());
        } else {
            auto& metadata = kafka_.getMetadata();
            // reserve enough space to hold the next payload
            payload_.reserve(nextPayload.size());
            // iterate over each row to the payload as i
            for (SizeT i = 0; i < nextPayload.size(); i += arity) {
                // iterate over each column of the payload as j
                for (SizeT j = 0; j < arity; ++j) {
                    // if the current index is a symbol type...
                    if (symbolMask.at(j)) {
                        // and get the index itself
                        const auto remoteIndex = nextPayload[i + j];
                        // if the remote index is in the local mapping for the client...
                        if (remoteIndex < metadata.getRemoteIndexSize(clientId_)) {
                            // ...then convert the remote index to its local index
                            payload_.push_back(metadata.getLocalIndex(clientId_, remoteIndex));
                        } else {
                            // ...otherwise, while the symbol for the remote index is not received
                            while (remoteIndex >= metadata.getRemoteIndexSize(clientId_)) {
                                // create a vector to hold the symbols
                                auto symbols = std::vector<std::string>();
                                // consume the symbols on the topic of the client
                                kafka_.consume(std::to_string(clientId_), symbols);
                                // for each of the received symbols...
                                for (const auto& symbol : symbols) {
                                    // add the local index to the mapping for the remote index of the symbol
                                    metadata.addLocalIndex(clientId_, symbolTable.unsafeLookup(symbol));
                                }
                            }
                            // finally, add the local index of the remote index to the payload
                            payload_.push_back(metadata.getLocalIndex(clientId_, remoteIndex));
                        }
                    } else {
                        // ...otherwise, the current index is not a symbol
                        payload_.push_back(nextPayload[i + j]);
                    }
                }
            }
        }
        return true;
    }
    void pollConsumer() {
        kafka_.pollConsumer();
    }
    bool hasConsumptionBegun() const {
        return kafka_.hasConsumptionBegun(relationName_);
    }
    bool hasConsumptionEnded() const {
        return kafka_.hasConsumptionEnded(relationName_);
    }
    void setPayload(const std::vector<RamDomain>& payload) {
        payload_ = payload;
    }
    const std::vector<RamDomain>& getPayload() const {
        return payload_;
    }

public:
    std::unique_ptr<RamDomain[]> readNextTuple() override {
        if (index_ == payload_.size() || (arity == 0 && payload_.empty())) {
            return nullptr;
        }
        if (arity == 0) {
            index_ = payload_.size();
        }
        std::unique_ptr<RamDomain[]> tuple = std::make_unique<RamDomain[]>(symbolMask.size());
        for (std::size_t column = 0; column < arity; ++column) {
            tuple[column] = payload_[index_];
            ++index_;
        }
        return tuple;
    }
};
class ReadStreamKafkaDefault : public ReadStreamKafka {
public:
    ReadStreamKafkaDefault(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : ReadStreamKafka(symbolMask, symbolTable, ioDirectives, auxiliaryArity) {}
    virtual ~ReadStreamKafkaDefault() = default;

private:
    void beginReader() override {
        beginConsumption();
        consumePayload();
    }
    void endReader() override {
        endConsumption();
    }
};
class ReadStreamKafkaNullPayload : public ReadStreamKafka {
public:
    ReadStreamKafkaNullPayload(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : ReadStreamKafka(symbolMask, symbolTable, ioDirectives, auxiliaryArity) {}
    virtual ~ReadStreamKafkaNullPayload() = default;

private:
    void beginReader() override {
        beginConsumption();
        while (consumePayload()) {
        }
    }
    void endReader() override {
        endConsumption();
    }
};
class ReadStreamKafkaWithTimeout : public ReadStreamKafka {
public:
    ReadStreamKafkaWithTimeout(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : ReadStreamKafka(symbolMask, symbolTable, ioDirectives, auxiliaryArity) {}
    virtual ~ReadStreamKafkaWithTimeout() = default;

private:
    void beginReader() override {
        // if the topic has not yet been created...
        if (!hasConsumptionBegun()) {
            // ...create the topic
            beginConsumption();
        }
        // if a null payload has already been received...
        if (hasConsumptionEnded()) {
            // ...set the payload to the null payload and return
            std::vector<RamDomain> nullPayload(arity ? arity : 1, std::numeric_limits<RamDomain>::max());
            setPayload(nullPayload);
            return;
        }
        // note that this timeout is arbitrary
        const int timeoutMs = 10;
        while (true) {
            const auto prevPayloadSize = getPayload().size();
            // if payload is not the null message...
            if (consumePayload(timeoutMs)) {
                const auto nextPayloadSize = getPayload().size();
                // if the consumer has timed out...
                if (nextPayloadSize == prevPayloadSize) {
                    // ...exit the reader
                    return;
                }
                // otherwise, continue until the consumer does time out
            } else {
                if (getPayload().empty()) {
                    // set the payload to the null message
                    std::vector<RamDomain> nullPayload(
                            arity ? arity : 1, std::numeric_limits<RamDomain>::max());
                    setPayload(nullPayload);
                }
                // end the consumer
                endConsumption();
                return;
            }
        }
    }
    void endReader() override {}
};
class ReadStreamKafkaFactory : public ReadStreamFactory {
public:
    std::unique_ptr<ReadStream> getReader(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity) override {
        if (ioDirectives.get("kafka") == "default") {
            // if (fact-dir, .facts, slave)
            return std::make_unique<ReadStreamKafkaDefault>(
                    symbolMask, symbolTable, ioDirectives, auxiliaryArity);
        } else if (ioDirectives.get("kafka") == "null-payload") {
            // if (output-dir, .facts, slave) or (output-dir, .csv, slave) or (output-dir, .csv, master)
            return std::make_unique<ReadStreamKafkaNullPayload>(
                    symbolMask, symbolTable, ioDirectives, auxiliaryArity);
        } else if (ioDirectives.get("kafka") == "with-timeout") {
            // if using non-blocking consumers in fixpoint loop
            return std::make_unique<ReadStreamKafkaWithTimeout>(
                    symbolMask, symbolTable, ioDirectives, auxiliaryArity);
        } else {
            assert(false);
        }
    }
    const std::string& getName() const override {
        static const std::string name = "kafka";
        return name;
    }
    ~ReadStreamKafkaFactory() override = default;
};
}  // namespace kafka
} /* namespace souffle */
