#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <common/utils.h>

namespace aoc::y2025 {

std::string Day04::part1(const std::string& input) {
    std::vector<std::string> lines = split(input);

    int accessible_rolls = 0;
    
    // Iterate over each position in the grid and check if paper roll (@) is accessible (meaning fewer than 4 rolls of paper adjacent)
    for(int row_i = 0; row_i < lines.size(); row_i++){
        const std::string& line = lines[row_i];
        for(int col_i = 0; col_i < line.size(); col_i++){
            char c = line[col_i];
            // Is paper roll (@) or empty (.)
            if(c != '@' && c != '.'){
                continue;
            }
            if(c == '@'){
                // Paper roll found
                int adjacent_rolls = 0;
                // Check all 4 directions

                for(const Point& dir : DIRECTIONS_8){
                    int new_row = row_i + dir.y;
                    int new_col = col_i + dir.x;
                    if(new_row >= 0 && new_row < lines.size() &&
                       new_col >= 0 && new_col < line.size()){
                        if(lines[new_row][new_col] == '@'){
                            adjacent_rolls++;
                        }
                    }
                }
                // If fewer than 4 adjacent rolls, it's accessible
                if(adjacent_rolls < 4){
                    accessible_rolls++;
                }
            }
        }
    }
    
    return std::to_string(accessible_rolls);
}

std::string Day04::part2(const std::string& input) {
    std::vector<std::string> grid = split(input);
    int rows = grid.size();
    int cols = grid[0].size();
    
    // Count adjacent rolls for each cell
    auto count_adjacent = [&](int r, int c) {
        int count = 0;
        for (const Point& dir : DIRECTIONS_8) {
            int nr = r + dir.y, nc = c + dir.x;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < static_cast<int>(grid[nr].size()) 
                && grid[nr][nc] == '@') {
                count++;
            }
        }
        return count;
    };
    
    std::queue<std::pair<int, int>> queue;
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < static_cast<int>(grid[r].size()); c++) {
            if (grid[r][c] == '@' && count_adjacent(r, c) < 4) {
                queue.emplace(r, c);
            }
        }
    }
    
    int total_removed = 0;
    
    while (!queue.empty()) {
        auto [r, c] = queue.front();
        queue.pop();
        
        if (grid[r][c] != '@') continue;  // already removed or empty
        
        grid[r][c] = '.';
        total_removed++;
        
        // can we access more rolls check
        for (const Point& dir : DIRECTIONS_8) {
            int nr = r + dir.y, nc = c + dir.x;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < static_cast<int>(grid[nr].size()) 
                && grid[nr][nc] == '@' && count_adjacent(nr, nc) < 4) {
                queue.emplace(nr, nc);
            }
        }
    }
    
    return std::to_string(total_removed);
}

REGISTER_SOLUTION(Day04)

} // namespace aoc::y2025
