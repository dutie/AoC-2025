#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"

namespace aoc::y2025 {

std::string Day03::part1(const std::string& input) {
    auto lines = split(input);
    uint64_t result = 0;

    for (const auto& line : lines) {
        if (line.empty()) continue;
        int n = line.size();
        // suffix_max[i] = largest digit from position i to end
        std::vector<int> suffix_max(n);
        suffix_max[n - 1] = line[n - 1] - '0';
        for (int i = n - 2; i >= 0; --i) {
            suffix_max[i] = std::max(line[i] - '0', suffix_max[i + 1]);
        }
        int best = 0;
        for (int i = 0; i < n - 1; ++i) {
            int tens = line[i] - '0';
            int ones = suffix_max[i + 1];
            best = std::max(best, tens * 10 + ones);
        }
        result += best;
    }
    return std::to_string(result);
}

std::string Day03::part2(const std::string& input) {
    auto lines = split(input);
    uint64_t result = 0;

    for (const auto& line : lines) {
        if (line.empty()) continue;
        
        int n = line.size();
        constexpr int k = 12;
        int to_skip = n - k;
        
        std::string stack;
        // stack.reserve(n);
        
        for (char c : line) {
            while (!stack.empty() && to_skip > 0 && stack.back() < c) {
                stack.pop_back();
                to_skip--;
            }
            stack.push_back(c);
        }
        
        stack.resize(k);  // geen idee of ik er maar 12 heb - keep the first k chars
        result += std::stoull(stack);
    }
    
    return std::to_string(result);
}

REGISTER_SOLUTION(Day03)

} // namespace aoc::y2025
