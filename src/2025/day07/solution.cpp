#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <queue>
#include <set>

namespace aoc::y2025 {

std::string Day07::part1(const std::string& input) {
    auto lines = split(input);

    uint64_t result = 0;

    char free_space = '.';
    char splitter = '^';
    char start = 'S';
    char beam = '|';

    std::set<std::pair<int, int>> visited_splitters;
    int width = lines[0].size();
    int height = lines.size();
    std::queue<std::pair<int, int>> beam_positions;
    // Find start position
    for (int x = 0; x < width; ++x) {
        if (lines[0][x] == start) {
            beam_positions.emplace(x, 0);
            continue;
        }
    }
    while (!beam_positions.empty()) {
        auto [x, y] = beam_positions.front();
        beam_positions.pop();

        // Move down until we hit something
        while (y + 1 < height) {
            y++;
            char current = lines[y][x];
            if (current == free_space || current == beam) {
                // Continue moving down
                continue;
            } else if (current == splitter) {
                // Record splitter and branch left and right
                auto vis_em = visited_splitters.emplace(x, y);
                if(!vis_em.second){
                    // Already visited this splitter
                    break;
                }
                if (x - 1 >= 0) {
                    beam_positions.emplace(x - 1, y);
                }
                if (x + 1 < width) {
                    beam_positions.emplace(x + 1, y);
                }
                break;
            } else {
                break;
            }
        }
    }
    
    
    return std::to_string(visited_splitters.size());
}

// DFS with memoization
uint64_t countTimeLines(int x, int y, std::map<std::pair<int, int>, int64_t>& memo, const std::vector<std::string>& lines, int width, int height) {
    auto key = std::make_pair(x, y);

    // Check memoization
    if (memo.find(key) != memo.end()) {
        return memo[key];
    }

    // Move down until we hit something
    while (y + 1 < height) {
        y++;
        char current = lines[y][x];
        if (current == '.' || current == '|') {
            // Continue moving down
            continue;
        } else if (current == '^') {
            // Record splitter and branch left and right
            uint64_t left_time = 0;
            uint64_t right_time = 0;
            if (x - 1 >= 0) {
                left_time = countTimeLines(x - 1, y, memo, lines, width, height);
            }
            if (x + 1 < width) {
                right_time = countTimeLines(x + 1, y, memo, lines, width, height);
            }
            memo[key] = left_time + right_time;
            return memo[key];
        } else {
            memo[key] = 0;
            return 0;
        }
    }
    return 1;
}

std::string Day07::part2(const std::string& input) {
    auto lines = split(input);

    uint64_t result = 0;

    char free_space = '.';
    char splitter = '^';
    char start = 'S';
    char beam = '|';

    int width = lines[0].size();
    int height = lines.size();
    int start_x = 0;
    // Find start position
    for (int x = 0; x < width; ++x) {
        if (lines[0][x] == start) {
            start_x = x;
            break;
        }
    }
    std::map<std::pair<int, int>, int64_t> memo;
    uint64_t max_time = 0;
    uint64_t time = countTimeLines(start_x, 0, memo, lines, width, height);


    
    return std::to_string(time);
}

REGISTER_SOLUTION(Day07)

} // namespace aoc::y2025
