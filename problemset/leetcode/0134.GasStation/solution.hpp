#pragma once

#include <vector>

class Solution {
 public:
  int canCompleteCircuit(std::vector<int>& gas, std::vector<int>& cost) {
    const size_t N{gas.size()};

    for (size_t start = 0; start < N;) {
      int level = gas[start];

      for (size_t next = start; next < N + start;) {
        level -= cost[next % N];
        if (level < 0) {
          if (next == start) {
            ++start;
          } else {
            start = next;
          }
          break;
        }
        ++next;
        level += gas[next % N];
      }
      if (level >= 0) {
        return start;
      }
    }
    return -1;
  }
};