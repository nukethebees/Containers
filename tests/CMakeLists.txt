enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(tests)
add_executable(containers::tests ALIAS tests)
target_sources(tests PRIVATE
    test_span.cpp
    test_array.cpp
    test_vector.cpp
    test_arena_allocator.cpp
)
target_compile_options(tests PRIVATE 
    /FAs 
    /Fa${CMAKE_BINARY_DIR}/asm\\
)
target_link_libraries(tests PRIVATE 
    GTest::gtest_main 
    containers::containers
)
target_include_directories(tests PRIVATE
    containers::containers
)