#pragma once
#include <sys/types.h>

#include <vector>

class Solution {
 public:
  static int minPathSum(std::vector<std::vector<int>>& grid) {
    const size_t right_col = grid[0].size() - 1;
    const size_t bottom_row = grid.size() - 1;

    // populate the edges
    for (ssize_t row = bottom_row - 1; row >= 0; --row) {
      grid[row][right_col] += grid[row + 1][right_col];
    }
    for (ssize_t col = right_col - 1; col >= 0; --col) {
      grid[bottom_row][col] += grid[bottom_row][col + 1];
    }

    // Now go back to base
    for (ssize_t row = bottom_row - 1; row >= 0; --row) {
      for (ssize_t col = right_col - 1; col >= 0; --col) {
        grid[row][col] += std::min(grid[row][col + 1], grid[row + 1][col]);
      }
    }
    return grid[0][0];
  }
};