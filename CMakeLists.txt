cmake_minimum_required(VERSION 3.20)
project(order_book)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(order_book src/main.cpp src/order_book.cpp src/matching_engine.cpp)
target_include_directories(order_book PRIVATE include)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
    tests/test_matching_engine.cpp
    src/order_book.cpp
    src/matching_engine.cpp
)
target_include_directories(tests PRIVATE include)
target_link_libraries(tests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)

