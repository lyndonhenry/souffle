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

#include <cassert>
#include <functional>
#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <mpi.h>

#ifndef NDEBUG

#include <iostream>
#include <sstream>

#define MPI_Send(data, count, type, destination, tag, comm)                                  \
    {                                                                                        \
        int rank, size;                                                                      \
        MPI_Comm_rank(comm, &rank);                                                          \
        MPI_Comm_size(comm, &size);                                                          \
        std::stringstream ss;                                                                \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank    \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size \
           << "); MPI_Send(void* data = " << data << ", int count = " << count               \
           << ",  MPI_Datatype type = " << type << ", int destination = " << destination     \
           << ", int tag = " << tag << ", MPI_Comm comm = " << comm << ");" << std::endl;    \
        std::cerr << ss.str();                                                               \
        MPI_Send(data, count, type, destination, tag, comm);                                 \
    }

#define MPI_Isend(data, count, type, destination, tag, comm, request)                        \
    {                                                                                        \
        int rank, size;                                                                      \
        MPI_Comm_rank(comm, &rank);                                                          \
        MPI_Comm_size(comm, &size);                                                          \
        std::stringstream ss;                                                                \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank    \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size \
           << "); MPI_Isend(void* data = " << data << ", int count = " << count              \
           << ",  MPI_Datatype type = " << type << ", int destination = " << destination     \
           << ", int tag = " << tag << ", MPI_Comm comm = " << comm                          \
           << ", MPI_Request* request = " << request << ");" << std::endl;                   \
        std::cerr << ss.str();                                                               \
        MPI_Isend(data, count, type, destination, tag, comm, request);                       \
    }

#define MPI_Ssend(data, count, type, destination, tag, comm)                                 \
    {                                                                                        \
        int rank, size;                                                                      \
        MPI_Comm_rank(comm, &rank);                                                          \
        MPI_Comm_size(comm, &size);                                                          \
        std::stringstream ss;                                                                \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank    \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size \
           << "); MPI_Ssend(void* data = " << data << ", int count = " << count              \
           << ",  MPI_Datatype type = " << type << ", int destination = " << destination     \
           << ", int tag = " << tag << ", MPI_Comm comm = " << comm << ");" << std::endl;    \
        std::cerr << ss.str();                                                               \
        MPI_Ssend(data, count, type, destination, tag, comm);                                \
    }

#define MPI_Irecv(data, count, type, source, tag, comm, request)                                           \
    {                                                                                                      \
        int rank, size;                                                                                    \
        MPI_Comm_rank(comm, &rank);                                                                        \
        MPI_Comm_size(comm, &size);                                                                        \
        std::stringstream ss;                                                                              \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank                  \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size               \
           << "); MPI_Irecv(void* data = " << data << ", int count = " << count                            \
           << ",  MPI_Datatype type = " << type << ", int source = " << source << ", int tag = " << tag    \
           << ", MPI_Comm comm = " << comm << ", MPI_Request* request = " << request << ");" << std::endl; \
        std::cerr << ss.str();                                                                             \
        MPI_Irecv(data, count, type, source, tag, comm, request);                                          \
    }

#define MPI_Recv(data, count, type, source, tag, comm, status)                                           \
    {                                                                                                    \
        int rank, size;                                                                                  \
        MPI_Comm_rank(comm, &rank);                                                                      \
        MPI_Comm_size(comm, &size);                                                                      \
        std::stringstream ss;                                                                            \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank                \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size             \
           << "); MPI_Recv(void* data = " << data << ", int count = " << count                           \
           << ",  MPI_Datatype type = " << type << ", int source = " << source << ", int tag = " << tag  \
           << ", MPI_Comm comm = " << comm << ", MPI_Request* status = " << status << ");" << std::endl; \
        std::cerr << ss.str();                                                                           \
        MPI_Recv(data, count, type, source, tag, comm, status);                                          \
    }

#define MPI_Probe(source, tag, comm, status)                                                             \
    {                                                                                                    \
        int rank, size;                                                                                  \
        MPI_Comm_rank(comm, &rank);                                                                      \
        MPI_Comm_size(comm, &size);                                                                      \
        std::stringstream ss;                                                                            \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank                \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size             \
           << "); MPI_Probe(int source = " << source << ", int tag = " << tag                            \
           << ", MPI_Comm comm = " << comm << ", MPI_Request* status = " << status << ");" << std::endl; \
        std::cerr << ss.str();                                                                           \
        MPI_Probe(source, tag, comm, status);                                                            \
    }

#define MPI_Iprobe(source, tag, comm, flag, status)                                          \
    {                                                                                        \
        int rank, size;                                                                      \
        MPI_Comm_rank(comm, &rank);                                                          \
        MPI_Comm_size(comm, &size);                                                          \
        std::stringstream ss;                                                                \
        ss << "MPI_Comm_rank(MPI_Communicator comm = " << comm << ", int rank = " << rank    \
           << "); MPI_Comm_size(MPI_Communicator comm = " << comm << ", int size = " << size \
           << "); MPI_Iprobe(int source = " << source << ", int tag = " << tag               \
           << ", MPI_Comm comm = " << comm << ", int* flag = " << flag                       \
           << ", MPI_Request* status = " << status << ");" << std::endl;                     \
        std::cerr << ss.str();                                                               \
        MPI_Iprobe(source, tag, comm, flag, status);                                         \
    }

#endif

namespace souffle {

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
inline int getCount(std::unique_ptr<MPI_Status>& status) {
    assert(status);
    int count;
    MPI_Get_count(status.get(), datatype<T>::get(), &count);
    return count;
}
}

