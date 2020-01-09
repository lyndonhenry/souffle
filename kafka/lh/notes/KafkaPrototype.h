#pragma once

#define BREAKPOINT             \
    (std::cerr << "BREAKPOINT" \
               << ":" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << std::endl)

#define BEGIN_TEST()           \
    (std::cerr << "BEGIN_TEST" \
               << ":" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << std::endl)
#define END_TEST()           \
    (std::cerr << "END_TEST" \
               << ":" << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << std::endl)

#include <istream>

namespace souffle {
namespace kafka {
namespace other {
template <typename T>
void read(std::istream&, T&);
}
}  // namespace kafka
}  // namespace souffle

#include <array>
#include <cassert>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace souffle {
namespace kafka {
namespace other {
namespace detail {
class read_detail {
public:
    template <typename T>
    static void read(std::istream& istream, T& data) {
        istream >> data;
    }
    static void read(std::istream& istream, std::string& data) {
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
    static void read(std::istream& istream, std::vector<T>& data) {
        size_t size;
        istream >> size;
        data.resize(size);
        for (auto& item : data) {
            read(istream, item);
        }
    }
    static void read(std::istream& istream, std::vector<bool>& data) {
        size_t size;
        istream >> size;
        data.reserve(size);
        for (size_t i = 0; i < size; ++i) {
            bool item;
            istream >> item;
            data.push_back(item);
        }
    }
    template <typename T>
    static void read(std::istream& istream, std::set<T>& data) {
        std::size_t size;
        istream >> size;
        for (std::size_t i = 0; i < size; ++i) {
            T item;
            read(istream, item);
            data.insert(item);
        }
    }
    template <typename T>
    static void read(std::istream& istream, std::unordered_set<T>& data) {
        std::size_t size;
        istream >> size;
        for (std::size_t i = 0; i < size; ++i) {
            T item;
            read(istream, item);
            data.insert(item);
        }
    }
    template <typename T1, typename T2>
    static void read(std::istream& istream, std::map<T1, T2>& data) {
        std::size_t size;
        istream >> size;
        for (std::size_t i = 0; i < size; ++i) {
            T1 first;
            read(istream, first);
            T2 second;
            read(istream, second);
            data[first] = second;
        }
    }
    template <typename T1, typename T2>
    static void read(std::istream& istream, std::unordered_map<T1, T2>& data) {
        std::size_t size;
        istream >> size;
        for (std::size_t i = 0; i < size; ++i) {
            T1 first;
            read(istream, first);
            T2 second;
            read(istream, second);
            data[first] = second;
        }
    }
    template <typename T, std::size_t size>
    static void read(std::istream& istream, std::array<T, size>& data) {
        for (auto& item : data) {
            read(istream, item);
        }
    }
    template <size_t index, typename... Ts>
    static void read(std::istream& istream, std::tuple<Ts...>& data,
            typename std::enable_if_t<index == 0>* = nullptr) {
        read(istream, std::get<0>(data));
    }
    template <size_t index, typename... Ts>
    static void read(std::istream& istream, std::tuple<Ts...>& data,
            typename std::enable_if_t<index != 0>* = nullptr) {
        read<index - 1, Ts...>(istream, data);
        read(istream, std::get<index>(data));
    }
    template <typename... Ts>
    static void read(std::istream& istream, std::tuple<Ts...>& data) {
        read<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(istream, data);
    }
};
}  // namespace detail
template <typename T>
void read(std::istream& istream, T& data) {
    detail::read_detail::read(istream, data);
}
}  // namespace other
}  // namespace kafka
}  // namespace souffle

#include <ostream>

namespace souffle {
namespace kafka {
namespace other {
template <typename T>
void write(std::ostream&, const T&);
}
}  // namespace kafka
}  // namespace souffle

#include <array>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace souffle {
namespace kafka {
namespace other {
namespace detail {
class write_detail {
public:
    template <typename T>
    static void write(std::ostream& ostream, const T& data) {
        ostream << data;
    }
    static void write(std::ostream& ostream, const std::string& data) {
        ostream << '"' << data << '"';
    }
    template <typename T>
    static void write(std::ostream& ostream, const std::vector<T>& data) {
        ostream << data.size();
        for (const auto& item : data) {
            ostream << " ";
            write(ostream, item);
        }
    }
    static void write(std::ostream& ostream, const std::vector<bool>& data) {
        ostream << data.size();
        for (size_t i = 0; i < data.size(); ++i) {
            ostream << " ";
            ostream << data.at(i);
        }
    }
    template <typename T>
    static void write(std::ostream& ostream, const std::set<T>& data) {
        ostream << data.size();
        for (const auto& item : data) {
            ostream << " ";
            write(ostream, item);
        }
    }
    template <typename T>
    static void write(std::ostream& ostream, const std::unordered_set<T>& data) {
        ostream << data.size();
        for (const auto& item : data) {
            ostream << " ";
            write(ostream, item);
        }
    }
    template <typename T1, typename T2>
    static void write(std::ostream& ostream, const std::map<T1, T2>& data) {
        ostream << data.size();
        for (const auto& item : data) {
            ostream << " ";
            write(ostream, item.first);
            ostream << " ";
            write(ostream, item.second);
        }
    }
    template <typename T1, typename T2>
    static void write(std::ostream& ostream, const std::unordered_map<T1, T2>& data) {
        ostream << data.size();
        for (const auto& item : data) {
            ostream << " ";
            write(ostream, item.first);
            ostream << " ";
            write(ostream, item.second);
        }
    }
    template <typename T, std::size_t size>
    static void write(std::ostream& ostream, const std::array<T, size>& data) {
        for (const auto& item : data) {
            ostream << " ";
            write(ostream, item);
        }
    }
    template <size_t index, typename... Ts>
    static void write(std::ostream& ostream, const std::tuple<Ts...>& data,
            typename std::enable_if_t<index == 0>* = nullptr) {
        write(ostream, std::get<0>(data));
    }
    template <size_t index, typename... Ts>
    static void write(std::ostream& ostream, const std::tuple<Ts...>& data,
            typename std::enable_if_t<index != 0>* = nullptr) {
        write<index - 1, Ts...>(ostream, data);
        ostream << " ";
        write(ostream, std::get<index>(data));
    }
    template <typename... Ts>
    static void write(std::ostream& ostream, const std::tuple<Ts...>& data) {
        write<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(ostream, data);
    }
};
}  // namespace detail
template <typename T>
void write(std::ostream& ostream, const T& data) {
    detail::write_detail::write(ostream, data);
}
}  // namespace other
}  // namespace kafka
}  // namespace souffle

