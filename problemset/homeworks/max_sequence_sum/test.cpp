
#include "max_sequence.hpp"

// Test
#include <gtest/gtest.h>

#include <numeric>

using namespace max_sequence;

static const std::array<uint32_t, 15> kSampleInput{2, 4, 3, 2, 1, 5, 5, 100,
                                                   1, 2, 3, 4, 5, 4, 6};

TEST(MaxSequenceTest, Invalid) {
  ASSERT_FALSE(max_configuous_sequence({}, 0).valid());
  ASSERT_FALSE(max_configuous_sequence({}, 100).valid());
}

TEST(MaxSequenceTest, Example) {
  const std::array<uint32_t, 5> input{2, 4, 3, 2, 1};
  ASSERT_TRUE(max_configuous_sequence(input, 10).valid());
  ASSERT_EQ(max_configuous_sequence(input, 10), (Result{1, 4, 10}));
}

TEST(MaxSequenceTest, MultipleMatches) {
  /* kSampleInput:
      2,

      // match 1 begin
      4, 3, 2, 1,  // match 1 end

      // match 2 begin
      5, 5,  // match 2 end

      // no match
      100,

      // match 3 begin
      1, 2, 3, 4,  // match 3 end

      // no match
      5,

      // match 4 begin
      4, 6  // match 4 end
  */
  ASSERT_TRUE(max_configuous_sequence(kSampleInput, 10).valid());
  ASSERT_EQ(max_configuous_sequence(kSampleInput, 10), (Result{1, 4, 10}));
}

TEST(MaxSequenceTest, NoMatches) {
  const auto more_than_sum =
      std::accumulate(kSampleInput.begin(), kSampleInput.end(), 0u) + 1;

  const auto result = max_configuous_sequence(kSampleInput, more_than_sum);

  ASSERT_FALSE(result.valid())
      << "Looking for sum=" << more_than_sum << ", got result: " << result;
}

TEST(MaxSequenceTest, CornerCaseMiddleElement) {
  ASSERT_EQ(max_configuous_sequence(kSampleInput, 5), (Result{2, 3, 5}));
}

TEST(MaxSequenceTest, CornerCaseFirstElement) {
  ASSERT_TRUE(max_configuous_sequence(kSampleInput, kSampleInput[0]).valid());
}

TEST(MaxSequenceTest, CornerCaseLastElement) {
  std::array<uint32_t, 5> input{5, 4, 3, 2, 1};
  ASSERT_TRUE(max_configuous_sequence(input, input.back()).valid());
}
