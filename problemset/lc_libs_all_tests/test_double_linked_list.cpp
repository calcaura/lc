// Test
#include <gtest/gtest.h>

// C++ STL
#include <algorithm>
#include <iostream>
#include <lc_libs/double_linked_list.hpp>
#include <lc_libs/simple_log.hpp>
#include <memory>
#include <sstream>
#include <vector>

namespace log = lc_libs::log;

using Node = lc_libs::DListNodeBase<int>;
using List = lc_libs::List<int>;

TEST(DoubleLinkedList, Ctor) {
  {
    Node n{};
    ASSERT_EQ(n.val, 0);
    ASSERT_EQ(n.next, nullptr);
    ASSERT_EQ(n.prev, nullptr);
  }

  {
    Node n{42};
    ASSERT_EQ(n.val, 42);
    ASSERT_EQ(n.next, nullptr);
    ASSERT_EQ(n.prev, nullptr);
  }
  {
    Node n{7, 9, 42};
    ASSERT_EQ(n.val, 7);
    ASSERT_EQ(n.prev, nullptr);
    ASSERT_NE(n.next, nullptr);

    auto c = n.next;
    ASSERT_EQ(c->val, 9);
    ASSERT_EQ(c->prev, &n);
    ASSERT_NE(c->next, nullptr);

    auto d = c->next;
    ASSERT_EQ(d->val, 42);
    ASSERT_EQ(d->prev, c);
    ASSERT_EQ(d->next, nullptr);
  }
}

TEST(DoubleLinkedList, Ostream) {
  const Node n{7, 9, 42};

  {
    std::stringstream ss;
    ss << n;
    auto result = ss.str();
    ASSERT_EQ(result, "7, 9, 42");
  }

  {
    std::stringstream ss;
    ss << n.view("/\\");
    auto result = ss.str();
    ASSERT_EQ(result, "7/\\9/\\42");
  }
}

TEST(DoubleLinkedList, Format) {
  const Node n{7, 9, 42};
  {
    auto result = std::format("{}", n);
    ASSERT_EQ(result, "7, 9, 42");
  }

  {
    auto result = std::format("{}", n.view("|"));
    ASSERT_EQ(result, "7|9|42");
  }
}

TEST(DoubleLinkedList, push_back) {
  List list{7, 9, 42};
  ASSERT_EQ(list, (std::initializer_list<int>{7, 9, 42}));
  ASSERT_EQ(list.size(), 3);
  ASSERT_FALSE(list.empty());

  list.push_back(73);
  ASSERT_EQ(list, (std::initializer_list<int>{7, 9, 42, 73}));
  ASSERT_EQ(list.size(), 4);

  ASSERT_EQ(list.at(1)->val, 9);
  ASSERT_EQ(list.at(3)->val, 73);
  ASSERT_EQ(list.at(4), nullptr);
}

TEST(DoubleLinkedList, push_front) {
  List list{7, 9, 42};
  ASSERT_EQ(list, (std::initializer_list<int>{7, 9, 42}));
  ASSERT_EQ(list.size(), 3);

  list.push_front_value(73);
  ASSERT_EQ(list, (std::initializer_list<int>{73, 7, 9, 42}));
  ASSERT_EQ(list.size(), 4);

  ASSERT_EQ(list.at(0)->val, 73);
  ASSERT_EQ(list.at(1)->val, 7);
  ASSERT_EQ(list.at(2)->val, 9);
  ASSERT_EQ(list.at(4), nullptr);
}

TEST(DoubleLinkedList, erase) {
  List list{7, 9, 42};
  ASSERT_EQ(list, (std::initializer_list<int>{7, 9, 42}));
  ASSERT_EQ(std::format("{}", list), "7, 9, 42");
  ASSERT_EQ(list.size(), 3);

  auto rm = list.back();
  ASSERT_EQ(rm->val, 42);
  ASSERT_EQ(list, (std::initializer_list<int>{7, 9, 42}));
  // NOLINTBEGIN(clang-analyzer-cplusplus.NewDeleteLeaks)
  list.erase(rm);
  ASSERT_EQ(list.size(), 2);
  list.push_front(rm);
  // NOLINTEND(clang-analyzer-cplusplus.NewDeleteLeaks)
  ASSERT_EQ(list.size(), 3);
  ASSERT_EQ(list, (std::initializer_list<int>{42, 7, 9}));
  ASSERT_EQ(std::format("{}", list), "42, 7, 9");
}

TEST(DoubleLinkedList, relocate_front) {
  List list{7, 9, 42, 73, 91};
  ASSERT_EQ(list, (std::initializer_list<int>{7, 9, 42, 73, 91}));
  ASSERT_EQ(std::format("{}", list), "7, 9, 42, 73, 91");
  ASSERT_EQ(list.size(), 5);

  list.relocate_front(list.at(2));
  ASSERT_EQ(list, (std::initializer_list<int>{42, 7, 9, 73, 91}));

  list.relocate_front(list.at(0));
  ASSERT_EQ(list, (std::initializer_list<int>{42, 7, 9, 73, 91}));

  list.relocate_front(list.at(4));
  ASSERT_EQ(list, (std::initializer_list<int>{91, 42, 7, 9, 73}));

  list.relocate_front(list.at(0));
  ASSERT_EQ(list, (std::initializer_list<int>{91, 42, 7, 9, 73}));
}
