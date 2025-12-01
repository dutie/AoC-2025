#ifndef AOC_COMMON_UTILS_H
#define AOC_COMMON_UTILS_H

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <regex>
#include <numeric>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <functional>
#include <cstdint>

// File I/O
std::string read_file(const std::string& filename);
std::vector<std::string> read_lines(const std::string& filename);

// String manipulation
std::vector<std::string> split(const std::string& str, char delimiter);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);
std::vector<std::string> split_regex(const std::string& str, const std::string& pattern);
std::string trim(const std::string& str);
std::string ltrim(const std::string& str);
std::string rtrim(const std::string& str);
std::string replace_all(std::string str, const std::string& from, const std::string& to);
bool starts_with(const std::string& str, const std::string& prefix);
bool ends_with(const std::string& str, const std::string& suffix);

// Number extraction and parsing
std::vector<int> extract_numbers(const std::string& str);
std::vector<int64_t> extract_numbers_int64(const std::string& str);
std::vector<double> extract_doubles(const std::string& str);
int parse_int(const std::string& str);
int64_t parse_int64(const std::string& str);
double parse_double(const std::string& str);

// Grid/2D array utilities
template<typename T>
using Grid = std::vector<std::vector<T>>;

template<typename T>
Grid<T> make_grid(size_t rows, size_t cols, T default_value = T{}) {
    return Grid<T>(rows, std::vector<T>(cols, default_value));
}

struct Point {
    int x, y;
    
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
    
    bool operator<(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
    
    Point operator+(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    Point operator-(const Point& other) const {
        return Point(x - other.x, y - other.y);
    }
    
    int manhattan_distance(const Point& other) const {
        return abs(x - other.x) + abs(y - other.y);
    }
};

// Direction helpers
const std::vector<Point> DIRECTIONS_4 = {{0, -1}, {1, 0}, {0, 1}, {-1, 0}};  // N, E, S, W
const std::vector<Point> DIRECTIONS_8 = {
    {0, -1}, {1, -1}, {1, 0}, {1, 1},
    {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}
};

// Grid parsing
Grid<char> parse_char_grid(const std::string& input);
Grid<int> parse_int_grid(const std::string& input);

// Common algorithms
template<typename T>
bool in_bounds(const Grid<T>& grid, int x, int y) {
    return x >= 0 && y >= 0 && 
           y < static_cast<int>(grid.size()) && 
           x < static_cast<int>(grid[0].size());
}

template<typename T>
bool in_bounds(const Grid<T>& grid, const Point& p) {
    return in_bounds(grid, p.x, p.y);
}

// BFS/DFS helpers
template<typename T>
std::vector<Point> get_neighbors(const Grid<T>& grid, const Point& p, bool diagonal = false) {
    std::vector<Point> neighbors;
    const auto& dirs = diagonal ? DIRECTIONS_8 : DIRECTIONS_4;
    
    for (const auto& dir : dirs) {
        Point next = p + dir;
        if (in_bounds(grid, next)) {
            neighbors.push_back(next);
        }
    }
    
    return neighbors;
}

// Flood fill
template<typename T>
std::set<Point> flood_fill(const Grid<T>& grid, const Point& start, 
                           std::function<bool(T, T)> can_move = [](T from, T to) { return from == to; }) {
    std::set<Point> visited;
    std::queue<Point> queue;
    
    queue.push(start);
    visited.insert(start);
    T start_value = grid[start.y][start.x];
    
    while (!queue.empty()) {
        Point current = queue.front();
        queue.pop();
        
        for (const Point& next : get_neighbors(grid, current)) {
            if (visited.find(next) == visited.end() && 
                can_move(start_value, grid[next.y][next.x])) {
                visited.insert(next);
                queue.push(next);
            }
        }
    }
    
    return visited;
}

// Math utilities
int64_t gcd(int64_t a, int64_t b);
int64_t lcm(int64_t a, int64_t b);
bool is_prime(int64_t n);
std::vector<int64_t> get_primes_up_to(int64_t n);
std::vector<int64_t> prime_factorization(int64_t n);

// Range utilities
template<typename T>
T sum(const std::vector<T>& vec) {
    return std::accumulate(vec.begin(), vec.end(), T{});
}

template<typename T>
T product(const std::vector<T>& vec) {
    return std::accumulate(vec.begin(), vec.end(), T{1}, std::multiplies<T>());
}

template<typename T>
std::pair<T, T> min_max(const std::vector<T>& vec) {
    auto [min_it, max_it] = std::minmax_element(vec.begin(), vec.end());
    return {*min_it, *max_it};
}

// Permutation/Combination helpers
template<typename T>
std::vector<std::vector<T>> get_permutations(std::vector<T> vec) {
    std::vector<std::vector<T>> result;
    std::sort(vec.begin(), vec.end());
    
    do {
        result.push_back(vec);
    } while (std::next_permutation(vec.begin(), vec.end()));
    
    return result;
}

// Hash function for Point (for use in unordered_set/unordered_map)
namespace std {
    template<>
    struct hash<Point> {
        size_t operator()(const Point& p) const {
            return hash<int>()(p.x) ^ (hash<int>()(p.y) << 1);
        }
    };
}

// Timing macro for debugging
#define TIME_IT(code) \
    do { \
        auto start = std::chrono::high_resolution_clock::now(); \
        code; \
        auto end = std::chrono::high_resolution_clock::now(); \
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); \
        std::cout << "Time: " << duration.count() << " ms\n"; \
    } while(0)

#endif // AOC_COMMON_UTILS_H
