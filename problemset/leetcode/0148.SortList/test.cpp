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

using Node = lc_libs::ListNodeBase<int>;

TEST(SortList, MiddelVal) {
  auto l1 = new Node{5, 2, 7, 3, 9, 17, 13};
  Node* middle = l1->middle_node();

  EXPECT_EQ(middle->val, 3);
  // Inseart an extra element
  auto l2 = new Node(42, l1);
  EXPECT_EQ(l2->middle_node()->val, 3);

  auto l3 = new Node(25, l2);
  EXPECT_EQ(l3->middle_node()->val, 7);
  delete l3;
}

TEST(SortList, MiddleSplit) {
  {
    auto l1 = new Node{};
    auto [first_half, second_half] = l1->middle_split();
    EXPECT_EQ(*first_half, *l1);
    EXPECT_EQ(second_half, nullptr);
    delete first_half;
  }

  {
    auto l1 = new Node{1, 2};
    auto [first_half, second_half] = l1->middle_split();
    Node expected_first{1};
    Node expected_second{2};
    EXPECT_EQ(*first_half, expected_first);
    EXPECT_EQ(*second_half, expected_second);
    delete first_half;
    delete second_half;
  }

  {
    auto l1 = new Node{1, 2, 3};
    auto [first_half, second_half] = l1->middle_split();
    Node expected_first{1};
    Node expected_second{2, 3};
    EXPECT_EQ(*first_half, expected_first);
    EXPECT_EQ(*second_half, expected_second);
    delete first_half;
    delete second_half;
  }

  {
    auto l1 = new Node{5, 2, 7, 3, 9, 17, 13, 42};
    auto [first_half, second_half] = l1->middle_split();
    Node expected_first{5, 2, 7, 3};
    Node expected_second{9, 17, 13, 42};
    EXPECT_EQ(*first_half, expected_first);
    EXPECT_EQ(*second_half, expected_second);
    delete first_half;
    delete second_half;
  }
}

TEST(SortList, SimpleMerge) {
  auto l1 = new Node{5, 7, 9, 13};
  auto l2 = new Node{2, 3, 11, 17};
  Node expected{2, 3, 5, 7, 9, 11, 13, 17};
  log::debug("Merging:\n\t[{}]\n\t[{}]", *l1, *l2);
  Node* merged = Node::merge_sorted_lists(l1, l2);
  EXPECT_EQ(*merged, expected);

  delete merged;
}

TEST(SortList, FullSort) {
  {
    Node* l1 = nullptr;
    Node* sorted = Node::sort(l1);
    EXPECT_EQ(sorted, nullptr);
  }

  {
    auto l1 = new Node{5};
    Node* sorted = Node::sort(l1);
    EXPECT_EQ(*sorted, *l1);
    delete sorted;
  }

  {
    auto l1 = new Node{5, 2};
    Node expected{2, 5};
    Node* sorted = Node::sort(l1);
    EXPECT_EQ(*sorted, expected);
    delete sorted;
  }

  {
    auto l1 = new Node{5, 2, 3};
    Node expected{2, 3, 5};
    Node* sorted = Node::sort(l1);
    EXPECT_EQ(*sorted, expected);
    delete sorted;
  }

  {
    auto l1 = new Node{4, 2, 1, 3};
    Node expected{1, 2, 3, 4};
    log::debug("Sorting list: [{}]", *l1);
    Node* sorted = Node::sort(l1);
    EXPECT_EQ(*sorted, expected);
    delete sorted;
  }

  {
    auto l1 = new Node{-1, 5, 3, 4, 0};
    Node expected{-1, 0, 3, 4, 5};
    log::debug("Sorting list: [{}]", *l1);
    Node* sorted = Node::sort(l1);
    EXPECT_EQ(*sorted, expected);
    delete sorted;
  }
}