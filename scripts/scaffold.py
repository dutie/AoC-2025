#!/usr/bin/env python3
"""Scaffold a new Advent of Code day."""

import argparse
import os
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
PROJECT_ROOT = SCRIPT_DIR.parent

SOLUTION_HPP_TEMPLATE = '''#pragma once

#include "common/solution.hpp"

namespace aoc::y{year} {{

class Day{day:02d} : public Solution {{
public:
    std::string part1(const std::string& input) override;
    std::string part2(const std::string& input) override;
    
    int year() const override {{ return {year}; }}
    int day() const override {{ return {day}; }}
}};

}} // namespace aoc::y{year}
'''

SOLUTION_CPP_TEMPLATE = '''#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"

namespace aoc::y{year} {{

std::string Day{day:02d}::part1(const std::string& input) {{
    auto lines = split(input);
    
    // TODO: Implement part 1
    
    return "TODO";
}}

std::string Day{day:02d}::part2(const std::string& input) {{
    auto lines = split(input);
    
    // TODO: Implement part 2
    
    return "TODO";
}}

REGISTER_SOLUTION(Day{day:02d})

}} // namespace aoc::y{year}
'''


def scaffold(year: int, day: int, fetch: bool = True):
    """Create scaffolding for a new day."""
    
    # Create source directory
    src_dir = PROJECT_ROOT / "src" / str(year) / f"day{day:02d}"
    src_dir.mkdir(parents=True, exist_ok=True)
    
    # Create data directory
    data_dir = PROJECT_ROOT / "data" / str(year) / f"day{day:02d}"
    data_dir.mkdir(parents=True, exist_ok=True)
    
    # Write solution header
    hpp_path = src_dir / "solution.hpp"
    if not hpp_path.exists():
        hpp_path.write_text(SOLUTION_HPP_TEMPLATE.format(year=year, day=day))
        print(f"✓ Created {hpp_path}")
    else:
        print(f"  Skipped {hpp_path} (exists)")
    
    # Write solution implementation
    cpp_path = src_dir / "solution.cpp"
    if not cpp_path.exists():
        cpp_path.write_text(SOLUTION_CPP_TEMPLATE.format(year=year, day=day))
        print(f"✓ Created {cpp_path}")
    else:
        print(f"  Skipped {cpp_path} (exists)")
    
    # Create empty example.txt
    example_path = data_dir / "example.txt"
    if not example_path.exists():
        example_path.touch()
        print(f"✓ Created {example_path}")
    
    # Optionally fetch input
    if fetch:
        import subprocess
        fetch_script = SCRIPT_DIR / "fetch_input.py"
        if fetch_script.exists():
            print(f"\nFetching input for {year} Day {day}...")
            subprocess.run(["python3", str(fetch_script), str(year), str(day)])
    
    print(f"\n🎄 Scaffolded {year} Day {day}")
    print(f"   Edit: {cpp_path}")
    print(f"   Example: {example_path}")
    print(f"\n   Build: make build")
    print(f"   Run:   make run YEAR={year} DAY={day}")


def main():
    parser = argparse.ArgumentParser(description="Scaffold a new AoC day")
    parser.add_argument("year", type=int, help="Year (e.g., 2024)")
    parser.add_argument("day", type=int, help="Day (1-25)")
    parser.add_argument("--no-fetch", action="store_true", help="Don't fetch input")
    
    args = parser.parse_args()
    
    if not (2015 <= args.year <= 2030):
        print(f"Invalid year: {args.year}")
        return 1
    
    if not (1 <= args.day <= 25):
        print(f"Invalid day: {args.day}")
        return 1
    
    scaffold(args.year, args.day, fetch=not args.no_fetch)
    return 0


if __name__ == "__main__":
    exit(main())
