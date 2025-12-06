#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"

namespace aoc::y2025 {

std::string Day05::part1(const std::string& input) {
    auto lines = split(input, '\n');
    
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    size_t start_ids_idx = 0;

    // Parse the ranges
    for (size_t i = 0; i < lines.size(); ++i) {
        const auto& line = lines[i];
        if (line.empty()) {
            start_ids_idx = i + 1;
            break;
        }
        auto parts = split(line, '-');
        if (parts.size() == 2) {
            uint64_t start = std::stoull(parts[0]);
            uint64_t end = std::stoull(parts[1]);
            ranges.emplace_back(start, end);
        } else {
            start_ids_idx = i;
            break;
        }
    }

    // Sort and merge overlapping/adjacent ranges
    std::sort(ranges.begin(), ranges.end());
    std::vector<std::pair<uint64_t, uint64_t>> merged;

    for (const auto& [start, end] : ranges) {
        if (merged.empty()) {
            merged.emplace_back(start, end);
            continue;
        }

        auto& [m_start, m_end] = merged.back();
        if (start > m_end + 1) {
            merged.emplace_back(start, end);
        } else {
            m_end = std::max(m_end, end);
        }
    }

    uint64_t result = 0;
    for (size_t i = start_ids_idx; i < lines.size(); ++i) {
        const auto& line = lines[i];
        if (line.empty()) continue;
        
        uint64_t id = std::stoull(line);
        
        // Binary search for the range that might contain id
        size_t left = 0, right = merged.size();
        while (left < right) {
            size_t mid = left + (right - left) / 2;
            const auto& [m_start, m_end] = merged[mid];
            
            if (id < m_start) {
                right = mid;
            } else if (id > m_end) {
                left = mid + 1;
            } else {
                // id is within [m_start, m_end]
                result++;
                break;
            }
        }
    }
    
    return std::to_string(result);
}

std::string Day05::part2(const std::string& input) {
    auto lines = split(input, '\n');
    
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    size_t start_ids_idx = 0;

    // Parse the ranges
    for (size_t i = 0; i < lines.size(); ++i) {
        const auto& line = lines[i];
        if (line.empty()) {
            start_ids_idx = i + 1;
            break;
        }
        auto parts = split(line, '-');
        if (parts.size() == 2) {
            uint64_t start = std::stoull(parts[0]);
            uint64_t end = std::stoull(parts[1]);
            ranges.emplace_back(start, end);
        } else {
            start_ids_idx = i;
            break;
        }
    }

    // Sort and merge overlapping/adjacent ranges
    std::sort(ranges.begin(), ranges.end());
    std::vector<std::pair<uint64_t, uint64_t>> merged;

    for (const auto& [start, end] : ranges) {
        if (merged.empty()) {
            merged.emplace_back(start, end);
            continue;
        }

        auto& [m_start, m_end] = merged.back();
        if (start > m_end + 1) {
            merged.emplace_back(start, end);
        } else {
            m_end = std::max(m_end, end);
        }
    }

    uint64_t result = 0;
    for(const auto&[start, end]: merged){
        result += (end - start + 1);
    }

    
    return std::to_string(result);
}

REGISTER_SOLUTION(Day05)

} // namespace aoc::y2025