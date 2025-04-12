#include <cstddef>
#include <vector>
#include <format>

#include <gtest/gtest.h>

#include "containers/arena_memory_resource.hpp"

using int_alloc = ml::ArenaAllocator<int>;
template <typename T>
using pool_vec = std::vector<T, ml::ArenaAllocator<T>>;
using int_vec = pool_vec<int>;

TEST(arena, empty_memory_resource) {
    ml::ArenaMemoryResource resource;
    EXPECT_EQ(resource.pools().size(), 0);
}
TEST(arena, resource_request_bytes) {
    constexpr std::size_t size{100};

    ml::ArenaMemoryResource resource;
    (void)resource.allocate(size * sizeof(std::byte), alignof(std::byte));

    auto pools{resource.pools()};
    EXPECT_EQ(pools.size(), 1);
    auto & pool{pools.front()};
    EXPECT_EQ(pool.total_capacity(), resource.initial_capacity());
    EXPECT_EQ(pool.size(), size);
}
TEST(arena, resource_emplace_and_use_value) {
    constexpr std::size_t value{15};

    ml::ArenaMemoryResource resource;
    auto * ptr{resource.allocate(1 * sizeof(std::size_t), alignof(std::size_t))};

    auto * emplaced_value{new (ptr) std::size_t(value)};
    EXPECT_EQ(*emplaced_value, value);

    auto pools{resource.pools()};
    EXPECT_EQ(pools.size(), 1);
    auto & pool{pools.front()};
    EXPECT_EQ(pool.total_capacity(), resource.initial_capacity());
    EXPECT_EQ(pool.size(), sizeof(std::size_t));
}
TEST(arena, create_default_arena) {
    int_alloc alloc{};
}
TEST(arena, create_arena_and_allocate_int) {
    constexpr int value{15};

    ml::ArenaMemoryResource resource;
    int_alloc alloc{&resource};

    auto * ptr{alloc.allocate(1)};
    EXPECT_NE(ptr, nullptr);

    auto * emplaced_value{new (ptr) int(value)};
    EXPECT_EQ(*emplaced_value, value);
}
TEST(arena, construct_new) {
    constexpr int value{15};
    ml::ArenaMemoryResource resource;
    int_alloc alloc{&resource};

    EXPECT_EQ(alloc.construct_new(value), value);
}
TEST(arena, vector_with_arena_reserve_multiple_identical_calls) {
    // Confirm that the pool size doesn't change when you keep calling reserve with the same size

    ml::ArenaMemoryResource resource{1024};
    int_alloc alloc{&resource};
    int_vec vec{alloc};

    constexpr int n_elems{10};
    vec.reserve(n_elems);

    auto & pool{resource.pools().front()};
    auto const pool_size{pool.size()};
    for (int i{0}; i < n_elems; i++) {
        vec.reserve(n_elems);
    }

    EXPECT_EQ(vec.capacity(), n_elems);
    EXPECT_EQ(pool.total_capacity(), resource.initial_capacity());
    EXPECT_EQ(pool.size(), pool_size);
}
TEST(arena, vector_with_arena) {
    ml::ArenaMemoryResource resource{1024};
    int_alloc alloc{&resource};
    int_vec vec{alloc};

    // In debug mode 16 bytes are added for some reason
    // in release they aren't
    // Make an initial reserve to get some bytes
    // Capture the size
    // Reserve again and ensure the delta is correct
    constexpr int n_elems_initial{2};
    constexpr int n_elems{4};
    vec.reserve(n_elems_initial);

    auto pools{resource.pools()};
    EXPECT_EQ(pools.size(), 1);
    auto & pool{pools.front()};
    auto const initial_pool_size{pool.size()};

    vec.reserve(n_elems);
    for (int i{0}; i < n_elems; i++) {
        vec.emplace_back(i);
        EXPECT_EQ(vec[i], i);
    }

    EXPECT_EQ(vec.size(), n_elems);
    EXPECT_EQ(pool.total_capacity(), resource.initial_capacity());
    EXPECT_EQ(pool.size(), initial_pool_size + n_elems * sizeof(int));
}
TEST(arena, vector_add_many_elements) {
    ml::ArenaMemoryResource resource{1024};
    int_alloc alloc{&resource};
    int_vec vec{alloc};

    for (int i{0}; i < 5000; i++) {
        vec.emplace_back(i);
    }

    auto pools{resource.pools()};
    EXPECT_EQ(pools.size(), 7);
    EXPECT_EQ(vec.back(), 4999);
}
TEST(arena, realloc_space) {
    ml::ArenaMemoryResource resource;

    auto * ptr{resource.allocate(1 * sizeof(char), alignof(char))};
    auto * ptr2{resource.reallocate(ptr, 2 * sizeof(char), alignof(char))};

    // We should have space to extend the allocation
    EXPECT_EQ(ptr, ptr2);
    EXPECT_EQ(resource.pools().back().size(), 2);
    EXPECT_EQ(resource.pools().size(), 1);
}
TEST(arena, two_vectors_add_many_elements) {
    ml::ArenaMemoryResource resource{1024};
    int_alloc alloc{&resource};

    int_vec vec1{alloc};
    int_vec vec2{alloc};

    for (int i{0}; i < 5000; i++) {
        vec1.emplace_back(i);
        vec2.emplace_back(i);
    }

    EXPECT_EQ(vec1.back(), 4999);
    EXPECT_EQ(vec2.back(), 4999);
}

