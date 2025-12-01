#include <iostream>
#include <string>
#include <cstring>
#include "common/solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"

void print_usage(const char* prog) {
    std::cerr << "Usage: " << prog << " <year> <day> [options]\n"
              << "       " << prog << " --list\n\n"
              << "Options:\n"
              << "  --bench    Run with benchmarking\n"
              << "  --example  Use example input instead of puzzle input\n"
              << "  --list     List all available solutions\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }
    
    // Handle --list
    if (std::strcmp(argv[1], "--list") == 0) {
        std::cout << "Available solutions:\n";
        for (auto [year, day] : aoc::Registry::instance().list()) {
            std::cout << "  " << year << " Day " << day << "\n";
        }
        return 0;
    }
    
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }
    
    int year = std::stoi(argv[1]);
    int day = std::stoi(argv[2]);
    
    bool benchmark = false;
    bool use_example = false;
    
    for (int i = 3; i < argc; ++i) {
        if (std::strcmp(argv[i], "--bench") == 0) benchmark = true;
        if (std::strcmp(argv[i], "--example") == 0) use_example = true;
    }
    
    auto solution = aoc::Registry::instance().get(year, day);
    if (!solution) {
        std::cerr << "No solution found for " << year << " Day " << day << "\n";
        return 1;
    }
    
    std::string input_file = aoc::input_path(year, day);
    if (use_example) {
        // Replace input.txt with example.txt
        auto pos = input_file.rfind("input.txt");
        if (pos != std::string::npos) {
            input_file.replace(pos, 9, "example.txt");
        }
    }
    
    try {
        std::string input = aoc::read_file(input_file);
        std::cout << "=== " << year << " Day " << day << " ===\n";
        solution->run(input, benchmark);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}
