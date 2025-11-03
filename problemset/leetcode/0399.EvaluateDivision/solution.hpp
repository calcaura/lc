#pragma once

#include <deque>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Graph {
 public:
  using edges_t = std::unordered_map<std::string, double>;
  using graph_t = std::unordered_map<std::string, edges_t>;
  using node_view_t = std::pair<std::string_view, const edges_t*>;

  void add(const std::vector<std::string>& edges, double cost) {
    _graph[edges[0]][edges[1]] = cost;
    if (cost) {
      _graph[edges[1]][edges[0]] = 1 / cost;
    }
  }

  double cost(const std::string& from, const std::string& to) const {
    auto from_it = _graph.find(from);
    if (from_it == _graph.end()) {
      return -1;
    }

    auto to_it = _graph.find(to);
    if (to_it == _graph.end()) {
      return -1;
    }
    if (from == to) {
      return 1;
    }
    const edges_t* stop_at = &to_it->second;

    using step_t = std::pair<const edges_t*, double>;

    std::deque<step_t> q;
    std::unordered_set<const edges_t*> visited;

    q.emplace_back(step_t(&from_it->second, 1));

    while (!q.empty()) {
      auto value = std::move(q.front());
      q.pop_front();
      auto r = visited.insert(value.first);
      if (!r.second) {
        continue;
      }
      if (value.first == stop_at) {
        return value.second;
      }
      const double crt_cost = value.second;

      for (auto& [name, cost] : *value.first) {
        auto next_it = _graph.find(name);
        if (next_it != _graph.end()) {
          q.emplace_back(step_t(&next_it->second, crt_cost * cost));
        }
      }
    }

    return -1;
  }

  void print(std::ostream& os) const {
    for (const auto& [k, edges] : _graph) {
      os << std::setw(5) << k << ": [";
      bool comma{};
      for (auto& [vertex, cost] : edges) {
        if (comma) {
          os << ", ";
        }
        comma = true;
        os << vertex << ":" << std::setprecision(2) << cost;
      }
      os << "]\n";
    }
  }

 private:
  graph_t _graph;
};

std::ostream& operator<<(std::ostream& os, const Graph& g) {
  g.print(os);
  return os;
}

class Solution {
 public:
  static std::vector<double> calcEquation(
      const std::vector<std::vector<std::string>>& equations,
      const std::vector<double>& values,
      const std::vector<std::vector<std::string>>& queries) {
    Graph graph;
    for (size_t idx = 0; idx < equations.size(); ++idx) {
      graph.add(equations[idx], values[idx]);
    }
    // std::cout << "Graph:\n" << graph << "\n";
    std::vector<double> result;
    result.reserve(queries.size());
    for (const auto& q : queries) {
      result.push_back(graph.cost(q[0], q[1]));
    }
    return result;
  }
};