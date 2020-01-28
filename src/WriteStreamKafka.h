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
    std::vector<RamDomain> payload_;

public:
    WriteStreamKafka(const std::vector<bool>& symbolMask, const SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t numberOfHeights = 0, const bool provenance = false)
            : WriteStream(symbolMask, symbolTable, provenance, numberOfHeights),
              relationName_(ioDirectives.getRelationNameSuffix()), kafka_(kafka::Kafka::getInstance()) {}
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
    }
    void endProduction() {
        kafka_.endProduction(relationName_);
    }
    void producePayload() {
        kafka_.produce(relationName_, payload_);
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
        for (std::size_t column = 0; column < arity; ++column) {
            payload_.push_back(tuple[column]);
        }
    }
};
class WriteStreamKafkaDefault : public WriteStreamKafka {
public:
    WriteStreamKafkaDefault(const std::vector<bool>& symbolMask, const SymbolTable& symbolTable,
            const IODirectives& ioDirectives, const size_t numberOfHeights = 0, const bool provenance = false)
            : WriteStreamKafka(symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance) {}
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
            const IODirectives& ioDirectives, const size_t numberOfHeights = 0, const bool provenance = false)
            : WriteStreamKafka(symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance) {}
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
            const SymbolTable& symbolTable, const IODirectives& ioDirectives, const bool provenance,
            const size_t numberOfHeights) override {
        if (ioDirectives.get("kafka") == "default") {
            // if (fact-dir, .facts, master)
            return std::make_unique<WriteStreamKafkaDefault>(
                    symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance);
        } else if (ioDirectives.get("kafka") == "null-payload") {
            // if (output-dir, .facts, slave) or (output-dir, .csv, slave)
            return std::make_unique<WriteStreamKafkaNullPayload>(
                    symbolMask, symbolTable, ioDirectives, numberOfHeights, provenance);
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
