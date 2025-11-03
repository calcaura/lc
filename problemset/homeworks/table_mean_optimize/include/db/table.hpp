#pragma once

#include <absl/types/span.h>

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <new>
#include <vector>

namespace db {

namespace data_type {

/*
 * @brief A run-time data type.
 */
enum class type { db_int32, db_int64, db_float };

/*
 * @brief Returns a type's size in bytes.
 */
constexpr size_t size(type type) {
  size_t size = 0;

  switch (type) {
    case type::db_int32:
      size = sizeof(std::int32_t);
      break;
    case type::db_int64:
      size = sizeof(std::int64_t);
      break;
    case type::db_float:
      size = sizeof(float);
      break;
    default:
      __builtin_unreachable();
  }

  return size;
}

/*
 * @brief Converts a static C++ type to a run-time DB type.
 */
template <typename T>
type to_type() {
  assert(false);  // invalid type
  return type::db_int32;
};

template <>
inline type to_type<int32_t>() {
  return type::db_int32;
}

template <>
inline type to_type<int64_t>() {
  return type::db_int64;
}

template <>
inline type to_type<float>() {
  return type::db_float;
}

}  // namespace data_type

/*
 * @brief Abstract base table
 *
 * `table` provides an API that should be implemented by a concrete table.
 *
 * DO: Use the provided `block_table` class, or design your own concrete table.
 *
 * DON'T: Modify the `table` API.
 */
class table {
 public:
  table() = default;
  table(table&&) = default;

  table(const table&) = delete;

  /*
   * @brief Returns the table size.
   */
  virtual size_t size() const = 0;

  /*
   * @brief Returns the number of columns in the table.
   */
  virtual size_t columns() const = 0;

  /*
   * @brief Returns the data type of a column.
   */
  virtual data_type::type type(int column_idx) const = 0;

  /*
   * @brief Push a new value to the back.
   */
  // FIXME: find solution for `error: templates may not be ‘virtual’`
  template <typename T>
  void push_back(int column_idx, T value) {
    assert(false);
  }
};

/*
 * @brief A fixed-size block of data.
 *
 * A block contains a partial data column. Data are type-erased to facilitate
 * typing at run-time. The block provides type-safe access to its contents.
 *
 * DO: Modify the `block` API or implementation.
 *
 * DON'T: No restrictions.
 */
class block {
 public:
  block(block&&) = default;

  block() = delete;
  block(const block&) = delete;

  /*
   * @brief Creates a new block.
   */
  block(data_type::type type, size_t capacity)
      : _type(type), _size(0), _capacity(capacity) {
    auto type_size = data_type::size(type);
    _data = std::unique_ptr<char[]>(
        new (std::align_val_t{type_size}) char[capacity * type_size]);
  }

  /*
   * @brief Returns a span over the block's contents.
   */
  template <typename T>
  absl::Span<T> span() {
    return absl::Span<T>(reinterpret_cast<T*>(_data.get()), _size);
  }

  /*
   * @brief Returns a const span over the block's contents.
   */
  template <typename T>
  absl::Span<const T> span() const {
    return absl::Span<const T>(reinterpret_cast<const T*>(_data.get()), _size);
  }

  /*
   * @brief Returns the block size.
   *
   * I.e., the number of items currently in the block.
   */
  inline size_t size() const noexcept { return _size; }

  /*
   * @brief Returns the block capacity.
   *
   * I.e., the maximum number of items the block can hold.
   */
  inline size_t capacity() const noexcept { return _capacity; }

  /*
   * @brief Push a new value to the back.
   */
  template <typename T>
  void push_back(T value) {
    assert(_size != _capacity);

    T* typed_block = reinterpret_cast<T*>(_data.get());
    typed_block[_size] = value;

    ++_size;
  }

 private:
  data_type::type _type;
  size_t _size;
  size_t _capacity;
  std::unique_ptr<char[]> _data;
};

/*
 * @brief A table consisting of fixed-size blocks.
 *
 * DO: Modify the `block_table` API or implementation.
 *
 * DON'T: No restrictions.
 */
class block_table : public table {
 public:
  template <typename T>
  class attribute_iter;

  template <typename T>
  class const_attribute_iter;

  block_table(block_table&&) = default;

  block_table() = delete;
  block_table(const block_table&) = delete;

