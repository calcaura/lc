#pragma once

#include <array>
#include <cstdint>
#include <limits>
#include <vector>

namespace lc_libs {

class Trie {
 public:
  Trie() : _nodes(1) {}

  void insert(const char* word) {
    size_t in{0};
    for (const char* c = word; *c; ++c) {
      const uint8_t idx = *c - 'a';
      size_t next = _nodes[in].children[idx];
      if (next == 0) {
        if (*(c + 1) == 0) {
          _nodes[in].children[idx] = kLeafValue;
          return;
        } else {
          next = newNode();
          _nodes[in].children[idx] = next;
        }
      } else if (next == kLeafValue) {
        if (*(c + 1) == 0) {
          break;
        } else {
          _nodes[in].children[idx] = next = newNode();
          _nodes[next].is_eow = true;
        }
      }
      in = next;
    }
    _nodes[in].is_eow = true;
  }

  bool search(const char* word) {
    size_t in{0};
    for (const char* c = word; *c; ++c) {
      const uint8_t idx = *c - 'a';
      size_t next = _nodes[in].children[idx];
      if (next == 0) {
        return false;
      }
      if (next == kLeafValue) {
        return *(c + 1) == 0;
      }
      in = next;
    }
    return _nodes[in].is_eow;
  }

  bool startsWith(const char* word) {
    size_t in{0};
    for (const char* c = word; *c; ++c) {
      const uint8_t idx = *c - 'a';
      size_t next = _nodes[in].children[idx];
      if (next == 0) {
        return false;
      }
      if (next == kLeafValue) {
        return *(c + 1) == 0;
      }
      in = next;
    }
    return true;
  }

 private:
  constexpr static inline const uint16_t kLeafValue =
      std::numeric_limits<uint16_t>::max();

  struct alignas(64) Node {
    std::array<uint16_t, 'z' - 'a' + 1> children{};
    bool is_eow{};
  };

  std::vector<Node> _nodes;

  size_t newNode() {
    auto sz = _nodes.size();
    _nodes.resize(sz + 1);
    return sz;
  }
};
}  // namespace lc_libs
