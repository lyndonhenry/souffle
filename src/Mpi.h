/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2018, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file Mpi.h
 *
 * C++ friendly support for MPI.
 *
 ***********************************************************************/

#pragma once

// TODO (lyndonhenry): shoulf remove the source of these warnings rather than just suppressing them
#pragma GCC diagnostic ignored "-Wunused-function"

#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <mpi.h>

// TODO (lyndonhenry): should add more debugging functionality here
#ifdef MPI_DEBUG

#define MPI_Send(data, size, type, destination, tag, communicator)                                \
    ([&]() {                                                                                      \
        int commRank, commSize;                                                                   \
        MPI_Comm_size(MPI_COMM_WORLD, &commSize);                                                 \
        MPI_Comm_rank(MPI_COMM_WORLD, &commRank);                                                 \
        std::cout << "(Mpi_Comm_rank = " << commRank << ", MPI_Comm_size = " << commSize << "): " \
                  << "MPI_Send(data = " << data << ", size = " << size << ", type = " << type     \
                  << ", destination = " << destination << ", tag = " << tag                       \
                  << ", communicator = " << communicator << ")" << std::endl;                     \
        MPI_Send(data, size, type, destination, tag, communicator);                               \
    })()

#define MPI_Recv(data, size, type, source, tag, communicator, status)                                    \
    ([&]() {                                                                                             \
        int commRank, commSize;                                                                          \
        MPI_Comm_size(MPI_COMM_WORLD, &commSize);                                                        \
        MPI_Comm_rank(MPI_COMM_WORLD, &commRank);                                                        \
        MPI_Recv(data, size, type, source, tag, communicator, status);                                   \
        std::cout << "(Mpi_Comm_rank = " << commRank << ", MPI_Comm_size = " << commSize << "): "        \
                  << "MPI_Recv(data = " << data << ", size = " << size << ", type = " << type            \
                  << ", source = " << source << ", tag = " << tag << ", communicator = " << communicator \
                  << ")" << std::endl;                                                                   \
    })()

#define MPI_Probe(source, tag, communicator, status)                                                     \
    ([&]() {                                                                                             \
        int commRank, commSize;                                                                          \
        MPI_Comm_size(MPI_COMM_WORLD, &commSize);                                                        \
        MPI_Comm_rank(MPI_COMM_WORLD, &commRank);                                                        \
        MPI_Probe(source, tag, communicator, status);                                                    \
        std::cout << "(Mpi_Comm_rank = " << commRank << ", MPI_Comm_size = " << commSize << "): "        \
                  << "MPI_Probe(source = " << status->MPI_SOURCE << ", tag = " << status->MPI_TAG << ")" \
                  << std::endl;                                                                          \
    })()

#endif