  /*
   * @brief Creates a new table.
   */
  block_table(std::vector<data_type::type> attributes,
              size_t block_capacity = 1024ul)
      : table(),
        block_capacity(block_capacity),
        _size(0),
        _capacity(0),
        _attribute_type(std::move(attributes)) {
    _attribute_blocks.resize(_attribute_type.size());
  }

  block_table& operator=(block_table&& other) {
    assert(_attribute_type == other._attribute_type);

    _size = std::move(other._size);
    _capacity = std::move(other._capacity);

    // Avoid allocating a new vector to keep `other` in a consistent state.
    std::swap(_attribute_blocks, other._attribute_blocks);

    // Reset attributes to initial state.
    for (auto& blocks : other._attribute_blocks) {
      blocks.clear();
    }

    other._size = 0;
    other._capacity = 0;

    return *this;
  }

  const size_t block_capacity;

  /*
   * @copydoc table::size
   */
  inline size_t size() const override { return _size; }

  /*
   * @brief Returns the table capacity.
   *
   * I.e., the maximum number of rows the table can hold without allocating more
   * space.
   */
  inline size_t capacity() const { return _capacity; }

  /*
   * @copydoc table::columns
   */
  inline size_t columns() const override { return _attribute_blocks.size(); }

  /*
   * @copydoc table::size
   */
  data_type::type type(int column_idx) const override {
    return _attribute_type[column_idx];
  }

  /*
   * @brief Returns a reference to the specified block.
   */
  block& block_at(int column_idx, ssize_t block_idx) noexcept {
    return _attribute_blocks[column_idx][block_idx];
  }

  /*
   * @brief Returns a const reference to the specified block.
   */
  const block& block_at(int column_idx, ssize_t block_idx) const noexcept {
    return _attribute_blocks[column_idx][block_idx];
  }

  /*
   * @brief Returns how many blocks the column consists of.
   */
  size_t num_blocks(int column_idx) const noexcept {
    return _attribute_blocks[column_idx].size();
  }

  /*
   * @brief Push a new value to the back of the column.
   */
  template <typename T>
  void push_back(int column_idx, T value) {
    assert(column_idx >= 0 && column_idx < _attribute_blocks.size());

    auto& attr = _attribute_blocks[column_idx];

    if (attr.empty() || attr.back().size() == attr.back().capacity()) {
      data_type::type type = data_type::to_type<T>();
      attr.emplace_back(type, block_capacity);

      _capacity += block_capacity;
    }

    attr.back().push_back(value);
    ++_size;
  }

  void horizontal_concatinate(block_table&& other) {
    assert(_attribute_blocks.empty() || _size == other._size);
    assert(_attribute_blocks.empty() || _capacity == other._capacity);

    if (_attribute_blocks.empty()) {
      _size = other._size;
      _capacity = other._capacity;
    }

    std::move(other._attribute_type.begin(), other._attribute_type.end(),
              std::back_inserter(_attribute_type));
    std::move(other._attribute_blocks.begin(), other._attribute_blocks.end(),
              std::back_inserter(_attribute_blocks));
  }

  void vertical_concatinate(block_table&& other) {
    assert(_attribute_type == other._attribute_type);
    assert(_attribute_blocks.size() == other._attribute_blocks.size());

    _size += other._size;
    _capacity += other._capacity;

    for (size_t b = 0; b < _attribute_blocks.size(); ++b) {
      std::move(other._attribute_blocks[b].begin(),
                other._attribute_blocks[b].end(),
                std::back_inserter(_attribute_blocks[b]));
    }
  }

  /*
   * @brief Returns an iterator at the beginning of the column.
   */
  template <typename T>
  attribute_iter<T> begin(int column_idx) {
    auto& attr = _attribute_blocks[column_idx];

    if (attr.empty()) {
      return {absl::Span<block>(), 0, 0};
    } else {
      return {absl::Span<block>(attr), 0, 0};
    }
  }

  /*
   * @brief Returns an iterator at the end of the column.
   */
  template <typename T>
  attribute_iter<T> end(int column_idx) {
    return {absl::Span<block>(), _attribute_blocks[column_idx].size(), 0};
  }

  /*
   * @brief An iterator over an attribute column.
   *
   * The iterator hides that a column consists of multiple blocks.
   */
  template <typename T>
  class attribute_iter : public std::iterator<std::forward_iterator_tag, T> {
   public:
    attribute_iter(absl::Span<block> blocks, size_t block, size_t pos)
        : _blocks(std::move(blocks)), _block(block), _pos(pos) {}

