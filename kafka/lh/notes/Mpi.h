#pragma once

#include </usr/include/openmpi/mpi.h>

#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

namespace mpi {
int comm_rank();
int comm_size();
void finalize();
int init(int, char**);
std::pair<bool, MPI_Status> iprobe(int, int);
MPI_Request irecv(std::vector<char>&, const MPI_Status&);
MPI_Request issend(const std::vector<char>&, int, int);
template <typename T>
void pack(const T&, std::vector<char>&);
MPI_Status probe(int, int);
template <typename T>
void read(std::istream&, T&);
bool test(MPI_Request&);
template <typename T>
void unpack(const std::vector<char>&, T&);
void wait(MPI_Request&);
template <typename T>
void write(std::ostream&, const T&);
}  // namespace mpi

namespace mpi {
namespace detail {
template <typename data_type>
struct is_datatype;
template <typename data_type>
MPI_Datatype datatype();
int get_count(const MPI_Status&);
template <typename T>
int pack_size(const T&);
}  // namespace detail
}  // namespace mpi

namespace mpi {
namespace detail {
template <typename T>
struct is_datatype : public std::false_type {};
template <>
struct is_datatype<short> : public std::true_type {};
template <>
struct is_datatype<int> : public std::true_type {};
template <>
struct is_datatype<long> : public std::true_type {};
template <>
struct is_datatype<long long> : public std::true_type {};
template <>
struct is_datatype<unsigned char> : public std::true_type {};
template <>
struct is_datatype<unsigned short> : public std::true_type {};
template <>
struct is_datatype<unsigned> : public std::true_type {};
template <>
struct is_datatype<unsigned long> : public std::true_type {};
template <>
struct is_datatype<unsigned long long> : public std::true_type {};
template <>
struct is_datatype<float> : public std::true_type {};
template <>
struct is_datatype<double> : public std::true_type {};
template <>
struct is_datatype<long double> : public std::true_type {};
template <>
struct is_datatype<char> : public std::true_type {};
}  // namespace detail
}  // namespace mpi

namespace mpi {
int comm_rank() {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    return rank;
}
}  // namespace mpi

namespace mpi {
int comm_size() {
    int size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    return size;
}
}  // namespace mpi

namespace mpi {
namespace detail {
template <typename data_type>
MPI_Datatype datatype() {
    return MPI_PACKED;
}
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
    return MPI_CHAR;
}
}  // namespace detail
}  // namespace mpi

namespace mpi {
void finalize() {
    MPI_Finalize();
}
}  // namespace mpi

namespace mpi {
namespace detail {
int get_count(const MPI_Status& status) {
    int count;
    MPI_Get_count(&status, MPI_PACKED, &count);
    return count;
}
}  // namespace detail
}  // namespace mpi

namespace mpi {
int init(int argc, char** argv) {
    return MPI_Init(&argc, &argv);
}
}  // namespace mpi

namespace mpi {
std::pair<bool, MPI_Status> iprobe(const int source, const int tag) {
    int flag;
    MPI_Status status;
    MPI_Iprobe(source, tag, MPI_COMM_WORLD, &flag, &status);
    return std::make_pair(static_cast<bool>(flag), status);
}
}  // namespace mpi

namespace mpi {
MPI_Request irecv(std::vector<char>& data, const MPI_Status& status) {
    MPI_Request request;
    const auto size = detail::get_count(status);
    data.resize(static_cast<size_t>(size));
    MPI_Irecv(data.data(), size, MPI_PACKED, status.MPI_SOURCE, status.MPI_TAG, MPI_COMM_WORLD, &request);
    return request;
}
}  // namespace mpi

namespace mpi {
MPI_Request issend(const std::vector<char>& data, const int destination, const int tag) {
    MPI_Request request;
    MPI_Issend(data.data(), static_cast<int>(data.size()), MPI_PACKED, destination, tag, MPI_COMM_WORLD,
            &request);
    return request;
}
}  // namespace mpi

