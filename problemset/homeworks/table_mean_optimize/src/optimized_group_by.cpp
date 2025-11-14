#include <db/group_by.hpp>
#include <db/table.hpp>
#include <tuple>
#include <unordered_map>
#include <utility>

namespace db {

group_by_table optimized_group_by(
    const group_by_table& input_table, const int key_ref,
    const std::vector<std::tuple<int, aggregation::type>> aggregates) {
  /*
   * FIXME: Implement your optimized group by here!
   */

  return group_by_table(std::vector<db::data_type::type>{});
}

}  // namespace db
