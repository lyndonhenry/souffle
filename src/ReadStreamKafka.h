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

public:
    ReadStreamKafka(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : ReadStream(symbolMask, symbolTable, auxiliaryArity),
              relationName_(ioDirectives.getRelationNameSuffix()), kafka_(kafka::Kafka::getInstance()),
              index_(0) {}
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
    }
    void endConsumption() {
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
        } else {
            payload_.insert(payload_.end(), nextPayload.begin(), nextPayload.end());
            return true;
        }
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
        if (index_ == payload_.size()) {
            return nullptr;
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
    // @TODO (lh): get this working with streaming strings without dictionaries for now, so that all tests
    // pass

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
        // @TODO (lh): figure out a good consumer timeout, maybe set with -X
        // possible set a timeout for the consumer
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
