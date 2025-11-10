#pragma once

#include <array>
#include <cstdint>
#include <limits>
#include <vector>

namespace lc_libs {

class Trie {
 public:
  Trie() : _nodes(1) {}

  /**
   * Inserts a word into the trie.
   * All characters are assumed to be 'a' to 'z'.
   * If characters outside this range are used, the behavior is undefined.
   */
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

  /**
   * Returns true if the word is in the trie.
   */
  bool search(const char* word) {
    size_t in{0};
    for (const char* c = word; *c; ++c) {
      const uint8_t idx = *c - 'a';
      // Sanity check
      if (idx > 'z' - 'a') {
        return false;
      }

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

  /**
   * Returns true if the word is in the trie. The word may contain the
   * wildcard character '.' to represent any one letter.
   */
  bool searchWildcard(const char* word, size_t in = 0) {
    for (const char* c = word; *c; ++c) {
      if (*c == '.') {
        // special case, . at the end
        if (*(c + 1) == 0) {
          for (auto v : _nodes[in].children) {
            if (v == kLeafValue) {
              return true;
            }
            if (v != 0 && _nodes[v].is_eow) {
              return true;
            }
          }
          return false;
        }  // end of special case, . at the end

        for (auto v : _nodes[in].children) {
          if (v == kLeafValue) {
            continue;
          }
          if (v != 0 && searchWildcard(c + 1, v)) {
            return true;
          }
        }
        return false;
      }  // End of wildcard,

      // Now try normal search
      const uint8_t idx = *c - 'a';
      // Sanity check
      if (idx > 'z' - 'a') {
        return false;
      }

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

  /**
   * Returns true if there is any word in the trie that starts with the given
   * prefix.
   */
  bool startsWith(const char* word) {
    size_t in{0};
    for (const char* c = word; *c; ++c) {
      const uint8_t idx = *c - 'a';
      // Sanity check
      if (idx > 'z' - 'a') {
        return false;
      }

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
