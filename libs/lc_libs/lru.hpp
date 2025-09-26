#include <list>
#include <optional>
#include <unordered_map>

namespace lru {
template <typename KeyType, typename ValueType>
class Lru {
 public:
  using LruList = std::list<std::pair<KeyType, ValueType>>;
  using Cache = std::unordered_map<KeyType, typename LruList::iterator>;

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
    mLru.splice(mLru.begin(), mLru, it->second);
    return it->second->second;
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
      it->second->second = std::move(value);
      mLru.splice(mLru.begin(), mLru, it->second);
      return true;
    }
    // Key not found
    mLru.push_front({key, value});
    if (mLru.size() > mCapacity) {
      mCache.erase(mLru.back().first);
      mLru.pop_back();
    }
    mCache[key] = mLru.begin();
    return false;
  }

#if __has_include(<iostream>)
  void dump_js(std::ostream& os) const {
    os << "{\"capacity\":" << mCapacity << ", \"lru\": [";
    bool add_comma{};
    for (auto& [k, v] : mLru) {
      if (add_comma) {
        os << ", ";
      }
      add_comma = true;
      os << "(" << k << ":" << v << ")";
    }
    add_comma = false;
    os << "], \"cache\": {";
    for (auto& [k, v] : mCache) {
      if (add_comma) {
        os << ", ";
      }
      add_comma = true;
      os << k << ":" << v->second;
    }
    os << "}}";
  }
#endif

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
}  // namespace lru

#if __has_include(<iostream>)
template <typename KeyType, typename ValueType>
std::ostream& operator<<(std::ostream& os,
                         const ::lru::Lru<KeyType, ValueType>& lru) {
  lru.dump_js(os);
  return os;
}
#endif