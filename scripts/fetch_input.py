#!/usr/bin/env python3
"""Fetch Advent of Code puzzle input."""

import argparse
import os
from pathlib import Path

try:
    import requests
    from dotenv import load_dotenv
except ImportError:
    print("Missing dependencies. Run: pip3 install requests python-dotenv")
    exit(1)

SCRIPT_DIR = Path(__file__).parent
PROJECT_ROOT = SCRIPT_DIR.parent


def fetch_input(year: int, day: int) -> str:
    """Fetch input from adventofcode.com."""
    
    load_dotenv(PROJECT_ROOT / ".env")
    session = os.getenv("AOC_SESSION")
    
    if not session:
        print("❌ AOC_SESSION not set in .env file")
        print("   Get your session cookie from adventofcode.com")
        print("   Add to .env: AOC_SESSION=your_session_cookie")
        return None
    
    url = f"https://adventofcode.com/{year}/day/{day}/input"
    headers = {"Cookie": f"session={session}"}
    
    response = requests.get(url, headers=headers)
    
    if response.status_code == 200:
        return response.text
    elif response.status_code == 404:
        print(f"❌ Puzzle not available yet: {year} Day {day}")
    elif response.status_code == 400:
        print(f"❌ Invalid session cookie")
    else:
        print(f"❌ HTTP {response.status_code}: {response.text}")
    
    return None


def main():
    parser = argparse.ArgumentParser(description="Fetch AoC puzzle input")
    parser.add_argument("year", type=int, help="Year (e.g., 2024)")
    parser.add_argument("day", type=int, help="Day (1-25)")
    parser.add_argument("--force", "-f", action="store_true", help="Overwrite existing")
    
    args = parser.parse_args()
    
    data_dir = PROJECT_ROOT / "data" / str(args.year) / f"day{args.day:02d}"
    data_dir.mkdir(parents=True, exist_ok=True)
    
    input_path = data_dir / "input.txt"
    
    if input_path.exists() and not args.force:
        print(f"  Input already exists: {input_path}")
        print(f"  Use --force to overwrite")
        return 0
    
    content = fetch_input(args.year, args.day)
    if content:
        input_path.write_text(content)
        print(f"✓ Saved to {input_path}")
        return 0
    
    return 1


if __name__ == "__main__":
    exit(main())
