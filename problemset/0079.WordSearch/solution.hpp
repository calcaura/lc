#pragma once

#include <string.h>

#include <format>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

class Searcher {
 public:
  std::string board_;
  size_t right_, bottom_, width_, height_;

  Searcher(const std::vector<std::vector<char>>& board) {
    width_ = board[0].size();
    height_ = board.size();

    right_ = width_ - 1;
    bottom_ = height_ - 1;
    board_.resize(width_ * height_);

    auto end = board_.data() + board_.size();
    auto row = 0;
    for (auto dst = board_.data(); dst < end; dst += width_) {
      memcpy(dst, board[row].data(), width_);
      ++row;
    }
  }

  bool exists(std::string_view word) {
    for (size_t y = 0; y <= bottom_; ++y) {
      for (size_t x = 0; x <= right_; ++x) {
        if (exists(y, x, word)) {
          return true;
        }
      }
    }
    return false;
  }

  bool exists(size_t y, size_t x, std::string_view word) {
    struct Guard {
      Guard(char* c) : cell_(c), value_(*c) { *cell_ = -1; }
      ~Guard() { *cell_ = value_; }
      char value() const { return value_; }

     private:
      char* cell_;
      char value_;
    };

    Guard g(board_.data() + y * width_ + x);
    if (g.value() != word[0]) {
      return false;
    }
    if (word.size() == 1) {
      return true;
    }
    auto suffix = word.substr(1);

    if (y > 0) {
      if (exists(y - 1, x, suffix)) {
        return true;
      }
    }

    if (y < bottom_) {
      if (exists(y + 1, x, suffix)) {
        return true;
      }
    }

    if (x > 0) {
      if (exists(y, x - 1, suffix)) {
        return true;
      }
    }

    if (x < right_) {
      if (exists(y, x + 1, suffix)) {
        return true;
      }
    }

    return false;
  }
};

class Solution {
 public:
  bool exist(std::vector<std::vector<char>>& board, std::string_view word) {
    Searcher sol(board);
    return sol.exists(word);
  }
};

std::ostream& operator<<(std::ostream& os, const Searcher& searcher) {
  for (size_t row = 0; row < searcher.height_; ++row) {
    os << std::string_view(searcher.board_.data() + row * searcher.width_,
                           searcher.width_);
    os << "\n";
  }
  return os;
}

namespace std {
template <>
struct formatter<Searcher> {
  template <typename ParseContext>
  constexpr auto parse(ParseContext& ctx) {
    return ctx.begin();
  }

  template <typename FormatContext>
  auto format(const Searcher& searcher, FormatContext& ctx) const {
    auto it = ctx.out();
    for (size_t row = 0; row < searcher.height_; ++row) {
      it = std::format_to(
          it, "{}\n",
          std::string_view(searcher.board_.data() + row * searcher.width_,
                           searcher.width_));
    }
    return it;
  }
};
}  // namespace std