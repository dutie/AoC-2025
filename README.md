# 🎄 Advent of Code C++ Template

A minimal scaffolding tool for solving Advent of Code puzzles in C++17.

<!--- advent_readme_stars table --->

<!--- advent_readme_stars table --->

## Quick Start

```bash
# 1. Install Python dependencies
pip3 install -r requirements.txt

# 2. Configure your AoC session cookie
cp .env.example .env
# Edit .env and add your session cookie from adventofcode.com

# 3. Build
make build

# 4. Scaffold a new day
make scaffold YEAR=2025 DAY=2
# OR
make scaffold 2025 04

# 5. Solve and run
make run YEAR=2025 DAY=4
# OR
make run 2025 04
```

## Commands

| Command | Description |
|---------|-------------|
| `make build` | Build the project |
| `make scaffold YEAR=2025 DAY=1` | Create new day with boilerplate |
| `make fetch YEAR=2025 DAY=1` | Download puzzle input |
| `make run YEAR=2025 DAY=1` | Run a solution |
| `make example YEAR=2025 DAY=1` | Run with example.txt input |
| `make bench` | Benchmark all solutions |
| `make bench YEAR=2025 DAY=1` | Benchmark specific day |
| `make list` | List all available solutions |
| `make clean` | Remove build files |

## Project Structure

```
aoc-cpp/
├── src/
│   ├── main.cpp           # Entry point
│   ├── common/
│   │   ├── solution.hpp   # Base Solution class
│   │   ├── registry.hpp   # Auto-registration system
│   │   └── utils.hpp      # Helper functions
│   └── 2025/
│       └── day01/
│           ├── solution.hpp
│           └── solution.cpp
├── data/
│   └── 2025/
│       └── day01/
│           ├── input.txt    # Puzzle input (fetched)
│           └── example.txt  # Example input (manual)
└── scripts/
    ├── scaffold.py        # Create new days
    ├── fetch_input.py     # Download inputs
    └── benchmark_all.py   # Run benchmarks
```

## Writing Solutions

Each solution inherits from `aoc::Solution`:

```cpp
#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"

namespace aoc::y2025 {

class Day01 : public Solution {
public:
    std::string part1(const std::string& input) override {
        auto lines = split(input);  // Utility function
        // Your solution here
        return std::to_string(answer);
    }

    std::string part2(const std::string& input) override {
        return "TODO";
    }

    int year() const override { return 2025; }
    int day() const override { return 1; }
};

REGISTER_SOLUTION(Day01)  // Auto-registers with the runner

} // namespace aoc::y2025
```

## Utilities

The `utils.hpp` header provides common helpers:

- `split(str, delim)` - Split string by delimiter
- `trim(str)` - Remove leading/trailing whitespace  
- `parse_ints(str)` - Extract all integers from a string
- `read_file(path)` - Read file contents
- `input_path(year, day)` - Get path to input file

**Note:** functions might not yet be fully implemented, I have not had that much time unfortunately 😅

## Getting Your Session Cookie

1. Log in to [adventofcode.com](https://adventofcode.com)
2. Open browser DevTools (F12)
3. Go to Application → Cookies → adventofcode.com
4. Copy the `session` cookie value
5. Add to `.env`: `AOC_SESSION=your_cookie_here`

## GitHub Actions Setup (Auto-update Stars)

This template includes a GitHub Action that automatically updates the README with your ⭐ progress.

### Setup Steps

1. **Create a private leaderboard** on [adventofcode.com](https://adventofcode.com):
   - Go to `https://adventofcode.com/{year}/leaderboard/private`
   - Click "Create It" if you don't have one

2. **Add Repository Secrets** (Settings → Secrets and variables → Actions → Secrets):
   | Secret | Description |
   |--------|-------------|
   | `AOC_USER_ID` | Your numeric ID from [settings page](https://adventofcode.com/settings) (number after `#`) |
   | `AOC_SESSION` | Your session cookie from browser DevTools |
   | `AOC_YEAR` | The year to track (e.g., `2025`) |

3. **Add Repository Variable** (Settings → Secrets and variables → Actions → Variables):
   | Variable | Value |
   |----------|-------|
   | `AOC_ENABLED` | `true` |

4. **Run the workflow**:
   - Go to Actions → "Update README ⭐" → "Run workflow"
   - Or just push changes to `src/` and it will run automatically

## Requirements

- CMake 3.14+
- C++17 compiler (g++, clang++)
- Python 3.8+
- `requests` and `python-dotenv` Python packages