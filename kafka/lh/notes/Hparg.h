#pragma once

#include <getopt.h>
#include <sys/resource.h>

#include <chrono>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <regex>
#include <set>
#include <thread>
#include <unordered_map>
#include <vector>

#include "Sigmod2017.h"
#include "Vldb2018.h"

#define BREAKPOINT             \
  std::cout << __FILE__ << ":" \
            << ":" << __FUNCTION__ << ":" << __LINE__ << std::endl

namespace hparg {
namespace detail {

template <typename T>
using adjacency_list = std::vector<std::vector<T>>;

template <typename T>
using disjoint_sets = std::vector<T>;

using vertex_labels = std::vector<std::string>;

template <typename key_type, typename value_type>
class Graph {
 private:
  using raw_keys_type = std::vector<key_type>;
  const raw_keys_type &raw_keys_;

 public:
  const raw_keys_type &raw_keys() const { return raw_keys_; }

 private:
  using raw_values_type = std::vector<value_type>;
  const raw_values_type &raw_values_;

 public:
  const raw_values_type &raw_values() const { return raw_values_; }

 private:
  using raw_data_type = std::vector<std::vector<key_type>>;
  const raw_data_type &raw_data_;

 public:
  const raw_data_type &raw_data() const { return raw_data_; }

 public:
  explicit Graph(const raw_keys_type &raw_keys,
                 const raw_values_type &raw_values,
                 const raw_data_type &raw_data)
      : raw_keys_(raw_keys), raw_values_(raw_values), raw_data_(raw_data) {}

  key_type key(const key_type i) const {
    if (raw_keys().at(i) == i) {
      return i;
    } else {
      return key(raw_keys().at(i));
    }
  }

  const value_type &value(const key_type i) const {
    return raw_values().at(key(i));
  }

  mutable std::vector<std::size_t> members_count_;

  std::size_t members_count(const key_type i) const {
    if (members_count_.empty()) {
      members_count_.resize(raw_keys().size(), 0);
      for (key_type j = 0; j < raw_keys().size(); ++j) {
        ++members_count_[key(j)];
      }
    }
    return members_count_.at(key(i));
  }

  bool members_bool(const key_type i,
                    const std::function<bool(const key_type)> f) const {
    const auto k = key(i);
    for (key_type j = 0; j < raw_keys().size(); ++j) {
      if (key(j) == k) {
        if (f(j)) {
          return true;
        }
      }
    }
    return false;
  }

  void members_void(const key_type i,
                    const std::function<void(const key_type)> f) const {
    members_bool(i, [&](const key_type j) -> bool {
      f(j);
      return false;
    });
  }

  bool members_bool(const key_type i,
                    const std::function<bool(const value_type &)> f) const {
    return members_bool(
        i, [&](const key_type j) -> bool { return f(raw_values().at(j)); });
  }

  void members_void(const key_type i,
                    const std::function<void(const value_type &)> f) const {
    members_void(i, [&](const key_type j) -> void { f(raw_values().at(j)); });
  }

  bool members_bool(
      const key_type i,
      const std::function<bool(const key_type, const value_type &)> f) const {
    return members_bool(
        i, [&](const key_type j) -> bool { return f(i, raw_values().at(j)); });
  }

  void members_void(
      const key_type i,
      const std::function<void(const key_type, const value_type &)> f) const {
    members_void(i,
                 [&](const key_type j) -> void { f(j, raw_values().at(j)); });
  }

  mutable std::size_t vertices_count_ = 0;

  std::size_t vertices_count_unsafe() const {
    if (vertices_count_ == 0) {
      key_type i = 0;
      for (; i < raw_keys().size(); ++i) {
        if (key(i) != i) {
          ++i;
          break;
        }
      }
      vertices_count_ = i;
    }
    return vertices_count_;
  }

  std::size_t vertices_count() const {
    if (vertices_count_ == 0) {
      for (key_type i = 0; i < raw_keys().size(); ++i) {
        if (key(i) == i) {
          ++vertices_count_;
        }
      }
    }
    return vertices_count_;
  }

  static bool dfs_bool_unsafe(const Graph &graph, const key_type k,
                              const std::function<bool(const key_type)> f) {
    auto visited = std::vector<bool>(graph.raw_data().size(), false);
    auto stack = std::stack<key_type>();
    stack.push(k);
    while (!stack.empty()) {
      const auto v = stack.top();
      stack.pop();
      if (!visited.at(v)) {
        visited[v] = true;
        if (f(v)) {
          return true;
        }
        for (const auto w : graph.raw_data().at(v)) {
          stack.push(w);
        }
      }
    }
    return false;
  }

  bool dfs_bool_unsafe(const key_type k,
                       const std::function<bool(const key_type)> f) const {
    return dfs_bool_unsafe(*this, k, f);
  }
  bool dfs_bool_unsafe(const key_type k,
                       const std::function<bool(const value_type &)> f) const {
    return dfs_bool_unsafe(
        k, [&](const key_type k) -> bool { return f(value(k)); });
  }
  bool dfs_bool_unsafe(
      const key_type k,
      const std::function<bool(const key_type, const value_type &)> f) const {
    return dfs_bool_unsafe(
        k, [&](const key_type k) -> bool { return f(k, value(k)); });
  }
  void dfs_void_unsafe(const key_type k,
                       const std::function<void(const key_type)> f) const {
    dfs_bool_unsafe(k, [&](const key_type k) -> bool {
      f(k);
      return false;
    });
  }
  void dfs_void_unsafe(const key_type k,
                       const std::function<void(const value_type &)> f) const {
    dfs_void_unsafe(k, [&](const key_type k) -> void { f(value(k)); });
  }
  void dfs_void_unsafe(
      const key_type k,
      const std::function<void(const key_type, const value_type &)> f) const {
    dfs_void_unsafe(k, [&](const key_type k) -> void { f(k, value(k)); });
  }

  bool vertices_bool_unsafe(key_type i,
                            const std::function<bool(const key_type)> f) const {
    for (; i < vertices_count_unsafe(); ++i) {
      if (f(i)) {
        return true;
      }
    }
    return false;
  }

  bool vertices_bool_unsafe(
      key_type i, const std::function<bool(const value_type &)> f) const {
    return vertices_bool_unsafe(
        i, [&](const key_type k) -> bool { return f(value(k)); });
  }

  bool vertices_bool_unsafe(
      key_type i,
      const std::function<bool(const key_type K, const value_type &)> f) const {
    return vertices_bool_unsafe(
        i, [&](const key_type k) -> bool { return f(k, value(k)); });
  }

  bool vertices_bool(const std::function<bool(const key_type)> f) const {
    for (key_type i = 0; i < raw_keys().size(); ++i) {
      if (key(i) == i) {
        if (f(i)) {
          return true;
        }
      }
    }
    return false;
  }
  void vertices_void(const std::function<void(const key_type)> f) const {
    vertices_bool([&](const key_type k) -> bool {
      f(k);
      return false;
    });
  }
  bool vertices_bool(const std::function<bool(const value_type &)> f) const {
    return vertices_bool([&](const key_type k) -> bool { return f(value(k)); });
  }
  void vertices_void(const std::function<void(const value_type &)> f) const {
    vertices_void([&](const key_type k) -> void { f(value(k)); });
  }
  bool vertices_bool(
      const std::function<bool(const key_type, const value_type &)> f) const {
    return vertices_bool(
        [&](const key_type k) -> bool { return f(k, value(k)); });
  }
  void vertices_void(
      const std::function<void(const key_type, const value_type &)> f) const {
    vertices_void([&](const key_type k) -> void { f(k, value(k)); });
  }
  bool edges_bool(const key_type i,
                  const std::function<bool(const key_type)> f) const {
    std::vector<key_type> a(raw_data().size(),
                            std::numeric_limits<key_type>::max());
    const auto ki = key(i);
    return members_bool(ki, [&](const key_type i) -> bool {
      if (!(i < raw_data().size())) {
        return false;
      }
      for (const auto j : raw_data().at(i)) {
        const auto kj = key(j);
        if (a.at(kj) != ki && kj != ki) {
          a[kj] = ki;
          if (f(kj)) {
            return true;
          }
        }
      }
      return false;
    });
  }
  void edges_void(const key_type i,
                  const std::function<void(const key_type)> f) const {
    edges_bool(i, [&](const key_type kj) -> bool {
      f(kj);
      return false;
    });
  }
  bool edges_bool(const key_type i,
                  const std::function<bool(const value_type &)> f) const {
    return edges_bool(i,
                      [&](const key_type kj) -> bool { return f(value(kj)); });
  }
  void edges_void(const key_type i,
                  const std::function<void(const value_type &)> f) const {
    edges_void(i, [&](const key_type kj) -> void { f(value(kj)); });
  }
  bool edges_bool(
      const key_type i,
      const std::function<bool(const key_type, const value_type &)> f) const {
    return edges_bool(
        i, [&](const key_type kj) -> bool { return f(kj, value(kj)); });
  }
  void edges_void(
      const key_type i,
      const std::function<void(const key_type, const value_type &)> f) const {
    edges_void(i, [&](const key_type kj) -> void { f(kj, value(kj)); });
  }
  bool edges_bool_unsafe(const key_type i,
                         const std::function<bool(const key_type)> f) const {
    for (const auto j : raw_data().at(i)) {
      if (f(j)) {
        return true;
      }
    }
    return false;
  }
  void edges_void_unsafe(const key_type i,
                         const std::function<void(const key_type)> f) const {
    edges_bool_unsafe(i, [&](const key_type kj) -> bool {
      f(kj);
      return false;
    });
  }
  bool edges_bool_unsafe(
      const key_type i, const std::function<bool(const value_type &)> f) const {
    return edges_bool_unsafe(
        i, [&](const key_type kj) -> bool { return f(value(kj)); });
  }
  void edges_void_unsafe(
      const key_type i, const std::function<void(const value_type &)> f) const {
    edges_void_unsafe(i, [&](const key_type kj) -> void { f(value(kj)); });
  }
  bool edges_bool_unsafe(
      const key_type i,
      const std::function<bool(const key_type, const value_type &)> f) const {
    return edges_bool_unsafe(
        i, [&](const key_type kj) -> bool { return f(kj, value(kj)); });
  }
  void edges_void_unsafe(
      const key_type i,
      const std::function<void(const key_type, const value_type &)> f) const {
    edges_void_unsafe(i, [&](const key_type kj) -> void { f(kj, value(kj)); });
  }
};

class Helper {
 public:
  template <typename... T>
  static bool returnTrue(T...) {
    return true;
  }

 public:
  template <typename... T>
  static bool returnFalse(T...) {
    return false;
  }

