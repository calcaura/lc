#include <gtest/gtest.h>

#include <db/group_by.hpp>
#include <db/table.hpp>

#include <vector>

static const int sampleKeyIndex {0};
static const int sampleAttributeIndex {1};

/*
 * @brief Construct a sample input table with two columns and populate it with 3 rows
 */
db::group_by_table get_sample_input() {
    db::group_by_table table({db::data_type::type::db_int32, db::data_type::type::db_int32});
    for (auto [key, attr_val] : std::vector<std::pair<int, int>>({{1, 2}, {4, 5}, {9, 8}})) {
        table.push_back(sampleKeyIndex, key);
        table.push_back(sampleAttributeIndex, attr_val);
    }
    return table;
}

TEST(tests, trivial_test) {
    db::group_by_table input = get_sample_input();

    // Compute mean on the second attribute
    db::group_by_table result = db::group_by(input, sampleKeyIndex, std::vector<db::aggregation_attribute>{{sampleAttributeIndex, db::aggregation::mean}});

    // Sanity check: Validate that result table is of correct shape and non-empty
    EXPECT_EQ(result.columns(), 2);
    EXPECT_EQ(result.type(sampleKeyIndex), db::data_type::type::db_int32);
    EXPECT_EQ(result.type(sampleAttributeIndex), db::data_type::type::db_int32);
    EXPECT_GT(result.size(), 0);
}

TEST(tests, my_own_test) {
    // Feel free to create as many test as required to ensure your optimized code outputs correct results.
    // You can also use other test methodologies, as appropriate.
    // Tests will not be considered for grading the submitted solution.

    // Overview of available googletest assertions:
    // https://google.github.io/googletest/reference/assertions.html
    // For example:
    // EXPECT_EQ(actual_value, expected_value);
    // EXPECT_TRUE(assertion);

    // You can manually see the output by using the overloaded << operator to print a table to stdout
    // db::group_by_table result
    // std::cout << result << "\n";
}