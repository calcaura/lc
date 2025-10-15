// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <bitset>
#include <lc_libs/bitset.hpp>
#include <lc_libs/simple_log.hpp>

namespace bs = lc_libs::bitset_ops;
namespace log = lc_libs::log;

TEST(BitSet, AllOps) {
  {
    std::bitset<16> b = bs::from_number<uint8_t, 2>({0x34, 0x12});
    ASSERT_EQ(b.to_ulong(), 0x1234);
  }
  {
    std::bitset<16> b = bs::from_number<uint16_t, 1>({0x1234});
    ASSERT_EQ(b.to_ulong(), 0x1234);
  }
  {
    std::bitset<32> b = bs::from_number<uint16_t, 2>({0x5678, 0x1234});
    ASSERT_EQ(b.to_ulong(), 0x12345678);
  }
  {
    std::bitset<64> b = bs::from_number<uint32_t, 2>({0x9abcdef0, 0x12345678});
    ASSERT_EQ(b.to_ullong(), 0x123456789abcdef0ULL);
  }

  {
    std::bitset<128> b = bs::from_number<uint32_t, 4>(
        {0xc0ffee, 0xb00b5, 0xdeadbea1, 0x12345678});
    ASSERT_EQ(bs::to_hex_string(b), "0x12345678deadbea1000b00b500c0ffee");
    ASSERT_EQ(bs::to_string(b), "24197857224293429071021657789149806574");

    std::array<uint32_t, 4> arr = bs::to_array<uint32_t>(b);
    ASSERT_EQ(arr[0], 0xc0ffee);
    ASSERT_EQ(arr[1], 0x000b00b5);
    ASSERT_EQ(arr[2], 0xdeadbea1);
    ASSERT_EQ(arr[3], 0x12345678);

    std::array<uint64_t, 2> arr64 = bs::to_array<uint64_t>(b);
    ASSERT_EQ(arr64[0], 0x000b00b500c0ffee);
    ASSERT_EQ(arr64[1], 0x12345678deadbea1);
  }
}