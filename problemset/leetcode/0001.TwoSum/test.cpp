
#include "solution.hpp"

// Test
#include <gtest/gtest.h>

TEST(TwoSumTest, BasicAssertions) {
  Solution solution;
  std::vector<int> nums = {2, 7, 11, 15};
  int target = 9;
  auto result = solution.twoSum(nums, target);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0], 0);
  EXPECT_EQ(result[1], 1);
}

TEST(TwoSumTest, NoSolution) {
  Solution solution;
  std::vector<int> nums = {1, 2, 3};
  int target = 7;
  auto result = solution.twoSum(nums, target);
  EXPECT_EQ(result.size(), 0);
}