#pragma once

#include <queue>
#include <vector>

class MedianFinder {
  std::priority_queue<int> left;
  std::priority_queue<int, std::vector<int>, std::greater<int>> right;

 public:
  MedianFinder() {}

  void addNum(int num) {
    if (left.empty()) {
      left.push(num);
      return;
    }
    if (num < left.top()) {
      left.push(num);
    } else {
      right.push(num);
    }
    // rebalance
    int balance = right.size() - left.size();

    if (balance < -1) {
      right.push(left.top());
      left.pop();
    } else if (balance > 1) {
      left.push(right.top());
      right.pop();
    }
  }

  double findMedian() {
    if (left.size() == right.size()) {
      return (double(left.top()) + right.top()) / 2.0;
    }
    if (left.size() > right.size()) {
      return left.top();
    }
    return right.top();
  }
};
