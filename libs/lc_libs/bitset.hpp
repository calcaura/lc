#pragma once

#include <array>
#include <bitset>
#include <initializer_list>

namespace lc_libs::bitset_ops {

template <typename ElemType, size_t Elements>
std::bitset<sizeof(ElemType) * 8 * Elements> from_number(
    std::array<ElemType, Elements> vals) {
  constexpr size_t bitset_size = sizeof(ElemType) * 8 * Elements;
  using Result = std::bitset<bitset_size>;
  Result result;

  size_t bit_index = 0;
  for (const auto &val : vals) {
    for (size_t i = 0; i < sizeof(ElemType) * 8; ++i) {
      if (val & (ElemType(1) << i)) {
        result.set(bit_index);
      }
      ++bit_index;
    }
  }
  return result;
}

template <size_t N>
std::string to_hex_string(const std::bitset<N> &b) {
  std::string result;
  result.reserve((N + 3) / 4 + 2);
  result += "0x";
  bool leading = true;
  for (int i = (N - 1) / 4; i >= 0; --i) {
    uint8_t val = 0;
    for (size_t j = 0; j < 4; ++j) {
      if (i * 4 + j < N && b[i * 4 + j]) {
        val |= (1 << j);
      }
    }
    if (leading && val == 0) {
      continue;
    }
    leading = false;
    if (val < 10) {
      result += ('0' + val);
    } else {
      result += ('a' + (val - 10));
    }
  }
  if (leading) {
    result += '0';
  }
  return result;
}

template <size_t N>
std::string to_string(const std::bitset<N> &b) {
  if (b.none()) {
    return "0";
  }

  std::string result;
  result.reserve((N + 2) / 3 + 1);  // rough estimate

  std::bitset<N> value = b;
  while (value.any()) {
    // Divide by 10
    std::bitset<N> quotient;
    uint32_t remainder = 0;  // can be up to 9

    for (int i = N - 1; i >= 0; --i) {
      remainder = (remainder << 1) | (value[i] ? 1 : 0);
      if (remainder >= 10) {
        quotient.set(i);
        remainder -= 10;
      }
    }
    result += ('0' + remainder);
    value = quotient;
  }
  std::reverse(result.begin(), result.end());
  return result;
}

template <size_t N>
std::bitset<N * 2> multiply(const std::bitset<N> &a, const std::bitset<N> &b) {
  std::bitset<N * 2> result;

  for (size_t i = 0; i < N; ++i) {
    if (b[i]) {
      // Shift left and add
      std::bitset<N * 2> temp;
      for (size_t j = 0; j < N; ++j) {
        if (a[j]) temp.set(i + j);
      }
      result ^= temp;  // use XOR for binary add without carry
    }
  }
  return result;
}

template <typename ArrayElemType, size_t BitsetSize>
constexpr size_t array_size() {
  const size_t bytes = BitsetSize / 8;
  const size_t extra = bytes % sizeof(ArrayElemType) == 0 ? 0 : 1;
  return bytes / sizeof(ArrayElemType) + extra;
}

template <typename ArrayElemType, size_t BitsetSize>
std::array<ArrayElemType, array_size<ArrayElemType, BitsetSize>()> to_array(
    const std::bitset<BitsetSize> &a) {
  std::array<ArrayElemType, array_size<ArrayElemType, BitsetSize>()> result{};
  for (size_t i = 0; i < result.size(); ++i) {
    ArrayElemType val = 0;
    for (size_t j = 0; j < sizeof(ArrayElemType) * 8; ++j) {
      if (a[i * sizeof(ArrayElemType) * 8 + j]) {
        val |= (ArrayElemType(1) << j);
      }
    }
    result[i] = val;
  }
  return result;
}

}  // namespace lc_libs::bitset_ops