namespace mpi {
namespace detail {
namespace pack_detail {
template <typename T, std::enable_if_t<is_datatype<T>::value, int> = 0>
void pack(const T& inData, std::vector<char>& outData, int& position) {
    MPI_Pack(&inData, 1, datatype<T>(), outData.data(), static_cast<int>(outData.size()), &position,
            MPI_COMM_WORLD);
}
template <typename T, std::enable_if_t<is_datatype<T>::value, int> = 0>
void pack(const std::vector<T>& inData, std::vector<char>& outData, int& position) {
    pack(static_cast<int>(inData.size()), outData, position);
    MPI_Pack(inData.data(), static_cast<int>(inData.size()), datatype<T>(), outData.data(),
            static_cast<int>(outData.size()), &position, MPI_COMM_WORLD);
}
void pack(const bool inData, std::vector<char>& outData, int& position) {
    return pack(static_cast<char>(inData), outData, position);
}
void pack(const std::vector<bool>& inData, std::vector<char>& outData, int& position) {
    std::vector<char> inBuffer(inData.size());
    for (size_t i = 0; i < inData.size(); ++i) {
        inBuffer[i] = static_cast<char>(inData.at(i));
    }
    pack(inBuffer, outData, position);
}
void pack(const std::string& inData, std::vector<char>& outData, int& position) {
    pack(std::vector<char>(inData.begin(), inData.end()), outData, position);
}
template <typename T, std::enable_if_t<!is_datatype<T>::value, int> = 0>
void pack(const std::vector<T>& inData, std::vector<char>& outData, int& position) {
    pack(static_cast<int>(inData.size()), outData, position);
    for (const auto& element : inData) {
        pack(element, outData, position);
    }
}
template <typename... Ts>
void pack(const std::tuple<Ts...>& inData, std::vector<char>& outData, int& position);
template <size_t index, typename... Ts>
void pack(const std::tuple<Ts...>& inData, std::vector<char>& outData, int& position,
        typename std::enable_if_t<index == 0>* = nullptr) {
    pack(std::get<0>(inData), outData, position);
}
template <size_t index, typename... Ts>
void pack(const std::tuple<Ts...>& inData, std::vector<char>& outData, int& position,
        typename std::enable_if_t<index != 0>* = nullptr) {
    pack<index - 1, Ts...>(inData, outData, position);
    pack(std::get<index>(inData), outData, position);
}
template <typename... Ts>
void pack(const std::tuple<Ts...>& inData, std::vector<char>& outData, int& position) {
    pack<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(inData, outData, position);
}
}  // namespace pack_detail
}  // namespace detail
template <typename T>
void pack(const T& inData, std::vector<char>& outData) {
    outData.resize(static_cast<size_t>(detail::pack_size(inData)));
    int position = 0;
    detail::pack_detail::pack(inData, outData, position);
}
}  // namespace mpi

namespace mpi {
namespace detail {
namespace pack_size_detail {
template <typename T, std::enable_if_t<is_datatype<T>::value, int> = 0>
int pack_size(const T& data) {
    (void)data;
    int size;
    MPI_Pack_size(1, datatype<T>(), MPI_COMM_WORLD, &size);
    return size;
}
template <typename T, std::enable_if_t<is_datatype<T>::value, int> = 0>
int pack_size(const std::vector<T>& data) {
    int size;
    MPI_Pack_size(static_cast<int>(data.size()), datatype<T>(), MPI_COMM_WORLD, &size);
    return pack_size(int()) + size;
}
int pack_size(const bool data) {
    return pack_size(static_cast<char>(data));
}
int pack_size(const std::vector<bool>& data) {
    std::vector<char> buffer(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        buffer[i] = static_cast<char>(data.at(i));
    }
    return pack_size(buffer);
}
int pack_size(const std::string& data) {
    return pack_size(std::vector<char>(data.begin(), data.end()));
}
template <typename T, std::enable_if_t<!is_datatype<T>::value, int> = 0>
int pack_size(const std::vector<T>& data) {
    int size = 0;
    for (const auto& element : data) {
        size += pack_size(element);
    }
    return pack_size(int()) + size;
}
template <typename... Ts>
int pack_size(const std::tuple<Ts...>& data);
template <size_t index, typename... Ts>
int pack_size(const std::tuple<Ts...>& data, typename std::enable_if_t<index == 0>* = nullptr) {
    return pack_size(std::get<0>(data));
}
template <size_t index, typename... Ts>
int pack_size(const std::tuple<Ts...>& data, typename std::enable_if_t<index != 0>* = nullptr) {
    return pack_size<index - 1, Ts...>(data) + pack_size(std::get<index>(data));
}
template <typename... Ts>
int pack_size(const std::tuple<Ts...>& data) {
    return pack_size<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(data);
}
}  // namespace pack_size_detail
template <typename T>
int pack_size(const T& data) {
    return pack_size_detail::pack_size(data);
}
}  // namespace detail
}  // namespace mpi

