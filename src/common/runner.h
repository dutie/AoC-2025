#ifndef AOC_COMMON_RUNNER_H
#define AOC_COMMON_RUNNER_H

#include <string>
#include <functional>
#include <map>
#include <utility>

struct Solution {
    std::function<std::string(const std::string&)> part1;
    std::function<std::string(const std::string&)> part2;
};

class SolutionRunner {
public:
    static SolutionRunner& getInstance();
    
    void registerSolution(int year, int day, Solution solution);
    Solution getSolution(int year, int day);
    bool hasSolution(int year, int day);
    
    std::vector<std::pair<int, int>> getAllSolutions();
    
private:
    SolutionRunner() = default;
    std::map<std::pair<int, int>, Solution> solutions;
};

// Macro for easy solution registration
#define REGISTER_SOLUTION(year, day, namespace) \
    static bool registered_##year##_##day = []() { \
        SolutionRunner::getInstance().registerSolution( \
            year, day, \
            {namespace::part1, namespace::part2} \
        ); \
        return true; \
    }();

#endif // AOC_COMMON_RUNNER_H
