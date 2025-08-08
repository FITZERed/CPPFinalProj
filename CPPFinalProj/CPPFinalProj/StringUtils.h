#pragma once
#include <string>
#include <algorithm>
#include <cctype>

inline std::string Trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    size_t last = str.find_last_not_of(" \t\r\n");
    if (first == std::string::npos || last == std::string::npos) return "";
    return str.substr(first, (last - first + 1));
}