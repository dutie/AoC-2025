#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <vector>
#include <cstdint>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <limits>

namespace aoc::y2025 {

struct Machine {
    std::vector<uint8_t> light_target;
    std::vector<std::vector<uint16_t>> buttons;
    std::vector<uint64_t> joltage_target;
    
    size_t num_lights() const { return light_target.size(); }
    size_t num_joltages() const { return joltage_target.size(); }
    size_t num_buttons() const { return buttons.size(); }
};

Machine parse_machine(const std::string& line) {
    Machine m;
    size_t i = 0;
    
    // Parse Lights Target [...#.]
    while (i < line.size() && line[i] != '[') i++;
    i++; 
    while (i < line.size() && line[i] != ']') {
        if (line[i] == '.') m.light_target.push_back(0);
        else if (line[i] == '#') m.light_target.push_back(1);
        i++;
    }
    i++;
    
    // Parse Buttons (...)
    while (i < line.size() && line[i] != '{') {
        if (line[i] == '(') {
            i++;
            std::vector<uint16_t> button;
            while (i < line.size() && line[i] != ')') {
                if (std::isdigit(line[i])) {
                    uint16_t num = 0;
                    while (i < line.size() && std::isdigit(line[i])) {
                        num = num * 10 + (line[i] - '0');
                        i++;
                    }
                    button.push_back(num);
                } else {
                    i++;
                }
            }
            m.buttons.push_back(button);
            i++; 
        } else {
            i++;
        }
    }
    
    // Parse Joltages {3,5,...}
    if (i < line.size() && line[i] == '{') {
        i++;
        while (i < line.size() && line[i] != '}') {
            if (std::isdigit(line[i])) {
                uint64_t num = 0;
                while (i < line.size() && std::isdigit(line[i])) {
                    num = num * 10 + (line[i] - '0');
                    i++;
                }
                m.joltage_target.push_back(num);
            } else {
                i++;
            }
        }
    }
    return m;
}
using BitRow = std::vector<uint8_t>;
using BitMatrix = std::vector<BitRow>;

BitMatrix build_gf2_matrix(const Machine& m) {
    size_t n = m.num_lights();
    size_t k = m.num_buttons();
    BitMatrix mat(n, BitRow(k + 1, 0));
    for (size_t j = 0; j < k; j++) {
        for (uint16_t light_idx : m.buttons[j]) {
            if (light_idx < n) mat[light_idx][j] ^= 1;
        }
    }
    for (size_t i = 0; i < n; i++) mat[i][k] = m.light_target[i];
    return mat;
}

struct GaussianResultGF2 {
    BitMatrix reduced;
    std::vector<int> pivot_col; // For each row, which col is pivot?
    std::vector<int> pivot_row; // For each col, which row is pivot?
    std::vector<int> free_vars;
    int rank;
    bool solvable;
};

GaussianResultGF2 gaussian_elimination_gf2(BitMatrix mat, size_t num_buttons) {
    GaussianResultGF2 result;
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    
    result.pivot_row.assign(num_buttons, -1);
    result.pivot_col.assign(rows, -1);
    
    size_t pivot_row = 0;
    
    // Iterate over every column (button)
    for (size_t col = 0; col < num_buttons; col++) {
        if (pivot_row >= rows) {
            result.free_vars.push_back(col);
            continue;
        }

        int pivot = -1;
        for (size_t row = pivot_row; row < rows; row++) {
            if (mat[row][col] == 1) {
                pivot = row;
                break;
            }
        }
        
        if (pivot == -1) {
            result.free_vars.push_back(col);
            continue;
        }
        
        std::swap(mat[pivot], mat[pivot_row]);
        result.pivot_row[col] = pivot_row;
        result.pivot_col[pivot_row] = col;
        
        for (size_t row = 0; row < rows; row++) {
            if (row != pivot_row && mat[row][col] == 1) {
                for (size_t c = 0; c < cols; c++) {
                    mat[row][c] ^= mat[pivot_row][c];
                }
            }
        }
        pivot_row++;
    }
    
    result.rank = pivot_row;
    result.reduced = mat;
    result.solvable = true;
    
    // Check for inconsistencies (0 = 1)
    for (size_t row = pivot_row; row < rows; row++) {
        if (mat[row][num_buttons] == 1) {
            result.solvable = false;
            break;
        }
    }
    
    return result;
}

int solve_part1(const Machine& m) {
    if (m.num_buttons() == 0) return 0; // simplified
    BitMatrix mat = build_gf2_matrix(m);
    auto gauss = gaussian_elimination_gf2(mat, m.num_buttons());
    
    if (!gauss.solvable) return -1;
    
    int min_presses = std::numeric_limits<int>::max();
    size_t num_free = gauss.free_vars.size();
    
    // Brute force free variables (usually very few)
    for (uint64_t mask = 0; mask < (1ULL << num_free); mask++) {
        std::vector<uint8_t> sol(m.num_buttons(), 0);
        for (size_t i = 0; i < num_free; i++) {
            sol[gauss.free_vars[i]] = (mask >> i) & 1;
        }
        
        // Back substitution
        for (int row = gauss.rank - 1; row >= 0; row--) {
            int col = gauss.pivot_col[row];
            if (col == -1) continue;
            uint8_t val = gauss.reduced[row][m.num_buttons()];
            for (size_t c = col + 1; c < m.num_buttons(); c++) {
                val ^= (gauss.reduced[row][c] & sol[c]);
            }
            sol[col] = val;
        }
        
        int presses = 0;
        for (auto s : sol) presses += s;
        min_presses = std::min(min_presses, presses);
    }
    return min_presses;
}
using RealMatrix = std::vector<std::vector<double>>;
const double EPS = 1e-9;

struct GaussianResultReal {
    RealMatrix reduced;
    std::vector<int> pivot_col; // Row -> Col
    std::vector<int> pivot_row; // Col -> Row
    std::vector<int> free_vars;
    int rank;
    bool solvable;
};

GaussianResultReal gaussian_elimination_real(RealMatrix mat, size_t num_vars) {
    GaussianResultReal res;
    size_t rows = mat.size();
    size_t cols = mat[0].size();
    
    res.pivot_row.assign(num_vars, -1);
    res.pivot_col.assign(rows, -1);
    size_t pivot_row = 0;
    
    for (size_t col = 0; col < num_vars; col++) {
        if (pivot_row >= rows) {
            res.free_vars.push_back(col);
            continue;
        }

        int pivot = -1;
        // Find pivot with largest magnitude for stability
        double max_val = -1.0;
        for (size_t row = pivot_row; row < rows; row++) {
            if (std::abs(mat[row][col]) > EPS && std::abs(mat[row][col]) > max_val) {
                max_val = std::abs(mat[row][col]);
                pivot = row;
            }
        }
        
        if (pivot == -1) {
            res.free_vars.push_back(col);
            continue;
        }
        
        std::swap(mat[pivot], mat[pivot_row]);
        res.pivot_row[col] = pivot_row;
        res.pivot_col[pivot_row] = col;
        
        // Normalize pivot row
        double div = mat[pivot_row][col];
        for (size_t c = col; c < cols; c++) mat[pivot_row][c] /= div;
        mat[pivot_row][col] = 1.0; // Force exact 1.0
        
        // Eliminate other rows
        for (size_t row = 0; row < rows; row++) {
            if (row != pivot_row && std::abs(mat[row][col]) > EPS) {
                double mul = mat[row][col];
                for (size_t c = col; c < cols; c++) {
                    mat[row][c] -= mul * mat[pivot_row][c];
                }
            }
        }
        pivot_row++;
    }
    
    res.rank = pivot_row;
    res.reduced = mat;
    res.solvable = true;
    
    // Check 0 = NonZero
    for (size_t row = pivot_row; row < rows; row++) {
        if (std::abs(mat[row][num_vars]) > EPS) {
            res.solvable = false;
            break;
        }
    }
    return res;
}

int64_t min_total_presses = -1;

void solve_recursive(const GaussianResultReal& gauss, 
                     const Machine& m,
                     std::vector<int64_t>& solution, 
                     size_t free_idx) 
{
    // Base Case: All free variables assigned
    if (free_idx == gauss.free_vars.size()) {
        int64_t current_sum = 0;
        
        for (int row = gauss.rank - 1; row >= 0; row--) {
            int p_col = gauss.pivot_col[row];
            if (p_col == -1) continue;
            
            double val = gauss.reduced[row][m.num_buttons()]; // Target
            
            // Subtract contributions from free variables (and previously solved pivots)
            for (size_t c = p_col + 1; c < m.num_buttons(); c++) {
                if (std::abs(gauss.reduced[row][c]) > EPS) {
                    val -= gauss.reduced[row][c] * (double)solution[c];
                }
            }
            
            if (val < -EPS) return; // Negative presses not allowed
            double nearest = std::round(val);
            if (std::abs(val - nearest) > 1e-4) return; // Non-integer presses not allowed
            
            solution[p_col] = (int64_t)nearest;
        }
        
        for (auto x : solution) current_sum += x;
        
        if (min_total_presses == -1 || current_sum < min_total_presses) {
            min_total_presses = current_sum;
        }
        return;
    }
    
    int f_col = gauss.free_vars[free_idx];
    
    for (int64_t val = 0; val <= 199; val++) { 
        solution[f_col] = val;
        solve_recursive(gauss, m, solution, free_idx + 1);
        
    }
}

int64_t solve_part2_machine(const Machine& m) {
    size_t rows = m.num_joltages();
    size_t cols = m.num_buttons();
    
    // Build Matrix [A | b]
    RealMatrix mat(rows, std::vector<double>(cols + 1, 0.0));
    for (size_t j = 0; j < cols; j++) {
        for (uint16_t c_idx : m.buttons[j]) {
            if (c_idx < rows) mat[c_idx][j] = 1.0;
        }
    }
    for (size_t i = 0; i < rows; i++) {
        mat[i][cols] = (double)m.joltage_target[i];
    }
    
    auto gauss = gaussian_elimination_real(mat, cols);
    
    if (!gauss.solvable) return 0; // Return 0 if no solution? Or -1? Problem implies solution exists.
    
    min_total_presses = -1;
    std::vector<int64_t> solution(cols, 0);
    
    solve_recursive(gauss, m, solution, 0);
    
    return (min_total_presses == -1) ? 0 : min_total_presses;
}

std::string Day10::part1(const std::string& input) {
    auto lines = split(input);
    int64_t total = 0;
    for (const auto& line : lines) {
        if (line.empty()) continue;
        int p = solve_part1(parse_machine(line));
        if (p != -1) total += p;
    }
    return std::to_string(total);
}

std::string Day10::part2(const std::string& input) {
    auto lines = split(input);
    int64_t total = 0;
    for (const auto& line : lines) {
        if (line.empty()) continue;
        Machine m = parse_machine(line);
        total += solve_part2_machine(m);
    }
    return std::to_string(total);
}

REGISTER_SOLUTION(Day10)

} // namespace aoc::y2025