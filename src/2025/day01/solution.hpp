#pragma once

#include "common/solution.hpp"

namespace aoc::y2025 {

class Day01 : public Solution {
public:
    std::string part1(const std::string& input) override;
    std::string part2(const std::string& input) override;
    
    int year() const override { return 2025; }
    int day() const override { return 1; }
};

} // namespace aoc::y2025
