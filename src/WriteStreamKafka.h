/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2020, The Souffle Developers. All rights reserved.
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file WriteStreamKafka.h
 *
 ***********************************************************************/

#pragma once

#include "IODirectives.h"
#include "Kafka.h"
#include "ParallelUtils.h"
#include "SymbolTable.h"
#include "WriteStream.h"

#include <cassert>
#include <fstream>
#include <memory>
#include <ostream>
#include <string>

namespace souffle {
namespace kafka {
class WriteStreamKafka : public WriteStream {
protected:
    const std::string relationName_;
    kafka::Kafka& kafka_;
    // id of client of relation, will be self for writer, other for reader
    const std::size_t clientId_;
    std::vector<RamDomain> payload_;
    std::vector<std::string> strings_;

public:
    WriteStreamKafka(const std::vector<bool>& symbolMask, const SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : WriteStream(symbolMask, symbolTable, auxiliaryArity),
              relationName_(ioDirectives.getRelationNameSuffix()), kafka_(kafka::Kafka::getInstance()),
              clientId_(kafka_.getMetadata().getClientWithTopic(relationName_)) {}
    virtual ~WriteStreamKafka() = default;

protected:
    virtual void beginWriter() = 0;
    virtual void endWriter() = 0;

private:
    void beforeWriteAll() override {
        beginWriter();
    }
    void afterWriteAll() override {
        endWriter();
    }

protected:
    void beginProduction() {
        kafka_.beginProduction(relationName_);
        kafka_.beginProduction(std::to_string(clientId_));
    }
    void endProduction() {
        kafka_.endProduction(relationName_);
        kafka_.endProduction(std::to_string(clientId_));
    }
    void producePayload() {
        kafka_.produce(relationName_, payload_);
        payload_.clear();
        kafka_.produce(std::to_string(clientId_), strings_);
        strings_.clear();
    }
    void produceNullPayload() {
        std::vector<RamDomain> nullPayload(arity ? arity : 1, std::numeric_limits<RamDomain>::max());
        kafka_.produce(relationName_, nullPayload);
    }
    void pollProducer() {
        kafka_.pollProducer();
    }
    void pollProducerNonBlocking() {
        kafka_.pollProducer(0);
    }
    void pollProducerUntilEmpty() {
        kafka_.pollProducerUntilEmpty();
    }

public:
    void writeNullary() override {
        payload_.push_back(1);
    }
    void writeNextTuple(const RamDomain* tuple) override {
        auto& metadata = kafka_.getMetadata();
        const auto begin = metadata.getOrElse(clientId_, 0);
        const auto end = symbolTable.size();
        // ensure a max message size of 1MB, note that this is untested for all execution configurations
        const auto next_payload_bytes = ((payload_.size() + arity) * 64) / 8;
        const auto next_string_bytes = (end - begin);
        const auto max_bytes = 1000000 ; // 1MB
        if (next_payload_bytes > max_bytes || next_string_bytes > max_bytes) {
            producePayload();
        }
        strings_.reserve(end - begin);
        for (auto i = begin; i < end; ++i) {
            strings_.push_back(symbolTable.unsafeResolve(i));
        }
        metadata.set(clientId_, end);
        for (std::size_t column = 0; column < arity; ++column) {
            payload_.push_back(tuple[column]);
        }
    }
};
class WriteStreamKafkaDefault : public WriteStreamKafka {
public:
    WriteStreamKafkaDefault(const std::vector<bool>& symbolMask, const SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : WriteStreamKafka(symbolMask, symbolTable, ioDirectives, auxiliaryArity) {}
    virtual ~WriteStreamKafkaDefault() = default;

private:
    void beginWriter() override {
        beginProduction();
    }
    void endWriter() override {
        producePayload();
        pollProducerNonBlocking();
    }
};
class WriteStreamKafkaNullPayload : public WriteStreamKafka {
public:
    WriteStreamKafkaNullPayload(const std::vector<bool>& symbolMask, const SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t auxiliaryArity = 0)
            : WriteStreamKafka(symbolMask, symbolTable, ioDirectives, auxiliaryArity) {}
    virtual ~WriteStreamKafkaNullPayload() = default;

private:
    void beginWriter() override {
        beginProduction();
    }
    void endWriter() override {
        produceNullPayload();
        pollProducerNonBlocking();
        endProduction();
    }
};
class WriteStreamKafkaFactory : public WriteStreamFactory {
public:
    std::unique_ptr<WriteStream> getWriter(const std::vector<bool>& symbolMask,
            const SymbolTable& symbolTable, const IODirectives& ioDirectives,
            const size_t auxiliaryArity) override {
        if (ioDirectives.get("kafka") == "default") {
            // if (fact-dir, .facts, master)
            return std::make_unique<WriteStreamKafkaDefault>(
                    symbolMask, symbolTable, ioDirectives, auxiliaryArity);
        } else if (ioDirectives.get("kafka") == "null-payload") {
            // if (output-dir, .facts, slave) or (output-dir, .csv, slave)
            return std::make_unique<WriteStreamKafkaNullPayload>(
                    symbolMask, symbolTable, ioDirectives, auxiliaryArity);
        } else {
            assert(false);
        }
    }
    const std::string& getName() const override {
        static const std::string name = "kafka";
        return name;
    }
    ~WriteStreamKafkaFactory() override = default;
};
}  // namespace kafka
} /* namespace souffle */
