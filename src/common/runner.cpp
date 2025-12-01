#include "runner.h"
#include <iostream>

SolutionRunner& SolutionRunner::getInstance() {
    static SolutionRunner instance;
    return instance;
}

void SolutionRunner::registerSolution(int year, int day, Solution solution) {
    solutions[{year, day}] = solution;
}

Solution SolutionRunner::getSolution(int year, int day) {
    auto it = solutions.find({year, day});
    if (it != solutions.end()) {
        return it->second;
    }
    return {nullptr, nullptr};
}

bool SolutionRunner::hasSolution(int year, int day) {
    return solutions.find({year, day}) != solutions.end();
}

std::vector<std::pair<int, int>> SolutionRunner::getAllSolutions() {
    std::vector<std::pair<int, int>> result;
    for (const auto& [key, _] : solutions) {
        result.push_back(key);
    }
    return result;
}
