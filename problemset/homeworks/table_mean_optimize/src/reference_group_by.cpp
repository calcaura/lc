#include <db/group_by.hpp>
#include <db/table.hpp>
#include <unordered_map>
#include <utility>

namespace db {

group_by_table aggregate_attribute(const group_by_table& input_table,
                                   const int key_ref, const int attr_ref,
                                   const aggregation::type agg_function) {
  std::unordered_multimap<key_type, attribute_type> grouping;
  grouping.reserve(input_table.size());

  auto kit = input_table.cbegin<key_type>(key_ref);
  auto ait = input_table.cbegin<attribute_type>(attr_ref);
  for (; kit != input_table.cend<key_type>(key_ref); ++kit, ++ait) {
    grouping.insert({*kit, *ait});
  }

  group_by_table aggregated({input_table.type(attr_ref)});
  auto it = grouping.cbegin();

  while (it != grouping.cend()) {
    key_type current_key = it->first;
    attribute_type aggregate = 0;
    size_t bucket_size = 0;

    do {
      switch (agg_function) {
        case aggregation::type::any_value:
          aggregate = it->second;
          break;
        case aggregation::mean:
          bucket_size++;
          aggregate += it->second;
          break;
      }

      ++it;
    } while (it != grouping.cend() && it->first == current_key);

    if (agg_function == aggregation::mean) {
      aggregate /= static_cast<attribute_type>(bucket_size);
    }

    aggregated.push_back(0, aggregate);
  }

  return aggregated;
}

group_by_table reference_group_by(
    const group_by_table& input_table, const int key_ref,
    const std::vector<aggregation_attribute> aggregates) {
  group_by_table result_table(std::vector<db::data_type::type>{});

  result_table.horizontal_concatinate(aggregate_attribute(
      input_table, key_ref, key_ref, aggregation::type::any_value));

  for (auto attribute : aggregates) {
    const auto [attr_ref, agg_function] = attribute;

    result_table.horizontal_concatinate(
        aggregate_attribute(input_table, key_ref, attr_ref, agg_function));
  }

  return result_table;
}

}  // namespace db
