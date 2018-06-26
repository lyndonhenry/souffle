/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2013, Oracle and/or its affiliates. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @SYMBOL_TABLE_file SymbolTable.h
 *
 * Data container to store symbols of the Datalog program.
 *
 ***********************************************************************/

#pragma once

#include "ParallelUtils.h"
#include "RamTypes.h"
#include "Util.h"

#ifdef USE_MPI
#include "Mpi.h"
#endif

#include <cassert>
#include <deque>
#include <initializer_list>
#include <iostream>
#include <string>
#include <unordered_map>

namespace souffle {

/**
 * @SYMBOL_TABLE_class SymbolTable
 *
 * Global pool of re-usable strings
 *
 * SymbolTable stores Datalog symbols and converts them to numbers and vice versa.
 */
class SymbolTable {
private:
#ifdef USE_MPI
    mutable std::unordered_map<std::string, size_t> strToNumCache;
    mutable std::unordered_map<size_t, std::string> numToStrCache;
    RamDomain cacheLookup(const std::string& symbol, const std::string& name) const {
        auto it = strToNumCache.find(symbol);
        if (it != strToNumCache.end()) {
            return it->second;
        }
        mpi::send(symbol, 0, mpi::tagOf(name));
        RamDomain index;
        mpi::recv(index, 0, mpi::tagOf(name));
        strToNumCache.insert(std::pair<std::string, size_t>(symbol, index));
        return numToStrCache.insert(std::pair<size_t, std::string>(index, symbol)).first->first;
    }
    const std::string& cacheResolve(const RamDomain index, const std::string& name) const {
        auto it = numToStrCache.find(index);
        if (it != numToStrCache.end()) {
            return it->second;
        }
        mpi::send(index, 0, mpi::tagOf(name));
        std::string symbol;
        mpi::recv(symbol, 0, mpi::tagOf(name));
        numToStrCache.insert(std::pair<size_t, std::string>(index, symbol));
        return strToNumCache.insert(std::pair<std::string, size_t>(symbol, index)).first->first;
    }
#endif

    /** A lock to synchronize parallel accesses */
    mutable Lock access;

    /** Map indices to strings. */
    std::deque<std::string> numToStr;

    /** Map strings to indices. */
    std::unordered_map<std::string, size_t> strToNum;

    /** Convenience method to place a new symbol in the table, if it does not exist, and return the index of
     * it. */
    inline size_t newSymbolOfIndex(const std::string& symbol) {
        size_t index;
        auto it = strToNum.find(symbol);
        if (it == strToNum.end()) {
            index = numToStr.size();
            strToNum[symbol] = index;
            numToStr.push_back(symbol);
        } else {
            index = it->second;
        }
        return index;
    }

    /** Convenience method to place a new symbol in the table, if it does not exist. */
    inline void newSymbol(const std::string& symbol) {
        if (strToNum.find(symbol) == strToNum.end()) {
            strToNum[symbol] = numToStr.size();
            numToStr.push_back(symbol);
        }
    }

public:
    /** Empty constructor. */
    SymbolTable() = default;

    /** Copy constructor, performs a deep copy. */
    SymbolTable(const SymbolTable& other) : numToStr(other.numToStr), strToNum(other.strToNum) {}

    /** Copy constructor for r-value reference. */
    SymbolTable(SymbolTable&& other) noexcept {
        numToStr.swap(other.numToStr);
        strToNum.swap(other.strToNum);
    }

    SymbolTable(std::initializer_list<std::string> symbols) {
        strToNum.reserve(symbols.size());
        for (const auto& symbol : symbols) {
            newSymbol(symbol);
        }
    }

    /** Destructor, frees memory allocated for all strings. */
    virtual ~SymbolTable() = default;

    /** Assignment operator, performs a deep copy and frees memory allocated for all strings. */
    SymbolTable& operator=(const SymbolTable& other) {
        if (this == &other) {
            return *this;
        }
        numToStr = other.numToStr;
        strToNum = other.strToNum;
        return *this;
    }

    /** Assignment operator for r-value references. */
    SymbolTable& operator=(SymbolTable&& other) noexcept {
        numToStr.swap(other.numToStr);
        strToNum.swap(other.strToNum);
        return *this;
    }

#ifdef USE_MPI

private:
    /** Handles mpi messages. */
    void handleMpi() {
        assert(mpi::commRank() == 0);

        // @TODO: must say this symbol is special
        const auto TERMINATE = mpi::tagOf("@SYMBOL_TABLE_TERMINATE");

        const auto LOOKUP = mpi::tagOf("@SYMBOL_TABLE_LOOKUP");
        const auto LOOKUP_EXISTING = mpi::tagOf("@SYMBOL_TABLE_LOOKUP_EXISTING");
        const auto UNSAFE_LOOKUP = mpi::tagOf("@SYMBOL_TABLE_UNSAFE_LOOKUP");
        const auto RESOLVE = mpi::tagOf("@SYMBOL_TABLE_RESOLVE");
        const auto UNSAFE_RESOLVE = mpi::tagOf("@SYMBOL_TABLE_UNSAFE_RESOLVE");
        const auto SIZE = mpi::tagOf("@SYMBOL_TABLE_SIZE");
        const auto PRINT = mpi::tagOf("@SYMBOL_TABLE_PRINT");
        const auto INSERT_STRING = mpi::tagOf("@SYMBOL_TABLE_INSERT_STRING");
        const auto INSERT_VECTOR_STRING = mpi::tagOf("@SYMBOL_TABLE_INSERT_VECTOR_STRING");

        while (true) {
            auto status = mpi::probe();
            if (status->MPI_TAG == TERMINATE) {
                return;
            } else if (status->MPI_TAG == LOOKUP) {
                std::string symbol;
                mpi::recv(symbol, status);
                mpi::send(lookup(symbol), status);
            } else if (status->MPI_TAG == LOOKUP_EXISTING) {
                std::string symbol;
                mpi::recv(symbol, status);
                mpi::send(lookupExisting(symbol), status);
            }
        }
        else if (status->MPI_TAG == UNSAFE_LOOKUP) {
            std::string symbol;
            mpi::recv(symbol, status);
            mpi::send(unsafeLookup(symbol), status);
        }
        else if (status->MPI_TAG == RESOLVE) {
            RamDomain index;
            mpi::recv(index, status);
            mpi::send(resolve(index), status);
        }
        else if (status->MPI_TAG == UNSAFE_RESOLVE) {
            RamDomain index;
            mpi::recv(index, status);
            mpi::send(unsafeResolve(index), status);
        }
        else if (status->MPI_TAG == SIZE) {
            mpi::send(size(), status);
        }
        else if (status->MPI_TAG == PRINT) {
            print(std::cout);
        }
        else if (status->MPI_TAG == INSERT_STRING) {
            std::string symbol;
            mpi::recv(symbol, status);
            insert(symbol);
        }
        else if (status->MPI_TAG == INSERT_VECTOR_STRING) {
            std::vector<std::string> symbols;
            mpi::recv(symbols, status);
            insert(symbols);
        }
    }

#endif

