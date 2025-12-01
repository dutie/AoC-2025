#pragma once

#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

namespace aoc {

class Solution {
public:
    virtual ~Solution() = default;
    
    virtual std::string part1(const std::string& input) = 0;
    virtual std::string part2(const std::string& input) = 0;
    
    virtual int year() const = 0;
    virtual int day() const = 0;
    
    void run(const std::string& input, bool benchmark = false) {
        if (benchmark) {
            run_benchmark(input);
        } else {
            std::cout << "Part 1: " << part1(input) << "\n";
            std::cout << "Part 2: " << part2(input) << "\n";
        }
    }
    
private:
    void run_benchmark(const std::string& input, int iterations = 100) {
        using Clock = std::chrono::high_resolution_clock;
        
        // Warmup
        part1(input);
        part2(input);
        
        // Benchmark Part 1
        auto start = Clock::now();
        std::string result1;
        for (int i = 0; i < iterations; ++i) {
            result1 = part1(input);
        }
        auto p1_time = std::chrono::duration_cast<std::chrono::microseconds>(
            Clock::now() - start).count() / iterations;
        
        // Benchmark Part 2
        start = Clock::now();
        std::string result2;
        for (int i = 0; i < iterations; ++i) {
            result2 = part2(input);
        }
        auto p2_time = std::chrono::duration_cast<std::chrono::microseconds>(
            Clock::now() - start).count() / iterations;
        
        std::cout << "Part 1: " << result1 << " (" << p1_time << " µs)\n";
        std::cout << "Part 2: " << result2 << " (" << p2_time << " µs)\n";
    }
};

} // namespace aoc