 public:
  template <typename T>
  static bool hasSortedSetIntersection(const std::vector<T> &first,
                                       const std::vector<T> &second) {
    assert(std::is_sorted(first.begin(), first.end()));
    assert(std::is_sorted(second.begin(), second.end()));
    // e.g. if first = [] or second = []
    if (first.empty() || second.empty()) return false;
    // e.g. if first = [3, 4] and second = [1, 2]
    if (*(first.begin()) > *(second.end() - 1)) return false;
    // e.g. if first = [1, 2] and second = [3, 4]
    if (*(second.begin()) > *(first.end() - 1)) return false;
    // get smallest and largest of first and second
    const auto &smallest = (first.size() <= second.size()) ? first : second;
    const auto &largest = (first.size() > second.size()) ? first : second;
    size_t i = 0;
    size_t j = 0;
    while (i < smallest.size() && j < smallest.size()) {
      if (smallest.at(i) < largest.at(j)) {
        ++i;
        continue;
      }
      if (smallest.at(i) > largest.at(j)) {
        ++j;
        continue;
      }
      assert(smallest.at(i) == largest.at(i));
      return true;
    }
    return false;
  }

 public:
  static bool isRoot(const std::string &v) { return v.at(0) == 'r'; }

 public:
  static bool isSink(const std::string &v) { return v.at(0) == 'h'; }

 public:
  static bool isNormal(const std::string &v) { return v.at(0) == 'v'; }

 public:
  static bool isNotRoot(const std::string &v) { return !isRoot(v); }

 public:
  static bool isNotSink(const std::string &v) { return !isSink(v); }

 public:
  static bool isNotNormal(const std::string &v) { return !isNormal(v); }

 public:
  static bool hasUnderscore(const std::string &v) {
    return v.find('_') != std::string::npos;
  }

 private:
  template <typename K, typename V>
  static std::size_t countVertices(const Graph<K, V> &g,
                                   const std::function<bool(const V &)> f) {
    std::size_t count = 0;
    g.vertices_void([&](const V &v) -> void {
      if (f(v)) {
        ++count;
      }
    });
    return count;
  }

 public:
  template <typename K, typename V>
  static std::size_t countVertices(const Graph<K, V> &g) {
    return countVertices<K, V>(g, returnTrue<const V &>);
  }

 public:
  template <typename K, typename V>
  static std::size_t countRootVertices(const Graph<K, V> &g) {
    return countVertices<K, V>(g, isRoot);
  }

 public:
  template <typename K, typename V>
  static std::size_t countSinkVertices(const Graph<K, V> &g) {
    return countVertices<K, V>(g, isSink);
  }

 public:
  template <typename K, typename V>
  static std::size_t countNormalVertices(const Graph<K, V> &g) {
    return countVertices<K, V>(g, isNormal);
  }

 public:
  template <typename K, typename V>
  static void assertCountVertices(const Graph<K, V> &g) {
    (void)g;
#ifndef NDEBUG
    const auto total = countVertices(g);
    const auto root = countRootVertices(g);
    const auto sink = countSinkVertices(g);
    const auto normal = countNormalVertices(g);
#endif
    assert(total >= 1);
    assert(root == 0 || root == 1);
    // assert(sink >= 1);
    assert(sink >= 0);
    assert(normal >= 0);
    assert(total == (root + sink + normal));
    assert(total == countDisjointSetRepresentatives(g.raw_keys()));
    assert(total == g.raw_data().size());
  }

 private:
  template <typename K, typename V>
  static std::size_t countEdges(
      const Graph<K, V> &g, const std::function<bool(const V &, const V &)> f) {
    std::size_t count = 0;
    g.vertices_void([&](const K k, const V &vi) -> void {
      g.edges_void_unsafe(k, [&](const V &vj) -> void {
        if (f(vi, vj)) {
          ++count;
        }
      });
    });
    return count;
  }

 public:
  template <typename K, typename V>
  static std::size_t countEdges(const Graph<K, V> &g) {
    return countEdges<K, V>(g,
                            [&](const V &, const V &) -> bool { return true; });
  }

  template <typename T>
  static std::size_t countEdgesUnsafe(const adjacency_list<T> &adjacencyList) {
    std::size_t count = 0;
    for (const auto &edges : adjacencyList) {
      count += edges.size();
    }
    return count;
  }

  template <typename T>
  static std::size_t countEdgesSafe(const disjoint_sets<T> &disjointSets,
                                    const adjacency_list<T> &adjacencyList) {
    std::size_t count = 0;
    for (const auto i : getDisjointSetRepresentatives(disjointSets)) {
      for (const auto j : adjacencyList.at(i)) {
        if (findDisjointSetRepresentative(disjointSets, j) == j) {
          ++count;
        }
      }
    }
    return count;
  }

 public:
  template <typename K, typename V>
  static std::size_t countRootEdges(const Graph<K, V> &g) {
    return countEdges<K, V>(g, [&](const V &vi, const V &vj) -> bool {
      return isRoot(vi) || isRoot(vj);
    });
  }

 public:
  template <typename K, typename V>
  static std::size_t countSinkEdges(const Graph<K, V> &g) {
    return countEdges<K, V>(g, [&](const V &vi, const V &vj) -> bool {
      return !(isRoot(vi) || isRoot(vj)) && (isSink(vi) || isSink(vj));
    });
  }

 public:
  template <typename K, typename V>
  static std::size_t countNormalEdges(const Graph<K, V> &g) {
    return countEdges<K, V>(g, [&](const V &vi, const V &vj) -> bool {
      return isNormal(vi) && isNormal(vj);
    });
  }

 public:
  template <typename K, typename V>
  static void assertCountEdges(const Graph<K, V> &g) {
#ifndef NDEBUG
    const auto total = countEdges(g);
    const auto root = countRootEdges(g);
    const auto sink = countSinkEdges(g);
    const auto normal = countNormalEdges(g);
#else
    (void)g;
#endif
    assert(total >= 0);
    assert(root == 0 || root == countSinkVertices(g));
    assert(sink >= 0);
    assert(normal >= 0);
    assert(total == (root + sink + normal));
    assert(total == countEdgesSafe(g.raw_keys(), g.raw_data()));
    assert(total == countEdgesUnsafe(g.raw_data()));
  }

 private:
  template <typename K, typename V>
  static bool hasEdge(const Graph<K, V> &g,
                      const std::function<bool(const V &)> fi,
                      const std::function<bool(const V &)> fj) {
    return g.vertices_bool([&](const K ki, const V &vi) -> bool {
      return fi(vi) && g.edges_bool_unsafe(
                           ki, [&](const V &vj) -> bool { return fj(vj); });
    });
  }

 public:
  template <typename K, typename V>
  static bool isBackwardDirected(const Graph<K, V> &g) {
    return hasEdge<K, V>(g, isSink, isNormal);
  }

 public:
  template <typename K, typename V>
  static bool isForwardDirected(const Graph<K, V> &g) {
    return hasEdge<K, V>(g, isNormal, isSink);
  }

 public:
  template <typename K, typename V>
  static void assertBackwardDirected(const Graph<K, V> &g) {
    (void)g;
    if (countEdges(g) == 0) return;
    assert(isBackwardDirected(g));
    assert(!isForwardDirected(g));
    assert((!hasEdge<K, V>(g, isNormal, isNotNormal)));
    // note that this allows sink-to-sink edges, change isRoot to isNotNormal to
    // disable
    assert((!hasEdge<K, V>(g, isSink, isRoot)));
    assert((!hasEdge<K, V>(g, isRoot, isNotSink)));
  }

 public:
  template <typename K, typename V>
  static void assertForwardDirected(const Graph<K, V> &g) {
    (void)g;
    if (countEdges(g) == 0) return;
    assert(isForwardDirected(g));
    assert(!isBackwardDirected(g));
    assert((!hasEdge<K, V>(g, isNormal, isRoot)));
    // note that this allows sink-to-sink edges, change isNormal to isNotRoot to
    // disable
    assert((!hasEdge<K, V>(g, isSink, isNormal)));
    assert((!hasEdge<K, V>(g, isRoot, returnTrue<const V &>)));
  }

 private:
  template <typename K, typename V>
  static void writeVertices(const Graph<K, V> &g, std::ostream &os,
                            const std::function<bool(const V &)> f) {
    g.vertices_void([&](const V &v) -> void {
      if (f(v)) {
        os << v << std::endl;
      }
    });
  }

 public:
  template <typename K, typename V>
  static void writeVertices(const Graph<K, V> &g, std::ostream &os) {
    writeVertices<K, V>(g, os, returnTrue<const V &>);
  }

 public:
  template <typename K, typename V>
  static void writeRootVertices(const Graph<K, V> &g, std::ostream &os) {
    writeVertices<K, V>(g, os, isRoot);
  }

 public:
  template <typename K, typename V>
  static void writeSinkVertices(const Graph<K, V> &g, std::ostream &os) {
    writeVertices<K, V>(g, os, isSink);
  }

 public:
  template <typename K, typename V>
  static void writeNormalVertices(const Graph<K, V> &g, std::ostream &os) {
    writeVertices<K, V>(g, os, isNormal);
  }

 public:
  template <typename K, typename V>
  static void writeEdges(const Graph<K, V> &g, std::ostream &os) {
    g.vertices_void([&](const K k, const V &vi) -> void {
      g.edges_void_unsafe(
          k, [&](const V &vj) -> void { os << vi << "\t" << vj << std::endl; });
    });
  }

 public:
  template <typename K, typename V>
  static void writeMembers(const Graph<K, V> &g, std::ostream &os) {
    g.vertices_void([&](const K k, const V &vi) -> void {
      g.members_void(
          k, [&](const V &vj) -> void { os << vi << "\t" << vj << std::endl; });
    });
  }

 public:
  template <typename K, typename V>
  static void writeVerticesAndEdges(const Graph<K, V> &g, std::ostream &os) {
    g.vertices_void([&](const K k, const V &vi) -> void {
      os << vi << std::endl;
      g.edges_void_unsafe(
          k, [&](const V &vj) -> void { os << vi << "\t" << vj << std::endl; });
    });
  }