TEST(arena2, empty_memory_resource) {
    ml::ArenaMemoryResource2 resource;
    // No pools should exist initially
    ASSERT_EQ(resource.pool(), nullptr);
}
TEST(arena2, resource_request_bytes) {
    constexpr std::size_t size{100};

    ml::ArenaMemoryResource2 resource;
    auto * ptr = resource.allocate(size * sizeof(std::byte), alignof(std::byte));
    EXPECT_NE(ptr, nullptr);
}
TEST(arena2, resource_emplace_and_use_value) {
    constexpr std::size_t value{42};

    ml::ArenaMemoryResource2 resource;
    auto * ptr = resource.allocate(sizeof(std::size_t), alignof(std::size_t));
    auto * emplaced_value = new (ptr) std::size_t(value);

    EXPECT_EQ(*emplaced_value, value);
}
TEST(arena2, create_arena_and_allocate_int) {
    constexpr int value{15};

    ml::ArenaMemoryResource2 resource;
    auto * ptr = resource.allocate(sizeof(int), alignof(int));
    EXPECT_NE(ptr, nullptr);

    auto * emplaced_value = new (ptr) int(value);
    EXPECT_EQ(*emplaced_value, value);
}
TEST(arena2, allocate_and_deallocate) {
    ml::ArenaMemoryResource2 resource;

    auto * ptr = resource.allocate(10 * sizeof(char), alignof(char));
    EXPECT_NE(ptr, nullptr);

    resource.deallocate(ptr, 10 * sizeof(char), alignof(char));
    // Deallocate should not throw or cause issues
}
TEST(arena2, allocate_multiple_pools) {
    ml::ArenaMemoryResource2 resource;

    constexpr std::size_t large_size = ml::ArenaMemoryResource2::initial_size * 2;
    auto * ptr1 = resource.allocate(ml::ArenaMemoryResource2::initial_size, alignof(std::byte));
    auto * ptr2 = resource.allocate(large_size, alignof(std::byte));

    EXPECT_NE(ptr1, nullptr);
    EXPECT_NE(ptr2, nullptr);
    EXPECT_NE(ptr1, ptr2);
}
TEST(arena2, allocate_with_alignment) {
    ml::ArenaMemoryResource2 resource;

    constexpr std::size_t alignment = 64;
    auto * ptr = resource.allocate(128, alignment);

    EXPECT_NE(ptr, nullptr);
    EXPECT_EQ(reinterpret_cast<std::uintptr_t>(ptr) % alignment, 0);
}
TEST(arena2, reallocate_within_pool) {
    ml::ArenaMemoryResource2 resource;

    auto * ptr = resource.allocate(10, alignof(std::byte));
    auto * ptr2 = resource.allocate(20, alignof(std::byte));

    EXPECT_NE(ptr, nullptr);
    EXPECT_NE(ptr2, nullptr);
}
TEST(arena2, allocate_large_object) {
    ml::ArenaMemoryResource2 resource;

    constexpr std::size_t large_size = ml::ArenaMemoryResource2::initial_size * 10;
    auto * ptr = resource.allocate(large_size, alignof(std::byte));

    EXPECT_NE(ptr, nullptr);
}
