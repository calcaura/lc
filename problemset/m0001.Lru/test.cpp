
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <lc_libs/lru.hpp>
#include <vector>

TEST(LruCacheTest, BasicOperations) {
  lru::Lru<int, float> cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};
  EXPECT_EQ(cache.capacity(), 3);
  EXPECT_EQ(cache.lru().size(), 3);
  EXPECT_EQ(cache.cache().size(), 3);
  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
}

TEST(LruCacheTest, DuplicateKeyCtor) {
  lru::Lru<int, float> cache{
      3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}, {10, 43.1f}}};
  EXPECT_EQ(cache.capacity(), 3);
  EXPECT_EQ(cache.lru().size(), 3);
  EXPECT_EQ(cache.cache().size(), 3);
  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {10, 43.1f}, {30, 101.0f}, {20, 73.0f}}));
}

TEST(LruCacheTest, SimpleReorderAfterGet) {
  lru::Lru<int, float> cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};

  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
  auto val = cache.get(20);
  ASSERT_TRUE(val.has_value());
  EXPECT_EQ(*val, 73.0f);
  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {20, 73.0f}, {30, 101.0f}, {10, 42.0f}}));
}

TEST(LruCacheTest, SimpleEviction) {
  lru::Lru<int, float> cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};

  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
  auto was_present = cache.put(44, 101.0f);
  ASSERT_FALSE(was_present);
  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {44, 101.0f},
                             {30, 101.0f},
                             {20, 73.0f},
                         }));
}

TEST(LruCacheTest, SimpleUpdate) {
  lru::Lru<int, float> cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};

  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
  auto was_present = cache.put(20, 101.0f);
  ASSERT_TRUE(was_present);
  EXPECT_EQ(cache.lru(), (std::list<std::pair<int, float>>{
                             {20, 101.0f},
                             {30, 101.0f},
                             {10, 42.0f},
                         }));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}