/* pack */
namespace {
template <typename T>
inline void pack(const T& oldData, std::vector<char>& newData);

template <>
inline void pack<std::vector<std::string>>(
        const std::vector<std::string>& oldData, std::vector<char>& newData) {
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
inline void unpack(const std::vector<char>& oldData, T& newData);

template <>
inline void unpack<std::vector<std::string>>(
        const std::vector<char>& oldData, std::vector<std::string>& newData) {
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
inline int init(int argc, char* argv[]) {
    auto flag = MPI_Init(&argc, &argv);
    return flag;
}
}

/* finalize */
namespace {

inline void finalize() {
    MPI_Finalize();
}
}

/* commSize */
namespace {
inline int commSize() {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    return size;
}
}

/* commRank */
namespace {
inline int commRank() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    return rank;
}
}

/* probe */
namespace {
inline Status probe(const int source, const int tag) {
    auto status = Status(new MPI_Status());
    MPI_Probe(source, tag, MPI_COMM_WORLD, status.get());
    assert(status);

    return status;
}

inline Status probe() {
    return probe(MPI_ANY_SOURCE, MPI_ANY_TAG);
}

inline Status probe(const Status& status) {
    return probe(status->MPI_SOURCE, status->MPI_TAG);
}
}

/* iprobe */
namespace {

inline Status iprobe(const int source, const int tag) {
    int flag;
    auto status = Status(new MPI_Status());
    MPI_Iprobe(source, tag, MPI_COMM_WORLD, &flag, status.get());
    return status;
}

inline Status iprobe() {
    return iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG);
}

inline Status iprobe(const Status& status) {
    return iprobe(status->MPI_SOURCE, status->MPI_TAG);
}
}

/* send */
namespace {
inline void send(const void* data, const int count, const MPI_Datatype type, const int destination,
        const int tag, const MPI_Comm comm) {
    MPI_Ssend(data, count, type, destination, tag, comm);
}

template <typename S>
inline void send(const std::vector<S>& data, const int destination, const int tag) {
    send(&data[0], (int)data.size(), datatype<S>::get(), destination, tag, MPI_COMM_WORLD);
}

template <>
inline void send<std::string>(const std::vector<std::string>& data, const int destination, const int tag) {
    std::vector<char> newData;
    pack(data, newData);
    send(&newData[0], (int)newData.size(), MPI_PACKED, destination, tag, MPI_COMM_WORLD);
}

template <typename S>
inline void send(const S& data, const int destination, const int tag) {
    std::vector<S> newData({data});
    send(newData, destination, tag);
}

template <typename S>
inline void send(const S& data, const Status& status) {
    send(data, status->MPI_SOURCE, status->MPI_TAG);
}

inline void send(const int destination, const int tag) {
    auto data = std::vector<char>(0);
    send(data, destination, tag);
}

inline void send(const Status& status) {
    send(status->MPI_SOURCE, status->MPI_TAG);
}

template <typename S>
inline void send(const S& data, const std::unordered_set<int>& destinations, const int tag) {
    for (const auto destination : destinations) {
        send(data, destination, tag);
    }
}

inline void send(const std::unordered_set<int>& destinations, const int tag) {
    auto data = std::vector<char>(0);
    send(data, destinations, tag);
}

template <typename S, typename T>
inline void send(const T& data, const size_t length, const int destination, const int tag) {
    const auto destinations = std::unordered_set<int>({destination});
    send<S>(data, length, destinations, tag);
}

template <typename S, typename T>
inline void send(const T& data, const size_t length, const Status& status) {
    send<S>(data, length, status->MPI_SOURCE, status->MPI_TAG);
}

template <typename S, typename T>
inline void send(
        const T& data, const size_t length, const std::unordered_set<int>& destinations, const int tag) {
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
inline void recv(void* data, const int count, const MPI_Datatype type, const int source, const int tag,
        const MPI_Comm comm, MPI_Status* status) {
    assert(source != commRank());
    MPI_Recv(data, count, type, source, tag, comm, status);
}

template <typename R>
inline void recv(std::vector<R>& data, Status& status) {
    assert(status);
    int count = getCount<R>(status);
    data.resize((size_t)count);
    recv(&data[0], count, datatype<R>::get(), status->MPI_SOURCE, status->MPI_TAG, MPI_COMM_WORLD,
            status.get());
}

template <>
inline void recv<std::string>(std::vector<std::string>& data, Status& status) {
    assert(status);
    int count = getCount<char>(status);
    std::vector<char> newData((size_t)count);
    recv(&newData[0], count, MPI_PACKED, status->MPI_SOURCE, status->MPI_TAG, MPI_COMM_WORLD, status.get());
    unpack(newData, data);
}

template <typename R>
inline void recv(R& data, Status& status) {
    std::vector<R> newData;
    recv(newData, status);
    data = newData.at(0);
}

template <typename R>
inline void recv(R& data, const int source, const int tag) {
    auto status = probe(source, tag);
    recv(data, status);
}

template <typename R, typename T>
inline void recv(T& data, const size_t length, Status& status) {
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
inline void recv(T& data, const size_t length, const int source, const int tag) {
    auto status = probe(source, tag);
    recv<R>(data, length, status);
}

template <typename T>
inline void recv(Status& status) {
    auto data = std::vector<T>(0);
    recv(data, status);
}

inline void recv(const int source, const int tag) {
    auto data = std::vector<char>(0);
    recv(data, source, tag);
}
}
}
}