#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"

namespace aoc::y2025 {

std::string Day01::part1(const std::string& input) {
    auto lines = split(input);

    int zeros = 0;
    int position = 50;
    for(std::string line : lines){
        char direction = line[0];
        int steps = std::stoi(line.substr(1));

        if(position == 0){
            zeros++;
        }

        if(direction == 'L'){
            position = (position - steps) % 100;
        } else if(direction == 'R'){
            position = (position + steps) % 100;
        }

    }

    std::string result = std::to_string(zeros);
    
    return result;
}

std::string Day01::part2(const std::string& input) {
    auto lines = split(input);

    int zeros = 0;
    int position = 50;
    for(std::string line : lines){
        char direction = line[0];
        int steps = std::stoi(line.substr(1));

        if(direction == 'L'){
            if(position > 0){
                if(steps >= position){
                    zeros += (steps - position) / 100 + 1;
                }
            } else{
                zeros += steps / 100;
            }
            position = ((position - steps) % 100 + 100) % 100;
        } else if(direction == 'R'){
            if(position > 0){
                if(steps >= 100 - position){
                    zeros += (steps - (100 - position)) / 100 + 1;
                }
            } else{
                zeros += steps / 100;
            }
            position = (position + steps) % 100;
        }

    }

    std::string result = std::to_string(zeros);
    
    return result;
}

REGISTER_SOLUTION(Day01)

} // namespace aoc::y2025
