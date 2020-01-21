/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2017, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file RamProgram.h
 *
 * Defines a Program of a relational algebra query
 *
 ***********************************************************************/

#pragma once

#include "RamStatement.h"
#include <map>
#include <memory>
#include <vector>

namespace souffle {

/**
 * @class RamProgram
 * @brief RAM program relation declaration and functions
 *
 * A typical example:
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * PROGRAM
 *   DECLARATION
 *     A(x:i:number)
 *   END DECLARATION
 *   BEGIN MAIN
 *     ...
 *   END MAIN
 * END PROGRAM
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
class RamProgram : public RamNode {
private:
    RamProgram() = default;

public:
    RamProgram(std::vector<std::unique_ptr<RamRelation>> rels, std::unique_ptr<RamStatement> main,
            std::map<std::string, std::unique_ptr<RamStatement>> subs)
            : relations(std::move(rels)), main(std::move(main)), subroutines(std::move(subs)) {
        assert(this->main != nullptr && "Main program is a null-pointer");
        for (const auto& rel : relations) {
            assert(rel != nullptr && "Relation is a null-pointer");
        }
        for (const auto& sub : subroutines) {
            assert(sub.second != nullptr && "Subroutine is a null-pointer");
        }
    }

    std::vector<const RamNode*> getChildNodes() const override {
        std::vector<const RamNode*> children;
        children = main->getChildNodes();
        for (auto& rel : relations) {
            children.push_back(rel.get());
        }
        for (auto& sub : subroutines) {
            children.push_back(sub.second.get());
        }
        return children;
    }

    void print(std::ostream& out) const override {
        out << "PROGRAM" << std::endl;
        out << " DECLARATION" << std::endl;
        for (const auto& rel : relations) {
            out << "  " << *rel << std::endl;
        }
        out << " END DECLARATION" << std::endl;
        for (const auto& sub : subroutines) {
            out << " SUBROUTINE " << sub.first << std::endl;
            sub.second->print(out, 2);
            out << " END SUBROUTINE" << std::endl;
        }
        out << " BEGIN MAIN" << std::endl;
        main->print(out, 2);
        out << " END MAIN" << std::endl;
        out << "END PROGRAM" << std::endl;
    }

    /** @brief Get main program */
    RamStatement& getMain() const {
        return *main.get();
    }

    /** @brief Get all relations of RAM program  */
    std::vector<RamRelation*> getRelations() const {
        return toPtrVector(relations);
    }

    /** @brief Get all subroutines of a RAM program */
    const std::map<std::string, RamStatement*> getSubroutines() const {
        std::map<std::string, RamStatement*> subroutineRefs;
        for (auto& sub : subroutines) {
            subroutineRefs.insert({sub.first, sub.second.get()});
        }
        return subroutineRefs;
    }

    /** @brief Get a specific subroutine */
    const RamStatement& getSubroutine(const std::string& name) const {
        return *subroutines.at(name);
    }

    RamProgram* clone() const override {
        auto* res = new RamProgram();
        res->main = std::unique_ptr<RamStatement>(main->clone());
        for (auto& rel : relations) {
            res->relations.push_back(std::unique_ptr<RamRelation>(rel->clone()));
        }
        for (auto& sub : subroutines) {
            res->subroutines[sub.first] = std::unique_ptr<RamStatement>(sub.second->clone());
        }
        std::map<const RamRelation*, const RamRelation*> refMap;
        res->apply(makeLambdaRamMapper([&](std::unique_ptr<RamNode> node) -> std::unique_ptr<RamNode> {
            // rewire relation references to newly cloned relations
            if (const RamRelationReference* relRef = dynamic_cast<RamRelationReference*>(node.get())) {
                const RamRelation* rel = refMap[relRef->get()];
                assert(rel != nullptr && "dangling RAM relation reference");
                return std::make_unique<RamRelationReference>(rel);
            } else {
                return node;
            }
        }));
        return res;
    }

    void apply(const RamNodeMapper& map) override {
        main = map(std::move(main));
        assert(main != nullptr && "Main program is a null-pointer");
        for (auto& rel : relations) {
            rel = map(std::move(rel));
            assert(rel != nullptr && "Relation is a null-pointer");
        }
        for (auto& sub : subroutines) {
            sub.second = map(std::move(sub.second));
            assert(sub.second != nullptr && "Subroutine is a null-pointer");
        }
    }

protected:
    bool equal(const RamNode& node) const override {
        assert(nullptr != dynamic_cast<const RamProgram*>(&node));
        const auto& other = static_cast<const RamProgram&>(node);
        if (relations.size() != other.relations.size() || subroutines.size() != other.subroutines.size()) {
            return false;
        }
        for (auto& sub : subroutines) {
            if (other.getSubroutine(sub.first) != getSubroutine(sub.first)) {
                return false;
            }
        }
        return getMain() == other.getMain() && equal_targets(relations, other.relations);
    }

protected:
    /** Relations of RAM program */
    std::vector<std::unique_ptr<RamRelation>> relations;

    /** Main program */
    std::unique_ptr<RamStatement> main;

    /** Subroutines for provenance system */
    std::map<std::string, std::unique_ptr<RamStatement>> subroutines;
};

}  // end of namespace souffle
