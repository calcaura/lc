#pragma once

#include <lc_libs/simple_log.hpp>
#include <lc_libs/trie.hpp>
#include <string>
#include <unordered_set>
#include <vector>

namespace lc_libs {

class Solution {
  lc_libs::Trie<uint32_t> _trie;
  size_t _width, _height;
  size_t _right, _bottom;
  std::vector<char> _data;
  std::string _str;
  std::unordered_set<std::string> _result;
  size_t _expected_words_count{};

  void search_board(const std::vector<std::vector<char>>& board) {
    _width = board[0].size();
    _height = board.size();

    _right = _width - 1;
    _bottom = _height - 1;

    _data.resize(_width * _height + 1);

    auto to = _data.begin();
    for (auto& row : board) {
      to = std::copy(row.begin(), row.end(), to);
    }
    *to = 0;

    for (size_t y = 0; y < _height; ++y) {
      for (size_t x = 0; x < _width; ++x) {
        _str.clear();
        search_board_from(y, x);
      }
    }
  }

  void search_board_from(size_t y, size_t x) {
    const size_t off = y * _width + x;
    const char c = _data[off];
    if (c == 0) {
      return;
    }

    _str.push_back(c);
    // check for early termination
    if (!_trie.startsWith(_str.c_str())) {
      _str.pop_back();
      return;
    }
    if (_trie.search(_str.c_str())) {
      _result.insert(_str);
      if (_result.size() >= _expected_words_count) {
        return;
      }
    }
    _data[off] = 0;

    if (x < _right) {
      search_board_from(y, x + 1);
    }
    if (x > 0) {
      search_board_from(y, x - 1);
    }

    if (y < _bottom) {
      search_board_from(y + 1, x);
    }
    if (y > 0) {
      search_board_from(y - 1, x);
    }

    _str.pop_back();
    _data[off] = c;
  }

 public:
  std::vector<std::string> findWords(
      const std::vector<std::vector<char>>& board,
      const std::vector<std::string>& words) {
    _expected_words_count = words.size();
    _trie.insert(words);
    search_board(board);

    std::vector<std::string> result(_result.begin(), _result.end());
    return result;
  }
};
}  // namespace lc_libs