namespace souffle {

// TODO (lyndonhenry): should do documentation for this whole class
namespace mpi {

/* typedefs */
namespace {
typedef std::unique_ptr<MPI_Status> Status;
}

/* datatype */
namespace {
template <typename T>
class datatype;

template <>
class datatype<short> {
public:
    static inline MPI_Datatype get() {
        return MPI_SHORT;
    }
};
template <>
class datatype<int> {
public:
    static inline MPI_Datatype get() {
        return MPI_INT;
    }
};
template <>
class datatype<long> {
public:
    static inline MPI_Datatype get() {
        return MPI_LONG;
    }
};
template <>
class datatype<long long> {
public:
    static inline MPI_Datatype get() {
        return MPI_LONG_LONG;
    }
};
template <>
class datatype<unsigned char> {
public:
    static inline MPI_Datatype get() {
        return MPI_UNSIGNED_CHAR;
    }
};
template <>
class datatype<unsigned short> {
public:
    static inline MPI_Datatype get() {
        return MPI_UNSIGNED_SHORT;
    }
};
template <>
class datatype<unsigned> {
public:
    static inline MPI_Datatype get() {
        return MPI_UNSIGNED;
    }
};
template <>
class datatype<unsigned long> {
public:
    static inline MPI_Datatype get() {
        return MPI_UNSIGNED_LONG;
    }
};
template <>
class datatype<unsigned long long> {
public:
    static inline MPI_Datatype get() {
        return MPI_UNSIGNED_LONG_LONG;
    }
};
template <>
class datatype<float> {
public:
    static inline MPI_Datatype get() {
        return MPI_FLOAT;
    }
};
template <>
class datatype<double> {
public:
    static inline MPI_Datatype get() {
        return MPI_DOUBLE;
    }
};
template <>
class datatype<long double> {
public:
    static inline MPI_Datatype get() {
        return MPI_LONG_DOUBLE;
    }
};
template <>
class datatype<char> {
public:
    static inline MPI_Datatype get() {
        return MPI_BYTE;
    }
};
}

/* getCount */
namespace {
template <typename T>
int getCount(std::unique_ptr<MPI_Status>& status) {
    assert(status);
    int count;
    MPI_Get_count(status.get(), datatype<T>::get(), &count);
    return count;
}
}

/* pack */
namespace {
template <typename T>
void pack(const T& oldData, std::vector<char>& newData);

template <>
void pack<std::vector<std::string>>(const std::vector<std::string>& oldData, std::vector<char>& newData) {
    std::vector<int> first = {(int)oldData.size()};
    std::vector<int> second(oldData.size());
    std::vector<char> last;
    size_t i = 0;
    for (auto it = oldData.begin(); it != oldData.end(); ++it) {
        second[i] = (int)it->size();
        last.insert(last.end(), it->begin(), it->end());
        ++i;
    }
    int firstPackSize;
    MPI_Datatype firstType = datatype<int>::get();
    MPI_Pack_size((int)first.size(), firstType, MPI_COMM_WORLD, &firstPackSize);
    int secondPackSize;
    MPI_Datatype secondType = datatype<int>::get();
    MPI_Pack_size((int)second.size(), secondType, MPI_COMM_WORLD, &secondPackSize);
    int lastPackSize;
    MPI_Datatype lastType = datatype<char>::get();
    MPI_Pack_size((int)last.size(), lastType, MPI_COMM_WORLD, &lastPackSize);
    newData.resize((size_t)firstPackSize + secondPackSize + lastPackSize);
    int position = 0;
    MPI_Pack(&first[0], (int)first.size(), firstType, &newData[0], (int)newData.capacity(), &position,
            MPI_COMM_WORLD);
    MPI_Pack(&second[0], (int)second.size(), secondType, &newData[0], (int)newData.capacity(), &position,
            MPI_COMM_WORLD);
    MPI_Pack(&last[0], (int)last.size(), lastType, &newData[0], (int)newData.capacity(), &position,
            MPI_COMM_WORLD);
}
}

/* unpack */
namespace {
template <typename T>
void unpack(const std::vector<char>& oldData, T& newData);

template <>
void unpack<std::vector<std::string>>(const std::vector<char>& oldData, std::vector<std::string>& newData) {
    int position = 0;
    std::vector<int> first(1);
    MPI_Unpack(&oldData[0], (int)oldData.size(), &position, &first[0], (int)first.capacity(),
            datatype<int>::get(), MPI_COMM_WORLD);
    std::vector<int> second((size_t)first.at(0));
    MPI_Unpack(&oldData[0], (int)oldData.size(), &position, &second[0], (int)second.capacity(),
            datatype<int>::get(), MPI_COMM_WORLD);
    std::vector<char> last(oldData.size() - position);
    MPI_Unpack(&oldData[0], (int)oldData.size(), &position, &last[0], (int)last.capacity(),
            datatype<char>::get(), MPI_COMM_WORLD);
    auto from = last.begin();
    for (auto length : second) {
        auto to = from + length;
        newData.push_back(std::string(from, to));
        from = to;
    }
}
}

/* init */
namespace {
int init(int argc, char* argv[]) {
    auto flag = MPI_Init(&argc, &argv);
    return flag;
}
}

/* finalize */
namespace {

void finalize() {
    MPI_Finalize();
}
}

/* commSize */
namespace {
int commSize() {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    return size;
}
}

/* commRank */
namespace {
int commRank() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    return rank;
}
}

/* probe */
namespace {
Status probe(const int source, const int tag) {
    auto status = Status(new MPI_Status());
    MPI_Probe(source, tag, MPI_COMM_WORLD, status.get());
    assert(status);

    return status;
}

Status probe() {
    return probe(MPI_ANY_SOURCE, MPI_ANY_TAG);
}

Status probe(const Status& status) {
    return probe(status->MPI_SOURCE, status->MPI_TAG);
}
}

/* send */
namespace {
template <typename S>
void send(const std::vector<S>& data, const int destination, const int tag) {
    MPI_Send(&data[0], (int)data.size(), datatype<S>::get(), destination, tag, MPI_COMM_WORLD);
}

template <>
void send<std::string>(const std::vector<std::string>& data, const int destination, const int tag) {
    std::vector<char> newData;
    pack(data, newData);
    MPI_Send(&newData[0], (int)newData.size(), MPI_PACKED, destination, tag, MPI_COMM_WORLD);
}

template <typename S>
void send(const S& data, const int destination, const int tag) {
    std::vector<S> newData({data});
    send(newData, destination, tag);
}

template <typename S>
void send(const S& data, const Status& status) {
    send(data, status->MPI_SOURCE, status->MPI_TAG);
}

void send(const int destination, const int tag) {
    auto data = std::vector<char>(0);
    send(data, destination, tag);
}

void send(const Status& status) {
    send(status->MPI_SOURCE, status->MPI_TAG);
}

template <typename S>
void send(const S& data, const std::unordered_set<int>& destinations, const int tag) {
    for (const auto destination : destinations) {
        send(data, destination, tag);
    }
}

void send(const std::unordered_set<int>& destinations, const int tag) {
    auto data = std::vector<char>(0);
    send(data, destinations, tag);
}

template <typename S, typename T>
void send(const T& data, const size_t length, const int destination, const int tag) {
    const auto destinations = std::vector<int>({destination});
    send<S>(data, length, destinations, tag);
}

template <typename S, typename T>
void send(const T& data, const size_t length, const Status& status) {
    send<S>(data, length, status->MPI_SOURCE, status->MPI_TAG);
}

template <typename S, typename T>
void send(const T& data, const size_t length, const std::unordered_set<int>& destinations, const int tag) {
    std::vector<S> buffer(data.size() * length);
    size_t i = 0;
    for (const auto& element : data) {
        for (size_t j = 0; j < length; ++j) {
            buffer[i] = element[j];
        }
    }
    send(buffer, destinations, tag);
}
}

/* recv */
namespace {
template <typename R>
void recv(std::vector<R>& data, Status& status) {
    assert(status);
    int count = getCount<R>(status);
    data.resize((size_t)count);
    MPI_Recv(&data[0], count, datatype<R>::get(), status->MPI_SOURCE, status->MPI_TAG, MPI_COMM_WORLD,
            status.get());
}

template <>
void recv<std::string>(std::vector<std::string>& data, Status& status) {
    assert(status);
    int count = getCount<char>(status);
    std::vector<char> newData((size_t)count);
    MPI_Recv(&newData[0], count, MPI_PACKED, status->MPI_SOURCE, status->MPI_TAG, MPI_COMM_WORLD,
            status.get());
    unpack(newData, data);
}

template <typename R>
void recv(R& data, Status& status) {
    std::vector<R> newData;
    recv(newData, status);
    data = newData.at(0);
}

template <typename R>
void recv(R& data, const int source, const int tag) {
    auto status = probe(source, tag);
    recv(data, status);
}

template <typename R, typename T>
void recv(T& data, const size_t length, Status& status) {
    std::vector<R> newData;
    recv(newData, status);
    auto it = newData.begin();
    while (it != newData.end()) {
        auto element = std::unique_ptr<R[]>(new R[length]);
        for (size_t i = 0; i < length; ++i) {
            element[i] = *it;
            ++it;
        }
        const R* pointer = element.get();
        data.insert(pointer);
    }
}

template <typename R, typename T>
void recv(T& data, const size_t length, const int source, const int tag) {
    auto status = probe(source, tag);
    recv<R>(data, length, status);
}

template <typename T>
void recv(Status& status) {
    auto data = std::vector<T>(0);
    recv(data, status);
}

void recv(const int source, const int tag) {
    auto data = std::vector<char>(0);
    recv(data, source, tag);
}
}

/* custom */
namespace {
// TODO (lyndonhenry): everything from here are extensions to mpi, they should be moved elsewhere

namespace {

const int tagOf(const std::string& name) {
    // TODO (lyndonhenry): should do this less verbosely
    static std::unordered_map<std::string, int> _tagOfMap;
    auto it = _tagOfMap.find(name);
    if (it != _tagOfMap.end()) {
        return it->second;
    }
    _tagOfMap.insert(std::pair<std::string, int>(name, _tagOfMap.size()));
    return _tagOfMap.size();
}
}

namespace {
int _jobCount;

void numberOfJobs(const int count) {
    _jobCount = count;
}
const std::unordered_set<int> jobsOfRank(const int rank) {
    // TODO (lyndonhenry): should implement this more efficiently
    std::unordered_set<int> jobs;
    if (commSize() > 1) {
        if (commRank() == 0) {
            jobs.insert(-1);
        } else {
            for (int i = rank - 1; i < _jobCount; i += mpi::commSize()) {
                jobs.insert(i);
            }
        }
    } else {
        for (int i = -1; i < _jobCount; ++i) {
            jobs.insert(i);
        }
    }
    return jobs;
}

const int rankOfJob(const int job) {
    if (commSize() == 1 || job == -1) {
        return 0;
    }
    return (job % (commSize() - 1)) + 1;
}
}
}
}
}