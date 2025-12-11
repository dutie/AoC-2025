#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <vector>
#include <utility>
#include <algorithm>
#include <set>

namespace aoc::y2025
{

std::string Day09::part1(const std::string &input) {
    auto lines = split(input);
    const size_t n = lines.size();
    
    std::vector<uint64_t> x(n), y(n);
    
    for (size_t i = 0; i < n; i++) {
        auto vals = split(lines[i], ',');
        x[i] = std::stoull(vals[0]);
        y[i] = std::stoull(vals[1]);
    }
    
    uint64_t max_area = 0;
    
    for (size_t i = 0; i < n; i++) {
        uint64_t xi = x[i], yi = y[i];
        for (size_t j = i + 1; j < n; j++) {
            uint64_t xj = x[j], yj = y[j];
            
            uint64_t dx = (xi > xj ? xi - xj : xj - xi) + 1;
            uint64_t dy = (yi > yj ? yi - yj : yj - yi) + 1;
            
            uint64_t area = dx * dy;
            max_area = (area > max_area) ? area : max_area;
        }
    }
    
    return std::to_string(max_area);
}

const char RED_TILE = '#';
const char EMPTY_TILE = '.';

struct point {
    int64_t x;
    int64_t y;
};

int get_index(const std::vector<uint64_t>& coordinates, uint64_t coordinate) {
    return int(std::lower_bound(coordinates.begin(), coordinates.end(), coordinate) - coordinates.begin());
}

void dfs(std::vector<std::vector<char>>& grid, size_t x, size_t y, size_t n) {
    if (x >= n || y >= n) return;
    if (grid[x][y] == RED_TILE) return;

    grid[x][y] = RED_TILE;

    if (x + 1 < n) dfs(grid, x + 1, y, n);
    if (x > 0)     dfs(grid, x - 1, y, n);
    if (y + 1 < n) dfs(grid, x, y + 1, n);
    if (y > 0)     dfs(grid, x, y - 1, n);
}

std::string Day09::part2(const std::string &input)
{
    auto lines = split(input);
    const size_t n = lines.size();
    
    std::vector<point> points(n);
    std::vector<uint64_t> x(n), y(n);
    
    for (size_t i = 0; i < n; i++) {
        auto vals = split(lines[i], ',');
        points[i].x = std::stoull(vals[0]);
        points[i].y = std::stoull(vals[1]);
        x[i] = points[i].x;
        y[i] = points[i].y;
    }

    std::vector<uint64_t> sorted_x = x, sorted_y = y;
    sort(sorted_x.begin(), sorted_x.end());
    sort(sorted_y.begin(), sorted_y.end());

    // Step 1: Create grid with compressed coordinates
    std::vector<std::vector<char>> grid(n, std::vector<char>(n, EMPTY_TILE));
    
    // Step 2: Rasterize polygon edges
    for (size_t i = 0; i < n; ++i) {
        size_t x_idx = get_index(sorted_x, points[i].x);
        size_t y_idx = get_index(sorted_y, points[i].y);

        size_t x_next_idx = get_index(sorted_x, points[(i + 1) % n].x);
        size_t y_next_idx = get_index(sorted_y, points[(i + 1) % n].y);

        if (x_idx == x_next_idx) {
            // Vertical edge
            size_t y_lo = std::min(y_idx, y_next_idx);
            size_t y_hi = std::max(y_idx, y_next_idx);
            for (size_t yp = y_lo; yp <= y_hi; ++yp) {
                grid[x_idx][yp] = RED_TILE;
            }
        } else if (y_idx == y_next_idx) {
            // Horizontal edge
            size_t x_lo = std::min(x_idx, x_next_idx);
            size_t x_hi = std::max(x_idx, x_next_idx);
            for (size_t xp = x_lo; xp <= x_hi; ++xp) {
                grid[xp][y_idx] = RED_TILE;
            }
        }
    }

    // Step 3: Flood fill interior
    dfs(grid, n / 2, n / 2, n);

    // Step 4: compute prefix sums -> gives O(1) range queries
    std::vector<std::vector<int>> row_prefix(n, std::vector<int>(n + 1, 0));
    std::vector<std::vector<int>> col_prefix(n + 1, std::vector<int>(n, 0));
    
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            int is_red = (grid[i][j] == RED_TILE) ? 1 : 0;
            row_prefix[i][j + 1] = row_prefix[i][j] + is_red;
        }
    }
    for (size_t j = 0; j < n; ++j) {
        for (size_t i = 0; i < n; ++i) {
            int is_red = (grid[i][j] == RED_TILE) ? 1 : 0;
            col_prefix[i + 1][j] = col_prefix[i][j] + is_red;
        }
    }
    
    auto row_all_red = [&](size_t row, size_t col1, size_t col2) -> bool {
        int count = row_prefix[row][col2 + 1] - row_prefix[row][col1];
        return count == static_cast<int>(col2 - col1 + 1);
    };
    
    auto col_all_red = [&](size_t col, size_t row1, size_t row2) -> bool {
        int count = col_prefix[row2 + 1][col] - col_prefix[row1][col];
        return count == static_cast<int>(row2 - row1 + 1);
    };

    // Step 5: Find max rectangle - O(n^2) over all point pairs
    uint64_t max_area = 0;
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            point p = points[i];
            point q = points[j];
            
            uint64_t candidate_area = (std::abs(p.x - q.x) + 1) * (std::abs(p.y - q.y) + 1);
            if (candidate_area <= max_area) continue;

            size_t x1_idx = get_index(sorted_x, std::min(p.x, q.x));
            size_t x2_idx = get_index(sorted_x, std::max(p.x, q.x));
            size_t y1_idx = get_index(sorted_y, std::min(p.y, q.y));
            size_t y2_idx = get_index(sorted_y, std::max(p.y, q.y));

            bool valid = row_all_red(x1_idx, y1_idx, y2_idx) &&
                         row_all_red(x2_idx, y1_idx, y2_idx) &&
                         col_all_red(y1_idx, x1_idx, x2_idx) &&
                         col_all_red(y2_idx, x1_idx, x2_idx);

            if (valid) {
                max_area = candidate_area;
            }
        }
    }
    
    return std::to_string(max_area);
}

REGISTER_SOLUTION(Day09)

} // namespace aoc::y2025