#include <array>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace souffle {
namespace kafka {
namespace other {
namespace detail {
class write_json_detail {
public:
    template <typename T>
    static void write(std::ostream& ostream, const T& data) {
        ostream << data;
        ostream.flush();
    }
    static void write(std::ostream& ostream, const std::string& data) {
        ostream << '"' << data << '"';
        ostream.flush();
    }
    template <typename T>
    static void write(std::ostream& ostream, const std::vector<T>& data) {
        ostream << "[";
        if (!data.empty()) {
            auto item = data.begin();
            write(ostream, *item);
            ++item;
            for (; item != data.end(); ++item) {
                ostream << ",";
                write(ostream, *item);
            }
        }
        ostream << "]";
        ostream.flush();
    }
    static void write(std::ostream& ostream, const std::vector<bool>& data) {
        ostream << "[";
        if (!data.empty()) {
            std::size_t i = 0;
            ostream << data.at(i);
            ++i;
            for (; i < data.size(); ++i) {
                ostream << ",";
                ostream << data.at(i);
            }
        }
        ostream << "]";
        ostream.flush();
    }
    template <typename T>
    static void write(std::ostream& ostream, const std::set<T>& data) {
        ostream << "[";
        if (!data.empty()) {
            auto item = data.begin();
            write(ostream, *item);
            ++item;
            for (; item != data.end(); ++item) {
                ostream << ",";
                write(ostream, *item);
            }
        }
        ostream << "]";
        ostream.flush();
    }
    template <typename T>
    static void write(std::ostream& ostream, const std::unordered_set<T>& data) {
        ostream << "[";
        if (!data.empty()) {
            auto item = data.begin();
            write(ostream, *item);
            ++item;
            for (; item != data.end(); ++item) {
                ostream << ",";
                write(ostream, *item);
            }
        }
        ostream << "]";
        ostream.flush();
    }
    template <typename T1, typename T2>
    static void write(std::ostream& ostream, const std::map<T1, T2>& data) {
        // @TODO: use more generic parameterisable writers, also pairs for maps as iterables with elements
        ostream << "{";
        if (!data.empty()) {
            auto item = data.begin();
            write(ostream, item->first);
            ostream << ":";
            write(ostream, item->second);
            ++item;
            for (; item != data.end(); ++item) {
                ostream << ",";
                write(ostream, item->first);
                ostream << ":";
                write(ostream, item->second);
            }
        }
        ostream << "}";
        ostream.flush();
    }
    template <typename T1, typename T2>
    static void write(std::ostream& ostream, const std::unordered_map<T1, T2>& data) {
        ostream << "{";
        if (!data.empty()) {
            auto item = data.begin();
            write(ostream, item->first);
            ostream << ":";
            write(ostream, item->second);
            ++item;
            for (; item != data.end(); ++item) {
                ostream << ",";
                write(ostream, item->first);
                ostream << ":";
                write(ostream, item->second);
            }
        }
        ostream << "}";
        ostream.flush();
    }
    template <typename T, std::size_t size>
    static void write(std::ostream& ostream, const std::array<T, size>& data) {
        ostream << "[";
        if (!data.empty()) {
            auto item = data.begin();
            write(ostream, *item);
            ++item;
            for (; item != data.end(); ++item) {
                ostream << ",";
                write(ostream, *item);
            }
        }
        ostream << "]";
        ostream.flush();
    }
    template <size_t index, typename... Ts>
    static void write(std::ostream& ostream, const std::tuple<Ts...>& data,
            typename std::enable_if_t<index == 0>* = nullptr) {
        write(ostream, std::get<0>(data));
        ostream.flush();
    }
    template <size_t index, typename... Ts>
    static void write(std::ostream& ostream, const std::tuple<Ts...>& data,
            typename std::enable_if_t<index != 0>* = nullptr) {
        write<index - 1, Ts...>(ostream, data);
        ostream << ",";
        write(ostream, std::get<index>(data));
        ostream.flush();
    }
    template <typename... Ts>
    static void write(std::ostream& ostream, const std::tuple<Ts...>& data) {
        ostream << "[";
        write<std::tuple_size<std::tuple<Ts...>>::value - 1, Ts...>(ostream, data);
        ostream << "]";
        ostream.flush();
    }
};
}  // namespace detail
template <typename T>
void write_json(std::ostream& ostream, const T& data) {
    detail::write_json_detail::write(ostream, data);
}
}  // namespace other
}  // namespace kafka
}  // namespace souffle

#include <functional>
#include <sstream>
#include <string>
#include <unordered_set>

namespace souffle {
namespace kafka {
namespace other {
template <typename Type>
using Function = std::function<Type>;
template <typename OutputIterable, typename InputIterable>
void aggregate(const InputIterable& inputIterable,
        Function<OutputIterable(const typename InputIterable::value_type&)> function,
        OutputIterable& outputIterable) {
    for (const auto& value : inputIterable) {
        const auto other = function(value);
        outputIterable.insert(other.begin(), other.end());
    }
}
template <typename OutputIterable, typename InputIterable>
OutputIterable aggregate(const InputIterable& inputIterable,
        Function<OutputIterable(const typename InputIterable::value_type&)> function) {
    OutputIterable outputIterable;
    for (const auto& value : inputIterable) {
        const auto other = function(value);
        outputIterable.insert(other.begin(), other.end());
    }
    return outputIterable;
}
std::stringstream stringstream(std::string string) {
    std::stringstream stringstream;
    stringstream << string;
    return stringstream;
}
template <typename Iterable>
void join(std::ostream& ostream, const Iterable& iterable, const std::string delimiter) {
    auto it = iterable.begin();
    ostream << *it;
    ++it;
    for (; it != iterable.end(); ++it) {
        ostream << delimiter << *it;
    }
}
template <typename Iterable>
std::string join(const Iterable& iterable, const std::string delimiter) {
    std::stringstream stringstream;
    join(stringstream, iterable, delimiter);
    return stringstream.str();
}
template <typename Set>
Set set_union(const Set& a, const Set& b) {
    Set c;
    c.insert(a.begin(), a.end());
    c.insert(b.begin(), b.end());
    return c;
}
}  // namespace other
}  // namespace kafka
}  // namespace souffle

