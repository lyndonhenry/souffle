/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2015, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file AstUtils.cpp
 *
 * A collection of utilities operating on AST constructs.
 *
 ***********************************************************************/

#include "AstUtils.h"
#include "AstArgument.h"
#include "AstClause.h"
#include "AstLiteral.h"
#include "AstProgram.h"
#include "AstRelation.h"
#include "AstVisitor.h"

namespace souffle {

std::vector<const AstVariable*> getVariables(const AstNode& root) {
    // simply collect the list of all variables by visiting all variables
    std::vector<const AstVariable*> vars;
    visitDepthFirst(root, [&](const AstVariable& var) { vars.push_back(&var); });
    return vars;
}

std::vector<const AstVariable*> getVariables(const AstNode* root) {
    return getVariables(*root);
}

std::vector<const AstRecordInit*> getRecords(const AstNode& root) {
    // simply collect the list of all records by visiting all records
    std::vector<const AstRecordInit*> recs;
    visitDepthFirst(root, [&](const AstRecordInit& rec) { recs.push_back(&rec); });
    return recs;
}

std::vector<const AstRecordInit*> getRecords(const AstNode* root) {
    return getRecords(*root);
}

const AstRelation* getAtomRelation(const AstAtom* atom, const AstProgram* program) {
    return program->getRelation(atom->getName());
}

const size_t getAuxiliaryArity(const AstAtom* atom, const AstProgram* program) {
    if (atom->getName().getName().rfind("@delta_") == 0) {
        const AstRelationIdentifier& originalRel = AstRelationIdentifier(atom->getName().getName().substr(7));
        return program->getRelation(originalRel)->getAuxiliaryArity();
    } else if (atom->getName().getName().rfind("@new_") == 0) {
        const AstRelationIdentifier& originalRel = AstRelationIdentifier(atom->getName().getName().substr(5));
        return program->getRelation(originalRel)->getAuxiliaryArity();
    } else if (atom->getName().getName().rfind("@info_") == 0) {
        return 0;
    }
    return program->getRelation(atom->getName())->getAuxiliaryArity();
}

const AstRelation* getHeadRelation(const AstClause* clause, const AstProgram* program) {
    return getAtomRelation(clause->getHead(), program);
}

std::set<const AstRelation*> getBodyRelations(const AstClause* clause, const AstProgram* program) {
    std::set<const AstRelation*> bodyRelations;
    for (const auto& lit : clause->getBodyLiterals()) {
        visitDepthFirst(
                *lit, [&](const AstAtom& atom) { bodyRelations.insert(getAtomRelation(&atom, program)); });
    }
    for (const auto& arg : clause->getHead()->getArguments()) {
        visitDepthFirst(
                *arg, [&](const AstAtom& atom) { bodyRelations.insert(getAtomRelation(&atom, program)); });
    }
    return bodyRelations;
}

bool hasClauseWithNegatedRelation(const AstRelation* relation, const AstRelation* negRelation,
        const AstProgram* program, const AstLiteral*& foundLiteral) {
    for (const AstClause* cl : relation->getClauses()) {
        for (const AstNegation* neg : cl->getNegations()) {
            if (negRelation == getAtomRelation(neg->getAtom(), program)) {
                foundLiteral = neg;
                return true;
            }
        }
    }
    return false;
}

bool hasClauseWithAggregatedRelation(const AstRelation* relation, const AstRelation* aggRelation,
        const AstProgram* program, const AstLiteral*& foundLiteral) {
    for (const AstClause* cl : relation->getClauses()) {
        bool hasAgg = false;
        visitDepthFirst(*cl, [&](const AstAggregator& cur) {
            visitDepthFirst(cur, [&](const AstAtom& atom) {
                if (aggRelation == getAtomRelation(&atom, program)) {
                    foundLiteral = &atom;
                    hasAgg = true;
                }
            });
        });
        if (hasAgg) {
            return true;
        }
    }
    return false;
}

bool isRecursiveClause(const AstClause& clause) {
    AstRelationIdentifier relationName = clause.getHead()->getName();
    bool recursive = false;
    visitDepthFirst(clause.getBodyLiterals(), [&](const AstAtom& atom) {
        if (atom.getName() == relationName) {
            recursive = true;
        }
    });
    return recursive;
}

std::set<const AstRelation*> getNegatedRelationsInClause(const AstClause* clause, const AstProgram* program) {
    std::set<const AstRelation*> negatedRelationsInClause;
    for (const AstNegation* negation : clause->getNegations()) {
        negatedRelationsInClause.insert(getAtomRelation(negation->getAtom(), program));
    }
    return negatedRelationsInClause;
}

std::set<const AstRelation*> getNegatedRelationsInClausesOfRelation(
        const AstRelation* relation, const AstProgram* program) {
    std::set<const AstRelation*> negatedRelationsInClausesOfRelation;
    for (const AstClause* clause : relation->getClauses()) {
        const auto negatedRelations = getNegatedRelationsInClause(clause, program);
        negatedRelationsInClausesOfRelation.insert(negatedRelations.begin(), negatedRelations.end());
    }
    return negatedRelationsInClausesOfRelation;
}

std::set<const AstRelation*> getAggregatedRelationsInClause(
        const AstClause* clause, const AstProgram* program) {
    std::set<const AstRelation*> aggregatedRelationsInClause;
    visitDepthFirst(*clause, [&](const AstAggregator& current) {
        visitDepthFirst(current, [&](const AstAtom& atom) {
            aggregatedRelationsInClause.insert(getAtomRelation(&atom, program));
        });
    });
    return aggregatedRelationsInClause;
}

std::set<const AstRelation*> getAggregatedRelationsInClausesOfRelation(
        const AstRelation* relation, const AstProgram* program) {
    std::set<const AstRelation*> aggregatedRelationsInClausesOfRelation;
    for (const AstClause* clause : relation->getClauses()) {
        const auto aggregatedRelations = getAggregatedRelationsInClause(clause, program);
        aggregatedRelationsInClausesOfRelation.insert(aggregatedRelations.begin(), aggregatedRelations.end());
    }
    return aggregatedRelationsInClausesOfRelation;
}

std::set<const AstRelation*> getAggregatedAndNegatedRelationsInClausesOfRelation(
        const AstRelation* relation, const AstProgram* program) {
    std::set<const AstRelation*> aggregatedAndNegatedRelationsInClausesOfRelation;
    {
        const auto aggregatedRelationsInClausesOfRelation =
                getAggregatedRelationsInClausesOfRelation(relation, program);
        aggregatedAndNegatedRelationsInClausesOfRelation.insert(
                aggregatedRelationsInClausesOfRelation.begin(), aggregatedRelationsInClausesOfRelation.end());
    }
    {
        const auto negatedRelationsInClausesOfRelation =
                getNegatedRelationsInClausesOfRelation(relation, program);
        aggregatedAndNegatedRelationsInClausesOfRelation.insert(
                negatedRelationsInClausesOfRelation.begin(), negatedRelationsInClausesOfRelation.end());
    }
    return aggregatedAndNegatedRelationsInClausesOfRelation;
}

}  // end of namespace souffle
