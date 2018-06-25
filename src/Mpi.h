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

// TODO (lyndonhenry): should remove all the warnings rather than ignoring the:
#pragma GCC diagnostic ignored "-Wunused-function"

#include <cassert>
#include <functional>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <mpi.h>

namespace souffle {

// TODO (lyndonhenry): should do documentation for this whole namespace
namespace mpi {

typedef std::unique_ptr<MPI_Request> Request;
typedef std::unique_ptr<MPI_Status> Status;

namespace {
template <typename T>
MPI_Datatype datatype();

template <>
MPI_Datatype datatype<short>() {
    return MPI_SHORT;
}

template <>
MPI_Datatype datatype<int>() {
    return MPI_INT;
}

template <>
MPI_Datatype datatype<long>() {
    return MPI_LONG;
}

template <>
MPI_Datatype datatype<long long>() {
    return MPI_LONG_LONG;
}

template <>
MPI_Datatype datatype<unsigned char>() {
    return MPI_UNSIGNED_CHAR;
}

template <>
MPI_Datatype datatype<unsigned short>() {
    return MPI_UNSIGNED_SHORT;
}

template <>
MPI_Datatype datatype<unsigned>() {
    return MPI_UNSIGNED;
}

template <>
MPI_Datatype datatype<unsigned long>() {
    return MPI_UNSIGNED_LONG;
}

template <>
MPI_Datatype datatype<unsigned long long>() {
    return MPI_UNSIGNED_LONG_LONG;
}

template <>
MPI_Datatype datatype<float>() {
    return MPI_FLOAT;
}

template <>
MPI_Datatype datatype<double>() {
    return MPI_DOUBLE;
}

template <>
MPI_Datatype datatype<long double>() {
    return MPI_LONG_DOUBLE;
}

template <>
MPI_Datatype datatype<char>() {
    return MPI_BYTE;
}
}

namespace {

int init(int argc, char* argv[]) {
    return MPI_Init(&argc, &argv);
}
}

namespace {

void finalize() {
    MPI_Finalize();
}
}

namespace {
int commSize() {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return size;
}
}

namespace {
int commRank() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
}
}

namespace {
template <typename T>
int getCount(std::unique_ptr<MPI_Status>& status) {
    assert(status);
    int count;
    MPI_Get_count(status.get(), datatype<T>(), &count);
    return count;
}
}

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
    MPI_Datatype firstType = datatype<int>();
    MPI_Pack_size((int)first.size(), firstType, MPI_COMM_WORLD, &firstPackSize);
    int secondPackSize;
    MPI_Datatype secondType = datatype<int>();
    MPI_Pack_size((int)second.size(), secondType, MPI_COMM_WORLD, &secondPackSize);
    int lastPackSize;
    MPI_Datatype lastType = datatype<char>();
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

namespace {

template <typename T>
void unpack(const std::vector<char>& oldData, T& newData);

template <>
void unpack<std::vector<std::string>>(const std::vector<char>& oldData, std::vector<std::string>& newData) {
    int position = 0;
    std::vector<int> first(1);
    MPI_Unpack(&oldData[0], (int)oldData.size(), &position, &first[0], (int)first.capacity(), datatype<int>(),
            MPI_COMM_WORLD);
    std::vector<int> second((size_t)first.at(0));
    MPI_Unpack(&oldData[0], (int)oldData.size(), &position, &second[0], (int)second.capacity(),
            datatype<int>(), MPI_COMM_WORLD);
    std::vector<char> last(oldData.size() - position);
    MPI_Unpack(&oldData[0], (int)oldData.size(), &position, &last[0], (int)last.capacity(), datatype<char>(),
            MPI_COMM_WORLD);
    auto from = last.begin();
    for (auto length : second) {
        auto to = from + length;
        newData.push_back(std::string(from, to));
        from = to;
    }
}
}

namespace {
Status test(const Request& request) {
    assert(request);
    int flag;
    auto status = Status(new MPI_Status());
    MPI_Test(request.get(), &flag, status.get());
    if (flag == 0) {
        status = Status(nullptr);
    } else {
        assert(status);
    }
    return status;
}

std::vector<Status> test(const std::unordered_set<Request>& requests) {
    auto statuses = std::vector<Status>(requests.size());
    size_t i = 0;
    for (const auto& request : requests) {
        statuses[i] = std::move(test(request));
        ++i;
    }
    return statuses;
}
}

namespace {
Status wait(const Request& request) {
    assert(request);
    auto status = Status(new MPI_Status());
    MPI_Wait(request.get(), status.get());
    assert(status);
    return status;
}

std::vector<Status> wait(const std::unordered_set<Request>& requests) {
    auto statuses = std::vector<Status>(requests.size());
    size_t i = 0;
    for (const auto& request : requests) {
        statuses[i] = std::move(wait(request));
        ++i;
    }
    return statuses;
}
}

namespace {
Status iprobe(const int source, const int tag) {
    auto status = Status(new MPI_Status());
    int flag;
    MPI_Iprobe(source, tag, MPI_COMM_WORLD, &flag, status.get());
    if (flag == 0) {
        status = Status(nullptr);
    } else {
        assert(status);
    }
    return status;
}

Status iprobe() {
    return iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG);
}

Status iprobe(const Status& status) {
    return iprobe(status->MPI_SOURCE, status->MPI_TAG);
}
}

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

