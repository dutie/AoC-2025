#include "solution.h"
#include "common/utils.h"
#include "common/runner.h"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>

namespace aoc{{YEAR}}::day{{DAY_PADDED}} {

// Parse input and prepare data structures
void parse_input(const std::string& input) {
    // TODO: Parse the input
    auto lines = split(input, '\n');
    
    for (const auto& line : lines) {
        if (line.empty()) continue;
        
        // Process each line
    }
}

std::string part1(const std::string& input) {
    // TODO: Implement part 1
    parse_input(input);
    
    int result = 0;
    
    return std::to_string(result);
}

std::string part2(const std::string& input) {
    // TODO: Implement part 2
    parse_input(input);
    
    int result = 0;
    
    return std::to_string(result);
}

} // namespace aoc{{YEAR}}::day{{DAY_PADDED}}

// Register this solution
REGISTER_SOLUTION({{YEAR}}, {{DAY}}, aoc{{YEAR}}::day{{DAY_PADDED}})