 public:
  template <typename K, typename V>
  static void writeGraphvizDotFirst(const Graph<K, V> &g, std::ostream &os) {
    os << "digraph {" << std::endl;
    {
      os << "graph [";
      { os << R"(bgcolor="transparent")"; }
      os << "];" << std::endl;
    }
    g.vertices_void([&](const K ki, const V &vi) -> void {
      // skip the root
      if (isRoot(vi)) {
        return;
      }
      os << " " << vi << " [";
      {
        os << R"(label="{)" << vi;
        g.members_void(ki, [&](const K kj, const V &vj) -> void {
          if (ki != kj) {
            os << ", " << vj;
          }
        });
        os << R"(}")";
      }
      { os << R"(, shape=circle)"; }
      { os << R"(, style=filled)"; }
      { os << R"(, fontcolor="transparent")"; }
      const auto color = [&](const V &v) -> std::string {
        if (isRoot(v)) return "00FF00";
        if (isSink(v)) return "FF0000";
        if (isNormal(v)) return "0000FF";
        return "000000";
      };
      { os << R"(, fillcolor="#)" << color(vi) << R"(")"; }
      { os << R"(, color="#)" << color(vi) << R"(")"; }
      os << "];" << std::endl;
    });
    g.vertices_void([&](const K ki, const V &vi) -> void {
      // skip the root
      if (isRoot(vi)) {
        return;
      }
      g.edges_void_unsafe(ki, [&](const V &vj) -> void {
        // skip the root
        if (isRoot(vj)) {
          return;
        }
        os << " " << vi << " -> " << vj << "[";
        { os << R"(color="#FFFFFF")"; }
        os << "];" << std::endl;
      });
    });
    os << "}";
  }

  template <typename K, typename V>
  static void writeGraphvizDot(const Graph<K, V> &g, std::ostream &os) {
    writeGraphvizDotFirst(g, os);
  }

 private:
  template <typename T, typename Sigmod2017OrVldb2018>
  static void initializeSigmod2017OrVldb2018Graph1(
      const adjacency_list<T> &adjacencyList,
      Sigmod2017OrVldb2018 &sigmod2017OrVldb2018Graph) {
    size_t size = adjacencyList.size();
    // assert(size >= 0 && size <= std::numeric_limits<int>::max());
    sigmod2017OrVldb2018Graph.vertexNum = static_cast<int>(size);
    sigmod2017OrVldb2018Graph.edgeNum = 0;
    for (auto i = 0; i < sigmod2017OrVldb2018Graph.vertexNum; ++i) {
      sigmod2017OrVldb2018Graph.edgeNum += adjacencyList.at(i).size();
    }
  }

  template <typename T, typename Sigmod2017OrVldb2018>
  static void initializeSigmod2017OrVldb2018Graph2(
      const adjacency_list<T> &adjacencyList,
      Sigmod2017OrVldb2018 &sigmod2017OrVldb2018Graph) {
    for (auto i = 0; i < sigmod2017OrVldb2018Graph.vertexNum; ++i) {
      sigmod2017OrVldb2018Graph.inEdge[i].degree = 0;
    }
    int m = 0;
    for (auto i = 0; i < sigmod2017OrVldb2018Graph.vertexNum; ++i) {
      sigmod2017OrVldb2018Graph.outEdge[i].start = m;
      sigmod2017OrVldb2018Graph.outEdge[i].degree = adjacencyList.at(i).size();
      for (auto j = 0; j < sigmod2017OrVldb2018Graph.outEdge[i].degree; ++j) {
        const auto k = adjacencyList.at(i).at(j);
        sigmod2017OrVldb2018Graph.inEdge[k].degree++;
        sigmod2017OrVldb2018Graph.edge[m++] = k;
      }
    }
  }

 private:
  template <typename T, typename Sigmod2017OrVldb2018>
  static adjacency_list<T> getSigmod2017OrVldb2018Graph(
      const Sigmod2017OrVldb2018 &sigmod2017OrVldb2018Graph) {
    auto adjacencyList = adjacency_list<T>(sigmod2017OrVldb2018Graph.vertexNum);
    for (auto i = 0; i < sigmod2017OrVldb2018Graph.vertexNum; ++i) {
      const auto degree = sigmod2017OrVldb2018Graph.outEdge[i].degree;
      if (degree <= 0) continue;
      adjacencyList[i].reserve(degree);
      const auto start = sigmod2017OrVldb2018Graph.outEdge[i].start;
      for (auto j = start; j < start + degree; ++j) {
        adjacencyList[i].push_back(sigmod2017OrVldb2018Graph.edge[j]);
      }
      adjacencyList[i].resize(adjacencyList[i].size());
    }
    return adjacencyList;
  }

 public:
  template <typename T>
  static sigmod2017::Sigmod2017 initializeButrGraph(
      const adjacency_list<T> &adjacencyList) {
    auto butrGraph = sigmod2017::Sigmod2017();
    initializeSigmod2017OrVldb2018Graph1(adjacencyList, butrGraph);
    butrGraph.initGraph();
    initializeSigmod2017OrVldb2018Graph2(adjacencyList, butrGraph);
    return butrGraph;
  }

 public:
  static void executeButrGraph(sigmod2017::Sigmod2017 &sigmod2017Graph) {
    sigmod2017Graph.reverse();
    // markCNRRN
    sigmod2017Graph.zeroVertex();
    sigmod2017Graph.initPMTTR();
    sigmod2017Graph.constructTree();
    sigmod2017Graph.constructTreeR();
    sigmod2017Graph.initURN();
    // buTR
    sigmod2017Graph.initBU();
    sigmod2017Graph.estimationOutG();
    sigmod2017Graph.sortNewGraph();
    sigmod2017Graph.getPoTree();
    sigmod2017Graph.buTree();
    sigmod2017Graph.fun();
  }

 public:
  template <typename T>
  static adjacency_list<T> getButrGraph(sigmod2017::Sigmod2017 &butrGraph) {
    return getSigmod2017OrVldb2018Graph<T>(butrGraph);
  }

 public:
  static void deleteButrGraph(sigmod2017::Sigmod2017 &sigmod2017Graph) {
    sigmod2017Graph.destroyBU();
    sigmod2017Graph.destroyPMTTR();
    sigmod2017Graph.destroyGraph();
  }

 public:
  template <typename T>
  static vldb2018::Vldb2018 initializeTwoHopLabel(
      const adjacency_list<T> &adjacencyList) {
    auto twoHopLabel = vldb2018::Vldb2018();
    initializeSigmod2017OrVldb2018Graph1(adjacencyList, twoHopLabel);
    twoHopLabel.initGraph();
    initializeSigmod2017OrVldb2018Graph2(adjacencyList, twoHopLabel);
    return twoHopLabel;
  }

 public:
  static void executeTwoHopLabel(vldb2018::Vldb2018 &vldb2018Graph) {
    vldb2018Graph.reverse();
    vldb2018Graph.zeroVertex();

    vldb2018Graph.initPMTTR();

    /*
       vldb2018Graph.constructTree();
       vldb2018Graph.constructTreeR();
       vldb2018Graph.initURN();
       vldb2018Graph.initBU();
       vldb2018Graph.estimationOutG();
       vldb2018Graph.sortNewGraph();
       vldb2018Graph.getPoTree();
       vldb2018Graph.buTree();
       vldb2018Graph.fun();
       */

    vldb2018Graph.initEqVertex();
    for (int i = 0; i < vldb2018Graph.vertexNum; i++) {
      vldb2018Graph.convertexEqualVertex[i] = i;
    }

    /*
       vldb2018Graph.equalNum();
       vldb2018Graph.delEqualVertex();
       */

    vldb2018Graph.initHopLabel();
    vldb2018Graph.initUpRoot();
    vldb2018Graph.initDownRoot();
    vldb2018Graph.portalVertexOrder();
    vldb2018Graph.createP2HLabel();

    vldb2018Graph.initDim();
    vldb2018Graph.createDimLabelLeftToRight();
    vldb2018Graph.createDimLabelRightToLeft();
    vldb2018Graph.createDimLabelRLeftToRight();
    vldb2018Graph.createDimLabelRRightToLeft();

    vldb2018Graph.sortIndex();
  }

 public:
  template <typename K>
  static bool queryTwoHopLabel(vldb2018::Vldb2018 &twoHopLabel, const K u,
                               const K v) {
    return twoHopLabel.queryImpImp(u, v);
  }

 public:
  static void deleteTwoHopLabel(vldb2018::Vldb2018 &vldb2018Graph) {
    vldb2018Graph.destroyPMTTR();
    vldb2018Graph.destroyBU();
    vldb2018Graph.destroyEqVertex();
    vldb2018Graph.destroyHopLabel();
    vldb2018Graph.destroyDimLabel();
    vldb2018Graph.destroyGraph();
  }

 public:
  template <typename K, typename V = std::string>
  static std::vector<std::vector<bool>> getReachabilityOracle(
      const hparg::detail::Graph<K, V> &graph) {
    const auto totalVertices = countVertices(graph);
    auto reachabilityOracle = std::vector<std::vector<bool>>(
        totalVertices, std::vector<bool>(totalVertices, false));
    graph.vertices_void([&](const K k1, const V &v1) -> void {
      if (isNormal(v1)) {
        graph.dfs_void_unsafe(k1, [&](const K k2, const V &v2) -> void {
          if (isSink(v2)) {
            reachabilityOracle[k1][k2] = true;
          }
        });
      }
    });
    return reachabilityOracle;
  }

 public:
  template <typename K, typename V = std::string>
  static bool queryReachabilityOracle(
      const Graph<K, V> &graph,
      const std::vector<std::vector<bool>> &reachabilityOracle, const K u,
      const K v) {
    return reachabilityOracle.at(graph.key(u)).at(graph.key(v));
  }

 private:
  template <typename K, typename V>
  static std::size_t countFromNormalToSinkReaches(
      const Graph<K, V> &graph,
      const std::function<bool(const K, const K)> fromNormalToSinkReaches) {
    /*
       const auto roots = countRootVertices(graph);
       const auto sinks = countSinkVertices(graph);
       */
    std::size_t count = 0;
    /*
       graph.vertices_bool_unsafe(roots, [&](const K &k1) -> bool {
       if (k1 == roots + sinks) return true;
       */
    graph.vertices_bool([&](const K &k1) -> bool {
      if (!isSink(graph.value(k1))) return false;
      std::size_t sinksInSink = 0, normalsInSink = 0;
      graph.members_void(k1, [&](const V &v2) -> void {
        if (isSink(v2)) {
          ++sinksInSink;
        } else if (isNormal(v2)) {
          ++normalsInSink;
        }
      });
      count += sinksInSink * normalsInSink;
      /*
         graph.vertices_bool_unsafe(roots + sinks, [&](const K &k2) -> bool {
         */
      graph.vertices_bool([&](const K &k2) -> bool {
        if (!isNormal(graph.value(k2))) return false;
        if (fromNormalToSinkReaches(k2, k1)) {
          std::size_t normalsInNormal = graph.members_count(k2);
          count += normalsInNormal * sinksInSink;
        }
        return false;
      });
      return false;
    });
    return count;
  }

 public:
  template <typename K, typename V>
  static std::size_t countTwoHopLabel(const Graph<K, V> &graph,
                                      vldb2018::Vldb2018 &twoHopLabel) {
    const auto f = [&](const K u, const K v) -> bool {
      return queryTwoHopLabel(twoHopLabel, u, v);
    };
    return countFromNormalToSinkReaches<K, V>(graph, f);
  }

 public:
  template <typename K, typename V>
  static std::size_t countReachabilityOracle(
      const Graph<K, V> &graph,
      const std::vector<std::vector<bool>> &reachabilityOracle) {
    const auto f = [&](const K u, const K v) -> bool {
      return queryReachabilityOracle(graph, reachabilityOracle, u, v);
    };
    return countFromNormalToSinkReaches<K, V>(graph, f);
  }

 public:
  static std::size_t sizeOfTwoHopLabel(const vldb2018::Vldb2018 &twoHopLabel) {
    return countEdgesUnsafe(twoHopLabel.inLabel) +
           countEdgesUnsafe(twoHopLabel.outLabel);
  }

 public:
  static std::vector<std::string> splitString(const std::string &string,
                                              const char delimiter) {
    if (string.empty()) return std::vector<std::string>({std::string()});
    if (string.find_first_of(delimiter) == std::string::npos)
      return std::vector<std::string>({string});
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(string);
    while (std::getline(stream, token, delimiter)) {
      tokens.push_back(token);
    }
    if (string.back() == delimiter) tokens.push_back(std::string());
    return tokens;
  }

 public:
  static std::string joinStrings(const std::vector<std::string> &strings,
                                 const char delimiter) {
    // note that an empty array of strings is different to an array with only
    // the empty string, hence the assertion
    if (strings.empty()) return std::string();
    if (strings.size() == 1) return strings.front();
    std::ostringstream stream;
    auto it = strings.begin();
    while (it != strings.end() - 1) {
      stream << *it;
      stream << delimiter;
      ++it;
    }
    stream << *it;
    return stream.str();
  }

 public:
  template <typename T>
  static std::vector<T> getPartialTopologicalOrder(
      const adjacency_list<T> &adjacencyList, const T begin) {
    // order maps old indices to new ones
    auto order =
        std::vector<T>(adjacencyList.size(), std::numeric_limits<T>::max());
    T size = 0;
    std::function<void(T)> visit = [&](T n) {
      if (order.at(n) != std::numeric_limits<T>::max()) return;
      for (const auto m : adjacencyList.at(n)) visit(m);
      order[n] = size++;
    };

    visit(begin);

    // max index is one less than size
    T max = size - 1;
    // compute order as per reverse topsort
    for (size_t i = 0; i < order.size(); ++i) {
      if (order.at(i) == std::numeric_limits<T>::max()) continue;
      order[i] = max - order.at(i);
    }
    return order;
  }

 public:
  template <typename T>
  static std::vector<T> getTotalTopologicalOrder(
      const adjacency_list<T> &adjacencyList) {
    // order maps old indices to new ones
    auto order =
        std::vector<T>(adjacencyList.size(), std::numeric_limits<T>::max());
    T count = 0;
    std::function<void(T)> visit = [&](T n) {
      if (order.at(n) != std::numeric_limits<T>::max()) return;
      for (const auto m : adjacencyList.at(n)) visit(m);
      order[n] = count++;
    };

    assert(static_cast<T>(adjacencyList.size()) == adjacencyList.size());
    for (T i = 0; i < static_cast<T>(adjacencyList.size()); ++i) {
      visit(i);
    }

    // max index is one less than size
    T max = count - 1;
    // compute order as per reverse topsort
    for (size_t i = 0; i < order.size(); ++i) {
      assert(order.at(i) != std::numeric_limits<T>::max());
      order[i] = max - order.at(i);
    }
    return order;
  }

 public:
  static std::vector<size_t> getOrderByFirstLetter(
      const std::vector<std::string> &strings,
      const std::vector<char> &letters) {
    auto indices =
        std::vector<size_t>(strings.size(), std::numeric_limits<size_t>::max());
    auto counts = std::vector<size_t>(letters.size(), 0);
    // e.g. counts = { root: 0, sink: 0, normal: 0}
    for (size_t i = 0; i < strings.size(); ++i) {
      for (size_t j = 0; j < letters.size(); ++j) {
        if (strings.at(i).at(0) == letters.at(j)) {
          indices[i] = counts.at(j);
          counts[j]++;
          break;
        }
        // should not reach here unless some vertex has no prefix
        assert(j != letters.size() - 1);
      }
    }
    // e.g. counts = { root: 1, sink: 2, normal: 3}
    for (size_t i = 1; i < letters.size(); ++i) {
      counts[i] += counts[i - 1];
    }
    assert(counts.at(0) == 1);
    // e.g. counts = { root: 1, sink: 3, normal: 6 }
    for (size_t i = 0; i < strings.size(); ++i) {
      for (size_t j = 1; j < letters.size(); ++j) {
        if (strings.at(i).at(0) == letters.at(j)) {
          indices[i] += counts[j - 1];
          break;
        }
        // should not reach here unless some vertex has no prefix
        assert(j != letters.size() - 1 || strings.at(i).at(0) == 'r');
      }
      assert(indices.at(i) < strings.size());
    }
    assert(std::find(indices.begin(), indices.end(),
                     std::numeric_limits<size_t>::max()) == indices.end());
    return indices;
  }

 public:
  template <typename T>
  static std::vector<T> getVectorOfRange(const T begin, const T end) {
    // get each index from begin to end
    if (begin == end) return std::vector<T>();
    assert(begin < end);
    auto elements = std::vector<T>(end - begin);
    for (T i = 0; i < elements.size(); ++i) {
      elements[i] = begin + i;
    }
    return elements;
  }

 public:
  template <typename T>
  static adjacency_list<T> getTransposedAdjacencyList(
      const adjacency_list<T> &a) {
    auto b = adjacency_list<T>(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
      for (size_t j = 0; j < a.at(i).size(); ++j) {
        const auto k = a.at(i).at(j);
        b[k].push_back(i);
      }
    }
    return b;
  }

 public:
  template <typename T>
  static T findDisjointSetRepresentative(const disjoint_sets<T> &disjointSets,
                                         const T element) {
    /*
       assert(element < disjointSets.size());
       if (disjointSets.at(element) == element) {
       return element;
       } else {
       T *ptr = const_cast<T *>(disjointSets.data()) + element;
       return *ptr = findDisjointSetRepresentative(disjointSets,
       disjointSets.at(element));
       }
       */
    return findDisjointSetRepresentative(
        const_cast<disjoint_sets<T> &>(disjointSets), element);
  }

 public:
  template <typename T>
  static T findDisjointSetRepresentative(disjoint_sets<T> &disjointSets,
                                         const T element) {
    assert(element < disjointSets.size());
    if (disjointSets.at(element) == element) {
      return element;
    } else {
      return disjointSets[element] = findDisjointSetRepresentative(
                 disjointSets, disjointSets.at(element));
    }
  }

 public:
  template <typename T>
  static void unifyDisjointSetsOfElements(disjoint_sets<T> &disjointSets,
                                          const T firstElement,
                                          const T secondElement) {
    if (firstElement == secondElement) return;
    const auto firstRepresentative =
        findDisjointSetRepresentative(disjointSets, firstElement);
    const auto secondRepresentative =
        findDisjointSetRepresentative(disjointSets, secondElement);
    if (firstRepresentative == secondRepresentative) return;
    if (firstRepresentative < secondRepresentative) {
      disjointSets[secondRepresentative] = firstRepresentative;
    } else {
      disjointSets[firstRepresentative] = secondRepresentative;
    }
  }

 public:
  template <typename T>
  static size_t countDisjointSetRepresentatives(
      const disjoint_sets<T> &disjointSets) {
    size_t count = 0;
    assert(disjointSets.size() == static_cast<T>(disjointSets.size()));
    for (T element = 0; element < static_cast<T>(disjointSets.size());
         ++element) {
      const auto representative =
          findDisjointSetRepresentative(disjointSets, element);
      if (element == representative) ++count;
    }
    return count;
  }

 public:
  template <typename T>
  static std::vector<T> getDisjointSetRepresentatives(
      const disjoint_sets<T> &disjointSets) {
    auto representatives = std::vector<T>();
    representatives.reserve(disjointSets.size());
    assert(disjointSets.size() == static_cast<T>(disjointSets.size()));
    for (T element = 0; element < static_cast<T>(disjointSets.size());
         ++element) {
      const auto representative =
          findDisjointSetRepresentative(disjointSets, element);
      if (element == representative) representatives.push_back(representative);
    }
    representatives.resize(representatives.size());
    return representatives;
  }

 public:
  template <typename T>
  static std::vector<size_t> getOrderWithRepresentativesFirst(
      const disjoint_sets<T> &disjointSets) {
    auto indices = std::vector<size_t>(disjointSets.size(),
                                       std::numeric_limits<size_t>::max());
    size_t index = 0;
    for (const T i : getDisjointSetRepresentatives(disjointSets)) {
      indices[i] = index++;
    }
    for (T i = 0; i < disjointSets.size(); ++i) {
      if (indices.at(i) == std::numeric_limits<T>::max()) indices[i] = index++;
    }
    return indices;
  }

 public:
  template <typename T>
  static void readGraphFromFile(const std::string &file,
                                vertex_labels &vertexLabels,
                                adjacency_list<T> &adjacencyList) {
    assert(vertexLabels.empty());
    assert(adjacencyList.empty());
    std::unordered_map<std::string, T> fromVertexToIndex;
    std::ifstream ifstream(file);
    std::string line;
    auto identifier = [&](const std::string &key) -> T {
      std::string k;
      if (!isNormal(key) && !isSink(key) && !isRoot(key)) {
        k = "v_" + key;
      } else {
        k = key;
      }
      auto iterator = fromVertexToIndex.find(k);
      if (iterator != fromVertexToIndex.end()) {
        return iterator->second;
      } else {
        auto value = static_cast<T>(fromVertexToIndex.size());
        fromVertexToIndex.insert(
            iterator,
            typename std::unordered_map<std::string, T>::value_type(k, value));
        vertexLabels.push_back(k);
        adjacencyList.emplace_back();
        return value;
      }
    };

    while (std::getline(ifstream, line)) {
      std::istringstream istringstream(line);
      std::string left;
      istringstream >> left;
      const auto leftIdentifier = identifier(left);
      std::string right;

      if (istringstream >> right) {
        const auto rightIdentifier = identifier(right);
        const auto &neighbourList = adjacencyList.at(leftIdentifier);
        if (std::find(neighbourList.begin(), neighbourList.end(),
                      rightIdentifier) == neighbourList.end()) {
          adjacencyList[leftIdentifier].push_back(rightIdentifier);
        }
      }
    }

    assert(vertexLabels.size() == fromVertexToIndex.size());
    assert(adjacencyList.size() == fromVertexToIndex.size());
  }

 public:
  template <typename T>
  static std::vector<T> depthFirstSearch(const adjacency_list<T> &adjacencyList,
                                         const T begin) {
    auto visited = std::vector<bool>(adjacencyList.size(), false);
    auto search = std::vector<T>();
    search.reserve(adjacencyList.size());
    auto stack = std::stack<T>();
    stack.push(begin);
    while (!stack.empty()) {
      const auto v = stack.top();
      stack.pop();
      if (!visited.at(v)) {
        visited[v] = true;
        // note that having the search push back here means the begin node is
        // not included in the search
        if (v != begin) search.push_back(v);
        for (const auto w : adjacencyList.at(v)) {
          stack.push(w);
        }
      }
    }
    search.resize(search.size());
    return search;
  }

  template <typename T>
  static std::vector<T> getDepthFirstSearchOrder(
      const adjacency_list<T> &adjacencyList, const T begin) {
    auto visited = std::vector<bool>(adjacencyList.size(), false);
    auto order =
        std::vector<T>(adjacencyList.size(), std::numeric_limits<T>::max());
    T count = 0;
    auto stack = std::stack<T>();
    stack.push(begin);
    while (!stack.empty()) {
      const auto v = stack.top();
      stack.pop();
      if (!visited.at(v)) {
        visited[v] = true;
        order[v] = count++;
        for (const auto w : adjacencyList.at(v)) {
          stack.push(w);
        }
      }
    }
    return order;
  }

 public:
  template <typename Iterable1, typename Iterable2,
            typename = std::enable_if<
                std::is_unsigned<typename Iterable2::value_type>::value>>
  static Iterable1 getAllFirstSortedBySecond(const Iterable1 &first,
                                             const Iterable2 &second) {
    // second is the indices or order
    assert(first.size() <= second.size());
    assert(
        std::find(second.begin(), second.end(),
                  std::numeric_limits<typename Iterable2::value_type>::max()) ==
        second.end());
    auto b = first;
    for (size_t i = 0; i < first.size(); ++i) {
      // a[newIndex(i)] is replaced by a[oldIndex(i)]
      assert(second.at(i) >= 0);
      assert(second.at(i) < b.size());
      b[second.at(i)] = first.at(i);
    }
    return b;
  }

 public:
  template <typename Iterable1, typename Iterable2,
            typename = std::enable_if<
                std::is_unsigned<typename Iterable2::value_type>::value>>
  static void replaceAllFirstBySecondAtFirst(Iterable1 &first,
                                             const Iterable2 &second) {
    // a[i] is replaced the value at the index a[i] in b
    for (size_t i = 0; i < first.size(); ++i) {
      first[i] = second.at(first.at(i));
    }
  }

 public:
  template <typename T>
  static void DEPRECATED_partialSccAlgorithmRecursive(
      const adjacency_list<T> &adjacencyList, const T begin,
      disjoint_sets<T> &disjointSets) {
    // merge all strongly connected components reachable from the begin node

    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));
    auto preOrder =
        std::vector<T>(adjacencyList.size(), std::numeric_limits<T>::max());
    T preOrderCount = 0;

    auto s = std::vector<T>();
    s.reserve(adjacencyList.size());
    T sTOS = std::numeric_limits<T>::max();

    auto p = std::vector<T>();
    p.reserve(adjacencyList.size());
    T pTOS = std::numeric_limits<T>::max();

    auto assigned = std::vector<bool>(adjacencyList.size(), false);

    std::function<void(T)> scR = [&](T w) -> void {
      preOrder[w] = preOrderCount++;
      p[++pTOS] = s[++sTOS] = w;
      for (const auto t : adjacencyList.at(w)) {
        if (preOrder[t] == std::numeric_limits<T>::max()) {
          scR(t);
        } else if (assigned[t] == false) {
          while (preOrder[p[pTOS]] > preOrder[t]) {
            --pTOS;
          }
        }
      }
      if (p[pTOS] == w) {
        --pTOS;
      } else {
        return;
      }
      T v;
      do {
        v = s[sTOS--];
        assigned[v] = true;
        unifyDisjointSetsOfElements(disjointSets, v, w);
      } while (v != w);
    };

    scR(begin);
  }

