cmake_minimum_required(VERSION 3.20)
project(order_book)

# --- Compiler settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Main executable ---
add_executable(order_book src/main.cpp src/order_book.cpp src/matching_engine.cpp src/memory_pool.cpp src/spsc_queue.cpp src/simulator.cpp)
target_include_directories(order_book PRIVATE include)

# --- Google Test ---
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# --- Test binary ---
add_executable(tests
    tests/test_matching_engine.cpp
    src/order_book.cpp
    src/matching_engine.cpp
    src/memory_pool.cpp
)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests GTest::gtest_main)

# --- Benchmark binary ---
add_executable(benchmark src/benchmark.cpp src/order_book.cpp src/matching_engine.cpp src/memory_pool.cpp src/spsc_queue.cpp  )
target_include_directories(benchmark PRIVATE include)
