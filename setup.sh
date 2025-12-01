#!/bin/bash

# Advent of Code C++ Template Setup Script

set -e

echo "🎄 Advent of Code C++ Template Setup 🎄"
echo "======================================="
echo ""

# Check for required tools
check_command() {
    if ! command -v $1 &> /dev/null; then
        echo "❌ $1 is not installed. Please install it first."
        exit 1
    fi
    echo "✓ $1 found"
}

echo "Checking requirements..."
check_command cmake
check_command make
check_command g++
check_command python3
check_command pip3

echo ""
echo "Installing Python dependencies..."
pip3 install -r requirements.txt

echo ""
echo "Creating .env file..."
if [ ! -f .env ]; then
    cp .env.example .env
    echo "✓ Created .env file from template"
    echo "⚠️  Please edit .env and add your AoC session token"
else
    echo "✓ .env file already exists"
fi

echo ""
echo "Creating build directory..."
mkdir -p build
cd build

echo ""
echo "Configuring CMake..."
cmake ..

echo ""
echo "Building project..."
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)

echo ""
echo "======================================="
echo "✓ Setup complete!"
echo ""
echo "Next steps:"
echo "1. Edit .env and add your AoC session token"
echo "2. Scaffold a new day: ./scripts/scaffold.py 2024 1"
echo "3. Run a solution: ./build/aoc 2024 1"
echo ""
echo "Happy coding! 🎅"