 public:
  template <typename T>
  static void DEPRECATED_totalSccAlgorithmRecursive(
      const adjacency_list<T> &adjacencyList, disjoint_sets<T> &disjointSets) {
    // merge all strongly connected components

    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));
    auto preOrder =
        std::vector<T>(adjacencyList.size(), std::numeric_limits<T>::max());
    T preOrderCount = 0;

    auto s = std::vector<T>();
    s.reserve(adjacencyList.size());
    T sTOS = std::numeric_limits<T>::max();

    auto p = std::vector<T>();
    p.reserve(adjacencyList.size());
    T pTOS = std::numeric_limits<T>::max();

    auto assigned = std::vector<bool>(adjacencyList.size(), false);

    std::function<void(T)> scR = [&](T w) -> void {
      preOrder[w] = preOrderCount++;
      p[++pTOS] = s[++sTOS] = w;
      for (const auto t : adjacencyList.at(w)) {
        if (preOrder[t] == std::numeric_limits<T>::max()) {
          scR(t);
        } else if (assigned[t] == false) {
          while (preOrder[p[pTOS]] > preOrder[t]) {
            --pTOS;
          }
        }
      }
      if (p[pTOS] == w) {
        --pTOS;
      } else {
        return;
      }
      T v;
      do {
        v = s[sTOS--];
        assigned[v] = true;
        unifyDisjointSetsOfElements(disjointSets, v, w);
      } while (v != w);
    };

