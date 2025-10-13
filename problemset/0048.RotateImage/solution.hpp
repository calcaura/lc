#pragma once

#include <algorithm>
#include <vector>

class Solution {
 public:
  using Matrix = std::vector<std::vector<int>>;
  static void rotate(Matrix& matrix) {
    transpose(matrix);
    mirror_vert(matrix);
  }

  // Transpose matrix.
  // This assumes that the matrix is square (width == height)
  static void transpose(Matrix& matrix) {
    const auto width = matrix[0].size();
    const auto height = matrix.size();
    for (size_t y = 0; y < height; ++y) {
      for (size_t x = y + 1; x < width; ++x) {
        std::swap(matrix[y][x], matrix[x][y]);
      }
    }
  }

  static void mirror_vert(Matrix& matrix) {
    for (auto& line : matrix) {
      std::reverse(line.begin(), line.end());
    }
  }
};