    /** Find the index of a symbol in the table, inserting a new symbol if it does not exist there
     * already. */
    RamDomain lookup(const std::string& symbol) {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            return cacheLookup(symbol, "@SYMBOL_TABLE_LOOKUP");
        } else
#endif
        {
            auto lease = access.acquire();
            (void)lease;  // avoid warning;
            return static_cast<RamDomain>(newSymbolOfIndex(symbol));
        }
    }

    /** Finds the index of a symbol in the table, giving an error if it's not found */
    RamDomain lookupExisting(const std::string& symbol) const {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            return cacheLookup(symbol, "@SYMBOL_TABLE_LOOKUP_EXISTING");
        } else
#endif
        {
            auto lease = access.acquire();
            (void)lease;  // avoid warning;
            auto result = strToNum.find(symbol);
            if (result == strToNum.end()) {
                std::cerr << "Error string not found in call to SymbolTable::lookupExisting.\n";
                exit(1);
            }
            return static_cast<RamDomain>(result->second);
        }
    }

    /** Find the index of a symbol in the table, inserting a new symbol if it does not exist there
     * already. */
    RamDomain unsafeLookup(const std::string& symbol) {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            return cacheLookup(symbol, "@SYMBOL_TABLE_UNSAFE_LOOKUP");
        } else
#endif
            return newSymbolOfIndex(symbol);
    }

    /** Find a symbol in the table by its index, note that this gives an error if the index is out of
     * bounds.
     */
    const std::string& resolve(const RamDomain index) const {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            return cacheResolve(index, "@SYMBOL_TABLE_RESOLVE");
        } else
#endif
        {
            auto lease = access.acquire();
            (void)lease;  // avoid warning;
            auto pos = static_cast<size_t>(index);
            if (pos >= size()) {
                // TODO: use different error reporting here!!
                std::cerr << "Error index out of bounds in call to SymbolTable::resolve.\n";
                exit(1);
            }
            return numToStr[pos];
        }
    }

    const std::string& unsafeResolve(const RamDomain index) const {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            return cacheResolve(index, "@SYMBOL_TABLE_UNSAFE_RESOLVE");
        } else
#endif
            return numToStr[static_cast<size_t>(index)];
    }

    /* Return the size of the symbol table, being the number of symbols it currently holds. */
    size_t size() const {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            mpi::send(0, mpi::tagOf("@SYMBOL_TABLE_SIZE"));
            size_t size;
            mpi::recv(size, 0, mpi::tagOf("@SYMBOL_TABLE_SIZE"));
            return size;
        } else
#endif
            return numToStr.size();
    }

    /** Bulk insert symbols into the table, note that this operation is more efficient than repeated
     * inserts
     * of single symbols. */
    void insert(const std::vector<std::string>& symbols) {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            mpi::send(symbols, 0, mpi::tagOf("@SYMBOL_TABLE_INSERT_VECTOR_STRING"));
        } else
#endif
        {
            auto lease = access.acquire();
            (void)lease;  // avoid warning;
            strToNum.reserve(size() + symbols.size());
            for (auto& symbol : symbols) {
                newSymbol(symbol);
            }
        }
    }

    /** Insert a single symbol into the table, not that this operation should not be used if inserting
     * symbols
     * in bulk. */
    void insert(const std::string& symbol) {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            mpi::send(symbol, 0, mpi::tagOf("@SYMBOL_TABLE_INSERT_STRING"));
        } else
#endif
        {
            auto lease = access.acquire();
            (void)lease;  // avoid warning;
            newSymbol(symbol);
        }
    }

    /** Print the symbol table to the given stream. */
    void print(std::ostream& out) const {
#ifdef USE_MPI
        if (mpi::commRank() != 0) {
            mpi::send(0, mpi::tagOf("@SYMBOL_TABLE_PRINT"));
        } else
#endif
        {
            out << "SymbolTable: {\n\t";
            out << join(strToNum, "\n\t", [](std::ostream& out,
                                                  const std::pair<std::string, std::size_t>& entry) {
                out << entry.first << "\t => " << entry.second;
            }) << "\n";
            out << "}\n";
        }
    }

    Lock::Lease acquireLock() const {
        return access.acquire();
    }

    /** Stream operator, used as a convenience for print. */
    friend std::ostream& operator<<(std::ostream& out, const SymbolTable& table) {
        table.print(out);
        return out;
    }
};

}  // namespace souffle
