// Must be included first
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <lc_libs/simple_log.hpp>
#include <vector>

namespace log = lc_libs::log;

TEST(FindMedianfromDataStream, BasicOperations) {
  MedianFinder mf;

  mf.addNum(1);
  EXPECT_DOUBLE_EQ(mf.findMedian(), 1.0);

  mf.addNum(2);
  EXPECT_DOUBLE_EQ(mf.findMedian(), 1.5);

  mf.addNum(3);
  EXPECT_DOUBLE_EQ(mf.findMedian(), 2.0);

  mf.addNum(4);
  EXPECT_DOUBLE_EQ(mf.findMedian(), 2.5);

  mf.addNum(5);
  EXPECT_DOUBLE_EQ(mf.findMedian(), 3.0);
}
