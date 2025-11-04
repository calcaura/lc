#pragma once

#include <cstdint>
#include <deque>
#include <format>
#include <iostream>
#include <lc_libs/bool_matrix.hpp>
#include <lc_libs/simple_log.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace log = lc_libs::log;

static inline bool diff_max1(const auto& w1, const auto& w2) {
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

// This solution is building the graph as an adjacency matrix
// It is twice as fast as solution1 and uses less memory.
class Solution {
 public:
  lc_libs::BoolMatrix _m;
  size_t _wordSize;

  inline void buildGraph(const std::vector<std::string>& wordList) {
    size_t idx{0};
    const std::string* begin = wordList.data();
    const std::string* end = begin + wordList.size();

    for (auto wordIt = begin; wordIt != end; ++wordIt) {
      for (auto nit = begin; nit < end; ++nit) {
        if (nit != wordIt && diff_max1(*nit, *wordIt)) {
          _m.set(idx);
        }
        ++idx;
      }
    }
    log::debug("Graph:\n{}", lc_libs::BoolMatrixView{&wordList, &_m});
  }

  int shortest_path_len(const size_t beginCol, const size_t solIdx) {
    const auto width = _m.width();

    log::debug(
        "Starting dfs from column {} and exepcting solution on column {}",
        beginCol, solIdx);

    std::deque<std::pair<uint16_t, uint16_t>> q;
    std::vector<bool> visited_rows(_m.height(), false);

    q.push_back({beginCol, 1});  // first word

    visited_rows[beginCol] = true;

    while (!q.empty()) {
      const auto [col, cost] = q.front();
      q.pop_front();

      if (_m.at(col, solIdx)) {
        log::debug(" !!! Solution on row {}", col);
        return cost + 1;
      }

      log::debug("Col:{} cost:{}", col, cost);

      for (size_t row = 0; row < width; ++row) {
        if (visited_rows[row]) {
          log::debug("Skpping row: {}", row);
          continue;
        }
        if (_m.at(row, col)) {
          log::debug(" Selecting row {}", row);
          q.push_back({row, cost + 1});
          visited_rows[row] = true;
        }
      }
      log::debug("______________________");
    }
    return 0;
  }

  int ladderLength(const std::string& beginWord, const std::string& endWord,
                   std::vector<std::string>& wordList) {
    int first_word_idx{-1}, last_word_idx{-1};

    for (size_t idx = 0; idx < wordList.size(); ++idx) {
      if (wordList[idx] == endWord) {
        last_word_idx = idx;
        log::debug("End word found at index {}", idx);
      }
      if (wordList[idx] == beginWord) {
        first_word_idx = idx;
        log::debug("Start word found at index {}", idx);
      }
    }
    if (last_word_idx == -1) {
      return 0;
    }
    if (first_word_idx == -1) {
      first_word_idx = wordList.size();
      wordList.push_back(beginWord);
    }
    _m = lc_libs::BoolMatrix{wordList.size(), wordList.size()};
    _wordSize = endWord.length();
    buildGraph(wordList);
    return shortest_path_len(last_word_idx, first_word_idx);
  }

  static int ladderLength(const char* beginWord, const char* endWord,
                          std::initializer_list<const char*> wordList) {
    std::vector<std::string> words;
    words.reserve(wordList.size());
    for (const auto& w : wordList) {
      words.emplace_back(w);
    }
    Solution sol;
    return sol.ladderLength(beginWord, endWord, words);
  }
};
