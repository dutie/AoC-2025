#include <gtest/gtest.h>
#include "solution.h"

namespace aoc{{YEAR}}::day{{DAY_PADDED}} {

const std::string EXAMPLE_INPUT = R"(
TODO: Add example input here
)";

TEST(Day{{DAY_PADDED}}, Part1Example) {
    // TODO: Update expected value
    EXPECT_EQ("42", part1(EXAMPLE_INPUT));
}

TEST(Day{{DAY_PADDED}}, Part2Example) {
    // TODO: Update expected value
    EXPECT_EQ("42", part2(EXAMPLE_INPUT));
}

// Uncomment to test with real input
// TEST(Day{{DAY_PADDED}}, Part1Real) {
//     std::string input = read_file("data/{{YEAR}}/day{{DAY_PADDED}}.txt");
//     std::string result = part1(input);
//     std::cout << "Part 1: " << result << std::endl;
//     // EXPECT_EQ("expected_answer", result);
// }

// TEST(Day{{DAY_PADDED}}, Part2Real) {
//     std::string input = read_file("data/{{YEAR}}/day{{DAY_PADDED}}.txt");
//     std::string result = part2(input);
//     std::cout << "Part 2: " << result << std::endl;
//     // EXPECT_EQ("expected_answer", result);
// }

} // namespace aoc{{YEAR}}::day{{DAY_PADDED}}