namespace souffle {
namespace kafka {
namespace detail {
class SouffleMetadata {
private:
    using Path = std::string;
    using RelationName = std::string;
    using StratumName = std::string;
    using RelationNames = std::unordered_set<RelationName>;
    using StratumNames = std::vector<StratumName>;
    using Stratum = std::tuple<StratumName,  // name
            RelationNames,                   // input
            RelationNames,                   // output
            RelationNames,                   // produced,
            RelationNames,                   // consumed
            RelationNames                    // negated
            >;
    enum {
        STRATUM_NAME = 0,
        INPUT_RELATION_NAMES = 1,
        OUTPUT_RELATION_NAMES = 2,
        PRODUCED_RELATION_NAMES = 3,
        CONSUMED_RELATION_NAMES = 4,
        NEGATED_RELATION_NAMES = 5
    };
    static const StratumName& stratumName(const Stratum& stratum) {
        return std::get<STRATUM_NAME>(stratum);
    }
    static const RelationNames& inputRelationNames(const Stratum& stratum) {
        return std::get<INPUT_RELATION_NAMES>(stratum);
    }
    static const RelationNames& outputRelationNames(const Stratum& stratum) {
        return std::get<OUTPUT_RELATION_NAMES>(stratum);
    }
    static const RelationNames& producedRelationNames(const Stratum& stratum) {
        return std::get<PRODUCED_RELATION_NAMES>(stratum);
    }
    static const RelationNames& consumedRelationNames(const Stratum& stratum) {
        return std::get<CONSUMED_RELATION_NAMES>(stratum);
    }
    static const RelationNames& negatedRelationNames(const Stratum& stratum) {
        return std::get<NEGATED_RELATION_NAMES>(stratum);
    }
    using Strata = std::vector<Stratum>;
    using Program = std::tuple<Path,  // input path
            Path,                     // output path
            Strata                    // strata
            >;
    enum { INPUT_PATH = 0, OUTPUT_PATH = 1, STRATA = 2 };
    static const Path& inputPath(const Program& program) {
        return std::get<INPUT_PATH>(program);
    }
    static const Path& outputPath(const Program& program) {
        return std::get<OUTPUT_PATH>(program);
    }
    static const Strata& strata(const Program& program) {
        return std::get<STRATA>(program);
    }
    static const StratumNames stratumNames(const Strata& strata) {
        StratumNames stratumNames_;
        stratumNames_.reserve(strata.size());
        for (const auto& stratum : strata) {
            stratumNames_.push_back(stratumName(stratum));
        }
        return stratumNames_;
    }
    static const RelationNames relationNames(const Stratum& stratum) {
        RelationNames relationNames_;
        {
            const auto& other = inputRelationNames(stratum);
            relationNames_.insert(other.begin(), other.end());
        }
        {
            const auto& other = outputRelationNames(stratum);
            relationNames_.insert(other.begin(), other.end());
        }
        {
            const auto& other = producedRelationNames(stratum);
            relationNames_.insert(other.begin(), other.end());
        }
        {
            const auto& other = consumedRelationNames(stratum);
            relationNames_.insert(other.begin(), other.end());
        }
        return relationNames_;
    }
    static const RelationNames relationNames(const Strata& strata) {
        return other::aggregate<RelationNames>(
                strata, [](const Stratum& stratum) -> RelationNames { return relationNames(stratum); });
    }
    static const RelationNames inputRelationNames(const Strata& strata) {
        return other::aggregate<RelationNames>(
                strata, [](const Stratum& stratum) -> RelationNames { return inputRelationNames(stratum); });
    }
    static const RelationNames outputRelationNames(const Strata& strata) {
        return other::aggregate<RelationNames>(
                strata, [](const Stratum& stratum) -> RelationNames { return outputRelationNames(stratum); });
    }

public:
    explicit SouffleMetadata() {}

    class StratumMetadata {
    private:
        const Stratum& stratum_;

    public:
        explicit StratumMetadata(const Stratum& stratum) : stratum_(stratum) {}
        const RelationNames& inputRelationNames() const {
            return SouffleMetadata::inputRelationNames(stratum_);
        }
        const RelationNames& outputRelationNames() const {
            return SouffleMetadata::outputRelationNames(stratum_);
        }
        const RelationNames& producedRelationNames() const {
            return SouffleMetadata::producedRelationNames(stratum_);
        }
        const RelationNames& consumedRelationNames() const {
            return SouffleMetadata::consumedRelationNames(stratum_);
        }
    };

    class ProgramMetadata {
    private:
        const Program program_;

