#pragma once

#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

static inline bool diff_max1(const auto& w1, const auto& w2) {
  if (w1.length() != w2.length()) {
    return false;
  }
  size_t diffs{};
  for (size_t it = 0; it < w1.length(); ++it) {
    if (w1[it] != w2[it]) {
      ++diffs;
    }
    if (diffs > 1) {
      return false;
    }
  }
  return diffs <= 1;
}

class Solution {
 public:
  using edges_t = std::vector<std::string_view>;
  using graph_t = std::unordered_map<std::string_view, edges_t>;

  static inline graph_t buildGraph(std::vector<std::string>& wordList) {
    graph_t graph;

    for (auto& w : wordList) {
      graph[w] = {};
    }

    for (auto& w : wordList) {
      for (auto& [k, e] : graph) {
        if (diff_max1(w, k)) {
          e.push_back(w);
        }
      }
    }

    return graph;
  }

  static int shortest_path_len(const graph_t& graph,
                               const std::string& beginWord,
                               const std::string& endWord) {
    auto final_it = graph.find(endWord);
    if (final_it == graph.end()) {
      return false;
    }

    std::deque<std::pair<std::string_view, int>> q;
    for (const auto& [k, _] : graph) {
      if (diff_max1(beginWord, k)) {
        q.push_back({k, 1});
      }
    }
    std::unordered_set<std::string_view> visited;

    while (!q.empty()) {
      auto [word, cost] = q.front();
      ++cost;
      if (word == endWord) {
        return cost;
      }

      q.pop_front();
      auto isit = visited.insert(word);
      if (!isit.second) {
        continue;
      }
      auto wit = graph.find(word);
      if (wit == graph.end()) {
        continue;
      }
      for (const auto& w : wit->second) {
        if (w == endWord) {
          return cost + 1;
        }
        q.push_back({w, cost});
      }
    }
    return 0;
  }

  static int ladderLength(const std::string& beginWord,
                          const std::string& endWord,
                          std::vector<std::string>& wordList) {
    graph_t graph{buildGraph(wordList)};
    return shortest_path_len(graph, beginWord, endWord);
  }

  static int ladderLength(const char* beginWord, const char* endWord,
                          std::initializer_list<const char*> wordList) {
    std::vector<std::string> words;
    words.reserve(wordList.size());
    for (const auto& w : wordList) {
      words.emplace_back(w);
    }
    return ladderLength(beginWord, endWord, words);
  }
};