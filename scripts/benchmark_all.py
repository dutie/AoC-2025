#!/usr/bin/env python3
"""Benchmark all Advent of Code solutions."""

import subprocess
import re
import argparse
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent
PROJECT_ROOT = SCRIPT_DIR.parent
AOC_BIN = PROJECT_ROOT / "build" / "aoc"


def find_solutions():
    """Find all implemented solutions by scanning source directories."""
    solutions = []
    src_dir = PROJECT_ROOT / "src"
    
    for year_dir in sorted(src_dir.glob("20*")):
        year = int(year_dir.name)
        for day_dir in sorted(year_dir.glob("day*")):
            day = int(day_dir.name[3:])
            if (day_dir / "solution.cpp").exists():
                solutions.append((year, day))
    
    return solutions


def run_benchmark(year: int, day: int) -> dict:
    """Run benchmark for a single day."""
    try:
        result = subprocess.run(
            [str(AOC_BIN), str(year), str(day), "--bench"],
            capture_output=True,
            text=True,
            timeout=60
        )
        
        if result.returncode != 0:
            return {"error": result.stderr.strip() or "Unknown error"}
        
        output = result.stdout
        times = {}
        
        # Parse output like "Part 1: 123 (456 µs)"
        for match in re.finditer(r"Part (\d): (.+) \((\d+) µs\)", output):
            part = f"part{match.group(1)}"
            times[part] = {
                "answer": match.group(2),
                "time_us": int(match.group(3))
            }
        
        return times
        
    except subprocess.TimeoutExpired:
        return {"error": "Timeout (>60s)"}
    except Exception as e:
        return {"error": str(e)}


def format_time(us: int) -> str:
    """Format microseconds nicely."""
    if us < 1000:
        return f"{us} µs"
    elif us < 1000000:
        return f"{us/1000:.2f} ms"
    else:
        return f"{us/1000000:.2f} s"


def main():
    parser = argparse.ArgumentParser(description="Benchmark all AoC solutions")
    parser.add_argument("--markdown", "-m", action="store_true", help="Output as Markdown table")
    parser.add_argument("--year", "-y", type=int, help="Filter by year")
    
    args = parser.parse_args()
    
    if not AOC_BIN.exists():
        print(f"❌ Build not found. Run: make build")
        return 1
    
    solutions = find_solutions()
    if args.year:
        solutions = [(y, d) for y, d in solutions if y == args.year]
    
    if not solutions:
        print("No solutions found.")
        return 0
    
    results = []
    for year, day in solutions:
        print(f"Running {year} Day {day:02d}...", end=" ", flush=True)
        result = run_benchmark(year, day)
        results.append((year, day, result))
        
        if "error" in result:
            print(f"❌ {result['error']}")
        else:
            p1 = format_time(result.get("part1", {}).get("time_us", 0))
            p2 = format_time(result.get("part2", {}).get("time_us", 0))
            print(f"✓ Part 1: {p1}, Part 2: {p2}")
    
    if args.markdown:
        print("\n## Benchmark Results\n")
        print("| Year | Day | Part 1 | Part 2 | Total |")
        print("|------|-----|--------|--------|-------|")
        
        for year, day, result in results:
            if "error" in result:
                print(f"| {year} | {day:02d} | Error | Error | - |")
            else:
                p1_us = result.get("part1", {}).get("time_us", 0)
                p2_us = result.get("part2", {}).get("time_us", 0)
                print(f"| {year} | {day:02d} | {format_time(p1_us)} | {format_time(p2_us)} | {format_time(p1_us + p2_us)} |")
    
    return 0


if __name__ == "__main__":
    exit(main())