    public:
        explicit ProgramMetadata(const Program&& program) : program_(program) {}
        const Path& inputPath() const {
            return SouffleMetadata::inputPath(program_);
        }
        const Path& outputPath() const {
            return SouffleMetadata::inputPath(program_);
        }
        const RelationNames inputRelationNames() const {
            return SouffleMetadata::inputRelationNames(strata(program_));
        }
        const RelationNames outputRelationNames() const {
            return SouffleMetadata::outputRelationNames(strata(program_));
        }
        const StratumNames stratumNames() const {
            return SouffleMetadata::stratumNames(strata(program_));
        }
        StratumMetadata createStratumMetadata(const StratumName& stratumName) const {
            for (const auto& stratum : SouffleMetadata::strata(program_)) {
                if (stratumName == SouffleMetadata::stratumName(stratum)) {
                    return StratumMetadata(stratum);
                }
            }
            assert(false);
            return StratumMetadata(Stratum());
        }
        void printDebug(std::ostream& ostream) const {
            other::write(ostream, program_);
        }
        void print(std::ostream& ostream) const {
            ostream << R"({ "RelationNames": [")";
            {
                const auto strings = relationNames(strata(program_));
                ostream << other::join(strings, "\" ,\"");
            }
            ostream << R"("], "StratumNames": [")";
            {
                const auto strings = SouffleMetadata::stratumNames(strata(program_));
                ostream << other::join(strings, "\" ,\"");
            }
            ostream << R"("], "ProgramMetadata": )";
            { other::write_json(ostream, strata(program_)); }
            ostream << R"(})";
            ostream << std::endl;
        }
    };
    ProgramMetadata createProgramMetadata(const std::string& string) const {
        auto stringstream = other::stringstream(string);
        Program program;
        other::read(stringstream, program);
        return ProgramMetadata(std::move(program));
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

#include "SouffleInterface.h"

namespace souffle {
namespace kafka {
namespace detail {
class SouffleWrapper {
private:
    using Path = std::string;
    using StratumName = std::string;
    using RelationName = std::string;
    using SouffleProgram = souffle::SouffleProgram;
    using SouffleRelation = souffle::Relation;
    using SouffleTuple = souffle::tuple;
    using SouffleRamDomain = souffle::RamDomain;
    static void loadAll(SouffleProgram* souffleProgram, const Path& inputPath) {
        souffleProgram->loadAll(inputPath);
    }
    static void printAll(SouffleProgram* souffleProgram, const Path& outputPath) {
        souffleProgram->printAll(outputPath);
    }
    static void run(SouffleProgram* souffleProgram, const StratumName& stratumName) {
        souffleProgram->run(std::stoul(stratumName));
    }
    static SouffleRelation* getRelation(SouffleProgram* souffleProgram, const RelationName& relationName) {
        return souffleProgram->getRelation(relationName);
    }
    static std::vector<SouffleRamDomain> getRelationData(const SouffleRelation* relation) {
        std::vector<SouffleRamDomain> data;
        for (auto& tuple : *relation) {
            for (std::size_t i = 0; i < relation->getArity(); ++i) {
                SouffleRamDomain x;
                tuple >> x;
                data.push_back(x);
            }
        }
        return data;
    }
    static void setRelationData(SouffleRelation* relation, const std::vector<SouffleRamDomain>& data) {
        if (data.empty()) return;
        for (std::size_t i = 0; i < data.size();) {
            SouffleTuple tuple(relation);
            for (std::size_t j = 0; j < relation->getArity(); ++j) {
                tuple << data.at(i);
                ++i;
            }
            relation->insert(tuple);
        }
    }

public:
    explicit SouffleWrapper() {}
    class RelationWrapper {
    private:
        SouffleRelation* relation_;

    public:
        explicit RelationWrapper(SouffleRelation* relation) : relation_(relation) {}
        std::vector<SouffleRamDomain> getRelationData() const {
            return SouffleWrapper::getRelationData(relation_);
        }
        void setRelationData(const std::vector<SouffleRamDomain>& data) {
            SouffleWrapper::setRelationData(relation_, data);
        }
    };
    class ProgramWrapper {
    private:
        SouffleProgram* souffleProgram_;

    public:
        explicit ProgramWrapper(SouffleProgram* souffleProgram) : souffleProgram_(souffleProgram) {}
        void loadAll(const Path& inputPath) {
            SouffleWrapper::loadAll(souffleProgram_, inputPath);
        }
        void printAll(const Path& inputPath) const {
            SouffleWrapper::printAll(souffleProgram_, inputPath);
        }
        void run(const StratumName& stratumName) const {
            SouffleWrapper::run(souffleProgram_, stratumName);
        }
        RelationWrapper createRelationWrapper(const RelationName& relationName) const {
            return RelationWrapper(getRelation(souffleProgram_, relationName));
        }
    };
    ProgramWrapper createProgramWrapper(SouffleProgram* souffleProgram) {
        return ProgramWrapper(souffleProgram);
    }
};

}  // namespace detail
}  // namespace kafka
}  // namespace souffle

#include <librdkafka/rdkafkacpp.h>

#include <atomic>
#include <cassert>
#include <csignal>

namespace souffle {
namespace kafka {
namespace detail {
static std::atomic<bool> exceptionThrown_;
class KafkaHelper {
public:
    static void unthrowException() {
        exceptionThrown_ = false;
    }
    static void throwException() {
        exceptionThrown_ = true;
    }
    static void throwException(const std::string& message) {
        std::cerr << message << std::endl;
        exceptionThrown_ = true;
    }
    static void throwFatalException() {
        throwException();
        assert(false);
    }
    static void throwFatalException(const std::string& message) {
        throwException(message);
        assert(false);
    }
    static bool exceptionThrown() {
        return exceptionThrown_;
    }
    static void sigintAndSigterm(int sig) {
        (void)sig;
        throwException();
    }
    static void setSigintAndSigterm() {
        signal(SIGINT, sigintAndSigterm);
        signal(SIGTERM, sigintAndSigterm);
    }
    static RdKafka::Conf* createConf(const RdKafka::Conf::ConfType confType) {
        RdKafka::Conf* conf = RdKafka::Conf::create(confType);
        if (!conf) {
            throwFatalException();
        }
        return conf;
    }
    static RdKafka::Conf* createGlobalConf() {
        return createConf(RdKafka::Conf::CONF_GLOBAL);
    }
    static RdKafka::Conf* createTopicConf() {
        return createConf(RdKafka::Conf::CONF_TOPIC);
    }
    template <typename T>
    static void setConf(RdKafka::Conf* conf, const std::string& key, T value) {
        assert(conf);
        std::string errstr;
        if (conf->set(key, value, errstr) != RdKafka::Conf::CONF_OK) {
            throwFatalException(errstr);
        }
    }
    static void setMetadataBrokerList(RdKafka::Conf* conf, const std::string& metadataBrokerList) {
        setConf(conf, "metadata.broker.list", metadataBrokerList);
    }
    static void eventCb(RdKafka::Event& event) {
        switch (event.type()) {
            case RdKafka::Event::EVENT_ERROR: {
                if (event.fatal()) {
                    const auto errstr = RdKafka::err2str(event.err());
                    throwFatalException(errstr);
                }
                break;
            }
            case RdKafka::Event::EVENT_STATS: {
                break;
            }
            case RdKafka::Event::EVENT_LOG: {
                break;
            }
            default: { break; }
        }
    }
    class EventCb : public RdKafka::EventCb {
    public:
        void event_cb(RdKafka::Event& event) {
            eventCb(event);
        }
    };
    static void setEventCb(RdKafka::Conf* conf, RdKafka::EventCb* eventCb) {
        setConf(conf, "event_cb", eventCb);
    }
    static void drCb(RdKafka::Message& message) {
        switch (message.status()) {
            case RdKafka::Message::MSG_STATUS_NOT_PERSISTED: {
                break;
            }
            case RdKafka::Message::MSG_STATUS_POSSIBLY_PERSISTED: {
                break;
            }
            case RdKafka::Message::MSG_STATUS_PERSISTED: {
                break;
            }
            default: { break; }
        }
    }
    class DeliveryReportCb : public RdKafka::DeliveryReportCb {
    public:
        void dr_cb(RdKafka::Message& message) {
            drCb(message);
        }
    };
    static void setDeliveryReportCb(RdKafka::Conf* conf, RdKafka::DeliveryReportCb* drCb) {
        setConf(conf, "dr_cb", drCb);
    }
    static void setDefaultTopicConf(RdKafka::Conf* conf, RdKafka::Conf* defaultTopicConf) {
        setConf(conf, "default_topic_conf", defaultTopicConf);
    }
    template <typename Handle>
    static Handle* createHandle(RdKafka::Conf* conf) {
        assert(conf);
        std::string errstr;
        Handle* handle = Handle::create(conf, errstr);
        if (!handle) {
            throwFatalException(errstr);
        }
        return handle;
    }
    static RdKafka::Producer* createProducer(RdKafka::Conf* conf) {
        return createHandle<RdKafka::Producer>(conf);
    }
    static RdKafka::Consumer* createConsumer(RdKafka::Conf* conf) {
        return createHandle<RdKafka::Consumer>(conf);
    }
    static RdKafka::Topic* createTopic(
            RdKafka::Conf* topicConf, RdKafka::Handle* base, const std::string& topicName) {
        assert(topicConf);
        assert(base);
        std::string errstr;
        RdKafka::Topic* topic = RdKafka::Topic::create(base, topicName, topicConf, errstr);
        if (!topic) {
            throwFatalException(errstr);
        }
        return topic;
    }
    static void pollHandle(RdKafka::Handle* handle, int timeout_ms) {
        // @TODO: do better polling and timeouts
        assert(handle);
        handle->poll(timeout_ms);
    }
    template <typename T>
    static void produceProducer(RdKafka::Producer* producer, RdKafka::Topic* topic, std::vector<T>& payload) {
        assert(producer);
        assert(topic);
        const std::int32_t partition = RdKafka::Topic::PARTITION_UA;
        const int msgflags = RdKafka::Producer::MSG_COPY;
        const std::string* key = nullptr;
        void* msg_opaque = nullptr;
        RdKafka::ErrorCode errorCode = producer->produce(
                topic, partition, msgflags, payload.data(), sizeof(T) * payload.size(), key, msg_opaque);
        if (errorCode != RdKafka::ERR_NO_ERROR) {
            throwFatalException(RdKafka::err2str(errorCode));
        }
        pollHandle(producer, 0);
    }
    static void pollProducerUntilEmpty(RdKafka::Producer* producer) {
        assert(producer);
        // -1 will perform an indefinate blocking wait
        const int timeout_ms = -1;
        while (!exceptionThrown() && producer->outq_len() > 0) {
            pollHandle(producer, timeout_ms);
        }
    }
    static void startConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic) {
        assert(consumer);
        assert(topic);
        const int32_t partition = 0;
        // @TODO: which offset to use? const int64_t offset = RdKafka::Topic::OFFSET_BEGINNING;
        const int64_t offset = RdKafka::Topic::OFFSET_BEGINNING;
        RdKafka::ErrorCode errorCode = consumer->start(topic, partition, offset);
        if (errorCode != RdKafka::ERR_NO_ERROR) {
            throwFatalException(RdKafka::err2str(errorCode));
        }
        pollHandle(consumer, 0);
    }
    static void stopConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic) {
        assert(consumer);
        assert(topic);
        const int32_t partition = RdKafka::Topic::PARTITION_UA;
        consumer->stop(topic, partition);
        pollHandle(consumer, 1000);
    }
    template <typename T>
    static void consumeConsumer(RdKafka::Consumer* consumer, RdKafka::Topic* topic, std::vector<T>& payload) {
        assert(consumer);
        assert(topic);
        // @TODO: maybe use std::numeric_limits<int>::max();
        const int32_t partition = 0;
        const int timeout_ms = 1000;
        while (true) {
            RdKafka::Message* message = consumer->consume(topic, partition, timeout_ms);
            pollHandle(consumer, 0);
            switch (message->err()) {
                case RdKafka::ERR__TIMED_OUT: {
                    delete message;
                    break;
                }
                case RdKafka::ERR__PARTITION_EOF: {
                    throwFatalException(message->errstr());
                    break;
                }
                case RdKafka::ERR__UNKNOWN_TOPIC: {
                    throwFatalException(message->errstr());
                    break;
                }
                case RdKafka::ERR__UNKNOWN_PARTITION: {
                    throwFatalException(message->errstr());
                    break;
                }
                case RdKafka::ERR_NO_ERROR: {
                    const T* rawPayload = static_cast<T*>(message->payload());
                    payload = std::vector<T>(rawPayload, rawPayload + (message->len() / sizeof(T)));
                    pollHandle(consumer, 0);
                    delete message;
                    return;
                }
                default: {
                    throwFatalException(message->errstr());
                    break;
                }
            }
        }
    }
    static void waitDestroyed() {
        // -1 will perform an indefinate blocking wait
        RdKafka::wait_destroyed(-1);
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

namespace souffle {
namespace kafka {
namespace detail {
class KafkaHelperTest {
private:
    const std::string brokers_;
    const std::vector<std::string> topicNames_;
    RdKafka::Conf* globalConf_;
    RdKafka::Conf* topicConf_;
    KafkaHelper::EventCb eventCb_;
    KafkaHelper::DeliveryReportCb deliveryReportCb_;
    RdKafka::Producer* producer_;
    RdKafka::Consumer* consumer_;
    std::vector<RdKafka::Topic*> producerTopics_;
    std::vector<RdKafka::Topic*> consumerTopics_;

public:
    explicit KafkaHelperTest(const std::string brokers, const std::vector<std::string> topicNames)
            : brokers_(brokers), topicNames_(topicNames) {}
    void setUpKafkaTest() {
        BEGIN_TEST();
        KafkaHelper::unthrowException();
        KafkaHelper::setSigintAndSigterm();
        globalConf_ = KafkaHelper::createGlobalConf();
        topicConf_ = KafkaHelper::createTopicConf();
        KafkaHelper::setMetadataBrokerList(globalConf_, brokers_);
        KafkaHelper::setEventCb(globalConf_, &eventCb_);
        KafkaHelper::setDeliveryReportCb(globalConf_, &deliveryReportCb_);
        KafkaHelper::setDefaultTopicConf(globalConf_, topicConf_);
        producer_ = KafkaHelper::createProducer(globalConf_);
        consumer_ = KafkaHelper::createConsumer(globalConf_);
        END_TEST();
    }
    void tearDownKafkaTest() {
        BEGIN_TEST();
        delete producer_;
        delete consumer_;
        KafkaHelper::waitDestroyed();
        delete globalConf_;
        delete topicConf_;
        END_TEST();
    }
    void produceProducerTest() {
        BEGIN_TEST();
        std::size_t i = 0;
        for (RdKafka::Topic* topic : producerTopics_) {
            {
                std::vector<std::size_t> data{i++};
                KafkaHelper::produceProducer(producer_, topic, data);
            }
            {
                std::vector<std::size_t> data{i++};
                KafkaHelper::produceProducer(producer_, topic, data);
            }
            {
                std::vector<std::size_t> data{};
                KafkaHelper::produceProducer(producer_, topic, data);
            }
        }
        END_TEST();
    }
    void consumeConsumerTest() {
        BEGIN_TEST();
        for (RdKafka::Topic* topic : consumerTopics_) {
            bool empty = false;
            while (!empty) {
                std::vector<std::size_t> data;
                KafkaHelper::consumeConsumer(consumer_, topic, data);
                std::cerr << consumer_->name() << " ";
                other::write(std::cerr, data);
                std::cerr << std::endl;
                empty = data.empty();
            }
        }
        END_TEST();
    }
    void startConsumerTest() {
        BEGIN_TEST();
        for (RdKafka::Topic* topic : consumerTopics_) {
            KafkaHelper::startConsumer(consumer_, topic);
        }
        END_TEST();
    }
    void stopConsumerTest() {
        BEGIN_TEST();
        for (RdKafka::Topic* topic : consumerTopics_) {
            KafkaHelper::stopConsumer(consumer_, topic);
        }
        END_TEST();
    }
    void pollProducerTest() {
        BEGIN_TEST();
        KafkaHelper::pollHandle(producer_, 1000);
        END_TEST();
    }
    void pollConsumerTest() {
        BEGIN_TEST();
        KafkaHelper::pollHandle(consumer_, 1000);
        END_TEST();
    }
    void pollProducerUntilEmptyTest() {
        BEGIN_TEST();
        KafkaHelper::pollProducerUntilEmpty(producer_);
        END_TEST();
    }
    void createProducerTopicsTest() {
        BEGIN_TEST();
        for (const auto& topicName : topicNames_) {
            RdKafka::Topic* topic = KafkaHelper::createTopic(topicConf_, producer_, topicName);
            producerTopics_.push_back(topic);
        }
        END_TEST();
    }
    void createConsumerTopicsTest() {
        BEGIN_TEST();
        for (const auto& topicName : topicNames_) {
            RdKafka::Topic* topic = KafkaHelper::createTopic(topicConf_, consumer_, topicName);
            consumerTopics_.push_back(topic);
        }
        END_TEST();
    }
    void deleteProducerTopicsTest() {
        BEGIN_TEST();
        for (RdKafka::Topic* topic : producerTopics_) {
            delete topic;
        }
        END_TEST();
    }
    void deleteConsumerTopicsTest() {
        BEGIN_TEST();
        for (RdKafka::Topic* topic : consumerTopics_) {
            delete topic;
        }
        END_TEST();
    }
    void runAllTests() {
        BEGIN_TEST();
        setUpKafkaTest();
        createProducerTopicsTest();
        createConsumerTopicsTest();
        startConsumerTest();
        produceProducerTest();
        consumeConsumerTest();
        stopConsumerTest();
        deleteConsumerTopicsTest();
        pollProducerUntilEmptyTest();
        deleteProducerTopicsTest();
        tearDownKafkaTest();
        END_TEST();
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

#define SOUFFLE_KAFKA_DEBUG

namespace souffle {
namespace kafka {
namespace detail {
class KafkaClient {
private:
    RdKafka::Conf* globalConf_;
    RdKafka::Conf* topicConf_;
    KafkaHelper::EventCb eventCb_;
    KafkaHelper::DeliveryReportCb deliveryReportCb_;
    RdKafka::Producer* producer_;
    RdKafka::Consumer* consumer_;
    std::unordered_map<std::string, RdKafka::Topic*> producerTopics_;
    std::unordered_map<std::string, RdKafka::Topic*> consumerTopics_;
#ifdef SOUFFLE_KAFKA_DEBUG
    RdKafka::Topic* debugTopic_;
#endif
private:
    explicit KafkaClient() {}

public:
    ~KafkaClient() {}

public:
    // get instance for singleton design pattern
    static KafkaClient& getInstance() {
        // @TODO: try with a full static methods only class?
        static KafkaClient instance = KafkaClient();
        return instance;
    }
    // deleted constructor for singleton design pattern
    KafkaClient(KafkaClient const&) = delete;
    // deleted constructor for singleton design pattern
    void operator=(KafkaClient const&) = delete;

public:
    void beginClient(const std::string& brokers) {
        KafkaHelper::unthrowException();
        KafkaHelper::setSigintAndSigterm();
        globalConf_ = KafkaHelper::createGlobalConf();
        topicConf_ = KafkaHelper::createTopicConf();
        KafkaHelper::setMetadataBrokerList(globalConf_, brokers);
        KafkaHelper::setEventCb(globalConf_, &eventCb_);
        KafkaHelper::setDeliveryReportCb(globalConf_, &deliveryReportCb_);
        KafkaHelper::setDefaultTopicConf(globalConf_, topicConf_);
        producer_ = KafkaHelper::createProducer(globalConf_);
        consumer_ = KafkaHelper::createConsumer(globalConf_);
        producerTopics_ = std::unordered_map<std::string, RdKafka::Topic*>();
        consumerTopics_ = std::unordered_map<std::string, RdKafka::Topic*>();
#ifdef SOUFFLE_KAFKA_DEBUG
        debugTopic_ = KafkaHelper::createTopic(topicConf_, producer_, "_DEBUG_");
#endif
    }
    void endClient() {
#ifdef SOUFFLE_KAFKA_DEBUG
        delete debugTopic_;
#endif
        delete producer_;
        delete consumer_;
        KafkaHelper::waitDestroyed();
        delete globalConf_;
        delete topicConf_;
    }

#ifdef SOUFFLE_KAFKA_DEBUG
    template <typename T>
    void debug(std::stringstream& stringstream, const std::vector<T>& payload) {
        other::write(stringstream, payload);
        stringstream << " " << __DATE__ << " " << __TIME__ << " ";
        const std::string str = stringstream.str();
        const char* cstr = str.c_str();
        std::vector<char> debugPayload(cstr, cstr + str.size());
        KafkaHelper::produceProducer(producer_, debugTopic_, debugPayload);
        // @TODO: maybe do the following for debugging KafkaHelper::pollProducerUntilEmpty(producer_, 1000);
    }
#endif
    void beginProduction(const std::string& topicName) {
        // @TODO: probably should do this more efficiently
        {
            auto iterator = producerTopics_.find(topicName);
            if (iterator != producerTopics_.end() && iterator->second != nullptr) {
                return;
            }
        }
        producerTopics_[topicName] = KafkaHelper::createTopic(topicConf_, producer_, topicName);
    }
    void endProduction(const std::string& topicName) {
        delete producerTopics_[topicName];
        producerTopics_[topicName] = nullptr;
    }
    void beginConsumption(const std::string& topicName) {
        // @TODO: probably should do this more efficiently
        {
            auto iterator = consumerTopics_.find(topicName);
            if (iterator != consumerTopics_.end() && iterator->second != nullptr) {
                return;
            }
        }
        consumerTopics_[topicName] = KafkaHelper::createTopic(topicConf_, consumer_, topicName);
        KafkaHelper::startConsumer(consumer_, consumerTopics_.at(topicName));
    }
    void endConsumption(const std::string& topicName) {
        KafkaHelper::stopConsumer(consumer_, consumerTopics_.at(topicName));
        delete consumerTopics_[topicName];
        consumerTopics_[topicName] = nullptr;
    }
    template <typename T>
    void produce(const std::string& topicName, std::vector<T>& payload) {
        RdKafka::Topic* topic = producerTopics_.at(topicName);
#ifdef SOUFFLE_KAFKA_DEBUG
        {
            std::stringstream stringstream;
            stringstream << producer_->name() << " " << topic->name() << " ";
            debug(stringstream, payload);
        }
#endif
        KafkaHelper::produceProducer(producer_, topic, payload);
    }
    template <typename T>
    void consume(const std::string& topicName, std::vector<T>& payload) {
        RdKafka::Topic* topic = consumerTopics_.at(topicName);
        KafkaHelper::consumeConsumer(consumer_, topic, payload);
#ifdef SOUFFLE_KAFKA_DEBUG
        {
            std::stringstream stringstream;
            stringstream << consumer_->name() << " " << topic->name() << " ";
            debug(stringstream, payload);
        }
#endif
    }
    void pollProducer() {
        const int timeout_ms = 1000;
        KafkaHelper::pollHandle(producer_, timeout_ms);
    }
    void pollConsumer() {
        const int timeout_ms = 1000;
        KafkaHelper::pollHandle(consumer_, timeout_ms);
    }
    void pollProducerUntilEmpty() {
        KafkaHelper::pollProducerUntilEmpty(producer_);
    }
};
}  // namespace detail
}  // namespace kafka
}  // namespace souffle

#include <fstream>

namespace souffle {
namespace kafka {
class Kafka {
private:
    detail::SouffleMetadata::ProgramMetadata programMetadata_;

public:
    explicit Kafka(const std::string& string)
            : programMetadata_(detail::SouffleMetadata().createProgramMetadata(string)) {}

    void debugPrint(std::ostream& ostream) const {
        (void)ostream;
        programMetadata_.print(std::cerr);
        programMetadata_.printDebug(std::cerr);
        std::cout << R"({"RelationNames": ["A","B","C"], "StratumNames":["0"]})" << std::endl;
    }

    void print(std::ostream& ostream) const {
        // @TODO: remove call to debug function when you get it working
        // debugPrint(ostream);
        // return;
        programMetadata_.print(ostream);
        // programMetadata_.printDebug(std::cerr);
    }

    void debugRun(souffle::SouffleProgram* souffleProgram, std::size_t stratumIndex) const {
        (void)souffleProgram;
        if (stratumIndex != 0) return;
        std::string brokers = "localhost";
        std::vector<std::string> topics{"A", "B", "C"};
        detail::KafkaHelperTest(brokers, topics).runAllTests();
    }

    void run(souffle::SouffleProgram* souffleProgram, std::size_t stratumIndex) const {
        // @TODO: remove call to debug function when you get it working
        // debugRun(souffleProgram, stratumIndex);
        // return;
        if (stratumIndex == static_cast<std::size_t>(-2)) return;

        auto programWrapper = detail::SouffleWrapper().createProgramWrapper(souffleProgram);
        const auto stratumName = std::to_string(stratumIndex);
        const auto brokers = "localhost";
        auto& kafkaClient = detail::KafkaClient::getInstance();
        auto haltPayload = std::vector<souffle::RamDomain>({std::numeric_limits<souffle::RamDomain>::max()});

        const auto beginConsumption = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto& topicName : topicNames) {
                kafkaClient.beginConsumption(topicName);
            }
        };

        const auto endConsumption = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto topicName : topicNames) {
                kafkaClient.endConsumption(topicName);
            }
        };

        const auto beginProduction = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto& topicName : topicNames) {
                kafkaClient.beginProduction(topicName);
            }
        };
        const auto endProduction = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto topicName : topicNames) {
                kafkaClient.endProduction(topicName);
            }
        };
        const auto produceToSlaves = [&]() {
            const auto topicName = "default";
            kafkaClient.beginProduction(topicName);
            kafkaClient.produce(topicName, haltPayload);
            kafkaClient.pollProducerUntilEmpty();
            kafkaClient.endProduction(topicName);
        };

        const auto produceToMaster = [&]() {
            const auto topicName = "default";
            kafkaClient.beginProduction(topicName);
            std::vector<souffle::RamDomain> payload;
            kafkaClient.produce(topicName, payload);
            kafkaClient.pollProducerUntilEmpty();
            kafkaClient.endProduction(topicName);
        };

        const auto consumeFromSlaves = [&](std::size_t count) {
            const auto topicName = "default";
            kafkaClient.beginConsumption(topicName);
            while (count > 0) {
                std::vector<souffle::RamDomain> payload;
                kafkaClient.consume(topicName, payload);
                --count;
            }
            kafkaClient.endConsumption(topicName);
        };

        const auto beginConsumptionFromMaster = [&]() {
            const auto topicName = "default";
            kafkaClient.beginConsumption(topicName);
        };

        const auto endConsumptionFromMaster = [&]() {
            const auto topicName = "default";
            while (true) {
                std::vector<souffle::RamDomain> payload;
                kafkaClient.consume(topicName, payload);
                if (payload == haltPayload) {
                    break;
                }
            }
            kafkaClient.endConsumption(topicName);
        };

        const auto produceOnTopics = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto topicName : topicNames) {
                auto relationWrapper = programWrapper.createRelationWrapper(topicName);
                auto relationData = relationWrapper.getRelationData();
                kafkaClient.produce(topicName, relationData);
            }
        };

        const auto produceHaltOnTopics = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto topicName : topicNames) {
                auto haltPayload =
                        std::vector<souffle::RamDomain>({std::numeric_limits<souffle::RamDomain>::max()});
                kafkaClient.produce(topicName, haltPayload);
            }
        };

        const auto produceUntilEmpty = [&]() { kafkaClient.pollProducerUntilEmpty(); };

        const auto consumeOnTopics = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto topicName : topicNames) {
                std::vector<souffle::RamDomain> newRelationData;
                kafkaClient.consume(topicName, newRelationData);
                auto relationWrapper = programWrapper.createRelationWrapper(topicName);
                auto relationData = relationWrapper.getRelationData();
                relationData.insert(relationData.begin(), newRelationData.begin(), newRelationData.end());
                relationWrapper.setRelationData(relationData);
            }
        };

        const auto consumeUntilHaltOnTopics = [&](const std::unordered_set<std::string>& topicNames) {
            for (const auto topicName : topicNames) {
                std::vector<souffle::RamDomain> deltaRelationData;
                while (true) {
                    std::vector<souffle::RamDomain> newRelationData;
                    kafkaClient.consume(topicName, newRelationData);
                    if (newRelationData == haltPayload) {
                        break;
                    }
                    deltaRelationData.insert(
                            deltaRelationData.begin(), newRelationData.begin(), newRelationData.end());
                }
                auto relationWrapper = programWrapper.createRelationWrapper(topicName);
                auto relationData = relationWrapper.getRelationData();
                relationData.insert(relationData.begin(), deltaRelationData.begin(), deltaRelationData.end());
                relationWrapper.setRelationData(relationData);
            }
        };

        /*bre @@@TODO: BREAKPOINT (lh) */
        
        kafkaClient.beginClient(brokers);
        souffleProgram->runAll(programMetadata_.inputPath(), programMetadata_.outputPath(), stratumIndex);
        kafkaClient.endClient();
        return;
        

        // @@@TODO
        kafkaClient.beginClient(brokers);
        if (stratumName == std::to_string(static_cast<std::size_t>(-1))) {
            BREAKPOINT;
            {
                std::stringstream stringstream;
                stringstream << __FILE__ << " " << __LINE__ << " ";
                kafkaClient.debug(stringstream, std::vector<char>());
            }
            souffleProgram->runAll(programMetadata_.inputPath(), programMetadata_.outputPath(), stratumIndex);
        {
                std::stringstream stringstream;
                stringstream << __FILE__ << " " << __LINE__ << " ";
                kafkaClient.debug(stringstream, std::vector<char>());
            }
            BREAKPOINT;
            /*
{
                // RamLoad: file, fact-dir, .facts, master
                programWrapper.loadAll(programMetadata_.inputPath());
                // RamStore: kafka/file, fact-dir, .facts, master
                const auto producerTopicNames = programMetadata_.inputRelationNames();
                beginProduction(producerTopicNames);
                produceOnTopics(producerTopicNames);
                produceUntilEmpty();
                endProduction(producerTopicNames);
            }
            {
                // RamLoad: kafka/file, output-dir, .csv, master
                const auto consumerTopicNames = programMetadata_.outputRelationNames();
                beginConsumption(consumerTopicNames);
                consumeUntilHaltOnTopics(consumerTopicNames);
                endConsumption(consumerTopicNames);
                // RamStore: file, output-dir, .csv, master
                programWrapper.printAll(programMetadata_.outputPath());
            }
            */
        } else {
            const auto stratumMetadata = programMetadata_.createStratumMetadata(stratumName);
            {
                // RamLoad: kafka/file, fact-dir, .facts, slave
                const auto consumerInputTopicNames = stratumMetadata.inputRelationNames();
                beginConsumption(consumerInputTopicNames);
                consumeOnTopics(consumerInputTopicNames);
                endConsumption(consumerInputTopicNames);
            }
            {
            // RamLoad: kafka/file, output-dir, .facts/.csv, slave
                const auto consumerConsumedTopicNames = stratumMetadata.consumedRelationNames();
                beginConsumption(consumerConsumedTopicNames);
                consumeUntilHaltOnTopics(consumerConsumedTopicNames);
                endConsumption(consumerConsumedTopicNames);
            }
            programWrapper.run(stratumName);
            {
            // RamStore: kafka/file, output-dir, .facts, slave
                const auto producerProducedTopicNames = stratumMetadata.producedRelationNames();
                beginProduction(producerProducedTopicNames);
                produceOnTopics(producerProducedTopicNames);
                produceHaltOnTopics(producerProducedTopicNames);
                produceUntilEmpty();
                endProduction(producerProducedTopicNames);
            }
            {
            // RamStore: kafka/file, output-dir, .csv, slave
                const auto producerOutputTopicNames = stratumMetadata.outputRelationNames();
                beginProduction(producerOutputTopicNames);
                produceOnTopics(producerOutputTopicNames);
                produceHaltOnTopics(producerOutputTopicNames);
                produceUntilEmpty();
                endProduction(producerOutputTopicNames);
            }
        }
        kafkaClient.endClient();
    }
};
}  // namespace kafka
}  // namespace souffle
