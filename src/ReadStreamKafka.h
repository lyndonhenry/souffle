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
    kafka::detail::KafkaClient& kafkaClient_;
    std::vector<RamDomain> payload_;
    std::size_t index_;
public:
    ReadStreamKafka(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t numberOfHeights = 0, const bool provenance = false)
            : ReadStream(symbolMask, symbolTable, provenance, numberOfHeights),
              relationName_(ioDirectives.getRelationName()),
              kafkaClient_(kafka::detail::KafkaClient::getInstance()), index_(0) {}
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
        kafkaClient_.beginConsumption(relationName_);
    }
    void endConsumption() {
        kafkaClient_.endConsumption(relationName_);
    }
    bool consumePayload() {
        std::vector<RamDomain> nextPayload;
        kafkaClient_.consume(relationName_, nextPayload);
        std::vector<RamDomain> nullPayload{std::numeric_limits<RamDomain>::max()};
        if (nextPayload == nullPayload) {
            return false;
        } else {
            payload_.insert(payload_.end(), nextPayload.begin(), nextPayload.end());
            return true;
        }
    }
    void pollConsumer() {
        kafkaClient_.pollConsumer();
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
public:
    ReadStreamKafkaDefault(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t numberOfHeights = 0, const bool provenance = false)
            : ReadStreamKafka(symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance) {}
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
            const IODirectives& ioDirectives, const size_t numberOfHeights = 0, const bool provenance = false)
            : ReadStreamKafka(symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance) {}
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
class ReadStreamKafkaFactory : public ReadStreamFactory {
public:
    std::unique_ptr<ReadStream> getReader(const std::vector<bool>& symbolMask, SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const bool provenance, const size_t numberOfHeights) override {
        if (ioDirectives.get("location") == "fact-dir") {
            // if (fact-dir, .facts, slave)
            return std::make_unique<ReadStreamKafkaDefault>(
                    symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance);
        } else if (ioDirectives.get("location") == "output-dir") {
            // if (output-dir, .facts, slave) or (output-dir, .csv, slave) or (output-dir, .csv, master)
            return std::make_unique<ReadStreamKafkaNullPayload>(
                    symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance);
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
    }
} /* namespace souffle */
