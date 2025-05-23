#include <algorithm>
#include <numeric>
#include <string>

#include <gtest/gtest.h>

#include "containers/linked_vector.hpp"

#include "configure_warning_pragmas.hpp"

// Construction
TEST(linked_vector, init_empty_int_vector) {
    ml::linked_vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}

// Iteration
TEST(linked_vector, range_for_sum) {
    ml::linked_vector<int> values;

    values.reserve(5);
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }

    int sum = 0;
    for (auto const& value : values) {
        sum += value;
    }

    EXPECT_EQ(sum, 10);
}
TEST(linked_vector, const_reverse_iter_sum) {
    ml::linked_vector<int> values;
    values.reserve(5);
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    int sum = 0;
    for (auto it = values.crbegin(); it != values.crend(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 10);
}
TEST(linked_vector, accumulate_fwd) {
    ml::linked_vector<int> values;
    values.reserve(5);
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    int sum = std::accumulate(values.begin(), values.end(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(linked_vector, accumulate_rev) {
    ml::linked_vector<int> values;
    values.reserve(5);
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    int sum = std::accumulate(values.rbegin(), values.rend(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(linked_vector, accumulate_const) {
    ml::linked_vector<int> values;
    values.reserve(5);
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    int sum = std::accumulate(values.cbegin(), values.cend(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(linked_vector, accumulate_const_reverse) {
    ml::linked_vector<int> values;
    values.reserve(5);
    for (int i = 0; i < 5; ++i) {
        values.push_back(i);
    }
    int sum = std::accumulate(values.crbegin(), values.crend(), 0);
    EXPECT_EQ(sum, 10);
}
TEST(linked_vector, iter_sum_empty) {
    ml::linked_vector<int> values;
    values.reserve(5);
    int sum = 0;
    for (auto it = values.begin(); it != values.end(); ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 0);
}
TEST(linked_vector, iter_sum_reverse_empty) {
    ml::linked_vector<int> values;
    values.reserve(5);
    int sum = 0;
    auto rend{values.rend()};
    for (auto it = values.rbegin(); it != rend; ++it) {
        sum += *it;
    }
    EXPECT_EQ(sum, 0);
}

// Modification
TEST(linked_vector, reserve_one_elem) {
    ml::linked_vector<int> values;
    values.reserve(1);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, push_back_int) {
    ml::linked_vector<int> values;
    values.reserve(1);
    values.push_back(42);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
TEST(linked_vector, clear_int) {
    ml::linked_vector<int> values;

    values.reserve(1);
    values.push_back(42);
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());

    values.clear();
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, push_back_string) {
    ml::linked_vector<std::string> values;
    values.reserve(1);
    values.push_back("Hello");
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
TEST(linked_vector, emplace_string) {
    ml::linked_vector<std::string> values;
    values.reserve(1);
    values.emplace_back("Hello");
    EXPECT_EQ(values.capacity(), 1);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());
}
TEST(linked_vector, pop_int) {
    ml::linked_vector<int> values;

    values.reserve(1);
    values.push_back(42);
    EXPECT_EQ(values.size(), 1);
    EXPECT_FALSE(values.empty());

    values.pop_back();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, pop_back_into_previous_segment) {
    ml::linked_vector<int> values;

    // Create first segment
    values.reserve(1);
    values.push_back(1);

    // Create a second segment
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);

    values.pop_back();
    values.pop_back();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}
TEST(linked_vector, add_elems_after_clear_2segs) {
    ml::linked_vector<int> values;

    // Create first segment
    values.reserve(1);
    values.push_back(1);

    // Create a second segment
    values.push_back(2);
    EXPECT_EQ(values.size(), 2);

    values.clear();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
    EXPECT_EQ(values.capacity(), 2);

    // Add elements back
    values.push_back(0);
    values.push_back(1);
    EXPECT_EQ(values.size(), 2);
}
