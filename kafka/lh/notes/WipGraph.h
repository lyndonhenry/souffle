namespace wip {

namespace graph {

using IStream = std::istream;

using OStream = std::ostream;

template <typename Input, typename... Output>
using Function = std::function<Input(Output...)>;

using SizeT = std::size_t;

constexpr SizeT NUMERIC_LIMITS_SIZE_T_MAX = std::numeric_limits<SizeT>::max();

using String = std::string;

template <typename Type>
using Vector = std::vector<Type>;

template <typename... Type>
using Tuple = std::tuple<Type...>;

template <typename FirstType, typename SecondType>
using UnorderedMap = std::unordered_map<FirstType, SecondType>;

class Helper {
public:
    template <typename Type>
    static UnorderedMap<Type, SizeT> fromValueToIndex(const Vector<Type>& vectorOfValues) {
        auto indexOf = UnorderedMap<Type, SizeT>();
        for (const auto& value : vectorOfValues) {
            if (indexOf.find(value) == indexOf.end()) {
                indexOf[value] = indexOf.size();
            }
        }
        return indexOf;
    }

    template <typename Type>
    static Vector<Type> fromIndexToValue(const UnorderedMap<Type, SizeT>& indexOf) {
        auto valueAt = Vector<Type>(indexOf.size(), Type());
        for (const auto& it : indexOf) {
            if (valueAt[it.second].empty()) {
                valueAt[it.second] = it.first;
            }
        }
        return valueAt;
    }

    template <typename Type>
    static Vector<Vector<SizeT>> asAdjacencyList(
            const Vector<Tuple<Type, Type>>& vectorOfPairs, const UnorderedMap<Type, SizeT>& indexOf) {
        auto adjacencyList = Vector<Vector<SizeT>>(indexOf.size());
        for (const auto& pairOfValues : vectorOfPairs) {
            adjacencyList[indexOf.at(std::get<0>(pairOfValues))].push_back(
                    indexOf.at(std::get<1>(pairOfValues)));
        }
        return adjacencyList;
    }

    static Vector<Vector<SizeT>> transposedAdjacencyList(const Vector<Vector<SizeT>>& a) {
        auto b = Vector<Vector<SizeT>>(a.size());
        for (SizeT i = 0; i < a.size(); ++i) {
            for (SizeT j = 0; j < a.at(i).size(); ++j) {
                const auto k = a.at(i).at(j);
                b[k].push_back(i);
            }
        }
        return b;
    }
    static void removeDuplicateEdges(Vector<Vector<SizeT>>& adjacencyList, const SizeT size) {
        auto foundBy = Vector<SizeT>(size, NUMERIC_LIMITS_SIZE_T_MAX);
        for (SizeT i = 0; i < adjacencyList.size(); ++i) {
            SizeT jNew = 0;
            for (SizeT jOld = 0; jOld < adjacencyList.at(i).size(); ++jOld) {
                const auto k = adjacencyList.at(i).at(jOld);
                if (foundBy.at(k) != i) {
                    foundBy[k] = i;
                    adjacencyList[i][jNew] = k;
                    ++jNew;
                }
            }
            adjacencyList.at(i).resize(jNew);
        }
    }
    static void removeDuplicateEdges(Vector<Vector<SizeT>>& adjacencyList) {
        removeDuplicateEdges(adjacencyList, adjacencyList.size());
    }

    static Vector<SizeT> disjointSetsOfSize(const SizeT size) {
        auto disjointSets = Vector<SizeT>(size);
        for (SizeT i = 0; i < size; ++i) {
            disjointSets[i] = i;
        }
        return disjointSets;
    }

    static SizeT findDisjointSetRepresentative(Vector<SizeT>& disjointSets, const SizeT element) {
        assert(element < disjointSets.size());
        if (disjointSets.at(element) == element) {
            return element;
        } else {
            return disjointSets[element] =
                           findDisjointSetRepresentative(disjointSets, disjointSets.at(element));
        }
    }

    static void unifyDisjointSetsOfElements(
            Vector<SizeT>& disjointSets, const SizeT firstElement, const SizeT secondElement) {
        if (firstElement == secondElement) return;
        const auto firstRepresentative = findDisjointSetRepresentative(disjointSets, firstElement);
        const auto secondRepresentative = findDisjointSetRepresentative(disjointSets, secondElement);
        if (firstRepresentative == secondRepresentative) return;
        if (firstRepresentative < secondRepresentative) {
            disjointSets[secondRepresentative] = firstRepresentative;
        } else {
            disjointSets[firstRepresentative] = secondRepresentative;
        }
    }

    static SizeT countDisjointSetRepresentatives(Vector<SizeT>& disjointSets) {
        SizeT count = 0;
        for (SizeT element = 0; element < disjointSets.size(); ++element) {
            const auto representative = findDisjointSetRepresentative(disjointSets, element);
            if (element == representative) ++count;
        }
        return count;
    }

    static Vector<SizeT> getDisjointSetRepresentatives(Vector<SizeT>& disjointSets) {
        auto representatives = Vector<SizeT>();
        representatives.reserve(disjointSets.size());
        for (SizeT element = 0; element < disjointSets.size(); ++element) {
            const auto representative = findDisjointSetRepresentative(disjointSets, element);
            if (element == representative) representatives.push_back(representative);
        }
        representatives.resize(representatives.size());
        return representatives;
    }

    static Vector<Vector<SizeT>> getElementsOfDisjointSets(Vector<SizeT>& disjointSets) {
        auto elementsAt = Vector<Vector<SizeT>>(countDisjointSetRepresentatives(disjointSets));
        for (SizeT i = 0; i < disjointSets.size(); ++i) {
            const auto j = findDisjointSetRepresentative(disjointSets, i);
            if (i != j) {
                elementsAt.at(i).push_back(j);
            }
        }
        return elementsAt;
    }

    static void tarjansSccAlgorithm(const Vector<Vector<SizeT>>& adjacencyList, Vector<SizeT>& disjointSets) {
        // assert(adjacencyList.size() == countDisjointSetRepresentatives(disjointSets));
        // @see https://llbit.se/?p=3379

        // number of vertices
        const auto N = adjacencyList.size();
        // adjacency list
        const auto& adj = adjacencyList;

        // next index
        auto next = static_cast<SizeT>(0);
        auto index = Vector<SizeT>(N, NUMERIC_LIMITS_SIZE_T_MAX);
        auto lowlink = Vector<SizeT>(N, NUMERIC_LIMITS_SIZE_T_MAX);
        auto onstack = Vector<bool>(N, false);
        auto stack = std::stack<SizeT>();

        const Function<void, const SizeT> sconnect = [&](const SizeT v) -> void {
            index[v] = next;
            lowlink[v] = next;
            next += 1;
            stack.push(v);
            onstack[v] = true;
            for (const auto w : adj.at(v)) {
                if (index.at(w) == NUMERIC_LIMITS_SIZE_T_MAX) {
                    sconnect(w);
                    assert(lowlink.at(v) != NUMERIC_LIMITS_SIZE_T_MAX);
                    assert(lowlink.at(w) != NUMERIC_LIMITS_SIZE_T_MAX);
                    lowlink[v] = std::min(lowlink.at(v), lowlink.at(w));
                } else if (onstack.at(w)) {
                    assert(lowlink.at(v) != NUMERIC_LIMITS_SIZE_T_MAX);
                    assert(index.at(w) != NUMERIC_LIMITS_SIZE_T_MAX);
                    lowlink[v] = std::min(lowlink.at(v), index.at(w));
                }
            }
            if (index.at(v) == lowlink.at(v)) {
                // note that we omit com and groups here
                auto u = NUMERIC_LIMITS_SIZE_T_MAX;
                while (true) {
                    const auto w = stack.top();
                    stack.pop();
                    onstack[w] = false;
                    if (u == NUMERIC_LIMITS_SIZE_T_MAX) {
                        u = w;
                    } else {
                        unifyDisjointSetsOfElements(disjointSets, u, w);
                    }
                    if (w == v) {
                        break;
                    }
                }
            }
        };

        for (SizeT v = 0; v < N; ++v) {
            if (index.at(v) == NUMERIC_LIMITS_SIZE_T_MAX) {
                sconnect(v);
            }
        }
    }

    static Vector<SizeT> getOrderWithRepresentativesFirst(Vector<SizeT>& disjointSets) {
        auto indices = Vector<SizeT>(disjointSets.size(), NUMERIC_LIMITS_SIZE_T_MAX);
        size_t index = 0;
        for (const auto i : getDisjointSetRepresentatives(disjointSets)) {
            indices[i] = index++;
        }
        for (SizeT i = 0; i < disjointSets.size(); ++i) {
            if (indices.at(i) == NUMERIC_LIMITS_SIZE_T_MAX) indices[i] = index++;
        }
        return indices;
    }

    static Vector<Vector<SizeT>> getProducedBy(const Vector<Vector<SizeT>>& adjacencyList,
            Vector<SizeT>& disjointSets, const Vector<SizeT>& indexOrder) {
        // get all relations produced by a stratum
        // get all relations with an outbound edge in the disjoint set of a representative
        auto producedBy = Vector<Vector<SizeT>>(countDisjointSetRepresentatives(disjointSets));
        for (SizeT i = 0; i < adjacencyList.size(); ++i) {
            // if the element i has any outbound edges
            if (!adjacencyList.at(i).empty()) {
                // let j be the representative of i
                const auto j = findDisjointSetRepresentative(disjointSets, i);
                // let k be the index of j
                const auto k = indexOrder.at(j);
                // add element i to the produced by set for index k of representative j of element i
                producedBy.at(k).push_back(i);
            }
        }
        // remove duplicate edges
        removeDuplicateEdges(producedBy, disjointSets.size());
        // return those produced by
        return producedBy;
    }

    static Vector<Vector<SizeT>> getProducedFor(const Vector<Vector<SizeT>>& adjacencyList,
            Vector<SizeT>& disjointSets, const Vector<SizeT>& indexOrder) {
        auto producedFor = Vector<Vector<SizeT>>(countDisjointSetRepresentatives(disjointSets));
        for (SizeT i = 0; i < adjacencyList.size(); ++i) {
            // if the element i has any outbound edges
            if (!adjacencyList.at(i).empty()) {
                // let j be the representative of i
                const auto j = findDisjointSetRepresentative(disjointSets, i);
                // let k be the index of j
                const auto k = indexOrder.at(j);
                for (SizeT e : adjacencyList.at(i)) {
                    if (j != findDisjointSetRepresentative(disjointSets, e)) {
                        producedFor.at(k).push_back(e);
                    }
                }
            }
        }
        // remove duplicate edges
        removeDuplicateEdges(producedFor, disjointSets.size());
        // return those produced by
        return producedFor;
    }

    static Vector<Vector<SizeT>> getConsumedBy(const Vector<Vector<SizeT>>& adjacencyList,
            Vector<SizeT>& disjointSets, const Vector<SizeT>& indexOrder) {
        const auto edgesReversed = transposedAdjacencyList(adjacencyList);
        return getProducedFor(edgesReversed, disjointSets, indexOrder);
    }
};

// @TODO
template <typename String = String>
class Graph {
public:
    explicit Graph(const Vector<Tuple<String, String>>& graph) {
        // @TODO
        (void)graph;
        /*
        const auto indexOf = Helper::fromValueToIndex(graph);
        const auto valueOf = Helper::fromIndexToValue(indexOf);
        auto adjacencyList = Helper::asAdjacencyList(graph, indexOf);
        Helper::removeDuplicateEdges(adjacencyList);
        auto disjointSets = Helper::makeDisjointSets(adjacencyList.size());
        Helper::tarjansSccAlgorithm(adjacencyList, disjointSets);
        */
    }
    class Producer {
    public:
        explicit Producer(const String& topic) {
            // @TODO
            (void)topic;
        }
        template <typename Type>
        void produce(const Type& data) {
            // @TODO
            (void)data;
        }
    };
    Producer producer(const String& topic) {
        return Producer(topic);
    }
    class Consumer {
    public:
        explicit Consumer(const String& topic) {
            // @TODO
            (void)topic;
        }
        template <typename Type>
        void consume(Type& data) {
            // @TODO
            (void)data;
        }
    };
    Consumer consumer(const String& topic) {
        return Consumer(topic);
    }
};

namespace test {

int main(int argc, char** argv) {
    (void)argc;
    (void)argv;

    const auto inputGraph = std::tuple<std::vector<std::string>,  // input
            std::vector<std::string>,                             // output
            std::vector<std::string>,                             // vertices
            std::vector<std::tuple<std::string, std::string>>     // edges
            >({{"a"}, {"d"},
            {
                    "a",
                    "b",
                    "c",
                    "d",
            },
            {
                    {"a", "b"},
                    {"b", "c"},
                    {"c", "b"},
                    {"c", "d"},
                    {"c", "d"},
            }});

    {
        std::stringstream stringstream;
        write(stringstream, inputGraph);
        assert(stringstream.str() ==
                R"(1 "a" 1 "d" 4 "a" "b" "c" "d" 5 "a" "b" "b" "c" "c" "b" "c" "d" "c" "d")");
        std::tuple<std::vector<std::string>,                       // input
                std::vector<std::string>,                          // output
                std::vector<std::string>,                          // vertices
                std::vector<std::tuple<std::string, std::string>>  // edges
                >
                tmpGraph;
        read(stringstream, tmpGraph);
        assert(tmpGraph == inputGraph);
    }

    std::unordered_map<std::string, std::size_t> indexOf;
    std::vector<std::string> valueAt;

    {
        indexOf = wip::graph::Helper::fromValueToIndex(std::get<2>(inputGraph));
        valueAt = wip::graph::Helper::fromIndexToValue(indexOf);
        std::stringstream ss;
        write(ss, valueAt);
        assert(ss.str() == R"(4 "a" "b" "c" "d")");
    }

    std::vector<std::vector<size_t>> adjacencyList;

    {
        adjacencyList = wip::graph::Helper::asAdjacencyList(std::get<3>(inputGraph), indexOf);
        std::stringstream ss;
        write(ss, adjacencyList);
        assert(ss.str() == R"(4 1 1 1 2 3 1 3 3 0)");
    }

    {
        wip::graph::Helper::removeDuplicateEdges(adjacencyList);
        std::stringstream ss;
        write(ss, adjacencyList);
        std::cout << ss.str() << std::endl;
        assert(ss.str() == R"(4 1 1 1 2 2 1 3 0)");
    }

    std::vector<std::size_t> disjointSets;

    {
        disjointSets = wip::graph::Helper::disjointSetsOfSize(adjacencyList.size());
        std::stringstream ss;
        write(ss, disjointSets);
        assert(ss.str() == R"(4 0 1 2 3)");
    }

    {
        wip::graph::Helper::tarjansSccAlgorithm(adjacencyList, disjointSets);
        std::stringstream ss;
        write(ss, disjointSets);
        assert(ss.str() == R"(4 0 1 1 3)");
    }

    const auto indexOrder = wip::graph::Helper::getOrderWithRepresentativesFirst(disjointSets);
    {
        const auto producedBy = wip::graph::Helper::getProducedBy(adjacencyList, disjointSets, indexOrder);
        std::stringstream ss;
        write(ss, producedBy);
        assert(ss.str() == R"(3 1 0 2 1 2 0)");
    }
    {
        const auto consumedBy = wip::graph::Helper::getConsumedBy(adjacencyList, disjointSets, indexOrder);
        std::stringstream ss;
        write(ss, consumedBy);
        assert(ss.str() == R"(3 0 1 0 1 2)");
    }

    return 0;
}

}  // namespace test
