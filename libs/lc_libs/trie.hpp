#pragma once

#include <assert.h>

#include <array>
#include <cstdint>
#include <limits>
#include <string>
#include <vector>

namespace lc_libs {

template <typename T = uint16_t>
class Trie {
 public:
  Trie() : _nodes(1) {}

  Trie(const Trie&) = delete;
  Trie& operator=(const Trie&) = delete;

  template <typename Iter>
  Trie(Iter begin, Iter end) : _nodes(1) {
    for (auto it = begin; it != end; ++it) {
      insert(*it);
    }
  }

  template <typename Word>
  Trie(const std::vector<Word>& words) : _nodes(1) {
    for (const auto& word : words) {
      insert(word);
    }
  }

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

  void insert(const std::string& word) { insert(word.c_str()); }

  template <typename Word>
  void insert(const std::vector<Word>& words) {
    for (const auto& word : words) {
      insert(word);
    }
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
  constexpr static inline const T kLeafValue = std::numeric_limits<T>::max();

  struct alignas(32) Node {
    std::array<T, 'z' - 'a' + 1> children{};
    bool is_eow{};
    bool is_used;
  };

  std::vector<Node> _nodes;
  std::vector<T> _free_nodes;

  size_t newNode() {
    if (!_free_nodes.empty()) {
      size_t n = _free_nodes.back();
      _free_nodes.pop_back();
      return n;
    }

    auto sz = _nodes.size();
    assert(sz < kLeafValue);
    _nodes.resize(sz + 1);
    _nodes[sz].is_used = true;
    return sz;
  }

  void freeNode(size_t n) {
    _nodes[n].is_used = false;
    _free_nodes.push_back(n);
  }
};
}  // namespace lc_libs
