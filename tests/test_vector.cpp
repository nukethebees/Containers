#include <cstddef>
#include <numeric>

#include <gtest/gtest.h>

#include "containers/vector.hpp"

TEST(vector, init_empty_int_vector) {
    ml::vector<int> values;
    EXPECT_EQ(values.capacity(), 0);
    EXPECT_EQ(values.size(), 0);
}
TEST(vector, emplace_single) {
    ml::vector<int> values;
    values.emplace_back(1);
    EXPECT_EQ(values[0], 1);
    EXPECT_EQ(values.size(), 1);
}
TEST(vector, reserve) {
    ml::vector<int> values;
    values.reserve(5);
    EXPECT_EQ(values.capacity(), 5);
}

TEST(vector, front) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    EXPECT_EQ(values.front(), 10);
}

TEST(vector, back) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    EXPECT_EQ(values.back(), 20);
}

TEST(vector, clear) {
    ml::vector<int> values;
    values.emplace_back(10);
    values.emplace_back(20);
    values.clear();
    EXPECT_EQ(values.size(), 0);
    EXPECT_TRUE(values.empty());
}

TEST(vector, empty) {
    ml::vector<int> values;
    EXPECT_TRUE(values.empty());
    values.emplace_back(10);
    EXPECT_FALSE(values.empty());
}

TEST(vector, size) {
    ml::vector<int> values;
    EXPECT_EQ(values.size(), 0);
    values.emplace_back(10);
    EXPECT_EQ(values.size(), 1);
    values.emplace_back(20);
    EXPECT_EQ(values.size(), 2);
}

TEST(vector, range_based_for) {
    ml::vector<int> values;
    values.emplace_back(1);
    values.emplace_back(2);
    values.emplace_back(3);

    int sum = 0;
    for (const auto& value : values) {
        sum += value;
    }

    EXPECT_EQ(sum, 6);
}