namespace {
template <typename S>
Request isend(const std::vector<S>& data, const int destination, const int tag) {
    auto request = Request(new MPI_Request());
    MPI_Isend(&data[0], (int)data.size(), datatype<S>(), destination, tag, MPI_COMM_WORLD, request.get());
    return request;
}

template <>
Request isend<std::string>(const std::vector<std::string>& data, const int destination, const int tag) {
    std::vector<char> newData;
    pack(data, newData);
    auto request = Request(new MPI_Request());
    MPI_Isend(&newData[0], (int)newData.size(), MPI_PACKED, destination, tag, MPI_COMM_WORLD, request.get());
    return request;
}

template <typename S>
Request isend(const S& data, const int destination, const int tag) {
    std::vector<S> newData({data});
    return isend(newData, destination, tag);
}

template <typename S>
Request isend(const S& data, const Status& status) {
    return isend(data, status->MPI_SOURCE, status->MPI_TAG);
}

Request isend(const int destination, const int tag) {
    auto data = std::vector<char>(0);
    return isend(data, destination, tag);
}

Request isend(const Status& status) {
    return isend(status->MPI_SOURCE, status->MPI_TAG);
}

template <typename S>
std::unordered_set<Request> isend(const S& data, const std::unordered_set<int>& destinations, const int tag) {
    std::unordered_set<Request> requests(destinations.size());
    for (const auto& destination : destinations) {
        requests.insert(std::move(isend(data, destination, tag)));
    }
    return requests;
}

std::unordered_set<Request> isend(const std::unordered_set<int>& destinations, const int tag) {
    auto data = std::vector<char>(0);
    return isend(data, destinations, tag);
}

template <typename S, typename T>
Request isend(const T& data, const size_t length, const int destination, const int tag) {
    const auto destinations = std::vector<int>({destination});
    return isend<S>(data, length, destinations, tag);
}

template <typename S, typename T>
Request isend(const T& data, const size_t length, const Status& status) {
    return isend<S>(data, length, status->MPI_SOURCE, status->MPI_TAG);
}

template <typename S, typename T>
std::unordered_set<Request> isend(const T& data, const size_t length, const std::unordered_set<int>& destinations, const int tag) {
    std::vector<S> buffer(data.size() * length);
    size_t i = 0;
    for (const auto& element : data) {
        for (size_t j = 0; j < length; ++j) {
            buffer[i] = element[j];
        }
    }
    return isend(buffer, destinations, tag);
}
}

namespace {
template <typename S>
void send(const std::vector<S>& data, const int destination, const int tag) {
    MPI_Send(&data[0], (int)data.size(), datatype<S>(), destination, tag, MPI_COMM_WORLD);
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
    wait(isend(data, destinations, tag));
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

namespace {
// TODO (lyndonhenry): should maybe figure out how to do this namespace for irecv, not so easy with
// unpacking, may need callbacks, but might not even be useful
template <typename R>
Request irecv(std::vector<R>& data, Status& status) {
    assert(status);
    int count = getCount<R>(status);
    data.resize((size_t)count);
    auto request = Request(new MPI_Request());
    MPI_Irecv(&data[0], count, datatype<R>(), status->MPI_SOURCE, status->MPI_TAG, MPI_COMM_WORLD,
            request.get());
    return request;
}
}

namespace {
template <typename R>
void recv(std::vector<R>& data, Status& status) {
    assert(status);
    int count = getCount<R>(status);
    data.resize((size_t)count);
    MPI_Recv(&data[0], count, datatype<R>(), status->MPI_SOURCE, status->MPI_TAG, MPI_COMM_WORLD,
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

namespace {

// TODO (lyndonhenry): should maybe re-structure this namespace, and perhaps also for this whole file in
// general

namespace {

std::unordered_map<std::string, int> _tagOfMap;
const int tagOf(const std::string& name) {
    return _tagOfMap.insert(std::pair<std::string, int>(name, (int)_tagOfMap.size())).first->second;
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
    for (int i = rank - 1; i < _jobCount; i += mpi::commSize()) {
        jobs.insert(i);
    }
    return jobs;
}

const int rankOfJob(const int job) {
    return (job % commSize()) + 1;
}
}

namespace {

std::unordered_map<int, size_t> _countOfMap;
std::unordered_map<int, std::function<void(Status&)>> _callbackOfMap;
size_t _countingSemaphor;
// @TODO: must ensure that this works, maybe try passing the callback by reference
void xtest(const int source, const int tag, const size_t count, std::function<void(Status&)> callback) {
    // TODO (lyndonhenry): should somehow be able to use the source parameter to make things more efficient
    (void)source;
    if (!_countOfMap.insert(std::pair<int, size_t>(tag, count)).second) {
        // if the tag already exists, replace it
        _countingSemaphor -= _countOfMap.at(tag);
        _countOfMap[tag] = count;
    }
    _countingSemaphor += count;
    _callbackOfMap[tag] = callback;
}

void xtest() {
    while (_countingSemaphor) {
        // TODO (lyndonhenry): should somehow be able to use the status source to make things more efficient
        auto status = probe(MPI_ANY_SOURCE, MPI_ANY_TAG);
        const int tag = status->MPI_TAG;
        _callbackOfMap.at(tag)(status);
        size_t count = _countOfMap.at(tag);
        if (count > 0) {
            if (count == 1) {
                _callbackOfMap.erase(tag);
                _countOfMap.erase(tag);
            }
            --_countingSemaphor;
        }
    }
    _countOfMap.clear();
    _callbackOfMap.clear();
}
}

namespace {
std::unordered_set<Request> _requests;

void xwait(Request& request) {
    // @TODO: must make sure this actually works
    _requests.insert(std::move(request));
}

void xwait(std::unordered_set<Request>& requests) {
    // @TODO: must make sure this actually works
    // https://stackoverflow.com/questions/25515284/is-it-possible-to-move-an-item-out-of-a-stdset
    for (auto& request : requests) {
        _requests.insert(std::unique_ptr<MPI_Request>(request.release()));
    }
    requests.clear();
}

void xwait() {
    for (const auto& request : _requests) {
        wait(request);
    }
    _requests.clear();
}
}
}
}
}
