#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <map>
#include <vector>

namespace aoc::y2025
{

namespace {

__int128 power_of_10(int n)
{
    __int128 result = 1;
    while (n-- > 0)
        result *= 10;
    return result;
}

// Computes the multiplier for a k-digit pattern repeated r times.
// E.g., k=2, r=3: "AB" repeated is AB*10^4 + AB*10^2 + AB = AB * 10101
// This is the geometric series: (10^(k*r) - 1) / (10^k - 1)
// I just learned that this has a name: a "repunit" :o - https://en.wikipedia.org/wiki/Repunit - let's use it :)
__int128 repunit_multiplier(int digits, int reps)
{
    if (reps == 1)
        return 1;
    return (power_of_10(digits * reps) - 1) / (power_of_10(digits) - 1);
}

__int128 arithmetic_sum(__int128 lo, __int128 hi)
{
    return (lo > hi) ? 0 : (hi - lo + 1) * (lo + hi) / 2;
}

std::vector<std::pair<uint64_t, uint64_t>> parse_ranges(const std::string& input)
{
    std::vector<std::pair<uint64_t, uint64_t>> ranges;
    for (const auto& token : split(input, ','))
    {
        auto parts = split(token, '-');
        if (parts.size() == 2 && !parts[0].empty() && !parts[1].empty())
            ranges.emplace_back(std::stoull(parts[0]), std::stoull(parts[1]));
    }
    return ranges;
}

// Sum invalid IDs in [lo, hi] where invalid = pattern repeated exactly twice
// Started from h * 10^k + h = h * (10^k + 1) for k-digit h (min_half \leq h \leq max_half)
uint64_t sum_doubled_patterns(uint64_t lo, uint64_t hi)
{
    __int128 total = 0;

    for (int k = 1; k <= 10; k++)
    {
        __int128 mult = repunit_multiplier(k, 2);
        __int128 min_half = power_of_10(k - 1);
        __int128 max_half = power_of_10(k) - 1;

        // Find range of k-digit "halves" whose doubled form lies in [lo, hi]
        __int128 half_lo = std::max(min_half, ((__int128)lo + mult - 1) / mult);
        __int128 half_hi = std::min(max_half, (__int128)hi / mult);

        total += mult * arithmetic_sum(half_lo, half_hi);
    }

    return static_cast<uint64_t>(total);
}

// Sum invalid IDs in [lo, hi] where invalid = pattern repeated 2+ times
// Uses inclusion-exclusion to avoid double-counting (e.g., "ABAB" counted once, not as both "AB"x2 and "ABAB"x1)
uint64_t sum_repeated_patterns(uint64_t lo, uint64_t hi)
{
    __int128 total = 0;

    for (int total_digits = 2; total_digits <= 20; total_digits++)
    {
        // Gather valid pattern lengths (proper divisors allowing 2+ reps)
        std::vector<int> lengths;
        for (int k = 1; k <= total_digits / 2; k++)
            if (total_digits % k == 0)
                lengths.push_back(k);

        if (lengths.empty())
            continue;

        // primitive_sum[k] = sum of patterns with *minimal* period k
        std::map<int, __int128> primitive_sum;

        for (int k : lengths)
        {
            int reps = total_digits / k;
            __int128 mult = repunit_multiplier(k, reps);
            __int128 min_pat = power_of_10(k - 1);
            __int128 max_pat = power_of_10(k) - 1;

            __int128 pat_lo = std::max(min_pat, ((__int128)lo + mult - 1) / mult);
            __int128 pat_hi = std::min(max_pat, (__int128)hi / mult);

            // Start with raw sum, then subtract patterns with smaller periods
            primitive_sum[k] = arithmetic_sum(pat_lo, pat_hi);
            for (int d : lengths)
                if (d < k && k % d == 0)
                    primitive_sum[k] -= repunit_multiplier(d, k / d) * primitive_sum[d];
        }

        for (int k : lengths)
            total += repunit_multiplier(k, total_digits / k) * primitive_sum[k];
    }

    return static_cast<uint64_t>(total);
}

} // anonymous namespace

std::string Day02::part1(const std::string& input)
{
    uint64_t result = 0;
    for (auto [start, end] : parse_ranges(input))
        result += sum_doubled_patterns(start, end);
    return std::to_string(result);
}

std::string Day02::part2(const std::string& input)
{
    uint64_t result = 0;
    for (auto [start, end] : parse_ranges(input))
        result += sum_repeated_patterns(start, end);
    return std::to_string(result);
}

REGISTER_SOLUTION(Day02)

} // namespace aoc::y2025