namespace mpi {
MPI_Status probe(const int source, const int tag) {
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    return status;
}
}  // namespace mpi

namespace mpi {
namespace detail {
namespace read_detail {
template <typename T>
void read(std::istream& istream, T& data) {
    istream >> data;
}
void read(std::istream& istream, std::string& data) {
    char character;
    istream >> std::skipws >> character;
    assert(character == '"');
    bool escaped = false;
    for (;;) {
        istream >> std::noskipws >> character;
        if (escaped) {
            data.push_back(character);
            escaped = false;
        } else if (character == '\\') {
            escaped = true;
        } else if (character == '"') {
            istream >> std::skipws;
            return;
        } else {
            data.push_back(character);
        }
    }
}
template <typename T>
void read(std::istream& istream, std::vector<T>& data) {
    size_t size;
    istream >> size;
    data.resize(size);
    for (auto& item : data) {
        read(istream, item);
    }
}
void read(std::istream& istream, std::vector<bool>& data) {
    size_t size;
    istream >> size;
    data.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        bool item;
        istream >> item;
        data.push_back(item);
    }
}
template <typename... Ts>
void read(std::istream& istream, std::tuple<Ts...>& data);
template <size_t index, typename... Ts>
void read(std::istream& istream, std::tuple<Ts...>& data, typename std::enable_if_t<index == 0>* = nullptr) {
    read(istream, std::get<0>(data));
}
template <size_t index, typename... Ts>
void read(std::istream& istream, std::tuple<Ts...>& data, typename std::enable_if_t<index != 0>* = nullptr) {
    read<index - 1, Ts...>(istream, data);
    read(istream, std::get<index>(data));
}
template <typename... Ts>
void read(std::istream& istream, std::tuple<Ts...>& data) {
    read<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(istream, data);
}
}  // namespace read_detail
}  // namespace detail
template <typename T>
void read(std::istream& istream, T& data) {
    detail::read_detail::read(istream, data);
}
}  // namespace mpi

namespace mpi {
bool test(MPI_Request& request) {
    int flag;
    MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
    return static_cast<bool>(flag);
}
}  // namespace mpi

