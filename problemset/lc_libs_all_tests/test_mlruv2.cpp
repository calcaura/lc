
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <lc_libs/lruv2.hpp>
#include <lc_libs/simple_log.hpp>
#include <vector>

namespace log = lc_libs::log;
using Lru = lruv2::Lru<int, float>;
using Entry = lruv2::Entry<int, float>;

TEST(LruCacheTest, BasicOperations) {
  Lru cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};
  EXPECT_EQ(cache.capacity(), 3);
  EXPECT_EQ(cache.lru().size(), 3);
  EXPECT_EQ(cache.cache().size(), 3);
  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
}

TEST(LruCacheTest, DuplicateKeyCtor) {
  Lru cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}, {10, 43.1f}}};
  ASSERT_EQ(cache.lru().get_cycle(), nullptr);
  EXPECT_EQ(cache.capacity(), 3);
  EXPECT_EQ(cache.lru().size(), 3);
  EXPECT_EQ(cache.cache().size(), 3);
  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {10, 43.1f}, {30, 101.0f}, {20, 73.0f}}));
}

TEST(LruCacheTest, SimpleReorderAfterGet) {
  Lru cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};

  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
  auto val = cache.get(20);
  ASSERT_TRUE(val.has_value());
  EXPECT_EQ(*val, 73.0f);
  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {20, 73.0f}, {30, 101.0f}, {10, 42.0f}}));
}

TEST(LruCacheTest, SimpleEviction) {
  Lru cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};

  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
  auto was_present = cache.put(44, 101.0f);
  ASSERT_FALSE(was_present);
  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {44, 101.0f},
                             {30, 101.0f},
                             {20, 73.0f},
                         }));
}

TEST(LruCacheTest, SimpleUpdate) {
  Lru cache{3lu, {{10, 42.0f}, {20, 73.0f}, {30, 101.0f}}};

  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {30, 101.0f}, {20, 73.0f}, {10, 42.0f}}));
  auto was_present = cache.put(20, 101.0f);
  ASSERT_TRUE(was_present);
  EXPECT_EQ(cache.lru(), (std::initializer_list<Entry>{
                             {20, 101.0f},
                             {30, 101.0f},
                             {10, 42.0f},
                         }));
}
