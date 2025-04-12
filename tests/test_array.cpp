#include <cstddef>
#include <numeric>
#include <algorithm>

#include <gtest/gtest.h>

#include "containers/array.hpp"

TEST(array, init_empty_int_array) {
    constexpr std::size_t size{3};
    ml::array<int, size> values;
    ASSERT_EQ(values.size(), size);
}
TEST(array, init_int_array) {
    constexpr std::size_t size{3};
    ml::array<int, size> values{1, 2, 3};
    ASSERT_EQ(values.size(), size);
}
TEST(array, accumulate_ints) {
    ml::array<int, 3> values{1, 2, 3};

    auto sum{std::accumulate(values.begin(), values.end(), 0)};

    ASSERT_EQ(sum, 6);
}
TEST(array, constexpr_indexing) {
    constexpr std::size_t size{3};
    constexpr ml::array<int, size> values{1, 2, 3};
    constexpr auto val{values[0]};

    ASSERT_EQ(val, 1);
}
TEST(array, at) {
    ml::array<int, 3> values{1, 2, 3};
    ASSERT_EQ(values.at(0), 1);
}

TEST(array, front) {
    ml::array<int, 3> values{1, 2, 3};
    ASSERT_EQ(values.front(), 1);
}

TEST(array, back) {
    ml::array<int, 3> values{1, 2, 3};
    ASSERT_EQ(values.back(), 3);
}

TEST(array, empty) {
    ml::array<int, 0> empty_array;
    ASSERT_TRUE(empty_array.empty());

    ml::array<int, 3> non_empty_array{1, 2, 3};
    ASSERT_FALSE(non_empty_array.empty());
}

TEST(array, size_bytes) {
    ml::array<int, 3> values{1, 2, 3};
    ASSERT_EQ(values.size_bytes(), 3 * sizeof(int));
}

TEST(array, iterators) {
    ml::array<int, 3> values{1, 2, 3};
    auto it = values.begin();
    ASSERT_EQ(*it, 1);
    ++it;
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_EQ(*it, 3);
    ++it;
    ASSERT_EQ(it, values.end());
}

TEST(array, const_iterators) {
    const ml::array<int, 3> values{1, 2, 3};
    auto it = values.cbegin();
    ASSERT_EQ(*it, 1);
    ++it;
    ASSERT_EQ(*it, 2);
    ++it;
    ASSERT_EQ(*it, 3);
    ++it;
    ASSERT_EQ(it, values.cend());
}

TEST(array, range_based_for) {
    ml::array<int, 3> values{1, 2, 3};
    int expected_sum = 6;
    int sum = 0;

    for (const auto& value : values) {
        sum += value;
    }

    ASSERT_EQ(sum, expected_sum);
}

TEST(array, find_element) {
    ml::array<int, 5> values{10, 20, 30, 40, 50};
    auto it = std::find(values.begin(), values.end(), 30);
    ASSERT_NE(it, values.end());
    ASSERT_EQ(*it, 30);

    it = std::find(values.begin(), values.end(), 100);
    ASSERT_EQ(it, values.end());
}

TEST(array, sort_elements) {
    ml::array<int, 5> values{50, 10, 40, 20, 30};
    std::sort(values.begin(), values.end());
    ASSERT_EQ(values[0], 10);
    ASSERT_EQ(values[1], 20);
    ASSERT_EQ(values[2], 30);
    ASSERT_EQ(values[3], 40);
    ASSERT_EQ(values[4], 50);
}

TEST(array, reverse_elements) {
    ml::array<int, 5> values{10, 20, 30, 40, 50};
    std::reverse(values.begin(), values.end());
    ASSERT_EQ(values[0], 50);
    ASSERT_EQ(values[1], 40);
    ASSERT_EQ(values[2], 30);
    ASSERT_EQ(values[3], 20);
    ASSERT_EQ(values[4], 10);
}
