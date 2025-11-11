#pragma once

#include <assert.h>

#include <array>
#include <cstdint>
#include <functional>
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
          next = newNode(*c);
          _nodes[in].children[idx] = next;
        }
      } else if (next == kLeafValue) {
        if (*(c + 1) == 0) {
          break;
        } else {
          _nodes[in].children[idx] = next = newNode(*c);
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
   * Removes a word from the trie.
   * Returns true if the word was removed, false if the word was not found.
   */
  bool remove(const char* word) {
    std::vector<T> path{get_path(word)};
    if (path.empty()) {
      return false;
    }

    if (path.back() == kLeafValue) {
      // Special case: word ends at a leaf
      path.pop_back();
      if (path.empty()) {
        return false;
      }
      T parent = path.back();
      char last_char = word[path.size() - 1];
      uint8_t idx = last_char - 'a';
      _nodes[parent].children[idx] = 0;
    } else {
      // Normal case
      T in = path.back();
      _nodes[in].is_eow = false;
    }

    while (!path.empty()) {
      const auto in = path.back();
      path.pop_back();
      if (_nodes[in].has_children() || _nodes[in].is_eow) {
        break;
      }
      // This node can be erased.
      // Update the parent first
      if (!path.empty()) {
        _nodes[path.back()].children[_nodes[in].value - 'a'] = 0;
      }
      freeNode(in);
    }

    return true;
  }

  /**
   * Removes a word from the trie.
   * Returns true if the word was removed, false if the word was not found.
   */
  bool remove(const std::string& word) { return remove(word.c_str()); }

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

  /**
   * Calls func for each word in the trie.
   * Note: The words are generated in lexicographical order.
   */
  void for_each_word(const std::function<void(const std::string&)>& func,
                     std::string current = "", size_t in = 0,
                     bool internal = false) const {
    const char* end_char = internal ? "$" : "";

    if (_nodes[in].is_eow) {
      func(current + end_char);
    }
    for (size_t i = 0; i < _nodes[in].children.size(); ++i) {
      size_t next = _nodes[in].children[i];
      if (next == 0) {
        continue;
      }
      char c = static_cast<char>(i + 'a');
      if (next == kLeafValue) {
        func(current + c + end_char);
      } else {
        if (internal) {
          func(current + c);
        }
        for_each_word(func, current + c, next, internal);
      }
    }
  }

  /**
   * Returns all words in the trie.
   * Note: The words are generated in lexicographical order.
   */
  std::vector<std::string> get_all_words(bool internal = false) const {
    std::vector<std::string> words;
    for_each_word([&words](const std::string& word) { words.push_back(word); },
                  "", 0, internal);
    return words;
  }

 private:
  constexpr static inline const T kLeafValue = std::numeric_limits<T>::max();

  struct alignas(32) Node {
    std::array<T, 'z' - 'a' + 1> children{};
    char value{};
    bool is_eow{};
    bool is_used;

    bool has_children() const {
      for (const auto& c : children) {
        if (c != 0) {
          return true;
        }
      }
      return false;
    }
  };

  std::vector<Node> _nodes;
  std::vector<T> _free_nodes;

  T newNode(char value) {
    if (!_free_nodes.empty()) {
      T n = _free_nodes.back();
      _free_nodes.pop_back();
      _nodes[n].is_used = true;
      _nodes[n].value = value;
      return n;
    }

    auto sz = _nodes.size();
    assert(sz < kLeafValue);
    _nodes.resize(sz + 1);
    _nodes[sz].is_used = true;
    _nodes[sz].value = value;
    return sz;
  }

  void freeNode(T n) {
    _nodes[n].is_used = false;
    _nodes[n].value = 0;
    _nodes[n].children.fill(0);
    _free_nodes.push_back(n);
  }

  bool is_eow(T n) const { return n == kLeafValue ? true : _nodes[n].is_eow; };
  bool has_children(T n) const {
    return n == kLeafValue ? false : _nodes[n].has_children();
  };

  std::vector<T> get_path(const char* word) const {
    std::vector<T> path;
    T in{0};
    for (const char* c = word; *c; ++c) {
      path.push_back(in);
      const uint8_t idx = *c - 'a';
      T next = _nodes[in].children[idx];
      if (next == 0) {
        return {};
      }
      if (next == kLeafValue) {
        if (*(c + 1) == 0) {
          path.push_back(kLeafValue);
          return path;
        } else {
          return {};
        }
      }
      in = next;
    }
    if (_nodes[in].is_eow == false) {
      return {};
    }
    path.push_back(in);
    return path;
  }
};
}  // namespace lc_libs