    attribute_iter<T>& operator++() noexcept {
      ++_pos;

      if (_pos == _blocks[_block].size()) {
        _pos = 0;
        ++_block;
      }

      return *this;
    }

    bool operator==(const attribute_iter<T>& other) const noexcept {
      return _block == other._block && _pos == other._pos;
    }

    bool operator!=(const attribute_iter<T>& other) const noexcept {
      return _block != other._block || _pos != other._pos;
    }

    [[nodiscard]] T& operator*() const noexcept {
      return _blocks[_block].template span<T>()[_pos];
    }

   private:
    absl::Span<block> _blocks;
    size_t _block;
    size_t _pos;
  };

  /*
   * @brief Returns a const iterator at the beginning of the column.
   */
  template <typename T>
  const_attribute_iter<T> cbegin(int column_idx) const {
    auto& attr = _attribute_blocks[column_idx];

    if (attr.empty()) {
      return {absl::Span<const block>(), 0, 0};
    } else {
      return {absl::Span<const block>(attr), 0, 0};
    }
  }

  /*
   * @brief Returns a const iterator at the end of the column.
   */
  template <typename T>
  const_attribute_iter<T> cend(int column_idx) const {
    return {absl::Span<const block>(), _attribute_blocks[column_idx].size(), 0};
  }


  /*
   * @brief A const iterator over an attribute column.
   *
   * Functionally the same as its non-const sibling.
   */
  template <typename T>
  class const_attribute_iter
      : public std::iterator<std::forward_iterator_tag, T> {
   public:
    const_attribute_iter(absl::Span<const block> blocks, size_t block,
                         size_t pos)
        : _blocks(std::move(blocks)), _block(block), _pos(pos) {}

    const_attribute_iter<T>& operator++() noexcept {
      ++_pos;

      if (_pos == _blocks[_block].size()) {
        _pos = 0;
        ++_block;
      }

      return *this;
    }

    bool operator==(const const_attribute_iter<T>& other) const noexcept {
      return _block == other._block && _pos == other._pos;
    }

    bool operator!=(const const_attribute_iter<T>& other) const noexcept {
      return _block != other._block || _pos != other._pos;
    }

    [[nodiscard]] const T& operator*() const noexcept {
      return _blocks[_block].template span<T>()[_pos];
    }

   private:
    absl::Span<const block> _blocks;
    size_t _block;
    size_t _pos;
  };

 private:
  size_t _size;
  size_t _capacity;
  std::vector<data_type::type> _attribute_type;
  std::vector<std::vector<block>> _attribute_blocks;
};

}  // namespace db

/*
 * @brief Prints the table content in a human-readable format to the provided ostream
 * @param buf buffer to write to
 * @param table input table
 * @return buffer
 */
inline std::ostream& operator<<(std::ostream& buf, const db::block_table& table) {
    if (table.columns() == 0) {
        buf << "Empty Table\n";
        return buf;
    }
    for(int columnIdx = 0; columnIdx < table.columns(); columnIdx++) {
        std::cout << "\t|\t";
        std::cout << columnIdx;
    }
    std::cout << "\t|\t\n";
    for(int columnIdx = 0; columnIdx < table.columns(); columnIdx++) {
        std::cout << "\t|\t=";
    }

    std::cout << "\t|\t\n";
    for (size_t blockIdx = 0; blockIdx < table.num_blocks(0); blockIdx++) {
        for(size_t row = 0; row < table.block_at(0, blockIdx).size(); row++) {
            for (int columnIdx = 0; columnIdx < table.columns(); columnIdx++) {
                std::cout << "\t|\t";
                switch (table.type(columnIdx)) {
                    case db::data_type::type::db_int32:
                        std::cout << table.block_at(columnIdx, blockIdx).span<int>()[row];
                        break;
                    case db::data_type::type::db_float:
                        std::cout << table.block_at(columnIdx, blockIdx).span<float>()[row];
                        break;
                    case db::data_type::type::db_int64:
                        std::cout << table.block_at(columnIdx, row).span<long>()[row];
                        break;
                }
            }
            std::cout << "\t|\t\n";
        }
    }
    return buf;
}