#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>

namespace aoc {

// Read entire file to string
inline std::string read_file(const std::string& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + path);
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Split string by delimiter
inline std::vector<std::string> split(const std::string& s, char delim = '\n') {
    std::vector<std::string> result;
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        if (!item.empty() || delim != '\n') {
            result.push_back(item);
        }
    }
    return result;
}

// Split string by string delimiter
inline std::vector<std::string> split(const std::string& s, const std::string& delim) {
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = s.find(delim, start)) != std::string::npos) {
        result.push_back(s.substr(start, end - start));
        start = end + delim.length();
    }
    result.push_back(s.substr(start));
    return result;
}

// Trim whitespace
inline std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

// Parse all integers from a string
inline std::vector<long long> parse_ints(const std::string& s) {
    std::vector<long long> result;
    std::istringstream iss(s);
    long long n;
    while (iss >> n) {
        result.push_back(n);
    }
    return result;
}

// Get input path for a given year/day
inline std::string input_path(int year, int day) {
    std::ostringstream oss;
    oss << DATA_DIR << "/" << year << "/day" 
        << std::setfill('0') << std::setw(2) << day << "/input.txt";
    return oss.str();
}

} // namespace aoc
