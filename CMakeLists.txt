cmake_minimum_required(VERSION 3.14)
project(AdventOfCode VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra")

# Collect all solution sources
file(GLOB_RECURSE SOLUTION_SOURCES "src/20*/day*/solution.cpp")

# Main executable
add_executable(aoc 
    src/main.cpp
    src/common/utils.cpp
    ${SOLUTION_SOURCES}
)

target_include_directories(aoc PRIVATE ${CMAKE_SOURCE_DIR}/src)

# Set data directory path
target_compile_definitions(aoc PRIVATE 
    DATA_DIR="${CMAKE_SOURCE_DIR}/data"
)
