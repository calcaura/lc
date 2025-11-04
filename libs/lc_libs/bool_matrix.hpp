#pragma once

#include <format>
#include <vector>

namespace lc_libs {
class BoolMatrix {
 public:
  using data_t = std::vector<bool>;
  using iterator = data_t::iterator;
  using const_iterator = data_t::const_iterator;

  BoolMatrix() : _data(), _width(0), _height(0) {}

  BoolMatrix(size_t height, size_t width)
      : _data(height * width, false), _width(width), _height(height) {}

  bool at(size_t row, size_t col) const { return _data[row * _width + col]; }
  bool at(size_t index) const { return _data[index]; }

  void set(size_t row, size_t col, bool value) {
    _data[row * _width + col] = value;
  }
  void set(size_t idx, bool value = true) { _data[idx] = value; }

  iterator begin() { return _data.begin(); }
  iterator end() { return _data.end(); }

  const_iterator begin() const { return _data.begin(); }
  const_iterator end() const { return _data.end(); }

  size_t width() const { return _width; }
  size_t height() const { return _height; }

 private:
  std::vector<bool> _data;
  size_t _width;
  size_t _height;
};

struct BoolMatrixView {
  using const_iterator = BoolMatrix::const_iterator;
  const std::vector<std::string>* lines;
  const BoolMatrix* matrix;

  size_t height() const { return lines->size(); }
  const_iterator begin() const { return matrix->begin(); }
  const_iterator end() const { return matrix->end(); }
};

}  // namespace lc_libs

template <>
struct std::formatter<lc_libs::BoolMatrix> : std::formatter<std::string_view> {
  auto format(const lc_libs::BoolMatrix& m, std::format_context& ctx) const {
    const size_t width = m.width();
    auto out = ctx.out();
    size_t x = 0;

    for (bool b : m) {
      out = std::format_to(out, "{} ", b ? "x" : "•");
      ++x;
      if (x == width) {
        out = std::format_to(out, "\n");
        x = 0;
      }
    }
    return out;
  }
};

// -- -MatrixView formatter-- -
template <>
struct std::formatter<lc_libs::BoolMatrixView>
    : std::formatter<std::string_view> {
  auto format(const lc_libs::BoolMatrixView& mv,
              std::format_context& ctx) const {
    const size_t height = mv.height();
    auto out = ctx.out();
    size_t x = 0, y = 0;
    auto lineIt = mv.lines->begin();

    out = std::format_to(out, "{:^2} | {:^4} | ", "-", "-");
    for (size_t it = 0; it < height; ++it) {
      out = std::format_to(out, " {}  ", it);
    }
    out = std::format_to(out, "\n");

    for (bool b : mv) {
      if (x == 0) out = std::format_to(out, "{:>2} | {:>4} | ", y, *lineIt);

      out = std::format_to(out, "{} ", b ? " x " : " • ");
      ++x;

      if (x == height) {
        out = std::format_to(out, "\n");
        x = 0;
        ++lineIt;
        ++y;
      }
    }

    return out;
  }
};