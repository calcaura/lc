
#include <algorithm>
#include <iostream>
#include <vector>

/**
 * Definition for singly-linked list.
 */
struct ListNode {
  int val;
  ListNode* next;
  ListNode() : val(0), next(nullptr) {}
  ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

std::ostream& operator<<(std::ostream& os, const ListNode* node) {
  while (node) {
    os << node->val;
    node = node->next;
    if (node) os << ", ";
  }

  return os;
}

bool equals(ListNode* l1, ListNode* l2) {
  while (l1 && l2) {
    if (l1->val != l2->val) return false;
    l1 = l1->next;
    l2 = l2->next;
  }
  return l1 == nullptr && l2 == nullptr;
}

ListNode* build(const std::vector<int>& values) {
  if (values.empty()) return nullptr;
  ListNode* head = new ListNode(values[0]);
  ListNode* current = head;
  for (size_t i = 1; i < values.size(); ++i) {
    current->next = new ListNode(values[i]);
    current = current->next;
  }
  return head;
}

class Solution {
 public:
  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode head;
    int carry = 0;
    ListNode* current = &head;

    while (l1 || l2 || carry) {
      if (l1) {
        carry += l1->val;
        l1 = l1->next;
      }
      if (l2) {
        carry += l2->val;
        l2 = l2->next;
      }

      ListNode* next = new ListNode(carry % 10);
      current->next = next;
      current = next;
      carry /= 10;
    }

    return head.next;
  }

  void test_add(ListNode* l1, ListNode* l2, ListNode* expected) {
    std::cout << "\n >>> Test: [" << l1 << "] + [" << l2 << "] == [" << expected
              << "]?\n";
    ListNode* result = addTwoNumbers(l1, l2);
    if (equals(result, expected)) {
      std::cout << "Test passed." << std::endl;
    } else {
      std::cout << "Test failed." << std::endl;
      std::cout << "Result: [" << result << "]\n";
    }
  }
};

void test_add(std::vector<int> v1, std::vector<int> v2,
              std::vector<int> expected_values) {
  ListNode* l1 = build(v1);
  ListNode* l2 = build(v2);
  ListNode* expected = build(expected_values);
  Solution solution;
  solution.test_add(l1, l2, expected);
}

int main() {
  test_add({2, 4, 3}, {5, 6, 4}, {7, 0, 8});
  test_add({9, 9, 9, 9, 9, 9, 9}, {9, 9, 9, 9}, {8, 9, 9, 9, 0, 0, 0, 1});
  test_add({0}, {0}, {0});
  return 0;
}