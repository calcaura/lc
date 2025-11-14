#pragma once

#include <absl/random/random.h>
#include <benchmark/benchmark.h>

#include <cstdint>
#include <db/group_by.hpp>
#include <db/table.hpp>
#include <limits>
#include <memory>
#include <utility>
#include <vector>

class group_by_benchmark : public ::benchmark::Fixture {
 public:
  static constexpr size_t default_nrows = 67ul * 1000ul * 1000ul;
  static constexpr int64_t default_cardinality = 1ll << 19;

  std::unique_ptr<db::group_by_table> _table;
  absl::InsecureBitGen _bitgen;

  /*
   * DO: Change the table type to your own implementation.
   */
  std::unique_ptr<db::group_by_table> new_table(
      std::vector<db::data_type::type>&& schema) {
    return std::make_unique<db::group_by_table>(std::move(schema));
  }

  void TearDown(const ::benchmark::State&) override { _table.reset(); }
};

template <typename T>
class attribute_generator {
 public:
  T operator()() {
    return absl::Uniform<T>(_bitgen, std::numeric_limits<T>::min(),
                            std::numeric_limits<T>::max());
  }

 private:
  absl::InsecureBitGen _bitgen;
};

BENCHMARK_DEFINE_F(group_by_benchmark, uniform_mean)
(::benchmark::State& state) {
  size_t nrows = state.range(0);
  db::key_type cardinality = state.range(1);
  attribute_generator<db::attribute_type> generate_value;

  auto schema = std::vector<db::data_type::type>{
      db::data_type::to_type<db::key_type>(),
      db::data_type::to_type<db::attribute_type>()};
  _table = new_table(std::move(schema));

  for (size_t r = 0; r < nrows; ++r) {
    _table->push_back(0, absl::Uniform<db::key_type>(_bitgen, 0, cardinality));
    _table->push_back(1, generate_value());
  }

  auto aggregates = std::vector<db::aggregation_attribute>{
      std::make_tuple(1, db::aggregation::type::mean)};

  for (auto _ : state) {
    db::group_by_table res = db::group_by(*_table, 0, aggregates);
  }

#ifdef DEBUG_KEY_GEN
  write_column_to_file(0, state.name() + "_" + std::to_string(nrows) +
                              std::to_string(cardinality) + ".txt");
#endif
}

BENCHMARK_REGISTER_F(group_by_benchmark, uniform_mean)
    ->Args({1ul << 20, 1ll << 16} /* small data */)
    ->Args({group_by_benchmark::default_nrows, 4ll} /* low cardinality */)
    ->Args({group_by_benchmark::default_nrows,
            group_by_benchmark::default_cardinality} /* medium cardinality */)
    ->Args({group_by_benchmark::default_nrows,
            9ll * 1000ll * 1000ll} /* high cardinality */);

BENCHMARK_F(group_by_benchmark, uniform_mean_mean)
(::benchmark::State& state) {
  attribute_generator<db::attribute_type> generate_value;

  auto schema = std::vector<db::data_type::type>{
      db::data_type::to_type<db::attribute_type>(),
      db::data_type::to_type<db::attribute_type>(),
      db::data_type::to_type<db::key_type>()};
  _table = new_table(std::move(schema));

  for (size_t r = 0; r < default_nrows; ++r) {
    _table->push_back(
        2, absl::Uniform<db::key_type>(_bitgen, 0, default_cardinality));
    _table->push_back(0, generate_value());
    _table->push_back(1, generate_value());
  }

  std::vector<db::aggregation_attribute> aggregates = {
      std::make_tuple(0, db::aggregation::type::mean),
      std::make_tuple(1, db::aggregation::type::mean)};

  for (auto _ : state) {
    db::group_by_table res = db::group_by(*_table, 2, aggregates);
  }

#ifdef DEBUG_KEY_GEN
  write_column_to_file(2, state.name() + ".txt");
#endif
}