    {
      bool allAssigned = false;
      while (!allAssigned) {
        allAssigned = true;
        for (T i = 0; i < assigned.size(); ++i) {
          if (!assigned.at(i)) {
            allAssigned = false;
            scR(i);
          }
        }
      }
    }
  }

 public:
  template <typename T>
  static void totalSccAlgorithmRecursive(const adjacency_list<T> &adjacencyList,
                                         disjoint_sets<T> &disjointSets) {
    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));
    // @see https://llbit.se/?p=3379

    // define None
    const auto None = std::numeric_limits<T>::max();

    // number of vertices
    const auto N = adjacencyList.size();
    // adjacency list
    const auto &adj = adjacencyList;

    // next index
    auto next = static_cast<T>(0);
    auto index = std::vector<T>(N, None);
    auto lowlink = std::vector<T>(N, None);
    auto onstack = std::vector<bool>(N, false);
    auto stack = std::stack<T>();

    const auto sconnect = [&](const T v) -> void {
      index[v] = next;
      lowlink[v] = next;
      next += 1;
      stack.push(v);
      onstack[v] = true;
      for (const auto w : adj.at(v)) {
        if (index.at(w) == None) {
          sconnect(w);
          assert(lowlink.at(v) != None);
          assert(lowlink.at(w) != None);
          lowlink[v] = std::min(lowlink.at(v), lowlink.at(w));
        } else if (onstack.at(w)) {
          assert(lowlink.at(v) != None);
          assert(index.at(w) != None);
          lowlink[v] = std::min(lowlink.at(v), index.at(w));
        }
      }
      if (index.at(v) == lowlink.at(v)) {
        // note that we omit com and groups here
        const auto u = None;
        while (true) {
          const auto w = stack.top();
          stack.pop();
          onstack[w] = false;
          if (u == None) {
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

    for (T v = 0; v < N; ++v) {
      if (index.at(v) == None) {
        sconnect(v);
      }
    }
  }

 public:
  template <typename T>
  static void totalSccAlgorithmNonRecursive(
      const adjacency_list<T> &adjacencyList, disjoint_sets<T> &disjointSets) {
    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));
    // @see https://llbit.se/?p=3379

    // define None
    const auto None = std::numeric_limits<T>::max();

    // number of vertices
    const auto N = adjacencyList.size();
    // adjacency list
    const auto &adj = adjacencyList;

    // next index
    auto next = static_cast<T>(0);
    auto index = std::vector<T>(N, None);
    auto lowlink = std::vector<T>(N, None);
    auto onstack = std::vector<bool>(N, false);
    auto stack = std::stack<T>();

    const auto sconnect = [&](T v) -> void {
      auto work = std::stack<std::pair<T, T>>();
      // new recursion stack
      work.push(std::make_pair(v, 0));
      while (!work.empty()) {
        // i is next successor to process
        const auto vi = work.top();
        work.pop();
        v = vi.first;
        const auto i = vi.second;
        // when first visiting a vertex
        if (i == 0) {
          index[v] = next;
          lowlink[v] = next;
          next += 1;
          stack.push(v);
          onstack[v] = true;
        }
        auto recurse = false;
        for (auto j = i; j < adj.at(v).size(); ++j) {
          const auto w = adj.at(v).at(j);
          if (index.at(w) == None) {
            // changed, add w to recursion stack
            work.push(std::make_pair(v, j + 1));
            work.push(std::make_pair(w, 0));
            recurse = true;
            break;
          } else if (onstack.at(w)) {
            assert(lowlink.at(v) != None);
            assert(index.at(w) != None);
            lowlink[v] = std::min(lowlink.at(v), index.at(w));
          }
        }
        if (recurse) {
          // new
          continue;
        }
        if (index.at(v) == lowlink.at(v)) {
          auto u = None;
          while (true) {
            const auto w = stack.top();
            stack.pop();
            onstack[w] = false;
            if (u == None) {
              u = w;
            } else {
              unifyDisjointSetsOfElements(disjointSets, u, w);
            }
            if (w == v) {
              break;
            }
          }
        }
        // v was recursively visited
        if (!work.empty()) {
          const auto w = v;
          v = work.top().first;
          assert(lowlink.at(v) != None);
          assert(lowlink.at(w) != None);
          lowlink[v] = std::min(lowlink.at(v), lowlink.at(w));
        }
      }
    };

    for (T v = 0; v < N; ++v) {
      if (index.at(v) == None) {
        sconnect(v);
      }
    }
  }

 public:
  template <typename T>
  static void acyclicTitanicAlgorithm(adjacency_list<T> &adjacencyList,
                                      const T begin,
                                      disjoint_sets<T> &disjointSets) {
    const auto order = getPartialTopologicalOrder(adjacencyList, begin);
    assert(static_cast<T>(adjacencyList.size()) == adjacencyList.size());
    for (T i = 0; i < static_cast<T>(adjacencyList.size()); ++i) {
      if (order.at(i) == std::numeric_limits<T>::max()) {
        unifyDisjointSetsOfElements(disjointSets, begin, i);
        adjacencyList.at(i).clear();
      } else {
        auto new_j = 0;
        for (size_t old_j = 0; old_j < adjacencyList.at(i).size(); ++old_j) {
          const auto k = adjacencyList.at(i).at(old_j);
          if (order.at(k) != std::numeric_limits<T>::max()) {
            adjacencyList[i][new_j] = k;
            ++new_j;
          }
        }
        adjacencyList[i].resize(new_j);
      }
    }
  }

  template <typename T>
  static void cyclicTitanicAlgorithm(adjacency_list<T> &adjacencyList,
                                     const T begin,
                                     disjoint_sets<T> &disjointSets) {
    const auto order = getDepthFirstSearchOrder(adjacencyList, begin);
    assert(static_cast<T>(adjacencyList.size()) == adjacencyList.size());
    for (T i = 0; i < static_cast<T>(adjacencyList.size()); ++i) {
      if (order.at(i) == std::numeric_limits<T>::max()) {
        unifyDisjointSetsOfElements(disjointSets, begin, i);
        adjacencyList.at(i).clear();
      } else {
        auto new_j = 0;
        for (size_t old_j = 0; old_j < adjacencyList.at(i).size(); ++old_j) {
          const auto k = adjacencyList.at(i).at(old_j);
          if (order.at(k) != std::numeric_limits<T>::max()) {
            adjacencyList[i][new_j] = k;
            ++new_j;
          }
        }
        adjacencyList[i].resize(new_j);
      }
    }
  }

 public:
  template <typename T>
  static void dominatorAlgorithm(const adjacency_list<T> &adjacencyList,
                                 const T begin,
                                 disjoint_sets<T> &disjointSets) {
    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));
    assert(begin == 0);

    auto idom = std::vector<T>(adjacencyList.size(), 0);
    auto level = std::vector<T>(adjacencyList.size(), 0);

    level[begin] = 0;
    idom[begin] = std::numeric_limits<T>::max();

    const auto topologicalOrder = getTotalTopologicalOrder(adjacencyList);

    const auto reverseTopsortedVertices = ([&]() -> std::vector<T> {
      auto topsortedVertices = getAllFirstSortedBySecond(
          getVectorOfRange<T>(0, adjacencyList.size()), topologicalOrder);
      std::reverse(topsortedVertices.begin(), topsortedVertices.end());
      return topsortedVertices;
    })();

    assert(reverseTopsortedVertices.front() == 0);

    for (const auto u : reverseTopsortedVertices) {
      assert(u != std::numeric_limits<T>::max());
      if (u == begin) continue;
      const auto m = adjacencyList.at(u).size();
      if (m == 1) {
        const auto v = adjacencyList.at(u).at(0);
        idom[u] = v;
        level[u] = level.at(v) + 1;
      } else {
        assert(m > 1);
        std::function<T(T, T)> nca = [&](T u, T v) -> T {
          assert(u != std::numeric_limits<T>::max());
          assert(v != std::numeric_limits<T>::max());
          if (u == v) {
            return u;
          } else if (level.at(u) == level.at(v)) {
            return nca(idom.at(u), idom.at(v));
          } else {
            while (level.at(u) < level.at(v)) {
              v = idom.at(v);
            }
            while (level.at(v) < level.at(u)) {
              u = idom.at(u);
            }
            return nca(u, v);
          }
        };
        auto idom_u = adjacencyList.at(u).at(0);
        assert(idom_u != std::numeric_limits<T>::max());
        for (const auto v : adjacencyList.at(u)) {
          idom_u = nca(idom_u, v);
        }
        assert(idom_u != std::numeric_limits<T>::max());
        idom[u] = idom_u;
        level[u] = level.at(idom_u) + 1;
      }
      assert(idom.at(u) != std::numeric_limits<T>::max());
      if (idom.at(idom.at(u)) != std::numeric_limits<T>::max()) {
        assert(u != begin);
        assert(idom.at(u) != begin);
        assert(findDisjointSetRepresentative(disjointSets, u) != begin);
        assert(findDisjointSetRepresentative(disjointSets, idom.at(u)) !=
               begin);
        unifyDisjointSetsOfElements(disjointSets, u, idom.at(u));
      }
    }
  }

 public:
  template <typename T>
  static void firstModularDecompositionAlgorithm(
      const adjacency_list<T> &adjacencyList, disjoint_sets<T> &disjointSets) {
    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));

    const auto t = getTotalTopologicalOrder(adjacencyList);

    auto t_min1 = std::vector<signed>(adjacencyList.size(),
                                      std::numeric_limits<signed>::max());
    auto t_min2 = std::vector<signed>(adjacencyList.size(),
                                      std::numeric_limits<signed>::max());
    auto t_max = std::vector<signed>(adjacencyList.size(),
                                     std::numeric_limits<signed>::min());

    assert(static_cast<T>(adjacencyList.size()) == adjacencyList.size());

    for (T u = 0; u < static_cast<T>(adjacencyList.size()); ++u) {
      if (adjacencyList.at(u).empty()) continue;
      for (const auto v : adjacencyList.at(u)) {
        if (static_cast<signed>(t.at(v)) < t_min1.at(u)) t_min1[u] = t.at(v);
        if (static_cast<signed>(t.at(v)) > t_max.at(u)) t_max[u] = t.at(v);
      }
      for (const auto v : adjacencyList.at(u)) {
        if (static_cast<signed>(t.at(v)) > t_min1.at(u) &&
            static_cast<signed>(t.at(v)) < t_min2.at(u))
          t_min2[u] = t.at(v);
      }
    }

    auto mark = std::vector<bool>(adjacencyList.size(), false);

    // for each vertex according to the topsort
    for (const auto u : getAllFirstSortedBySecond(
             getVectorOfRange<T>(0, adjacencyList.size()), t)) {
      // skip nodes without successors
      if (adjacencyList.at(u).empty()) continue;
      // merge nodes with one successor
      if (adjacencyList.at(u).size() == 1) {
        const auto v = adjacencyList.at(u).at(0);
        unifyDisjointSetsOfElements(disjointSets, u, v);
        continue;
      }

      for (const auto v : adjacencyList.at(u)) {
        if (static_cast<signed>(t.at(v)) == t_min1.at(u) &&
            adjacencyList.at(u).size() == adjacencyList.at(v).size() + 1 &&
            t_min2.at(u) == t_min1.at(v) && t_max.at(u) == t_max.at(v)) {
          mark[v] = true;
          for (const auto w : adjacencyList.at(v)) mark[w] = true;
          auto marked = true;
          for (const auto w : adjacencyList.at(u)) {
            if (!mark[w]) {
              marked = false;
              break;
            }
          }
          if (marked) {
            unifyDisjointSetsOfElements(disjointSets, u, v);
          }
          mark[v] = false;
          for (const auto w : adjacencyList.at(v)) mark[w] = false;
        }
      }
    }
  }

 public:
  template <typename T>
  static void secondModularDecompositionAlgorithm(
      const adjacency_list<T> &adjacencyList, disjoint_sets<T> &disjointSets) {
    assert(adjacencyList.size() ==
           countDisjointSetRepresentatives(disjointSets));

    const auto t = getTotalTopologicalOrder(adjacencyList);
    auto t_min1 = std::vector<signed>(adjacencyList.size(),
                                      std::numeric_limits<signed>::max());
    auto t_min2 = std::vector<signed>(adjacencyList.size(),
                                      std::numeric_limits<signed>::max());
    auto t_max = std::vector<signed>(adjacencyList.size(),
                                     std::numeric_limits<signed>::min());

    assert(static_cast<T>(adjacencyList.size()) == adjacencyList.size());

    for (T u = 0; u < static_cast<T>(adjacencyList.size()); ++u) {
      if (adjacencyList.at(u).empty()) continue;
      for (const auto v : adjacencyList.at(u)) {
        if (static_cast<signed>(t.at(v)) < t_min1.at(u)) t_min1[u] = t.at(v);
        if (static_cast<signed>(t.at(v)) > t_max.at(u)) t_max[u] = t.at(v);
      }
      for (const auto v : adjacencyList.at(u)) {
        if (static_cast<signed>(t.at(v)) > t_min1.at(u) &&
            static_cast<signed>(t.at(v)) < t_min2.at(u))
          t_min2[u] = t.at(v);
      }
    }

    auto mark = std::vector<bool>(adjacencyList.size(), false);

    const auto reverseAdjacencyList = getTransposedAdjacencyList(adjacencyList);

    const auto reverseTopsortedVertices = ([&]() -> std::vector<T> {
      auto topsortedVertices = getAllFirstSortedBySecond(
          getVectorOfRange<T>(0, adjacencyList.size()), t);
      std::reverse(topsortedVertices.begin(), topsortedVertices.end());
      return topsortedVertices;
    })();

    // for each vertex according to the reverse topsort
    for (const auto v : reverseTopsortedVertices) {
      // skip nodes without successors
      if (adjacencyList.at(v).empty()) continue;
      // merge nodes with one successor
      if (adjacencyList.at(v).size() == 1) {
        const auto u = adjacencyList.at(v).at(0);
        unifyDisjointSetsOfElements(disjointSets, v, u);
        continue;
      }
      mark[v] = true;
      for (const auto w : adjacencyList.at(v)) mark[w] = true;
      for (const auto u : reverseAdjacencyList.at(v)) {
        if (static_cast<signed>(t.at(v)) == t_min1.at(u) &&
            adjacencyList.at(u).size() <= adjacencyList.at(v).size() + 1 &&
            t_min2.at(u) >= t_min1.at(v) && t_max.at(u) <= t_max.at(v)) {
          auto marked = true;
          for (const auto w : adjacencyList.at(u)) {
            if (!mark[w]) {
              marked = false;
              break;
            }
          }
          if (marked) unifyDisjointSetsOfElements(disjointSets, u, v);
        }
      }
      mark[v] = false;
      for (const auto w : adjacencyList.at(v)) mark[w] = false;
    }
  }

 public:
  template <typename T>
  static void thirdModularDecompositionAlgorithm(
      const vertex_labels &vertexLabels, const adjacency_list<T> &adjacencyList,
      disjoint_sets<T> &disjointSets) {
    // a mapping from a vertex to its new disjoint set representative
    auto fromVertexToRep = getVectorOfRange<T>(0, adjacencyList.size());

    {
      // a mapping from a vertex to its old set
      auto fromVertexToCurrentSet = std::vector<T>(adjacencyList.size(), 0);

      // a mapping from a vertex to its new set
      auto fromVertexToNewSet = std::vector<T>(adjacencyList.size() + 1,
                                               std::numeric_limits<T>::max());

      {
        // set counter for the head
        size_t headSetCounter = 1;
        // set counter for the tail
        size_t tailSetCounter = 0;

        // a list of usable candidate set ids, initially there are none
        auto candidateSets = getVectorOfRange<T>(0, adjacencyList.size() + 1);

        // a mapping from a set to the number of elements in it
        auto fromSetToCount = std::vector<T>(adjacencyList.size() + 1, 0);
        // initally, set 0 contains all vertices
        fromSetToCount[0] = adjacencyList.size();

        // a function to be run at each iteration
        const auto iteration = [&](const adjacency_list<T> &adjacencyList,
                                   const size_t v_i) {
          // for ever neighbour of the current vertex
          for (const auto v_j : adjacencyList.at(v_i)) {
            // obtain the old set of that neighbour
            const auto currentSet = fromVertexToCurrentSet.at(v_j);
            // if the old set of that neighbour is not yet assigned
            if (fromVertexToNewSet.at(currentSet) ==
                std::numeric_limits<T>::max()) {
              assert(headSetCounter <= adjacencyList.size());
              // set the new set of the old set to the next candidate set
              fromVertexToNewSet[currentSet] = candidateSets.at(headSetCounter);
              // increment the candidate set index
              ++headSetCounter;
              // set the representative of the vertex at the tail to the old set
              fromVertexToRep[tailSetCounter] = currentSet;
              // increment the tail set index
              ++tailSetCounter;
            }
            // set the old set of the neighbour to the new set of the old set
            fromVertexToCurrentSet[v_j] = fromVertexToNewSet.at(currentSet);
            // increment the set count at the new set of the old set
            fromSetToCount[fromVertexToNewSet.at(currentSet)]++;
            // decrement the set count for the old set
            --fromSetToCount[currentSet];
            // if there are none left in the old set
            if (fromSetToCount.at(currentSet) == 0) {
              // decrement the index of the next candidate set
              --headSetCounter;
              // add the old set back to the candidate sets
              candidateSets[headSetCounter] = currentSet;
            }
          }
          while (tailSetCounter > 0) {
            --tailSetCounter;
            fromVertexToNewSet[fromVertexToRep.at(tailSetCounter)] =
                std::numeric_limits<T>::max();
          }
        };
        for (const auto v_i : getAllFirstSortedBySecond(
                 getVectorOfRange<T>(0, adjacencyList.size()),
                 getTotalTopologicalOrder(adjacencyList))) {
          iteration(adjacencyList, v_i);
        }
      }

      for (size_t v_i = 0; v_i < adjacencyList.size(); ++v_i) {
        const auto currentSet = fromVertexToCurrentSet.at(v_i);
        if (fromVertexToNewSet.at(currentSet) ==
            std::numeric_limits<T>::max()) {
          fromVertexToNewSet[currentSet] = v_i;
          fromVertexToRep[v_i] = v_i;
        } else {
          fromVertexToRep[v_i] = fromVertexToNewSet[currentSet];
        }
      }
    }

    for (T i = 0; i < fromVertexToRep.size(); ++i) {
      const auto representative = fromVertexToRep.at(i);
      if (representative == std::numeric_limits<T>::max()) continue;
      if (!(isSink(vertexLabels.at(i)) ||
            isSink(vertexLabels.at(representative)))) {
        // no sinks can be merged
        unifyDisjointSetsOfElements(disjointSets, i, representative);
      }
    }
  }

 public:
  template <typename T>
  static void linearErAlgorithm(const vertex_labels &vertexLabels,
                                const adjacency_list<T> &adjacencyList,
                                disjoint_sets<T> &disjointSets) {
    auto convertexEqualVertex = getVectorOfRange<T>(0, adjacencyList.size());

    {
      auto vertexset = std::vector<T>(adjacencyList.size(), 0);
      auto newset = std::vector<T>(adjacencyList.size() + 1,
                                   std::numeric_limits<T>::max());

      {
        size_t setnum_head = 1;
        size_t roll_tail = 0;

        auto candidate_setIDs =
            getVectorOfRange<T>(0, adjacencyList.size() + 1);

        auto setnum = std::vector<T>(adjacencyList.size() + 1, 0);
        setnum[0] = adjacencyList.size();

        const auto iteration = [&](const adjacency_list<T> &adjacencyList,
                                   const size_t v_i) {
          for (const auto v_j : adjacencyList.at(v_i)) {
            const auto old_id = vertexset.at(v_j);
            if (newset.at(old_id) == std::numeric_limits<T>::max()) {
              assert(setnum_head <= adjacencyList.size());
              newset[old_id] = candidate_setIDs.at(setnum_head++);
              convertexEqualVertex[roll_tail++] = old_id;
            }
            vertexset[v_j] = newset.at(old_id);
            setnum[newset.at(old_id)]++;
            if (--setnum[old_id] == 0) {
              candidate_setIDs[--setnum_head] = old_id;
            }
          }
          while (roll_tail > 0) {
            newset[convertexEqualVertex.at(--roll_tail)] =
                std::numeric_limits<T>::max();
          }
        };
        adjacency_list<T> reverseAdjacencyList;
        reverseAdjacencyList = getTransposedAdjacencyList(adjacencyList);
        for (size_t v_i = 0; v_i < adjacencyList.size(); ++v_i) {
          iteration(adjacencyList, v_i);
          iteration(reverseAdjacencyList, v_i);
        }
      }

      for (size_t v_i = 0; v_i < adjacencyList.size(); ++v_i) {
        const auto setnum = vertexset.at(v_i);
        if (newset.at(setnum) == std::numeric_limits<T>::max()) {
          newset[setnum] = v_i;
          convertexEqualVertex[v_i] = v_i;
        } else {
          convertexEqualVertex[v_i] = newset[setnum];
        }
      }
    }

    for (T i = 0; i < convertexEqualVertex.size(); ++i) {
      const auto representative = convertexEqualVertex.at(i);
      if (representative == std::numeric_limits<T>::max()) continue;
      if (!(isSink(vertexLabels.at(i)) ||
            isSink(vertexLabels.at(representative)))) {
        // no sinks can be merged
        unifyDisjointSetsOfElements(disjointSets, i, representative);
      }
    }
  }

 public:
  template <typename T>
  static bool isAcyclicGraph(const adjacency_list<T> &adjacencyList,
                             const disjoint_sets<T> &disjointSets) {
    auto permanentMark = std::vector<bool>(adjacencyList.size(), false);
    auto temporaryMark = std::vector<bool>(adjacencyList.size(), false);
    std::function<bool(T)> visit = [&](T n) {
      if (permanentMark.at(n)) return true;
      if (temporaryMark.at(n)) return false;
      temporaryMark[n] = true;
      for (const auto m : adjacencyList.at(n))
        if (!visit(m)) return false;
      temporaryMark[n] = false;
      permanentMark[n] = true;
      return true;
    };
    for (const auto n : getDisjointSetRepresentatives(disjointSets)) {
      if (!visit(n)) {
        return false;
      }
    }
    return true;
  }

 public:
  template <typename T>
  static bool hasUniqueNeighbourSets(const adjacency_list<T> &adjacencyList) {
    for (size_t i = 0; i < adjacencyList.size(); ++i) {
      auto neighbourSet = std::set<T>();
      for (const auto j : adjacencyList.at(i)) {
        const auto oldSize = neighbourSet.size();
        neighbourSet.insert(j);
        const auto newSize = neighbourSet.size();
        if (oldSize >= newSize) {
          return false;
        }
      }
    }
    return true;
  }

 public:
  template <typename T>
  static void assertGraphIsValid(const vertex_labels &vertexLabels,
                                 const adjacency_list<T> &adjacencyList,
                                 const disjoint_sets<T> &disjointSets) {
    const auto graph =
        Graph<T, std::string>(disjointSets, vertexLabels, adjacencyList);

    assertCountVertices(graph);
    assertCountEdges(graph);
    if (isBackwardDirected(graph)) {
      assertBackwardDirected(graph);
    }
    if (isForwardDirected(graph)) {
      assertForwardDirected(graph);
    }

    assert(vertexLabels.size() >= adjacencyList.size());
    assert(vertexLabels.size() >=
           countDisjointSetRepresentatives(disjointSets));
    assert(countDisjointSetRepresentatives(disjointSets) ==
           adjacencyList.size());
    assert(
        getDisjointSetRepresentatives(disjointSets) ==
        getVectorOfRange<T>(0, countDisjointSetRepresentatives(disjointSets)));

    assert(hasUniqueNeighbourSets(adjacencyList));
  }

 public:
  template <typename T>
  static void applyIndexationOnGraph(const std::vector<size_t> &indices,
                                     vertex_labels &vertexLabels,
                                     adjacency_list<T> &adjacencyList,
                                     disjoint_sets<T> &disjointSets) {
    // note that this applies a simple indexation
    // all indices are assumed to be assigned
    // with indices.at(oldIndex) = newIndex
    vertexLabels = getAllFirstSortedBySecond(vertexLabels, indices);

    disjointSets = getAllFirstSortedBySecond(disjointSets, indices);
    replaceAllFirstBySecondAtFirst(disjointSets, indices);

    // sort all outer lists and rename inner lists
    adjacencyList = getAllFirstSortedBySecond(adjacencyList, indices);
    for (auto &neighbours : adjacencyList) {
      replaceAllFirstBySecondAtFirst(neighbours, indices);
    }
    adjacencyList.resize(countDisjointSetRepresentatives(disjointSets));
  }

 public:
  template <typename T>
  static void applyDisjointSetsOnGraph(adjacency_list<T> &adjacencyList,
                                       const disjoint_sets<T> &disjointSets) {
    // first part, replace vertices by their representatives
    {
      T max_k = 0;
      // iterate over each each pair
      for (size_t i = 0; i < adjacencyList.size(); ++i) {
        // find the representative of the current from vertex
        const auto k_i = findDisjointSetRepresentative(disjointSets, i);
        // record the maximum representative encountered
        if (k_i > max_k) {
          max_k = k_i;
        }
        size_t new_j = 0;
        for (size_t old_j = 0; old_j < adjacencyList.at(i).size(); ++old_j) {
          const auto k_j = findDisjointSetRepresentative(
              disjointSets, adjacencyList.at(i).at(old_j));
          if (k_j != k_i) {
            // replace each to vertex with its representative
            adjacencyList[i][new_j] = k_j;
            ++new_j;
          }
        }
        adjacencyList[i].resize(new_j);
        // move the to vertices of the current from vertex to the to vertices of
        // the current from vertex's representative
        if (k_i != i) {
          adjacencyList[k_i].insert(adjacencyList[k_i].end(),
                                    adjacencyList.at(i).begin(),
                                    adjacencyList.at(i).end());
          adjacencyList[i].clear();
        }
      }
      // resize the adjacency list to one more than the maximum representative
      // encountered
      adjacencyList.resize(max_k + 1);
    }
    // second part, remove duplicate representatives
    {
      auto found =
          std::vector<T>(adjacencyList.size(), std::numeric_limits<T>::max());
      // iterate over each edge pair
      for (size_t i = 0; i < adjacencyList.size(); ++i) {
        size_t j_new = 0;
        for (size_t j_old = 0; j_old < adjacencyList.at(i).size(); ++j_old) {
          // get the to vertex at the old index
          const auto k = adjacencyList.at(i).at(j_old);
          // if that to vertex has not yet been found for the current from
          // vertex
          if (found.at(k) != i) {
            // mark the to vertex as last found by the current from vertex
            found[k] = i;
            // set the to vertex at the new index to that of the old index
            adjacencyList[i][j_new] = k;
            // increment the new index
            ++j_new;
          }
        }
        // resize the current from vertex list to one more than the maximum
        // assigned from vertex index
        adjacencyList[i].resize(j_new);
      }
    }
  }

 public:
  template <typename T>
  static void perfectVertexKernelAlgorithm(const vertex_labels &vertexLabels,
                                           adjacency_list<T> &adjacencyList,
                                           disjoint_sets<T> &disjointSets) {
    auto fromNormalRepToSinkReps =
        getTransposedAdjacencyList(([&]() -> adjacency_list<T> {
          const auto reverseAdjacencyList =
              getTransposedAdjacencyList(adjacencyList);
          // make a mapping from sink representatives to sets of reachable
          // normal representatives
          auto fromSinkRepToNormalReps =
              adjacency_list<T>(reverseAdjacencyList.size());
          // for each representative of a disjoint set
          for (const auto representative :
               getDisjointSetRepresentatives(disjointSets)) {
            // if that representative is a sink
            if (isSink(vertexLabels.at(representative))) {
              // perform a depth first search from that representative
              fromSinkRepToNormalReps[representative] =
                  depthFirstSearch(reverseAdjacencyList, representative);
            }
          }
          return fromSinkRepToNormalReps;
        })());

    bool allEmpty = true;
    for (size_t i = 0; i < fromNormalRepToSinkReps.size(); ++i) {
      if (!fromNormalRepToSinkReps.at(i).empty()) allEmpty = false;
      if (fromNormalRepToSinkReps.at(i).size() == 1) {
        // merge nodes reaching one sink with that sink
        unifyDisjointSetsOfElements(disjointSets, i,
                                    fromNormalRepToSinkReps.at(i).at(0));
      }
    }
    if (allEmpty) return;
    // sort each neighbour list
    for (auto &neighbours : fromNormalRepToSinkReps) {
      std::sort(neighbours.begin(), neighbours.end(), std::less<T>());
    }
    // sort the neighbour sets by size then order
    auto indices = getVectorOfRange<size_t>(0, fromNormalRepToSinkReps.size());
    std::sort(indices.begin(), indices.end(),
              [&](const size_t firstIndex, const size_t secondIndex) -> bool {
                const auto firstSize =
                    fromNormalRepToSinkReps.at(firstIndex).size();
                const auto secondSize =
                    fromNormalRepToSinkReps.at(secondIndex).size();
                if (firstSize != secondSize) {
                  return firstSize < secondSize;
                } else {
                  for (size_t j = 0; j < firstSize; ++j) {
                    const auto firstValue =
                        fromNormalRepToSinkReps.at(firstIndex).at(j);
                    const auto secondValue =
                        fromNormalRepToSinkReps.at(secondIndex).at(j);
                    if (firstValue != secondValue) {
                      return firstValue < secondValue;
                    }
                  }
                  return false;
                }
              });

    // merge those sinks with equal reaching normals
    size_t i = 0;
    while (fromNormalRepToSinkReps.at(indices.at(i)).empty()) ++i;
    assert(i != fromNormalRepToSinkReps.size());
    auto prev = indices.at(i);
    ++i;
    for (; i < indices.size(); ++i) {
      auto next = indices.at(i);
      auto prevSize = fromNormalRepToSinkReps.at(prev).size();
      auto nextSize = fromNormalRepToSinkReps.at(next).size();
      if (prevSize == nextSize) {
        size_t j = 0;
        for (; j < prevSize; ++j) {
          const auto firstValue = fromNormalRepToSinkReps.at(prev).at(j);
          const auto secondValue = fromNormalRepToSinkReps.at(next).at(j);
          if (firstValue != secondValue) {
            break;
          }
        }
        if (j == prevSize) {
          unifyDisjointSetsOfElements(disjointSets, prev, next);
        }
      }
      prev = next;
    }
  }

 public:
  template <typename T>
  static void mirrorWithSinkCopiesAlgorithm(vertex_labels &vertexLabels,
                                            adjacency_list<T> &adjacencyList,
                                            disjoint_sets<T> &disjointSets) {
    // note that here we assume that a root vertex exists with index 0 but has
    // no in or outbound edges

    // copy all disjoint sets except root and append with linear transform on
    // index
    auto dsvc = disjointSets;
    const auto n = dsvc.size();
    for (size_t i = 1; i < n; ++i) {
      dsvc.push_back(dsvc.at(i) + n - 1);
    }
    // copy all vertex labels except root and append
    auto vlvc = vertexLabels;
    assert(n == vlvc.size());
    for (size_t i = 1; i < n; ++i) {
      std::stringstream ss;
      ss << vlvc.at(i) << "_" << vlvc.at(i);
      vlvc.push_back(ss.str());
    }
    // resize adjacency list to side of vertex labels and disjoint sets
    auto alvc = adjacencyList;
    alvc.resize(n);
    {
      // transpose adjacency list
      auto alvcr = getTransposedAdjacencyList(alvc);
      // linearly transform indices in adjacency list
      for (size_t i = 0; i < alvcr.size(); ++i) {
        for (size_t j = 0; j < alvcr.at(i).size(); ++j) {
          alvcr[i][j] = (alvcr.at(i).at(j) + n) - 1;
        }
      }
      // insert translated adjacency list without root at end of original
      alvc.insert(alvc.end(), alvcr.begin() + 1, alvcr.end());
    }
    // make an indexation
    std::vector<size_t> indices((n + n) - 1);
    // map the index of the root to itself
    indices[0] = 0;
    size_t i = 1;
    // i is now the index of the first old sink representative
    while (i < indices.size() && isSink(vlvc.at(i)) && dsvc.at(i) == i) {
      // map the index of each old sink to itself
      indices[i] = i;
      ++i;
    }
    size_t k = i;
    // k is now the index of the first old normal representative
    size_t j = n;
    // j is now the index of the first new sink representative
    while (i < indices.size() && isSink(vlvc.at(j)) && dsvc.at(j) == j) {
      // map the index of each new sink to a location after the old sinks
      indices[j] = i;
      ++i;
      ++j;
    }
    // j is now the index of the first new normal representative
    while (i < indices.size() && isNormal(vlvc.at(k)) && dsvc.at(k) == k) {
      // map the index of each old normal to a location after the new sinks
      indices[k] = i;
      ++i;
      ++k;
    }
    // k is now the index of the first old non-representative
    while (i < indices.size() && isNormal(vlvc.at(j)) && dsvc.at(j) == j) {
      // map the index of each new normal to a location after the old normals
      indices[j] = i;
      ++i;
      ++j;
    }
    // j is now the index of the first new non-representative
    while (k < n) {
      assert(dsvc.at(k) != k);
      // map the index of each old non-representative to a location after the
      // new normals
      indices[k] = i;
      ++i;
      ++k;
    }
    // k is now n
    while (j < ((n + n) - 1)) {
      assert(dsvc.at(j) != j);
      indices[j] = i;
      ++i;
      ++j;
    }
    // j is now n + n - 1
    // assign values accordingly
    vertexLabels = vlvc;
    disjointSets = dsvc;
    adjacencyList = alvc;

    // apply indexation and resize adjacency list
    applyIndexationOnGraph(indices, vertexLabels, adjacencyList, disjointSets);
  }

 private:
  template <typename T>
  static void swapSinksAndNewNormals(vertex_labels &vertexLabels,
                                     adjacency_list<T> &adjacencyList,
                                     disjoint_sets<T> &disjointSets) {
    for (auto &v : vertexLabels) {
      if (hasUnderscore(v)) {
        // we don't convert sinks as if a new sink is a representative it is not
        // re-mapped
        /*if (isSink(v)) {
        // convert new sinks to new normals
        v = "v_" + v;
        } else */
        if (isNormal(v)) {
          // convert new normals to new sinks
          v = "h_" + v;
        }
      } else {
        if (isSink(v)) {
          // convert old sinks to old normals
          v = "v_" + v;
        }
      }
    }
    (void)adjacencyList;
    (void)disjointSets;
    /*
       applyIndexationOnGraph(
       getOrderByFirstLetter(vertexLabels, std::vector<char>{'r', 'h', 'v'}),
       vertexLabels, adjacencyList, disjointSets);

       applyIndexationOnGraph(getOrderWithRepresentativesFirst(disjointSets),
       vertexLabels, adjacencyList, disjointSets);
       */
  }

 public:
  template <typename T>
  static void mirrorWithNoSinkEdgesAlgorithm(vertex_labels &vertexLabels,
                                             adjacency_list<T> &adjacencyList,
                                             disjoint_sets<T> &disjointSets) {
    // get the index of the last sink
    size_t i = 1;
    // now i is the index of the first old sink
    while (isSink(vertexLabels.at(i))) ++i;
    // now i is one more than the index of the last old sink
    // apply mirror
    mirrorWithSinkCopiesAlgorithm(vertexLabels, adjacencyList, disjointSets);
    // unify old and new sinks
    // now j is the index of the first old sink
    size_t j = 1;
    while (isSink(vertexLabels.at(i))) {
      // unify old sinks and new sinks
      unifyDisjointSetsOfElements(disjointSets, j, i);
      ++i;
      ++j;
    }
    // now j is the index of the first new sink
    // now i is one more than the index of the last new sink
    // apply disjoint sets
    applyDisjointSetsOnGraph(adjacencyList, disjointSets);
    // apply indexation
    applyIndexationOnGraph(getOrderWithRepresentativesFirst(disjointSets),
                           vertexLabels, adjacencyList, disjointSets);
    // transform sinks to normals and new normals to sinks
    swapSinksAndNewNormals(vertexLabels, adjacencyList, disjointSets);
  }

 public:
  template <typename T>
  static void mirrorWithSinkEdgesAlgorithm(vertex_labels &vertexLabels,
                                           adjacency_list<T> &adjacencyList,
                                           disjoint_sets<T> &disjointSets) {
    // get the index of the last sink
    size_t i = 1;
    // now i is the index of the first old sink
    while (isSink(vertexLabels.at(i))) ++i;
    // now i is one more than the index of the last old sink
    // apply mirror
    mirrorWithSinkCopiesAlgorithm(vertexLabels, adjacencyList, disjointSets);
    // unify old and new sinks
    // now j is the index of the first old sink
    size_t j = 1;
    while (isSink(vertexLabels.at(i))) {
      // add edge from old sink to new sink
      adjacencyList[j].push_back(i);
      ++i;
      ++j;
    }
    // now j is the index of the first new sink
    // now i is one more than the index of the last new sink
    // transform sinks to normals and new normals to sinks
    swapSinksAndNewNormals(vertexLabels, adjacencyList, disjointSets);
  }

 public:
  template <typename T>
  static void transformNoOutNeighbourRepresentativesIntoSinks(
      vertex_labels &vertexLabels, const adjacency_list<T> &adjacencyList,
      const disjoint_sets<T> &disjointSets) {
    for (const auto representative :
         getDisjointSetRepresentatives(disjointSets)) {
      if (adjacencyList.at(representative).empty()) {
        vertexLabels[representative] = "h_" + vertexLabels.at(representative);
      }
    }
  }

 public:
  template <typename T>
  static std::vector<T> getRandomSample(const std::vector<T> &population,
                                        const std::size_t sampleSize) {
    std::vector<T> randomSample;
    randomSample.reserve(sampleSize);
    std::sample(population.begin(), population.end(),
                std::back_inserter(randomSample), sampleSize,
                std::mt19937{std::random_device{}()});
    return randomSample;
  }
};
}  // namespace detail
}  // namespace hparg
