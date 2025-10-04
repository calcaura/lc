#include <optional>
#include <unordered_map>

#include "double_linked_list.hpp"

namespace lruv2 {

template <typename KeyType, typename ValueType>
struct Entry {
  KeyType key;
  ValueType value;

  Entry(KeyType k, ValueType v) : key{std::move(k)}, value{std::move(v)} {}

  bool operator==(const Entry& o) const {
    return o.key == key && o.value == value;
  }
};

template <typename KeyType, typename ValueType>
class Lru {
 public:
  using Entry = Entry<KeyType, ValueType>;
  using LruList = lc_libs::List<Entry>;
  using Cache = std::unordered_map<KeyType, typename LruList::Node*>;

  /**
   * @brief Construct a new Lru empty object
   * @param capacity Maximum number of elements in the cache
   */
  Lru(std::size_t capacity) : mCapacity(capacity) {}

  /**
   * @brief Construct a new Lru object with initial values
   * @param capacity Maximum number of elements in the cache
   * @param values Initial key-value pairs to populate the cache
   */
  Lru(std::size_t capacity,
      std::initializer_list<std::pair<KeyType, ValueType>> values)
      : mCapacity(capacity) {
    for (auto [k, v] : values) {
      put(k, std::move(v));
    }
  }

  /**
   * @brief Get a value from the cache
   * @param key Key to search for
   * @return std::optional<ValueType&> Value associated with the key, or empty
   * if not found
   */
  std::optional<ValueType> get(KeyType key) {
    auto it = mCache.find(key);
    if (it == mCache.end()) {
      return {};
    }
    // Relocate the element to the front of LRU
    mLru.relocate_front(it->second);
    return it->second->val.value;
  }

  /**
   * @brief Put a key-value pair into the cache
   * @param key Key to insert or update
   * @param value Value to associate with the key
   * @return true if the key was already present and updated, false if it was
   * newly inserted (possibly evicting the least recently used item)
   */
  bool put(KeyType key, ValueType&& value) {
    auto it = mCache.find(key);
    if (it != mCache.end()) {
      // Element found, just relocate the entry from LRU
      it->second->val.value = std::move(value);
      mLru.relocate_front(it->second);
      return true;
    }
    // Key not found
    mLru.push_front_value({key, value});
    if (mLru.size() > mCapacity) {
      mCache.erase(mLru.back()->val.key);
      delete mLru.pop_back();
    }
    mCache[key] = mLru.front();
    return false;
  }

  /**
   * @brief Get the capacity of the cache
   * @return std::size_t Capacity of the cache
   */
  std::size_t capacity() const { return mCapacity; }

  /**
   * @brief Get the current LRU list
   * @return const LruList& Current LRU list
   */
  const LruList& lru() const { return mLru; }

  /**
   * @brief Get the current cache map
   * @return const Cache& Current cache map
   */
  const Cache& cache() const { return mCache; }

  bool operator==(const Lru& other) const {
    return mCapacity == other.mCapacity && mLru == other.mLru &&
           mCache == other.mCache;
  }

 private:
  const std::size_t mCapacity;
  LruList mLru;
  Cache mCache;
};
}  // namespace lruv2

#if __has_include(<iostream>)
#include <iostream>
namespace lruv2 {

template <typename K, typename V>
std::ostream& operator<<(std::ostream& os, const Entry<K, V>& node) {
  return os << "{" << node.key << ":" << node.value << "}";
}
}  // namespace lruv2
#endif