#pragma once

#include <cstdint>
#include <db/table.hpp>
#include <utility>
#include <vector>

namespace db {

namespace aggregation {

/*
 * @brief Aggregation functions supported by group by.
 *
 * `any_value` returns an arbitrary value from the input values in a group.
 * `mean` returns the mean of the input values in a group.
 */
enum type { any_value, mean };

}  // namespace aggregation

/*
 * @brief The data type of the grouping key.
 */
using key_type = int32_t;

/*
 * @brief The data type of the aggregation attributes.
 */
using attribute_type = int32_t;

/*
 * @brief The table type used in group by.
 *
 * DO: Change the table type to a custom implementation.
 */
using group_by_table = db::block_table;

/*
 * @brief A type alias for group by's aggregation type argument.
 */
using aggregation_attribute = std::tuple<int, aggregation::type>;

/*
 * @brief Returns a grouped aggregation over the table.
 *
 * DON'T: Change the group by API.
 */
group_by_table reference_group_by(
    const group_by_table& base_table, const int key_col_ref,
    const std::vector<aggregation_attribute> aggregates);

/*
 * @brief Returns a grouped aggregation over the table.
 *
 * DON'T: Change the group by API.
 */
group_by_table optimized_group_by(
    const group_by_table& base_table, const int key_col_ref,
    const std::vector<std::tuple<int, aggregation::type>> aggregates);

/*
 * @brief Alias to switch the group by implementation.
 *
 * DO: Switch to `optimized_group_by`.
 */
constexpr auto group_by = reference_group_by;

}  // namespace db
