#pragma once

#include <lc_libs/trie.hpp>
#include <string>

class WordDictionary {
  lc_libs::Trie<uint16_t> _trie;

 public:
  WordDictionary() {}

  void addWord(const std::string& word) { _trie.insert(word.c_str()); }

  bool search(const std::string& word) {
    return _trie.searchWildcard(word.c_str());
  }
};