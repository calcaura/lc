#pragma once

#include <cstdint>
#include <limits>
#include <ostream>
#include <span>

namespace max_sequence {

struct Result {
  size_t begin{};
  size_t end{};
  uint32_t sum{std::numeric_limits<uint32_t>::max()};

  // Check if the result is valid
  bool valid() const { return sum != std::numeric_limits<uint32_t>::max(); }

  // Used for tests
  bool operator==(const Result& other) const {
    return begin == other.begin && end == other.end && sum == other.sum;
  }
};

// Finds the maximum contiguous subsequence in `input`
// such that the sum of its elements does not exceed `max_sum`.
// Returns an invalid Result if no such subsequence exists.
// Notes:
// - The subsequence is defined by the range [begin, end] (end is inclusive).
// - If multiple subsequences have the same maximum sum, the first one is
// returned.
// - The result is zero-based indexed.
static inline Result max_configuous_sequence(
    const std::span<const uint32_t> input, const uint32_t max_sum) {
  if (input.empty() || max_sum == 0) {
    return {};
  }

  uint32_t crt_sum{};
  uint32_t best_sum{};

  size_t leftIt{}, bestLeftIt{}, bestRightIt{};

  for (size_t right = 0; right < input.size(); ++right) {
    crt_sum += input[right];

    // If the sum exceeds max_sum, we need to shrink the window
    while (crt_sum > max_sum && leftIt <= right) {
      crt_sum -= input[leftIt];
      ++leftIt;
    }

    // Check if we have better results now
    if (crt_sum <= max_sum && crt_sum > best_sum) {
      best_sum = crt_sum;
      bestLeftIt = leftIt;
      bestRightIt = right;
    }
    if (best_sum == max_sum) {
      // Can't do better than this
      break;
    }
  }

  if (bestRightIt < bestLeftIt || best_sum != max_sum) {
    // No solution
    return {};
  }

  return Result{bestLeftIt, bestRightIt, best_sum};
}

inline std::ostream& operator<<(std::ostream& os, const Result& result) {
  os << "Result{.begin=" << result.begin << ", .end=" << result.end
     << ", .sum=" << result.sum << "}";
  return os;
}
}  // namespace max_sequence
