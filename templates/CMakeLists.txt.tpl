# Day {{DAY}} solution
add_library(aoc_{{YEAR}}_day{{DAY_PADDED}} STATIC
    solution.cpp
)

target_link_libraries(aoc_{{YEAR}}_day{{DAY_PADDED}} PUBLIC aoc_common)

# Test executable
if(BUILD_TESTS)
    add_executable(test_{{YEAR}}_{{DAY_PADDED}} test.cpp)
    target_link_libraries(test_{{YEAR}}_{{DAY_PADDED}} 
        aoc_{{YEAR}}_day{{DAY_PADDED}}
        aoc_common
        gtest_main
    )
    
    add_test(NAME test_{{YEAR}}_{{DAY_PADDED}} COMMAND test_{{YEAR}}_{{DAY_PADDED}})
endif()