namespace mpi {
namespace detail {
namespace unpack_detail {
template <typename data_type, std::enable_if_t<is_datatype<data_type>::value, int> = 0>
void unpack(const std::vector<char>& inData, int& position, data_type& outData) {
    MPI_Unpack(inData.data(), static_cast<int>(inData.size()), &position, &outData, 1, datatype<data_type>(),
            MPI_COMM_WORLD);
}
template <typename data_type, std::enable_if_t<is_datatype<data_type>::value, int> = 0>
void unpack(const std::vector<char>& inData, int& position, std::vector<data_type>& outData) {
    int outDataSize;
    unpack(inData, position, outDataSize);
    outData.resize(static_cast<size_t>(outDataSize));
    MPI_Unpack(inData.data(), static_cast<int>(inData.size()), &position, outData.data(),
            static_cast<int>(outData.size()), datatype<data_type>(), MPI_COMM_WORLD);
}
void unpack(const std::vector<char>& inData, int& position, bool& outData) {
    char outBuffer;
    unpack(inData, position, outBuffer);
    outData = static_cast<bool>(outBuffer);
}
void unpack(const std::vector<char>& inData, int& position, std::vector<bool>& outData) {
    std::vector<char> outBuffer;
    unpack(inData, position, outBuffer);
    outData.reserve(outBuffer.size());
    for (size_t i = 0; i < outBuffer.size(); ++i) {
        outData.push_back(static_cast<bool>(outBuffer.at(i)));
    }
}
void unpack(const std::vector<char>& inData, int& position, std::string& outData) {
    std::vector<char> outBuffer;
    unpack(inData, position, outBuffer);
    outData = std::string(outBuffer.begin(), outBuffer.end());
}
template <typename data_type, std::enable_if_t<!is_datatype<data_type>::value, int> = 0>
void unpack(const std::vector<char>& inData, int& position, std::vector<data_type>& outData) {
    int outDataSize;
    unpack(inData, position, outDataSize);
    outData.resize(static_cast<size_t>(outDataSize));
    for (size_t i = 0; i < outData.size(); ++i) {
        unpack(inData, position, outData[i]);
    }
}
template <typename... Ts>
void unpack(const std::vector<char>& inData, int& position, std::tuple<Ts...>& outData);
template <size_t index, typename... Ts>
void unpack(const std::vector<char>& inData, int& position, std::tuple<Ts...>& outData,
        typename std::enable_if_t<index == 0>* = nullptr) {
    unpack(inData, position, std::get<0>(outData));
}
template <size_t index, typename... Ts>
void unpack(const std::vector<char>& inData, int& position, std::tuple<Ts...>& outData,
        typename std::enable_if_t<index != 0>* = nullptr) {
    unpack<index - 1, Ts...>(inData, position, outData);
    unpack(inData, position, std::get<index>(outData));
}
template <typename... Ts>
void unpack(const std::vector<char>& inData, int& position, std::tuple<Ts...>& outData) {
    unpack<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(inData, position, outData);
}
}  // namespace unpack_detail
}  // namespace detail
template <typename T>
void unpack(const std::vector<char>& inData, T& outData) {
    int position = 0;
    detail::unpack_detail::unpack(inData, position, outData);
}
}  // namespace mpi

namespace mpi {
void wait(MPI_Request& request) {
    MPI_Wait(&request, MPI_STATUS_IGNORE);
}
}  // namespace mpi

namespace mpi {
namespace detail {
namespace write_detail {
template <typename T>
void write(std::ostream& ostream, const T& data) {
    ostream << data;
}
void write(std::ostream& ostream, const std::string& data) {
    ostream << '"' << data << '"';
}
template <typename T>
void write(std::ostream& ostream, const std::vector<T>& data) {
    ostream << data.size();
    for (const auto& item : data) {
        ostream << " ";
        write(ostream, item);
    }
}

void write(std::ostream& ostream, const std::vector<bool>& data) {
    ostream << data.size();
    for (size_t i = 0; i < data.size(); ++i) {
        ostream << " ";
        ostream << data.at(i);
    }
}
template <typename... Ts>
void write(std::ostream& ostream, const std::tuple<Ts...>& data);
template <size_t index, typename... Ts>
void write(std::ostream& ostream, const std::tuple<Ts...>& data,
        typename std::enable_if_t<index == 0>* = nullptr) {
    write(ostream, std::get<0>(data));
}
template <size_t index, typename... Ts>
void write(std::ostream& ostream, const std::tuple<Ts...>& data,
        typename std::enable_if_t<index != 0>* = nullptr) {
    write<index - 1, Ts...>(ostream, data);
    ostream << " ";
    write(ostream, std::get<index>(data));
}
template <typename... Ts>
void write(std::ostream& ostream, const std::tuple<Ts...>& data) {
    write<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(ostream, data);
}
}  // namespace write_detail
}  // namespace detail
template <typename T>
void write(std::ostream& ostream, const T& data) {
    detail::write_detail::write(ostream, data);
}
}  // namespace mpi
