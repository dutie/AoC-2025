cmake_minimum_required(VERSION 3.14)

# Common utilities library
add_library(aoc_common STATIC
    utils.cpp
    runner.cpp
)

target_include_directories(aoc_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)
