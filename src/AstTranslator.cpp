/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, 2015, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file AstTranslator.cpp
 *
 * Translator from AST to RAM structures.
 *
 ***********************************************************************/

#include "AstTranslator.h"
#include "AstArgument.h"
#include "AstAttribute.h"
#include "AstClause.h"
#include "AstFunctorDeclaration.h"
#include "AstIO.h"
#include "AstLiteral.h"
#include "AstNode.h"
#include "AstProgram.h"
#include "AstRelation.h"
#include "AstTranslationUnit.h"
#include "AstTypeEnvironmentAnalysis.h"
#include "AstUtils.h"
#include "AstVisitor.h"
#include "BinaryConstraintOps.h"
#include "DebugReport.h"
#include "Global.h"
#include "IODirectives.h"
#include "LogStatement.h"
#include "PrecedenceGraph.h"
#include "RamCondition.h"
#include "RamExpression.h"
#include "RamNode.h"
#include "RamOperation.h"
#include "RamProgram.h"
#include "RamRelation.h"
#include "RamStatement.h"
#include "RamTranslationUnit.h"
#include "SrcLocation.h"
#include "TypeSystem.h"
#include "Util.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstddef>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <typeinfo>
#include <utility>
#include <vector>

namespace souffle {

class ErrorReport;
class SymbolTable;

std::unique_ptr<RamTupleElement> AstTranslator::makeRamTupleElement(const Location& loc) {
    return std::make_unique<RamTupleElement>(loc.identifier, loc.element);
}

void AstTranslator::makeRamLoad(std::unique_ptr<RamStatement>& current, std::size_t scc,
        const AstRelation* relation, const std::string& inputDirectory, const std::string& fileExtension,
        const std::string& ioType = Global::config().has("engine") ? Global::config().get("engine") : "file",
        std::unique_ptr<RamRelationReference> ramRelationReference = nullptr) {
    IODirectives ioDirectives;
    ioDirectives.set("location", inputDirectory);
    ioDirectives.set("directory", Global::config().get(inputDirectory));
    ioDirectives.set("extension", fileExtension);
    ioDirectives.setIOType(ioType);
    ioDirectives.set(Global::config().get("engine"), "");
    ioDirectives.set("stratum", (scc == (size_t)-1) ? "master" : "slave");

    std::unique_ptr<RamStatement> statement = std::make_unique<RamLoad>(
            (ramRelationReference == nullptr)
                    ? std::unique_ptr<RamRelationReference>(translateRelation(relation))
                    : std::move(ramRelationReference),
            getInputIODirectives(relation, ioDirectives));

    if (Global::config().has("profile")) {
        const std::string logTimerStatement =
                LogStatement::tRelationLoadTime(toString(relation->getName()), relation->getSrcLoc());
        statement = std::make_unique<RamLogRelationTimer>(std::move(statement), logTimerStatement,
                std::unique_ptr<RamRelationReference>(translateRelation(relation)));
    }
    appendStmt(current, std::move(statement));
}

void AstTranslator::makeRamStore(std::unique_ptr<RamStatement>& current, std::size_t scc,
        const AstRelation* relation, const std::string& outputDirectory, const std::string& fileExtension,
        const std::string& engineDirectives = "default",
        const std::string& ioType = Global::config().has("engine") ? Global::config().get("engine") : "file",
        std::unique_ptr<RamRelationReference> ramRelationReference = nullptr) {
    // @@@TODO (lh): HERE! THE NEXT STEP IS TO MAKE IT SO THAT THERE ARE SEPARATE HALT STATEMENTS FOR
    // PRODUCERS IN THE BLOCKING VARIANT, THEN MOVE TO THE NON-BLOCKING --
    IODirectives ioDirectives;
    ioDirectives.set("location", outputDirectory);
    ioDirectives.set("directory", Global::config().get(outputDirectory));
    ioDirectives.set("extension", fileExtension);
    ioDirectives.setIOType(ioType);

    // @@@TODO (lh): HERE! OK, SO MUST SPLIT INTO PAYLOAD BEARING AND END OF STREAM
    ioDirectives.set(Global::config().get("engine"), engineDirectives);

    ioDirectives.set("stratum", (scc == (size_t)-1) ? "master" : "slave");
// @TODO (lh): ensure that this is done correctly with -a/--async
if (Global::config().has("use-general")) {
    ioDirectives.set("append", "true");
} else {
    ioDirectives.set("append", "false");
}

    std::unique_ptr<RamStatement> statement = std::make_unique<RamStore>(
            (ramRelationReference == nullptr)
                    ? std::unique_ptr<RamRelationReference>(translateRelation(relation))
                    : std::move(ramRelationReference),
            getOutputIODirectives(relation, ioDirectives));

    if (Global::config().has("profile")) {
        const std::string logTimerStatement =
                LogStatement::tRelationSaveTime(toString(relation->getName()), relation->getSrcLoc());
        statement = std::make_unique<RamLogRelationTimer>(std::move(statement), logTimerStatement,
                std::unique_ptr<RamRelationReference>(translateRelation(relation)));
    }
    appendStmt(current, std::move(statement));
}

void AstTranslator::makeIODirective(IODirectives& ioDirective, const AstRelation* rel) {
    // set relation name correctly
    ioDirective.setRelationName(getRelationName(rel->getName()));

    // set a default IO type of file and a default filename if not supplied
    if (!ioDirective.has("IO")) {
        ioDirective.setIOType("file");
    }

    // load intermediate relations from correct files
    if (ioDirective.getIOType() == "file") {
        // set filename by relation if not given, or if relation is intermediate
        if (!ioDirective.has("filename") || ioDirective.get("intermediate") == "true") {
            ioDirective.setFileName(ioDirective.getRelationName() + ioDirective.get("extension"));
        }

        // if filename is not an absolute path, concat with cmd line directory
        if (ioDirective.getIOType() == "file" && ioDirective.getFileName().front() != '/') {
            ioDirective.setFileName(ioDirective.get("directory") + "/" + ioDirective.getFileName());
        }
    }
}

std::vector<IODirectives> AstTranslator::getInputIODirectives(
        const AstRelation* rel, IODirectives& baseIoDirectives) {
    const auto location = baseIoDirectives.get("location");
    const auto filePath = baseIoDirectives.get("directory");
    const auto fileExt = baseIoDirectives.get("extension");
    const auto stratumIndex = baseIoDirectives.get("stratum");
    const auto ioType = baseIoDirectives.getIOType();
    const auto engineConfig = baseIoDirectives.get(Global::config().get("engine"));

    std::vector<IODirectives> inputDirectives;

    for (const auto& current : rel->getLoads()) {
        IODirectives ioDirectives;
        for (const auto& currentPair : current->getIODirectiveMap()) {
            ioDirectives.set(currentPair.first, currentPair.second);
        }
        inputDirectives.push_back(ioDirectives);
    }

    if (inputDirectives.empty()) {
        inputDirectives.emplace_back();
    }

    const std::string inputFilePath = (filePath.empty()) ? Global::config().get("fact-dir") : filePath;
    const std::string inputFileExt = (fileExt.empty()) ? ".facts" : fileExt;

    const bool isIntermediate =
            (Global::config().has("engine") && inputFilePath == Global::config().get("output-dir") &&
                    inputFileExt == ".facts");

    for (auto& ioDirective : inputDirectives) {
        // all intermediate relations are given the default delimiter and have no headers
        if (isIntermediate) {
            ioDirective.set("intermediate", "true");
            ioDirective.set("delimiter", "\t");
            ioDirective.set("headers", "false");
        }
        ioDirective.set("location", location);
        ioDirective.set("directory", inputFilePath);
        ioDirective.set("extension", inputFileExt);
        ioDirective.set("stratum", stratumIndex);
        ioDirective.setIOType(ioType);
        ioDirective.set(Global::config().get("engine"), engineConfig);
        makeIODirective(ioDirective, rel);
    }

    return inputDirectives;
}

std::vector<IODirectives> AstTranslator::getOutputIODirectives(
        const AstRelation* rel, IODirectives& baseIoDirectives) {
    const auto location = baseIoDirectives.get("location");
    const auto filePath = baseIoDirectives.get("directory");
    const auto fileExt = baseIoDirectives.get("extension");
    const auto stratumIndex = baseIoDirectives.get("stratum");
    const auto ioType = baseIoDirectives.getIOType();
    const auto engineConfig = baseIoDirectives.get(Global::config().get("engine"));
    const auto isAppend = baseIoDirectives.get("append");

    std::vector<IODirectives> outputDirectives;

    // If stdout is requested then remove all directives from the datalog file.
    if (Global::config().get("output-dir") == "-") {
        bool hasOutput = false;
        for (const auto* current : rel->getStores()) {
            IODirectives ioDirectives;
            if (dynamic_cast<const AstPrintSize*>(current) != nullptr) {
                ioDirectives.setIOType("stdoutprintsize");
                outputDirectives.push_back(ioDirectives);
            } else if (!hasOutput) {
                hasOutput = true;
                ioDirectives.setIOType("stdout");
                ioDirectives.set("headers", "true");
                outputDirectives.push_back(ioDirectives);
            }
        }
    } else {
        for (const auto* current : rel->getStores()) {
            IODirectives ioDirectives;
            for (const auto& currentPair : current->getIODirectiveMap()) {
                ioDirectives.set(currentPair.first, currentPair.second);
            }
            outputDirectives.push_back(ioDirectives);
        }
    }

    if (outputDirectives.empty()) {
        outputDirectives.emplace_back();
    }

    const std::string outputFilePath = (filePath.empty()) ? Global::config().get("output-dir") : filePath;
    const std::string outputFileExt = (fileExt.empty()) ? ".csv" : fileExt;

    const bool isIntermediate =
            (Global::config().has("engine") && outputFilePath == Global::config().get("output-dir") &&
                    outputFileExt == ".facts");

    for (auto& ioDirective : outputDirectives) {
        // all intermediate relations are given the default delimiter and have no headers
        if (isIntermediate) {
            ioDirective.set("intermediate", "true");
            ioDirective.set("delimiter", "\t");
            ioDirective.set("headers", "false");
        }
        ioDirective.set("location", location);
        ioDirective.set("directory", outputFilePath);
        ioDirective.set("extension", outputFileExt);
        ioDirective.set("stratum", stratumIndex);
        ioDirective.set("append", isAppend);
        ioDirective.setIOType(ioType);
        ioDirective.set(Global::config().get("engine"), engineConfig);
        makeIODirective(ioDirective, rel);

        if (!ioDirective.has("attributeNames")) {
            std::string delimiter("\t");
            if (ioDirective.has("delimiter")) {
                delimiter = ioDirective.get("delimiter");
            }
            std::vector<std::string> attributeNames;
            for (unsigned int i = 0; i < rel->getArity(); i++) {
                attributeNames.push_back(rel->getAttribute(i)->getAttributeName());
            }

            if (Global::config().has("provenance")) {
                std::vector<std::string> originalAttributeNames(
                        attributeNames.begin(), attributeNames.end() - 1 - rel->numberOfHeightParameters());
                ioDirective.set("attributeNames", toString(join(originalAttributeNames, delimiter)));
            } else {
                ioDirective.set("attributeNames", toString(join(attributeNames, delimiter)));
            }
        }
    }

    return outputDirectives;
}

std::unique_ptr<RamRelationReference> AstTranslator::createRelationReference(const std::string name,
        const size_t arity, const size_t numberOfHeights, const std::vector<std::string> attributeNames,
        const std::vector<std::string> attributeTypeQualifiers, const RelationRepresentation representation) {
    auto it = ramRels.find(name);
    if (it == ramRels.end()) {
        ramRels[name] = std::make_unique<RamRelation>(
                name, arity, numberOfHeights, attributeNames, attributeTypeQualifiers, representation);
        it = ramRels.find(name);
    }
    assert(it != ramRels.end() && "relation name not found");

    const RamRelation* relation = it->second.get();
    return std::make_unique<RamRelationReference>(relation);
}

std::unique_ptr<RamRelationReference> AstTranslator::createRelationReference(
        const std::string name, const size_t arity, const size_t numberOfHeights) {
    return createRelationReference(name, arity, numberOfHeights, {}, {}, {});
}

std::unique_ptr<RamRelationReference> AstTranslator::translateRelation(const AstAtom* atom) {
    if (const auto rel = getAtomRelation(atom, program)) {
        return translateRelation(rel);
    } else {
        return createRelationReference(
                getRelationName(atom->getName()), atom->getArity(), getNumberOfHeights(atom, program));
    }
}

std::unique_ptr<RamRelationReference> AstTranslator::translateRelation(
        const AstRelation* rel, const std::string relationNamePrefix) {
    std::vector<std::string> attributeNames;
    std::vector<std::string> attributeTypeQualifiers;
    for (size_t i = 0; i < rel->getArity(); ++i) {
        attributeNames.push_back(rel->getAttribute(i)->getAttributeName());
        if (typeEnv != nullptr) {
            attributeTypeQualifiers.push_back(
                    getTypeQualifier(typeEnv->getType(rel->getAttribute(i)->getTypeName())));
        }
    }

    return createRelationReference(relationNamePrefix + getRelationName(rel->getName()), rel->getArity(),
            rel->numberOfHeightParameters(), attributeNames, attributeTypeQualifiers,
            rel->getRepresentation());
}

std::unique_ptr<RamRelationReference> AstTranslator::translateDeltaRelation(const AstRelation* rel) {
    return translateRelation(rel, "@delta_");
}

std::unique_ptr<RamRelationReference> AstTranslator::translateNewRelation(const AstRelation* rel) {
    return translateRelation(rel, "@new_");
}

std::unique_ptr<RamExpression> AstTranslator::translateValue(
        const AstArgument* arg, const ValueIndex& index) {
    if (arg == nullptr) {
        return nullptr;
    }

    class ValueTranslator : public AstVisitor<std::unique_ptr<RamExpression>> {
        AstTranslator& translator;
        const ValueIndex& index;

    public:
        ValueTranslator(AstTranslator& translator, const ValueIndex& index)
                : translator(translator), index(index) {}

        std::unique_ptr<RamExpression> visitVariable(const AstVariable& var) override {
            assert(index.isDefined(var) && "variable not grounded");
            return makeRamTupleElement(index.getDefinitionPoint(var));
        }

        std::unique_ptr<RamExpression> visitUnnamedVariable(const AstUnnamedVariable& var) override {
            return std::make_unique<RamUndefValue>();
        }

        std::unique_ptr<RamExpression> visitConstant(const AstConstant& c) override {
            return std::make_unique<RamNumber>(c.getIndex());
        }

        std::unique_ptr<RamExpression> visitIntrinsicFunctor(const AstIntrinsicFunctor& inf) override {
            std::vector<std::unique_ptr<RamExpression>> values;
            for (const auto& cur : inf.getArguments()) {
                values.push_back(translator.translateValue(cur, index));
            }
            return std::make_unique<RamIntrinsicOperator>(inf.getFunction(), std::move(values));
        }

        std::unique_ptr<RamExpression> visitUserDefinedFunctor(const AstUserDefinedFunctor& udf) override {
            std::vector<std::unique_ptr<RamExpression>> values;
            for (const auto& cur : udf.getArguments()) {
                values.push_back(translator.translateValue(cur, index));
            }
            const AstFunctorDeclaration* decl = translator.program->getFunctorDeclaration(udf.getName());
            std::string type = decl->getType();
            return std::make_unique<RamUserDefinedOperator>(udf.getName(), type, std::move(values));
        }

        std::unique_ptr<RamExpression> visitCounter(const AstCounter&) override {
            return std::make_unique<RamAutoIncrement>();
        }

        std::unique_ptr<RamExpression> visitRecordInit(const AstRecordInit& init) override {
            std::vector<std::unique_ptr<RamExpression>> values;
            for (const auto& cur : init.getArguments()) {
                values.push_back(translator.translateValue(cur, index));
            }
            return std::make_unique<RamPackRecord>(std::move(values));
        }

        std::unique_ptr<RamExpression> visitAggregator(const AstAggregator& agg) override {
            // here we look up the location the aggregation result gets bound
            return translator.makeRamTupleElement(index.getAggregatorLocation(agg));
        }

        std::unique_ptr<RamExpression> visitSubroutineArgument(const AstSubroutineArgument& subArg) override {
            return std::make_unique<RamSubroutineArgument>(subArg.getNumber());
        }
    };

    return ValueTranslator(*this, index)(*arg);
}

std::unique_ptr<RamCondition> AstTranslator::translateConstraint(
        const AstLiteral* lit, const ValueIndex& index) {
    class ConstraintTranslator : public AstVisitor<std::unique_ptr<RamCondition>> {
        AstTranslator& translator;
        const ValueIndex& index;

    public:
        ConstraintTranslator(AstTranslator& translator, const ValueIndex& index)
                : translator(translator), index(index) {}

        /** for atoms */
        std::unique_ptr<RamCondition> visitAtom(const AstAtom&) override {
            return nullptr;  // covered already within the scan/lookup generation step
        }

        /** for binary relations */
        std::unique_ptr<RamCondition> visitBinaryConstraint(const AstBinaryConstraint& binRel) override {
            std::unique_ptr<RamExpression> valLHS = translator.translateValue(binRel.getLHS(), index);
            std::unique_ptr<RamExpression> valRHS = translator.translateValue(binRel.getRHS(), index);
            return std::make_unique<RamConstraint>(binRel.getOperator(),
                    translator.translateValue(binRel.getLHS(), index),
                    translator.translateValue(binRel.getRHS(), index));
        }

        /** for negations */
        std::unique_ptr<RamCondition> visitNegation(const AstNegation& neg) override {
            // get contained atom
            const auto* atom = neg.getAtom();
            auto arity = atom->getArity();
            auto numberOfHeightParameters = getNumberOfHeights(atom, translator.program);

            // account for extra provenance columns
            if (Global::config().has("provenance")) {
                // rule number
                arity -= 1;
                // height parameters
                arity -= numberOfHeightParameters;
            }

            std::vector<std::unique_ptr<RamExpression>> values;

            for (size_t i = 0; i < arity; i++) {
                values.push_back(translator.translateValue(atom->getArgument(i), index));
            }

            // we don't care about the provenance columns when doing the existence check
            if (Global::config().has("provenance")) {
                values.push_back(std::make_unique<RamUndefValue>());
                for (size_t h = 0; h < numberOfHeightParameters; h++) {
                    values.push_back(std::make_unique<RamUndefValue>());
                }
            }

            // add constraint
            if (arity > 0) {
                return std::make_unique<RamNegation>(std::make_unique<RamExistenceCheck>(
                        translator.translateRelation(atom), std::move(values)));
            } else {
                return std::make_unique<RamEmptinessCheck>(translator.translateRelation(atom));
            }
        }

        /** for provenance negation */
        std::unique_ptr<RamCondition> visitProvenanceNegation(const AstProvenanceNegation& neg) override {
            // get contained atom
            const AstAtom* atom = neg.getAtom();
            auto arity = atom->getArity();
            auto numberOfHeightParameters = getNumberOfHeights(atom, translator.program);

            // account for extra provenance columns
            if (Global::config().has("provenance")) {
                // rule number
                arity -= 1;
                // level numbers
                arity -= numberOfHeightParameters;
            }

            std::vector<std::unique_ptr<RamExpression>> values;

            for (size_t i = 0; i < arity; i++) {
                const auto& arg = atom->getArgument(i);
                // for (const auto& arg : atom->getArguments()) {
                values.push_back(translator.translateValue(arg, index));
            }

            // we don't care about the provenance columns when doing the existence check
            if (Global::config().has("provenance")) {
                values.push_back(std::make_unique<RamUndefValue>());
                // add the height annotation for provenanceNotExists
                for (size_t h = 0; h < numberOfHeightParameters; h++) {
                    values.push_back(translator.translateValue(atom->getArgument(arity + h + 1), index));
                }
            }

            // add constraint
            return std::make_unique<RamNegation>(std::make_unique<RamProvenanceExistenceCheck>(
                    translator.translateRelation(atom), std::move(values)));
        }
    };

    return ConstraintTranslator(*this, index)(*lit);
}

std::unique_ptr<AstClause> AstTranslator::ClauseTranslator::getReorderedClause(
        const AstClause& clause, const int version) const {
    const auto plan = clause.getExecutionPlan();

    // check whether there is an imposed order constraint
    if (plan != nullptr && plan->hasOrderFor(version)) {
        // get the imposed order
        const auto& order = plan->getOrderFor(version);

        // create a copy and fix order
        std::unique_ptr<AstClause> reorderedClause(clause.clone());

        // Change order to start at zero
        std::vector<unsigned int> newOrder(order.size());
        std::transform(order.begin(), order.end(), newOrder.begin(),
                [](unsigned int i) -> unsigned int { return i - 1; });

        // re-order atoms
        reorderedClause->reorderAtoms(newOrder);

        // clear other order and fix plan
        reorderedClause->clearExecutionPlan();
        reorderedClause->setFixedExecutionPlan();

        return reorderedClause;
    }

    return nullptr;
}

AstTranslator::ClauseTranslator::arg_list* AstTranslator::ClauseTranslator::getArgList(
        const AstNode* curNode, std::map<const AstNode*, std::unique_ptr<arg_list>>& nodeArgs) const {
    if (nodeArgs.count(curNode) == 0u) {
        if (auto rec = dynamic_cast<const AstRecordInit*>(curNode)) {
            nodeArgs[curNode] = std::make_unique<arg_list>(rec->getArguments());
        } else if (auto atom = dynamic_cast<const AstAtom*>(curNode)) {
            nodeArgs[curNode] = std::make_unique<arg_list>(atom->getArguments());
        } else {
            assert(false && "node type doesn't have arguments!");
        }
    }
    return nodeArgs[curNode].get();
}

void AstTranslator::ClauseTranslator::indexValues(const AstNode* curNode,
        std::map<const AstNode*, std::unique_ptr<arg_list>>& nodeArgs,
        std::map<const arg_list*, int>& arg_level, RamRelationReference* relation) {
    arg_list* cur = getArgList(curNode, nodeArgs);
    for (size_t pos = 0; pos < cur->size(); ++pos) {
        // get argument
        auto& arg = (*cur)[pos];

        // check for variable references
        if (auto var = dynamic_cast<const AstVariable*>(arg)) {
            if (pos < relation->get()->getArity()) {
                valueIndex.addVarReference(
                        *var, arg_level[cur], pos, std::unique_ptr<RamRelationReference>(relation->clone()));
            } else {
                valueIndex.addVarReference(*var, arg_level[cur], pos);
            }
        }

        // check for nested records
        if (auto rec = dynamic_cast<const AstRecordInit*>(arg)) {
            // introduce new nesting level for unpack
            op_nesting.push_back(rec);
            arg_level[getArgList(rec, nodeArgs)] = level++;

            // register location of record
            valueIndex.setRecordDefinition(*rec, arg_level[cur], pos);

            // resolve nested components
            indexValues(rec, nodeArgs, arg_level, relation);
        }
    }
}

/** index values in rule */
void AstTranslator::ClauseTranslator::createValueIndex(const AstClause& clause) {
    for (const AstAtom* atom : clause.getAtoms()) {
        // std::map<const arg_list*, int> arg_level;
        std::map<const AstNode*, std::unique_ptr<arg_list>> nodeArgs;

        std::map<const arg_list*, int> arg_level;
        nodeArgs[atom] = std::make_unique<arg_list>(atom->getArguments());
        // the atom is obtained at the current level
        // increment nesting level for the atom
        arg_level[nodeArgs[atom].get()] = level++;
        op_nesting.push_back(atom);

        indexValues(atom, nodeArgs, arg_level, translator.translateRelation(atom).get());
    }

    // add aggregation functions
    visitDepthFirstPostOrder(clause, [&](const AstAggregator& cur) {
        // add each aggregator expression only once
        if (any_of(aggregators, [&](const AstAggregator* agg) { return *agg == cur; })) {
            return;
        }

        int aggLoc = level++;
        valueIndex.setAggregatorLocation(cur, Location({aggLoc, 0}));

        // bind aggregator variables to locations
        assert(nullptr != dynamic_cast<const AstAtom*>(cur.getBodyLiterals()[0]));
        const AstAtom& atom = static_cast<const AstAtom&>(*cur.getBodyLiterals()[0]);
        for (size_t pos = 0; pos < atom.getArguments().size(); ++pos) {
            if (const auto* var = dynamic_cast<const AstVariable*>(atom.getArgument(pos))) {
                valueIndex.addVarReference(*var, aggLoc, (int)pos, translator.translateRelation(&atom));
            }
        };

        // and remember aggregator
        aggregators.push_back(&cur);
    });
}

std::unique_ptr<RamOperation> AstTranslator::ClauseTranslator::createOperation(const AstClause& clause) {
    const auto head = clause.getHead();

    std::vector<std::unique_ptr<RamExpression>> values;
    for (AstArgument* arg : head->getArguments()) {
        values.push_back(translator.translateValue(arg, valueIndex));
    }

    std::unique_ptr<RamOperation> project =
            std::make_unique<RamProject>(translator.translateRelation(head), std::move(values));

    if (head->getArity() == 0) {
        project = std::make_unique<RamFilter>(
                std::make_unique<RamEmptinessCheck>(translator.translateRelation(head)), std::move(project));
    }

    // check existence for original tuple if we have provenance
    // only if we don't compile
    if (Global::config().has("provenance") &&
            ((!Global::config().has("compile") && !Global::config().has("dl-program") &&
                    !Global::config().has("generate")))) {
        size_t numberOfHeights = getNumberOfHeights(head, translator.program);

        auto arity = head->getArity() - 1 - numberOfHeights;

        std::vector<std::unique_ptr<RamExpression>> values;

        bool isVolatile = true;
        // add args for original tuple
        for (size_t i = 0; i < arity; i++) {
            auto arg = head->getArgument(i);

            // don't add counters
            visitDepthFirst(*arg, [&](const AstCounter& cur) { isVolatile = false; });
            values.push_back(translator.translateValue(arg, valueIndex));
        }

        // add unnamed args for provenance columns
        values.push_back(std::make_unique<RamUndefValue>());
        for (size_t h = 0; h < numberOfHeights; h++) {
            values.push_back(std::make_unique<RamUndefValue>());
        }

        if (isVolatile) {
            return std::make_unique<RamFilter>(
                    std::make_unique<RamNegation>(std::make_unique<RamExistenceCheck>(
                            translator.translateRelation(head), std::move(values))),
                    std::move(project));
        }
    }

    // build up insertion call
    return project;  // start with innermost
}

std::unique_ptr<RamOperation> AstTranslator::ProvenanceClauseTranslator::createOperation(
        const AstClause& clause) {
    std::vector<std::unique_ptr<RamExpression>> values;

    // get all values in the body
    for (AstLiteral* lit : clause.getBodyLiterals()) {
        if (auto atom = dynamic_cast<AstAtom*>(lit)) {
            for (AstArgument* arg : atom->getArguments()) {
                values.push_back(translator.translateValue(arg, valueIndex));
            }
        } else if (auto neg = dynamic_cast<AstNegation*>(lit)) {
            for (AstArgument* arg : neg->getAtom()->getArguments()) {
                values.push_back(translator.translateValue(arg, valueIndex));
            }
        } else if (auto con = dynamic_cast<AstBinaryConstraint*>(lit)) {
            values.push_back(translator.translateValue(con->getLHS(), valueIndex));
            values.push_back(translator.translateValue(con->getRHS(), valueIndex));
        } else if (auto neg = dynamic_cast<AstProvenanceNegation*>(lit)) {
            size_t numberOfHeights = getNumberOfHeights(neg->getAtom(), translator.program);

            // non provenance arguments
            for (size_t i = 0; i < neg->getAtom()->getArguments().size() - 1 - numberOfHeights; ++i) {
                auto arg = neg->getAtom()->getArguments()[i];
                values.push_back(translator.translateValue(arg, valueIndex));
            }

            // provenance annotation arguments
            for (size_t i = 0; i < numberOfHeights + 1; ++i) {
                values.push_back(std::make_unique<RamNumber>(-1));
            }
        }
    }

    return std::make_unique<RamSubroutineReturnValue>(std::move(values));
}

std::unique_ptr<RamCondition> AstTranslator::ClauseTranslator::createCondition(
        const AstClause& originalClause) {
    const auto head = originalClause.getHead();

    // add stopping criteria for nullary relations
    // (if it contains already the null tuple, don't re-compute)
    if (head->getArity() == 0) {
        return std::make_unique<RamEmptinessCheck>(translator.translateRelation(head));
    }
    return nullptr;
}

std::unique_ptr<RamCondition> AstTranslator::ProvenanceClauseTranslator::createCondition(
        const AstClause& originalClause) {
    return nullptr;
}

/** generate RAM code for a clause */
std::unique_ptr<RamStatement> AstTranslator::ClauseTranslator::translateClause(
        const AstClause& clause, const AstClause& originalClause, const int version) {
    if (auto reorderedClause = getReorderedClause(clause, version)) {
        // translate reordered clause
        return translateClause(*reorderedClause, originalClause, version);
    }

    // get extract some details
    const AstAtom* head = clause.getHead();

    // handle facts
    if (clause.isFact()) {
        // translate arguments
        std::vector<std::unique_ptr<RamExpression>> values;
        for (auto& arg : head->getArguments()) {
            values.push_back(translator.translateValue(arg, ValueIndex()));
        }

        // create a fact statement
        return std::make_unique<RamQuery>(
                std::make_unique<RamProject>(translator.translateRelation(head), std::move(values)));
    }

    // the rest should be rules
    assert(clause.isRule());

    createValueIndex(clause);

    // -- create RAM statement --

    std::unique_ptr<RamOperation> op = createOperation(clause);

    /* add equivalence constraints imposed by variable binding */
    for (const auto& cur : valueIndex.getVariableReferences()) {
        // the first appearance
        const Location& first = *cur.second.begin();
        // all other appearances
        for (const Location& loc : cur.second) {
            if (first != loc && !valueIndex.isAggregator(loc.identifier)) {
                op = std::make_unique<RamFilter>(
                        std::make_unique<RamConstraint>(
                                BinaryConstraintOp::EQ, makeRamTupleElement(first), makeRamTupleElement(loc)),
                        std::move(op));
            }
        }
    }

    /* add conditions caused by atoms, negations, and binary relations */
    for (const auto& lit : clause.getBodyLiterals()) {
        if (auto condition = translator.translateConstraint(lit, valueIndex)) {
            op = std::make_unique<RamFilter>(std::move(condition), std::move(op));
        }
    }

    // add aggregator conditions
    size_t curLevel = op_nesting.size() - 1;
    for (auto it = op_nesting.rbegin(); it != op_nesting.rend(); ++it, --curLevel) {
        const AstNode* cur = *it;

        if (const auto* atom = dynamic_cast<const AstAtom*>(cur)) {
            // add constraints
            for (size_t pos = 0; pos < atom->argSize(); ++pos) {
                if (auto* agg = dynamic_cast<AstAggregator*>(atom->getArgument(pos))) {
                    auto loc = valueIndex.getAggregatorLocation(*agg);
                    op = std::make_unique<RamFilter>(std::make_unique<RamConstraint>(BinaryConstraintOp::EQ,
                                                             std::make_unique<RamTupleElement>(curLevel, pos),
                                                             makeRamTupleElement(loc)),
                            std::move(op));
                }
            }
        }
    }

    // add aggregator levels
    --level;
    for (auto it = aggregators.rbegin(); it != aggregators.rend(); ++it, --level) {
        const AstAggregator* cur = *it;

        // translate aggregation function
        AggregateFunction fun = souffle::MIN;
        switch (cur->getOperator()) {
            case AstAggregator::min:
                fun = souffle::MIN;
                break;
            case AstAggregator::max:
                fun = souffle::MAX;
                break;
            case AstAggregator::count:
                fun = souffle::COUNT;
                break;
            case AstAggregator::sum:
                fun = souffle::SUM;
                break;
        }

        // condition for aggregate and helper function to add terms
        std::unique_ptr<RamCondition> aggCondition;
        auto addAggCondition = [&](std::unique_ptr<RamCondition>& arg) {
            if (aggCondition == nullptr) {
                aggCondition = std::move(arg);
            } else {
                aggCondition = std::make_unique<RamConjunction>(std::move(aggCondition), std::move(arg));
            }
        };

        // translate constraints of sub-clause
        for (const auto& lit : cur->getBodyLiterals()) {
            if (auto newCondition = translator.translateConstraint(lit, valueIndex)) {
                addAggCondition(newCondition);
            }
        }

        // get the first predicate of the sub-clause
        // NB: at most one atom is permitted in a sub-clause
        const AstAtom* atom = nullptr;
        for (const auto& lit : cur->getBodyLiterals()) {
            if (atom == nullptr) {
                atom = dynamic_cast<const AstAtom*>(lit);
            } else {
                assert(dynamic_cast<const AstAtom*>(lit) != nullptr &&
                        "Unsupported complex aggregation body encountered!");
            }
        }

        // translate arguments's of atom (if exists) to conditions
        if (atom != nullptr) {
            for (size_t pos = 0; pos < atom->argSize(); ++pos) {
                // variable bindings are issued differently since we don't want self
                // referential variable bindings
                if (const auto* var = dynamic_cast<const AstVariable*>(atom->getArgument(pos))) {
                    for (const Location& loc :
                            valueIndex.getVariableReferences().find(var->getName())->second) {
                        if (level != loc.identifier || (int)pos != loc.element) {
                            std::unique_ptr<RamCondition> newCondition = std::make_unique<RamConstraint>(
                                    BinaryConstraintOp::EQ, makeRamTupleElement(loc),
                                    std::make_unique<RamTupleElement>(level, pos));
                            addAggCondition(newCondition);
                            break;
                        }
                    }
                } else if (atom->getArgument(pos) != nullptr) {
                    std::unique_ptr<RamExpression> value =
                            translator.translateValue(atom->getArgument(pos), valueIndex);
                    if (value != nullptr && !isRamUndefValue(value.get())) {
                        std::unique_ptr<RamCondition> newCondition =
                                std::make_unique<RamConstraint>(BinaryConstraintOp::EQ,
                                        std::make_unique<RamTupleElement>(level, pos), std::move(value));
                        addAggCondition(newCondition);
                    }
                }
            }
        }

        // translate aggregate expression
        std::unique_ptr<RamExpression> expr =
                translator.translateValue(cur->getTargetExpression(), valueIndex);
        if (expr == nullptr) {
            expr = std::make_unique<RamUndefValue>();
        }

        if (aggCondition == nullptr) {
            aggCondition = std::make_unique<RamTrue>();
        }

        // add Ram-Aggregation layer
        std::unique_ptr<RamAggregate> aggregate = std::make_unique<RamAggregate>(std::move(op), fun,
                translator.translateRelation(atom), std::move(expr), std::move(aggCondition), level);
        op = std::move(aggregate);
    }

    // build operation bottom-up
    while (!op_nesting.empty()) {
        // get next operator
        const AstNode* cur = op_nesting.back();
        op_nesting.pop_back();

        // get current nesting level
        auto level = op_nesting.size();

        if (const auto* atom = dynamic_cast<const AstAtom*>(cur)) {
            // add constraints
            for (size_t pos = 0; pos < atom->argSize(); ++pos) {
                if (auto* c = dynamic_cast<AstConstant*>(atom->getArgument(pos))) {
                    op = std::make_unique<RamFilter>(std::make_unique<RamConstraint>(BinaryConstraintOp::EQ,
                                                             std::make_unique<RamTupleElement>(level, pos),
                                                             std::make_unique<RamNumber>(c->getIndex())),
                            std::move(op));
                }
            }

            // check whether all arguments are unnamed variables
            bool isAllArgsUnnamed = true;
            for (auto* argument : atom->getArguments()) {
                if (dynamic_cast<AstUnnamedVariable*>(argument) == nullptr) {
                    isAllArgsUnnamed = false;
                }
            }

            // add check for emptiness for an atom
            op = std::make_unique<RamFilter>(
                    std::make_unique<RamNegation>(
                            std::make_unique<RamEmptinessCheck>(translator.translateRelation(atom))),
                    std::move(op));

            // add a scan level
            if (atom->getArity() != 0 && !isAllArgsUnnamed) {
                if (head->getArity() == 0) {
                    op = std::make_unique<RamBreak>(
                            std::make_unique<RamNegation>(
                                    std::make_unique<RamEmptinessCheck>(translator.translateRelation(head))),
                            std::move(op));
                }
                if (Global::config().has("profile")) {
                    std::stringstream ss;
                    ss << head->getName();
                    ss.str("");
                    ss << "@frequency-atom" << ';';
                    ss << originalClause.getHead()->getName() << ';';
                    ss << version << ';';
                    ss << stringify(toString(clause)) << ';';
                    ss << stringify(toString(*atom)) << ';';
                    ss << stringify(toString(originalClause)) << ';';
                    ss << level << ';';
                    op = std::make_unique<RamScan>(
                            translator.translateRelation(atom), level, std::move(op), ss.str());
                } else {
                    op = std::make_unique<RamScan>(translator.translateRelation(atom), level, std::move(op));
                }
            }

            // TODO: support constants in nested records!
        } else if (const auto* rec = dynamic_cast<const AstRecordInit*>(cur)) {
            // add constant constraints
            for (size_t pos = 0; pos < rec->getArguments().size(); ++pos) {
                if (AstConstant* c = dynamic_cast<AstConstant*>(rec->getArguments()[pos])) {
                    op = std::make_unique<RamFilter>(std::make_unique<RamConstraint>(BinaryConstraintOp::EQ,
                                                             std::make_unique<RamTupleElement>(level, pos),
                                                             std::make_unique<RamNumber>(c->getIndex())),
                            std::move(op));
                } else if (AstFunctor* func = dynamic_cast<AstFunctor*>(rec->getArguments()[pos])) {
                    op = std::make_unique<RamFilter>(std::make_unique<RamConstraint>(BinaryConstraintOp::EQ,
                                                             std::make_unique<RamTupleElement>(level, pos),
                                                             translator.translateValue(func, valueIndex)),
                            std::move(op));
                }
            }

            // add an unpack level
            const Location& loc = valueIndex.getDefinitionPoint(*rec);
            op = std::make_unique<RamUnpackRecord>(
                    std::move(op), level, makeRamTupleElement(loc), rec->getArguments().size());
        } else {
            assert(false && "Unsupported AST node for creation of scan-level!");
        }
    }

    /* generate the final RAM Insert statement */
    std::unique_ptr<RamCondition> cond = createCondition(originalClause);
    if (cond != nullptr) {
        return std::make_unique<RamQuery>(std::make_unique<RamFilter>(std::move(cond), std::move(op)));
    } else {
        return std::make_unique<RamQuery>(std::move(op));
    }
}

/* utility for appending statements */
void AstTranslator::appendStmt(std::unique_ptr<RamStatement>& stmtList, std::unique_ptr<RamStatement> stmt) {
    if (stmt) {
        if (stmtList) {
            RamSequence* stmtSeq;
            if ((stmtSeq = dynamic_cast<RamSequence*>(stmtList.get())) != nullptr) {
                stmtSeq->add(std::move(stmt));
            } else {
                stmtList = std::make_unique<RamSequence>(std::move(stmtList), std::move(stmt));
            }
        } else {
            stmtList = std::move(stmt);
        }
    }
}

/** generate RAM code for a non-recursive relation */
std::unique_ptr<RamStatement> AstTranslator::translateNonRecursiveRelation(
        const AstTranslationUnit& translationUnit, const AstRelation& rel) {
    const auto* recursiveClauses = translationUnit.getAnalysis<RecursiveClauses>();

    /* start with an empty sequence */
    std::unique_ptr<RamStatement> res;

    // the ram table reference
    std::unique_ptr<RamRelationReference> rrel = translateRelation(&rel);

    /* iterate over all clauses that belong to the relation */
    for (AstClause* clause : rel.getClauses()) {
        // skip recursive rules
        if (recursiveClauses->recursive(clause)) {
            continue;
        }

        // translate clause
        std::unique_ptr<RamStatement> rule = ClauseTranslator(*this).translateClause(*clause, *clause);

        // add logging
        if (Global::config().has("profile")) {
            const std::string& relationName = toString(rel.getName());
            const SrcLocation& srcLocation = clause->getSrcLoc();
            const std::string clauseText = stringify(toString(*clause));
            const std::string logTimerStatement =
                    LogStatement::tNonrecursiveRule(relationName, srcLocation, clauseText);
            const std::string logSizeStatement =
                    LogStatement::nNonrecursiveRule(relationName, srcLocation, clauseText);
            rule = std::make_unique<RamSequence>(std::make_unique<RamLogRelationTimer>(std::move(rule),
                    logTimerStatement, std::unique_ptr<RamRelationReference>(rrel->clone())));
        }

        // add debug info
        std::ostringstream ds;
        ds << toString(*clause) << "\nin file ";
        ds << clause->getSrcLoc();
        rule = std::make_unique<RamDebugInfo>(std::move(rule), ds.str());

        // add rule to result
        appendStmt(res, std::move(rule));
    }

    // add logging for entire relation
    if (Global::config().has("profile")) {
        const std::string& relationName = toString(rel.getName());
        const SrcLocation& srcLocation = rel.getSrcLoc();
        const std::string logSizeStatement = LogStatement::nNonrecursiveRelation(relationName, srcLocation);

        // add timer if we did any work
        if (res) {
            const std::string logTimerStatement =
                    LogStatement::tNonrecursiveRelation(relationName, srcLocation);
            res = std::make_unique<RamLogRelationTimer>(
                    std::move(res), logTimerStatement, std::unique_ptr<RamRelationReference>(rrel->clone()));
        } else {
            // add table size printer
            appendStmt(res, std::make_unique<RamLogSize>(
                                    std::unique_ptr<RamRelationReference>(rrel->clone()), logSizeStatement));
        }
    }

    // done
    return res;
}

/**
 * A utility function assigning names to unnamed variables such that enclosing
 * constructs may be cloned without losing the variable-identity.
 */
void AstTranslator::nameUnnamedVariables(AstClause* clause) {
    // the node mapper conducting the actual renaming
    struct Instantiator : public AstNodeMapper {
        mutable int counter = 0;

        Instantiator() = default;

        std::unique_ptr<AstNode> operator()(std::unique_ptr<AstNode> node) const override {
            // apply recursive
            node->apply(*this);

            // replace unknown variables
            if (dynamic_cast<AstUnnamedVariable*>(node.get()) != nullptr) {
                auto name = " _unnamed_var" + toString(++counter);
                return std::make_unique<AstVariable>(name);
            }

            // otherwise nothing
            return node;
        }
    };

    // name all variables in the atoms
    Instantiator init;
    for (auto& atom : clause->getAtoms()) {
        atom->apply(init);
    }
}

/** generate RAM code for recursive relations in a strongly-connected component */
std::unique_ptr<RamStatement> AstTranslator::translateRecursiveRelation(
        const AstTranslationUnit& translationUnit, std::size_t scc) {
    // @TODO (lh): make the sets computed here mutable, don't recompute always
    const auto* sccGraph = translationUnit.getAnalysis<SCCGraph>();

    const auto allInterns = sccGraph->getInternalRelations(scc);
    const auto internIns = sccGraph->getInternalInputRelations(scc);
    const auto internOuts = sccGraph->getInternalOutputRelations(scc);
    const auto externPreds = sccGraph->getExternalPredecessorRelations(scc);
    const auto externOutPreds = sccGraph->getExternalOutputPredecessorRelations(scc);
    const auto externNonOutPreds = sccGraph->getExternalNonOutputPredecessorRelations(scc);
    const auto externAggNegPreds = sccGraph->getAggregatedAndNegatedExternalPredecessorRelations(scc);
    const auto internNonOutsWithExternSuccs =
            sccGraph->getInternalNonOutputRelationsWithExternalSuccessors(scc);

    // @TODO (lh): use the variables above, remove unused, don't reference
    const auto& internalRelationsOfScc = allInterns;
    const auto& externalPredecessorRelationsOfScc = externPreds;
    const auto& internalOutputRelations = internOuts;
    const auto& internalNonOutputRelationsWithExternalSuccessors = internNonOutsWithExternSuccs;

    const auto* recursiveClauses = translationUnit.getAnalysis<RecursiveClauses>();

    // initialize sections
    std::unique_ptr<RamStatement> preamble(new RamSequence());
    std::unique_ptr<RamSequence> updateTable(new RamSequence());
    std::unique_ptr<RamStatement> postamble(new RamSequence());

    // --- create preamble ---

    // clear mappings for temporary relations
if (!Global::config().has("use-general")) {
    rrel.clear();
    relDelta.clear();
    relNew.clear();
}

if (Global::config().has("use-general")) {
if (!Global::config().has("engine")) { 
    for (const AstRelation* relation : externalPredecessorRelationsOfScc) {
        if (!externAggNegPreds.count(relation)) {
            appendStmt(preamble,
                    std::make_unique<RamMerge>(std::unique_ptr<RamRelationReference>(relDelta[relation]->clone()),
                            std::unique_ptr<RamRelationReference>(rrel[relation]->clone())));
        }
    }
} else {
    
    // @@@@@@@@@@@@@@@@@@@ HERE @@@@@@@@@@@@@@@@@@@@@@@@

    // @@@@@@@@@@@@@TODO
    // Ok, for halting with continuous consumption, need to do as follows:
    // - while (every @new_rel does not contain a halt)
    // -- for each rel
    // --- if @new_rel is empty (does not contain a halt, has not recvd)
    // ---- recv to @new_myRel
    // --- if @new_myRel does not contain halt AND @new_myRela is not empty
    // ---- merge new_myRel with @delta_myRel
    // ---- exit the loop
    // - endwhile
    // - if (every @new_rel does contain a halt) and (all the deltas are empty)
    // -- exit the program
    // - otherwise
    // -- do seminaive again
    // Yay! Can also use exponential backoff for consumer timeouts...

    /*
        // @TODO: get this working, it is the next step
        // so just after the delta and new have been swapped and the new is clear
        LOOP
        - forall @new_rel : 
        - IF (@new_rel = ∅)
        - -  LOAD DATA FOR @new_rel FROM {...}
        - IF (NOT (@new_rel) = ∅)) 
        - - IF (NOT ((number(-1)) ∈ @new_rel)
        - - - MERGE @delta_rel WITH @new_rel
        - - - EXIT ()
        END LOOP
        ...
        // then in outer loop
        EXIT ((forall @new_rel : (NOT ((number(-1)) ∈ @new_rel)) 
            AND (forall @delta_rel : (@new_rel = ∅)))
    */
    

    // load all external output predecessor relations from the output dir with a .csv extension
    for (const auto& relation : externOutPreds) {
        if (!externAggNegPreds.count(relation)) {
            makeRamLoad(preamble, scc, relation, "output-dir", ".csv",
                    (Global::config().has("engine")) ? Global::config().get("engine") : "file",
                    std::unique_ptr<RamRelationReference>(relDelta[relation]->clone()));
        }
    }
    // load all external non-output predecessor relations from the output dir with a .facts
    // extension
    for (const auto& relation : externNonOutPreds) {
        if (!externAggNegPreds.count(relation)) {
            makeRamLoad(preamble, scc, relation, "output-dir", ".facts", 
                    (Global::config().has("engine")) ? Global::config().get("engine") : "file",
                    std::unique_ptr<RamRelationReference>(relDelta[relation]->clone()));
        }
    }
}
}

    /* Compute non-recursive clauses for relations in scc and push
       the results in their delta tables. */
    for (const AstRelation* rel : internalRelationsOfScc) {
        std::unique_ptr<RamStatement> updateRelTable;

if (!Global::config().has("use-general")) {
        /* create two temporary tables for relaxed semi-naive evaluation */
        rrel[rel] = translateRelation(rel);
        relDelta[rel] = translateDeltaRelation(rel);
        relNew[rel] = translateNewRelation(rel);
}

        /* create update statements for fixpoint (even iteration) */
        appendStmt(updateRelTable,
                std::make_unique<RamSequence>(
                        std::make_unique<RamMerge>(std::unique_ptr<RamRelationReference>(rrel[rel]->clone()),
                                std::unique_ptr<RamRelationReference>(relNew[rel]->clone())),
                        std::make_unique<RamSwap>(
                                std::unique_ptr<RamRelationReference>(relDelta[rel]->clone()),
                                std::unique_ptr<RamRelationReference>(relNew[rel]->clone())),
                        std::make_unique<RamClear>(
                                std::unique_ptr<RamRelationReference>(relNew[rel]->clone()))));

if (Global::config().has("use-general-producers")) {
        // @@@TODO (lh): this does not work as IO directives need ast relations, but delta and new are ram
        // relation references
        if (internalOutputRelations.count(rel)) {
            makeRamStore(updateRelTable, scc, rel, "output-dir", ".csv", "default",
                    (Global::config().has("engine")) ? Global::config().get("engine") : "file",
                    std::unique_ptr<RamRelationReference>(relDelta[rel]->clone()));
        } else if (Global::config().has("engine") &&
                   internalNonOutputRelationsWithExternalSuccessors.count(rel)) {
            makeRamStore(updateRelTable, scc, rel, "output-dir", ".facts", "default",
                    (Global::config().has("engine")) ? Global::config().get("engine") : "file",
                    std::unique_ptr<RamRelationReference>(relDelta[rel]->clone()));
        }
}

        /* measure update time for each relation */
        if (Global::config().has("profile")) {
            updateRelTable = std::make_unique<RamLogRelationTimer>(std::move(updateRelTable),
                    LogStatement::cRecursiveRelation(toString(rel->getName()), rel->getSrcLoc()),
                    std::unique_ptr<RamRelationReference>(relNew[rel]->clone()));
        }

        /* drop temporary tables after recursion */
        appendStmt(postamble, std::make_unique<RamSequence>(
                                      std::make_unique<RamClear>(
                                              std::unique_ptr<RamRelationReference>(relDelta[rel]->clone())),
                                      std::make_unique<RamClear>(
                                              std::unique_ptr<RamRelationReference>(relNew[rel]->clone()))));

if (!Global::config().has("use-general")) {
        /* Generate code for non-recursive part of relation */
        appendStmt(preamble, translateNonRecursiveRelation(translationUnit, *rel));

        /* Generate merge operation for temp tables */
        appendStmt(preamble,
                std::make_unique<RamMerge>(std::unique_ptr<RamRelationReference>(relDelta[rel]->clone()),
                        std::unique_ptr<RamRelationReference>(rrel[rel]->clone())));
}

        /* Add update operations of relations to parallel statements */
        updateTable->add(std::move(updateRelTable));
    }

if (Global::config().has("use-general")) {
    for (const AstRelation* rel : externalPredecessorRelationsOfScc) {
        if (!externAggNegPreds.count(rel)) {
                std::unique_ptr<RamStatement> updateRelTable;

            /* Generate merge operation for temp tables */
            appendStmt(updateRelTable,
                    std::make_unique<RamSequence>(
                            std::make_unique<RamMerge>(std::unique_ptr<RamRelationReference>(rrel[rel]->clone()),
                                    std::unique_ptr<RamRelationReference>(relNew[rel]->clone())),
                            std::make_unique<RamSwap>(
                                    std::unique_ptr<RamRelationReference>(relDelta[rel]->clone()),
                                    std::unique_ptr<RamRelationReference>(relNew[rel]->clone())),
                            std::make_unique<RamClear>(
                                    std::unique_ptr<RamRelationReference>(relNew[rel]->clone()))));

            /* Add update operations of relations to parallel statements */
            updateTable->add(std::move(updateRelTable));
        }
    }
}

    // --- build main loop ---

    std::unique_ptr<RamParallel> loopSeq(new RamParallel());

    // create a utility to check SCC membership
    auto isInSameSCC = [&](const AstRelation* rel) {
        return std::find(internalRelationsOfScc.begin(), internalRelationsOfScc.end(), rel) !=
               internalRelationsOfScc.end();
    };
if (Global::config().has("use-general")) {
    (void)isInSameSCC;
}

    /* Compute temp for the current tables */
    for (const AstRelation* rel : internalRelationsOfScc) {
        std::unique_ptr<RamStatement> loopRelSeq;

        /* Find clauses for relation rel */
        for (size_t i = 0; i < rel->clauseSize(); i++) {
            AstClause* cl = rel->getClause(i);

if (!Global::config().has("use-general")) {
            // skip non-recursive clauses
            if (!recursiveClauses->recursive(cl)) {
                continue;
            }
}

            // each recursive rule results in several operations
            int version = 0;
            const auto& atoms = cl->getAtoms();

if (Global::config().has("use-general")) {
            if (atoms.size() == 0) {
                // modify the processed rule to use relDelta and write to relNew
                std::unique_ptr<AstClause> r1(cl->clone());
                r1->getHead()->setName(relNew[rel]->get()->getName());

                if (r1->getHead()->getArity() > 0) {
                    r1->addToBody(
                            std::make_unique<AstNegation>(std::unique_ptr<AstAtom>(cl->getHead()->clone())));
                }

                std::unique_ptr<RamStatement> rule =
                        ClauseTranslator(*this).translateClause(*r1, *cl, version);

                // add to loop body
                appendStmt(loopRelSeq, std::move(rule));

                // @TODO (lh): handle logging, profiling, execution plans, and provenance, where possible
            }
}

            for (size_t j = 0; j < atoms.size(); ++j) {
                const AstAtom* atom = atoms[j];
                const AstRelation* atomRelation = getAtomRelation(atom, program);

if (!Global::config().has("use-general")) {
                // only interested in atoms within the same SCC
                if (!isInSameSCC(atomRelation)) {
                    continue;
                }
}

                // modify the processed rule to use relDelta and write to relNew
                std::unique_ptr<AstClause> r1(cl->clone());
                r1->getHead()->setName(relNew[rel]->get()->getName());
                r1->getAtoms()[j]->setName(relDelta[atomRelation]->get()->getName());
                if (Global::config().has("provenance")) {
                    r1->addToBody(std::make_unique<AstProvenanceNegation>(
                            std::unique_ptr<AstAtom>(cl->getHead()->clone())));
                } else {
                    if (r1->getHead()->getArity() > 0) {
                        r1->addToBody(std::make_unique<AstNegation>(
                                std::unique_ptr<AstAtom>(cl->getHead()->clone())));
                    }
                }

                // replace wildcards with variables (reduces indices when wildcards are used in recursive
                // atoms)
                nameUnnamedVariables(r1.get());

                // reduce R to P ...
                for (size_t k = j + 1; k < atoms.size(); k++) {
if (!Global::config().has("use-general")) {
                    if (isInSameSCC(getAtomRelation(atoms[k], program))) {
}
                        AstAtom* cur = r1->getAtoms()[k]->clone();
                        cur->setName(relDelta[getAtomRelation(atoms[k], program)]->get()->getName());
                        r1->addToBody(std::make_unique<AstNegation>(std::unique_ptr<AstAtom>(cur)));
if (!Global::config().has("use-general")) {
                    }
}
                }

                std::unique_ptr<RamStatement> rule =
                        ClauseTranslator(*this).translateClause(*r1, *cl, version);

                /* add logging */
                if (Global::config().has("profile")) {
                    const std::string& relationName = toString(rel->getName());
                    const SrcLocation& srcLocation = cl->getSrcLoc();
                    const std::string clauseText = stringify(toString(*cl));
                    const std::string logTimerStatement =
                            LogStatement::tRecursiveRule(relationName, version, srcLocation, clauseText);
                    const std::string logSizeStatement =
                            LogStatement::nRecursiveRule(relationName, version, srcLocation, clauseText);
                    rule = std::make_unique<RamSequence>(
                            std::make_unique<RamLogRelationTimer>(std::move(rule), logTimerStatement,
                                    std::unique_ptr<RamRelationReference>(relNew[rel]->clone())));
                }

                // add debug info
                std::ostringstream ds;
                ds << toString(*cl) << "\nin file ";
                ds << cl->getSrcLoc();
                rule = std::make_unique<RamDebugInfo>(std::move(rule), ds.str());

                // add to loop body
                appendStmt(loopRelSeq, std::move(rule));

                // increment version counter
                version++;
            }
            assert(cl->getExecutionPlan() == nullptr || version > cl->getExecutionPlan()->getMaxVersion());
        }

        // if there was no rule, continue
        if (loopRelSeq == nullptr) {
            continue;
        }

        // label all versions
        if (Global::config().has("profile")) {
            const std::string& relationName = toString(rel->getName());
            const SrcLocation& srcLocation = rel->getSrcLoc();
            const std::string logTimerStatement = LogStatement::tRecursiveRelation(relationName, srcLocation);
            const std::string logSizeStatement = LogStatement::nRecursiveRelation(relationName, srcLocation);
            loopRelSeq = std::make_unique<RamLogRelationTimer>(std::move(loopRelSeq), logTimerStatement,
                    std::unique_ptr<RamRelationReference>(relNew[rel]->clone()));
        }

        /* add rule computations of a relation to parallel statement */
        loopSeq->add(std::move(loopRelSeq));
    }

    /* construct exit conditions for odd and even iteration */
    auto addCondition = [](std::unique_ptr<RamCondition>& cond, std::unique_ptr<RamCondition> clause) {
        cond = ((cond) ? std::make_unique<RamConjunction>(std::move(cond), std::move(clause))
                       : std::move(clause));
    };

    std::unique_ptr<RamCondition> exitCond;
    for (const AstRelation* rel : internalRelationsOfScc) {
        addCondition(exitCond, std::make_unique<RamEmptinessCheck>(
                                       std::unique_ptr<RamRelationReference>(relNew[rel]->clone())));
    }

    /* construct fixpoint loop  */
    std::unique_ptr<RamStatement> res;
    if (preamble != nullptr) {
        appendStmt(res, std::move(preamble));
    }
    if (!loopSeq->getStatements().empty() && exitCond && updateTable) {
        appendStmt(res, std::make_unique<RamLoop>(std::move(loopSeq),
                                std::make_unique<RamExit>(std::move(exitCond)), std::move(updateTable)));
    }
    if (postamble != nullptr) {
        appendStmt(res, std::move(postamble));
    }
    if (res != nullptr) {
        return res;
    }

    assert(false && "Not Implemented");
    return nullptr;
}

/** make a subroutine to search for subproofs */
std::unique_ptr<RamStatement> AstTranslator::makeSubproofSubroutine(const AstClause& clause) {
    // make intermediate clause with constraints
    std::unique_ptr<AstClause> intermediateClause(clause.clone());

    // name unnamed variables
    nameUnnamedVariables(intermediateClause.get());

    // add constraint for each argument in head of atom
    AstAtom* head = intermediateClause->getHead();
    size_t numberOfHeights = program->getRelation(head->getName())->numberOfHeightParameters();
    for (size_t i = 0; i < head->getArguments().size() - 1 - numberOfHeights; i++) {
        auto arg = head->getArgument(i);

        if (auto var = dynamic_cast<AstVariable*>(arg)) {
            intermediateClause->addToBody(std::make_unique<AstBinaryConstraint>(BinaryConstraintOp::EQ,
                    std::unique_ptr<AstArgument>(var->clone()), std::make_unique<AstSubroutineArgument>(i)));
        } else if (auto func = dynamic_cast<AstFunctor*>(arg)) {
            intermediateClause->addToBody(std::make_unique<AstBinaryConstraint>(BinaryConstraintOp::EQ,
                    std::unique_ptr<AstArgument>(func->clone()), std::make_unique<AstSubroutineArgument>(i)));
        } else if (auto rec = dynamic_cast<AstRecordInit*>(arg)) {
            intermediateClause->addToBody(std::make_unique<AstBinaryConstraint>(BinaryConstraintOp::EQ,
                    std::unique_ptr<AstArgument>(rec->clone()), std::make_unique<AstSubroutineArgument>(i)));
        }
    }

    if (Global::config().get("provenance") == "subtreeHeights") {
        // starting index of subtree level arguments in argument list
        // starts immediately after original arguments as height and rulenumber of tuple are not passed to
        // subroutine
        size_t levelIndex = head->getArguments().size() - numberOfHeights - 1;

        // add level constraints
        for (size_t i = 0; i < intermediateClause->getBodyLiterals().size(); i++) {
            auto lit = intermediateClause->getBodyLiteral(i);
            if (auto atom = dynamic_cast<AstAtom*>(lit)) {
                auto arity = atom->getArity();
                auto literalHeights = program->getRelation(atom->getName())->numberOfHeightParameters();
                auto literalLevelIndex = arity - literalHeights;

                intermediateClause->addToBody(std::make_unique<AstBinaryConstraint>(BinaryConstraintOp::EQ,
                        std::unique_ptr<AstArgument>(atom->getArgument(literalLevelIndex)->clone()),
                        std::make_unique<AstSubroutineArgument>(levelIndex)));
            }
            levelIndex++;
        }
    } else {
        // index of level argument in argument list
        size_t levelIndex = head->getArguments().size() - numberOfHeights - 1;

        // add level constraints
        for (size_t i = 0; i < intermediateClause->getBodyLiterals().size(); i++) {
            auto lit = intermediateClause->getBodyLiteral(i);
            if (auto atom = dynamic_cast<AstAtom*>(lit)) {
                auto arity = atom->getArity();

                // arity - 1 is the level number in body atoms
                intermediateClause->addToBody(std::make_unique<AstBinaryConstraint>(BinaryConstraintOp::LT,
                        std::unique_ptr<AstArgument>(atom->getArgument(arity - 1)->clone()),
                        std::make_unique<AstSubroutineArgument>(levelIndex)));
            }
        }
    }
    return ProvenanceClauseTranslator(*this).translateClause(*intermediateClause, clause);
}

/** make a subroutine to search for subproofs for the non-existence of a tuple */
std::unique_ptr<RamStatement> AstTranslator::makeNegationSubproofSubroutine(const AstClause& clause) {
    // TODO (taipan-snake): Currently we only deal with atoms (no constraints or negations or aggregates
    // or anything else...)

    // clone clause for mutation
    auto clauseReplacedAggregates = std::unique_ptr<AstClause>(clause.clone());

    int aggNumber = 0;
    struct AggregatesToVariables : public AstNodeMapper {
        int& aggNumber;

        AggregatesToVariables(int& aggNumber) : aggNumber(aggNumber) {}

        std::unique_ptr<AstNode> operator()(std::unique_ptr<AstNode> node) const override {
            if (dynamic_cast<AstAggregator*>(node.get()) != nullptr) {
                return std::make_unique<AstVariable>("agg_" + std::to_string(aggNumber++));
            }

            node->apply(*this);
            return node;
        }
    };

    AggregatesToVariables aggToVar(aggNumber);
    clauseReplacedAggregates->apply(aggToVar);

    // build a vector of unique variables
    std::vector<const AstVariable*> uniqueVariables;

    visitDepthFirst(*clauseReplacedAggregates, [&](const AstVariable& var) {
        if (var.getName().find("@level_num") == std::string::npos) {
            // use find_if since uniqueVariables stores pointers, and we need to dereference the pointer to
            // check equality
            if (std::find_if(uniqueVariables.begin(), uniqueVariables.end(),
                        [&](const AstVariable* v) { return *v == var; }) == uniqueVariables.end()) {
                uniqueVariables.push_back(&var);
            }
        }
    });

    // a mapper to replace variables with subroutine arguments
    struct VariablesToArguments : public AstNodeMapper {
        const std::vector<const AstVariable*>& uniqueVariables;

        VariablesToArguments(const std::vector<const AstVariable*>& uniqueVariables)
                : uniqueVariables(uniqueVariables) {}

        std::unique_ptr<AstNode> operator()(std::unique_ptr<AstNode> node) const override {
            // replace unknown variables
            if (auto varPtr = dynamic_cast<const AstVariable*>(node.get())) {
                if (varPtr->getName().find("@level_num") == std::string::npos) {
                    size_t argNum = std::find_if(uniqueVariables.begin(), uniqueVariables.end(),
                                            [&](const AstVariable* v) { return *v == *varPtr; }) -
                                    uniqueVariables.begin();

                    return std::make_unique<AstSubroutineArgument>(argNum);
                } else {
                    return std::make_unique<AstUnnamedVariable>();
                }
            }

            // apply recursive
            node->apply(*this);

            // otherwise nothing
            return node;
        }
    };

    // the structure of this subroutine is a sequence where each nested statement is a search in each
    // relation
    std::unique_ptr<RamSequence> searchSequence = std::make_unique<RamSequence>();

    // make a copy so that when we mutate clause, pointers to objects in newClause are not affected
    auto newClause = std::unique_ptr<AstClause>(clauseReplacedAggregates->clone());

    // go through each body atom and create a return
    size_t litNumber = 0;
    for (const auto& lit : newClause->getBodyLiterals()) {
        if (auto atom = dynamic_cast<AstAtom*>(lit)) {
            size_t numberOfHeights = program->getRelation(atom->getName())->numberOfHeightParameters();
            // get a RamRelationReference
            auto relRef = translateRelation(atom);

            // construct a query
            std::vector<std::unique_ptr<RamExpression>> query;

            // translate variables to subroutine arguments
            VariablesToArguments varsToArgs(uniqueVariables);
            atom->apply(varsToArgs);

            // add each value (subroutine argument) to the search query
            for (size_t i = 0; i < atom->getArity() - 1 - numberOfHeights; i++) {
                auto arg = atom->getArgument(i);
                query.push_back(translateValue(arg, ValueIndex()));
            }

            // fill up query with nullptrs for the provenance columns
            query.push_back(std::make_unique<RamUndefValue>());
            for (size_t h = 0; h < numberOfHeights; h++) {
                query.push_back(std::make_unique<RamUndefValue>());
            }

            // ensure the length of query tuple is correct
            assert(query.size() == atom->getArity() && "wrong query tuple size");

            // make the nested operation to return the atom number if it exists
            std::vector<std::unique_ptr<RamExpression>> returnValue;
            returnValue.push_back(std::make_unique<RamNumber>(litNumber));

            // create a search
            // a filter to find whether the current atom exists or not
            auto searchFilter = std::make_unique<RamFilter>(
                    std::make_unique<RamExistenceCheck>(
                            std::unique_ptr<RamRelationReference>(relRef->clone()), std::move(query)),
                    std::make_unique<RamSubroutineReturnValue>(std::move(returnValue)));

            // now, return the values of the atoms, with a separator
            // between atom number and atom
            std::vector<std::unique_ptr<RamExpression>> returnAtom;
            returnAtom.push_back(std::make_unique<RamUndefValue>());
            // the actual atom
            for (size_t i = 0; i < atom->getArity() - 1 - numberOfHeights; i++) {
                returnAtom.push_back(translateValue(atom->getArgument(i), ValueIndex()));
            }

            // chain the atom number and atom value together
            auto atomSequence = std::make_unique<RamSequence>();
            atomSequence->add(std::make_unique<RamQuery>(std::move(searchFilter)));
            atomSequence->add(std::make_unique<RamQuery>(
                    std::make_unique<RamSubroutineReturnValue>(std::move(returnAtom))));

            // append search to the sequence
            searchSequence->add(std::move(atomSequence));
        } else if (auto con = dynamic_cast<AstConstraint*>(lit)) {
            VariablesToArguments varsToArgs(uniqueVariables);
            con->apply(varsToArgs);

            // translate to a RamCondition
            auto condition = translateConstraint(con, ValueIndex());

            // create a return value
            std::vector<std::unique_ptr<RamExpression>> returnValue;
            returnValue.push_back(std::make_unique<RamNumber>(litNumber));

            // create a filter
            auto filter = std::make_unique<RamFilter>(
                    std::move(condition), std::make_unique<RamSubroutineReturnValue>(std::move(returnValue)));

            // now, return the values of the literal, with a separator
            // between atom number and atom
            std::vector<std::unique_ptr<RamExpression>> returnLit;
            returnLit.push_back(std::make_unique<RamUndefValue>());
            // add return values for binary constraints and negations
            if (auto binaryConstraint = dynamic_cast<AstBinaryConstraint*>(con)) {
                returnLit.push_back(translateValue(binaryConstraint->getLHS(), ValueIndex()));
                returnLit.push_back(translateValue(binaryConstraint->getRHS(), ValueIndex()));
            } else if (auto negation = dynamic_cast<AstNegation*>(con)) {
                auto vals = negation->getAtom()->getArguments();
                auto numberOfHeights =
                        program->getRelation(negation->getAtom()->getName())->numberOfHeightParameters();
                for (size_t i = 0; i < vals.size() - 1 - numberOfHeights; i++) {
                    returnLit.push_back(translateValue(vals[i], ValueIndex()));
                }
            }

            // chain the atom number and atom value together
            auto litSequence = std::make_unique<RamSequence>();
            litSequence->add(std::make_unique<RamQuery>(std::move(filter)));
            litSequence->add(std::make_unique<RamQuery>(
                    std::make_unique<RamSubroutineReturnValue>(std::move(returnLit))));

            // append search to the sequence
            searchSequence->add(std::move(litSequence));
        }

        litNumber++;
    }

    return std::move(searchSequence);
}

/** translates the given datalog program into an equivalent RAM program  */
void AstTranslator::translateProgram(const AstTranslationUnit& translationUnit) {
    // @TODO (lh): enable non-blocking communication for kafka engine when -a/--async option enabled

    // obtain type environment from analysis
    typeEnv = &translationUnit.getAnalysis<TypeEnvironmentAnalysis>()->getTypeEnvironment();

    // obtain strongly connected component (SCC) graph from analysis
    const auto& sccGraph = *translationUnit.getAnalysis<SCCGraph>();

    // obtain some topological order over the nodes of the SCC graph
    const auto& sccOrder = *translationUnit.getAnalysis<TopologicallySortedSCCGraph>();

    // obtain the schedule of relations expired at each index of the topological order
    const auto& expirySchedule = translationUnit.getAnalysis<RelationSchedule>()->schedule();

if (Global::config().has("use-general")) {
    for (const AstRelation* rel : translationUnit.getProgram()->getRelations()) {
        rrel[rel] = translateRelation(rel);
        relDelta[rel] = translateDeltaRelation(rel);
        relNew[rel] = translateNewRelation(rel);
    }
}

    // start with an empty sequence of ram statements
    std::unique_ptr<RamStatement> res = std::make_unique<RamSequence>();

    // handle the case of an empty SCC graph
    if (sccGraph.getNumberOfSCCs() == 0) return;

    // a function to drop relations
    const auto& makeRamClear = [&](std::unique_ptr<RamStatement>& current, const AstRelation* relation) {
        appendStmt(current, std::make_unique<RamClear>(translateRelation(relation)));
    };

    if (Global::config().has("engine")) {
        // @@@TODO (lh): try with a ram parallel statement for kafka loads and stores
        std::size_t masterScc = -1;

        // make a new ram statement for the master stratum
        std::unique_ptr<RamStatement> current;

        // iterate over each scc and load all input relations
        for (const auto& scc : sccOrder.order()) {
            // get all input relations of the current scc
            const auto& inputRelations = sccGraph.getInternalInputRelations(scc);

            // make load statements for each input relation
            for (const auto& inputRelation : inputRelations) {
                makeRamLoad(current, masterScc, inputRelation, "fact-dir", ".facts", "file");
            }

            // make store statements for each input relation to produce with kafka
            for (const auto& inputRelation : inputRelations) {
                makeRamStore(current, masterScc, inputRelation, "fact-dir", ".facts");
            }
        }

        // iterate over each scc and store all output relations
        for (const auto& scc : sccOrder.order()) {
            // get all output relations of the current scc
            const auto& outputRelations = sccGraph.getInternalOutputRelations(scc);

            // make load statements for each output relation to consume with kafak
            for (const auto& outputRelation : outputRelations) {
                makeRamLoad(current, masterScc, outputRelation, "output-dir", ".csv");
            }

            // make store statements for each output relation
            for (const auto& outputRelation : outputRelations) {
                makeRamStore(current, masterScc, outputRelation, "output-dir", ".csv", "default", "file");
            }
        }

        // ensure that current exists, i.e. there is at least one output or printSize relation in the program
        assert(current);

        // append the master stratum
        appendStmt(res, std::make_unique<RamStratum>(std::move(current), (size_t)-1));
    }

    // iterate over each SCC according to the topological order
    for (const auto& scc : sccOrder.order()) {
        // make a new ram statement for the current SCC
        std::unique_ptr<RamStatement> current;

        // @@@TODO: move this down to the conditional,
        // then just use if (Global::config().has("use-general")) { for generalisation,
        // and change cli option "async" to "append" for now,
        // then get working with blocking kafka/file IO and
        // USE_GENERAL defined, then start with *production*
        // (not *consumption*) of the new relations with
        // USE_GENERAL_WRITE/USE_GENERAL_PRODUCTION defined for both kafka and file
        // (and file must have -a or maybe just remove -a for now
        // and do USE_GENERAL_PRODUCTION instead
        // then move on to CONSUMPTION

        // find out if the current SCC is recursive
const auto isRecursive =  (!Global::config().has("use-general")) ? sccGraph.isRecursive(scc): true;

        // make variables for particular sets of relations contained within the current SCC, and,
        // predecessors and successor SCCs thereof
        const auto allInterns = sccGraph.getInternalRelations(scc);
        const auto internIns = sccGraph.getInternalInputRelations(scc);
        const auto internOuts = sccGraph.getInternalOutputRelations(scc);
        const auto externPreds = sccGraph.getExternalPredecessorRelations(scc);
        const auto externOutPreds = sccGraph.getExternalOutputPredecessorRelations(scc);
        const auto externNonOutPreds = sccGraph.getExternalNonOutputPredecessorRelations(scc);
        const auto externAggNegPreds = sccGraph.getAggregatedAndNegatedExternalPredecessorRelations(scc);
        const auto internNonOutsWithExternSuccs =
                sccGraph.getInternalNonOutputRelationsWithExternalSuccessors(scc);

        // make a variable for all relations that are expired at the current SCC
        const auto& internExps = expirySchedule.at(scc).expired();

        {
            // load all internal input relations from the facts dir with a .facts extension
            for (const auto& relation : internIns) {
                makeRamLoad(current, scc, relation, "fact-dir", ".facts");
            }

            // if a communication engine has been specified...
            if (Global::config().has("engine")) {
                // load all external aggregated or negated predecessor relations
                for (const auto& relation : externAggNegPreds) {
                    if (externOutPreds.count(relation)) {
                        // if relation is output, load from the output dir with a .csv extension
                        makeRamLoad(current, scc, relation, "output-dir", ".csv");
                    } else {
                        // otherwise, relation is non-output, load from the output dir with a .facts extension
                        makeRamLoad(current, scc, relation, "output-dir", ".facts");
                    }
                }
if (!Global::config().has("use-general")) {
                // load all external output predecessor relations from the output dir with a .csv
                // extension
                for (const auto& relation : externOutPreds) {
                    if (!externAggNegPreds.count(relation)) {
                        makeRamLoad(current, scc, relation, "output-dir", ".csv");
                    }
                }
                // load all external non-output predecessor relations from the output dir with a .facts
                // extension
                for (const auto& relation : externNonOutPreds) {
                    if (!externAggNegPreds.count(relation)) {
                        makeRamLoad(current, scc, relation, "output-dir", ".facts");
                    }
                }
}
            }
        }

        // compute the relations themselves
        std::unique_ptr<RamStatement> bodyStatement =
                (!isRecursive) ? translateNonRecursiveRelation(
                                         translationUnit, *((const AstRelation*)*allInterns.begin()))
                               : translateRecursiveRelation(translationUnit, scc);
        appendStmt(current, std::move(bodyStatement));
if (!Global::config().has("use-general-producers")) {
        {
            // if a communication engine is enabled...
            if (Global::config().has("engine")) {
                // store all internal non-output relations with external successors to the output dir with
                // a .facts extension
                for (const auto& relation : internNonOutsWithExternSuccs) {
                    makeRamStore(current, scc, relation, "output-dir", ".facts");
                }
            }

            // store all internal output relations to the output dir with a .csv extension
            for (const auto& relation : internOuts) {
                makeRamStore(current, scc, relation, "output-dir", ".csv");
            }
        }
}
        {
            // if a communication engine is enabled and is kafka...
            if (Global::config().get("engine") == "kafka") {
                // send halts for all internal relations
                for (const auto& relation : internNonOutsWithExternSuccs) {
                    makeRamStore(current, scc, relation, "output-dir", ".facts", "null-payload");
                }
                // send halts for all output relations
                for (const auto& relation : internOuts) {
                    makeRamStore(current, scc, relation, "output-dir", ".csv", "null-payload");
                }
            }
        }

        // if provenance is not enabled...
        if (!Global::config().has("provenance")) {
            // if a communication engine is enabled...
            if (Global::config().has("engine")) {
                // drop all internal relations
                for (const auto& relation : allInterns) {
                    makeRamClear(current, relation);
                }
                // drop external output predecessor relations
                for (const auto& relation : externOutPreds) {
                    makeRamClear(current, relation);
                }
                // drop external non-output predecessor relations
                for (const auto& relation : externNonOutPreds) {
                    makeRamClear(current, relation);
                }
            } else {
                // otherwise, drop all  relations expired as per the topological order
                for (const auto& relation : internExps) {
                    makeRamClear(current, relation);
                }
            }
        }

        if (current) {
            // append the current SCC as a stratum to the sequence
            appendStmt(res, std::make_unique<RamStratum>(std::move(current), scc));
        }
    }

    // add main timer if profiling
    if (res && Global::config().has("profile")) {
        res = std::make_unique<RamLogTimer>(std::move(res), LogStatement::runtime());
    }

    // done for main prog
    ramMain = std::move(res);

    // add subroutines for each clause
    if (Global::config().has("provenance")) {
        visitDepthFirst(program->getRelations(), [&](const AstClause& clause) {
            std::stringstream relName;
            relName << clause.getHead()->getName();

            // do not add subroutines for info relations or facts
            if (relName.str().find("@info") != std::string::npos || clause.getBodyLiterals().empty()) {
                return;
            }

            std::string subroutineLabel =
                    relName.str() + "_" + std::to_string(clause.getClauseNum()) + "_subproof";
            ramSubs[subroutineLabel] = makeSubproofSubroutine(clause);

            std::string negationSubroutineLabel =
                    relName.str() + "_" + std::to_string(clause.getClauseNum()) + "_negation_subproof";
            ramSubs[negationSubroutineLabel] = makeNegationSubproofSubroutine(clause);
        });
    }
}

std::unique_ptr<RamTranslationUnit> AstTranslator::translateUnit(AstTranslationUnit& tu) {
    auto ram_start = std::chrono::high_resolution_clock::now();
    program = tu.getProgram();
    translateProgram(tu);
    SymbolTable& symTab = tu.getSymbolTable();
    ErrorReport& errReport = tu.getErrorReport();
    DebugReport& debugReport = tu.getDebugReport();
    std::vector<std::unique_ptr<RamRelation>> rels;
    for (auto& cur : ramRels) {
        rels.push_back(std::move(cur.second));
    }
    if (nullptr == ramMain) {
        ramMain = std::make_unique<RamSequence>();
    }
    std::unique_ptr<RamProgram> ramProg =
            std::make_unique<RamProgram>(std::move(rels), std::move(ramMain), std::move(ramSubs));
    if (!Global::config().get("debug-report").empty()) {
        if (ramProg) {
            auto ram_end = std::chrono::high_resolution_clock::now();
            std::string runtimeStr =
                    "(" + std::to_string(std::chrono::duration<double>(ram_end - ram_start).count()) + "s)";
            std::stringstream ramProgStr;
            ramProgStr << *ramProg;
            debugReport.addSection(DebugReporter::getCodeSection(
                    "ram-program", "RAM Program " + runtimeStr, ramProgStr.str()));
        }
    }
    return std::make_unique<RamTranslationUnit>(std::move(ramProg), symTab, errReport, debugReport);
}

}  // end of namespace souffle
