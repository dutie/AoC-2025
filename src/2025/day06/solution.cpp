#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <vector>
#include <algorithm>
#include <cctype>

namespace aoc::y2025 {

std::string Day06::part1(const std::string& input) {
    auto lines = split(input, '\n');
    
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    
    if (lines.empty()) return "0";
    
    size_t max_width = 0;
    for (const auto& line : lines) {
        max_width = std::max(max_width, line.size());
    }
    for (auto& line : lines) {
        line.resize(max_width, ' ');
    }
    
    std::vector<bool> is_separator(max_width, true);
    for (const auto& line : lines) {
        for (size_t col = 0; col < max_width; ++col) {
            if (line[col] != ' ') {
                is_separator[col] = false;
            }
        }
    }
    
    std::vector<std::pair<size_t, size_t>> problem_ranges; // [start, end)
    size_t start = 0;
    bool in_problem = false;
    
    for (size_t col = 0; col <= max_width; ++col) {
        bool sep = (col == max_width) || is_separator[col];
        if (!sep && !in_problem) {
            start = col;
            in_problem = true;
        } else if (sep && in_problem) {
            problem_ranges.emplace_back(start, col);
            in_problem = false;
        }
    }
    
    // operations
    const auto& op_line = lines.back();
    
    uint64_t grand_total = 0;
    
    for (const auto& [col_start, col_end] : problem_ranges) {
        char op = '+';
        for (size_t col = col_start; col < col_end; ++col) {
            if (op_line[col] == '*' || op_line[col] == '+') {
                op = op_line[col];
                break;
            }
        }
        
        uint64_t result = (op == '*') ? 1 : 0;
        
        for (size_t row = 0; row + 1 < lines.size(); ++row) {
            std::string num_str;
            for (size_t col = col_start; col < col_end; ++col) {
                char c = lines[row][col];
                if (std::isdigit(static_cast<unsigned char>(c))) {
                    num_str += c;
                }
            }
            
            if (!num_str.empty()) {
                uint64_t num = std::stoull(num_str);
                if (op == '*') {
                    result *= num;
                } else {
                    result += num;
                }
            }
        }
        
        grand_total += result;
    }
    
    return std::to_string(grand_total);
}

std::string Day06::part2(const std::string& input) {
    auto lines = split(input, '\n');
    
    while (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }
    
    if (lines.empty()) return "0";
    
    size_t max_width = 0;
    for (const auto& line : lines) {
        max_width = std::max(max_width, line.size());
    }
    for (auto& line : lines) {
        line.resize(max_width, ' ');
    }
    
    std::vector<bool> is_separator(max_width, true);
    for (const auto& line : lines) {
        for (size_t col = 0; col < max_width; ++col) {
            if (line[col] != ' ') {
                is_separator[col] = false;
            }
        }
    }
    
    std::vector<std::pair<size_t, size_t>> problem_ranges;
    size_t start = 0;
    bool in_problem = false;
    
    for (size_t col = 0; col <= max_width; ++col) {
        bool sep = (col == max_width) || is_separator[col];
        if (!sep && !in_problem) {
            start = col;
            in_problem = true;
        } else if (sep && in_problem) {
            problem_ranges.emplace_back(start, col);
            in_problem = false;
        }
    }
    
    const auto& op_line = lines.back();
    
    uint64_t grand_total = 0;
    
    for (const auto& [col_start, col_end] : problem_ranges) {
        char op = '+';
        for (size_t col = col_start; col < col_end; ++col) {
            if (op_line[col] == '*' || op_line[col] == '+') {
                op = op_line[col];
                break;
            }
        }
        
        uint64_t result = (op == '*') ? 1 : 0;
        
        for (size_t col = col_start; col < col_end; ++col) {
            std::string num_str;
            for (size_t row = 0; row + 1 < lines.size(); ++row) {
                char c = lines[row][col];
                if (std::isdigit(static_cast<unsigned char>(c))) {
                    num_str += c;
                }
            }
            
            if (!num_str.empty()) {
                uint64_t num = std::stoull(num_str);
                if (op == '*') {
                    result *= num;
                } else {
                    result += num;
                }
            }
        }
        
        grand_total += result;
    }
    
    return std::to_string(grand_total);
}

REGISTER_SOLUTION(Day06)

} // namespace aoc::y2025