BENCHMARK_DEFINE_F(group_by_benchmark, uniform_width)
(::benchmark::State& state) {
  const int nattributes = state.range(0);
  // Retain a constant table size, and cardinality ratio
  const size_t nrows = default_nrows / (nattributes + 2);
  attribute_generator<db::attribute_type> generate_value;

  std::vector<db::data_type::type> schema;
  schema.reserve(nattributes + 1);
  schema.push_back(db::data_type::to_type<db::key_type>());  // Key

  for (int a = 0; a < nattributes; ++a) {
    schema.push_back(db::data_type::to_type<db::attribute_type>());  // Payloads
  }

  _table = new_table(std::move(schema));

  for (size_t r = 0; r < nrows; ++r) {
    _table->push_back(
        0, absl::Uniform<db::key_type>(_bitgen, 0, default_cardinality));

    for (int a = 1; a <= nattributes; ++a) {
      _table->push_back(a, generate_value());
    }
  }

  std::vector<db::aggregation_attribute> aggregates;
  aggregates.reserve(nattributes);

  for (int a = 1; a <= nattributes; ++a) {
    aggregates.emplace_back(a, db::aggregation::type::mean);
  }

  for (auto _ : state) {
    db::group_by_table res = db::group_by(*_table, 0, aggregates);
  }

#ifdef DEBUG_KEY_GEN
  write_column_to_file(
      0, state.name() + "_" + std::to_string(nattributes) + ".txt");
#endif
}

BENCHMARK_REGISTER_F(group_by_benchmark, uniform_width)
    ->Arg(0 /* distinct */)
    ->Arg(1 /* narrow */)
    ->Arg(11 /* wide */);

BENCHMARK_F(group_by_benchmark, heavy_hitter_mean)
(::benchmark::State& state) {
  const double heavy_hitter_probability = 0.5;
  attribute_generator<db::attribute_type> generate_value;

  std::vector<db::data_type::type> schema = {
      db::data_type::to_type<db::key_type>(),
      db::data_type::to_type<db::attribute_type>()};
  _table = new_table(std::move(schema));

  const db::key_type hitter =
      absl::Uniform<db::key_type>(_bitgen, 0, default_cardinality);

  for (size_t r = 0; r < default_nrows; ++r) {
    db::key_type key = -1;
    if (absl::Bernoulli(_bitgen, heavy_hitter_probability)) {
      key = hitter;
    } else {
      do {
        key = absl::Uniform<db::key_type>(_bitgen, 0, default_cardinality);
      } while (key == hitter);
    }

    _table->push_back(0, key);
    _table->push_back(1, generate_value());
  }

  std::vector<db::aggregation_attribute> aggregates = {
      std::make_tuple(1, db::aggregation::type::mean)};

  for (auto _ : state) {
    db::group_by_table res = db::group_by(*_table, 0, aggregates);
  }

#ifdef DEBUG_KEY_GEN
  write_column_to_file(0, state.name() + ".txt");
#endif
}

BENCHMARK_F(group_by_benchmark, sorted_mean)
(::benchmark::State& state) {
  // average run length should be nrows / cardinality; average value of uniform
  // distribution is (max - min) / 2; thus max must be 2 * average run length +
  // min;
  const size_t min_run_length = 1;
  const size_t max_run_length =
      (2ul * default_nrows) / default_cardinality + min_run_length;
  attribute_generator<db::attribute_type> generate_value;

  std::vector<db::data_type::type> schema = {
      db::data_type::to_type<db::key_type>(),
      db::data_type::to_type<db::attribute_type>()};
  _table = new_table(std::move(schema));

  size_t r = 0;
  db::key_type key = 0;
  do {
    size_t rl = absl::Uniform<size_t>(_bitgen, min_run_length, max_run_length);

    for (size_t i = r; i < r + rl; ++i) {
      _table->push_back(0, key);
      _table->push_back(1, generate_value());
    }

    key++;
    r += rl;
  } while (r <= default_nrows);

  std::vector<db::aggregation_attribute> aggregates = {
      std::make_tuple(1, db::aggregation::type::mean)};

  for (auto _ : state) {
    db::group_by_table res = db::group_by(*_table, 0, aggregates);
    assert(res.size() == key);
  }

#ifdef DEBUG_KEY_GEN
  write_column_to_file(0, state.name() + ".txt");
#endif
}

BENCHMARK_MAIN();
