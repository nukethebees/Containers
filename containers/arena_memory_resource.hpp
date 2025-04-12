﻿#pragma once

#include <compare>
#include <cstddef>
#include <memory>
#include <new>
#include <span>
#include <stdexcept>
#include <vector>

#include "memory_resource_allocator.hpp"

namespace ml {
class ArenaMemoryResource {
public:
    class Pool {
    private:
        std::unique_ptr<std::byte[]> buffer{nullptr};
        std::size_t total_capacity_{0};
        std::size_t remaining_capacity_{0};
        void * last_allocation_{nullptr};
    public:
        Pool() = delete;
        explicit Pool(std::size_t capacity);

        auto total_capacity() const->std::size_t;
        auto remaining_capacity() const->std::size_t;
        auto size() const->std::size_t;

        [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void *;
        [[nodiscard]] auto reallocate(void * alloc, std::size_t new_size_bytes, std::size_t alignment) -> void *;
    };
private:
    std::vector<Pool> pools_;
    std::size_t initial_capacity_{1024};
public:
    static constexpr std::size_t growth_factor{2};

    ArenaMemoryResource() noexcept = default;
    ArenaMemoryResource(std::size_t initial_capacity) noexcept;

    auto get_sufficient_pool(std::size_t n_bytes) -> Pool &;
    [[nodiscard]] auto allocate(std::size_t n_bytes, std::size_t alignment) -> void *;
    [[nodiscard]] auto reallocate(void * alloc, std::size_t n_bytes, std::size_t alignment) -> void *;
    auto initial_capacity() const->std::size_t;

    template <typename Self>
    auto pools(this Self && self) {
        return std::span<Pool const>(std::forward<Self>(self).pools_);
    }
};

template <typename T>
using ArenaAllocator = MemoryResourceAllocator<T, ArenaMemoryResource>;

class Pool2 {
    Pool2 * next_pool{nullptr};
    std::byte * buffer{nullptr}; // Non-owning pointer to the buffer
    std::size_t total_capacity_{0};
    std::size_t remaining_capacity_{0};
public:
    Pool2(std::byte * buffer, std::size_t capacity)
        : buffer{buffer}
        , total_capacity_{capacity}
        , remaining_capacity_{capacity} {}
    ~Pool2() {
        if (next_pool) {
            next_pool->~Pool2();
            delete[] reinterpret_cast<std::byte *>(next_pool);
        }
    }

    static auto create_pool(std::size_t initial_size) -> Pool2 * {
        std::size_t const bytes_needed{initial_size + sizeof(Pool2)};
        auto * buffer{new std::byte[bytes_needed]};
        return new (buffer) Pool2(buffer + sizeof(Pool2), initial_size);
    }
};

class ArenaMemoryResource2 {
public:
    static constexpr std::size_t initial_size{1024};
private:
    Pool2 * pool{nullptr};
public:
    ArenaMemoryResource2() = default;
    ~ArenaMemoryResource2() {
        if (pool) {
            pool->~Pool2();
            delete[] reinterpret_cast<std::byte *>(pool);
        }
    }

    ArenaMemoryResource2(ArenaMemoryResource2 const &) = delete;
    ArenaMemoryResource2(ArenaMemoryResource2 && other)
        : pool{other.pool} {
        other.pool = nullptr;
    }
    auto operator=(ArenaMemoryResource2 const &)->ArenaMemoryResource2 & = delete;
    auto operator=(ArenaMemoryResource2 && other) -> ArenaMemoryResource2 & {
        if (this != &other) {
            if (pool) {
                pool->~Pool2();
                delete[] reinterpret_cast<std::byte *>(pool);
            }
            pool = other.pool;
            other.pool = nullptr;
        }
        return *this;
    }
private:
    void create_pool() {
        pool = Pool2::create_pool(initial_size);
    }
};
}