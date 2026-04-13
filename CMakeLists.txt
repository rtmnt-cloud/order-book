cmake_minimum_required(VERSION 3.20)
project(order_book)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(order_book src/main.cpp)
target_include_directories(order_book PRIVATE include)

