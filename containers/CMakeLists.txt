﻿add_library(containers STATIC)
add_library(containers::containers ALIAS containers)

target_sources(containers PRIVATE
  "arena_memory_resource.cpp"
 "multi_arena_resource.cpp")
target_sources(containers PUBLIC
  FILE_SET HEADERS
  BASE_DIRS ../
  FILES
  "arena_memory_resource.hpp"
  "array.hpp"
  "memory_resource_allocator.hpp"
  "multi_arena_resource.hpp"
  "misc.hpp"
  "polymorphic_allocator.hpp"
  "span.hpp"
  "span_iterator.hpp"
  "stack_buffer_memory_resource.hpp"
  "vector.hpp"
)

target_compile_options(containers PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/FAs /Fa${CMAKE_BINARY_DIR}/asm\\>
)
