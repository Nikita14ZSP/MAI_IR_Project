#include "string_utils.h"
#include <cctype>

Vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    Vector<std::string> result;
    std::string current;
    
    for (size_t i = 0; i < str.length(); ++i) {
        char c = str[i];
        if (c == delimiter) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        result.push_back(current);
    }
    
    return result;
}

std::string StringUtils::join(const Vector<std::string>& parts, const std::string& delimiter) {
    if (parts.empty()) {
        return "";
    }
    
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += delimiter + parts[i];
    }
    
    return result;
}

std::string StringUtils::trim(const std::string& str) {
    if (str.empty()) {
        return str;
    }
    
    size_t start = 0;
    while (start < str.length() && std::isspace(static_cast<unsigned char>(str[start]))) {
        ++start;
    }
    
    if (start == str.length()) {
        return "";
    }
    
    size_t end = str.length() - 1;
    while (end > start && std::isspace(static_cast<unsigned char>(str[end]))) {
        --end;
    }
    
    return str.substr(start, end - start + 1);
}

std::string StringUtils::to_lower(const std::string& str) {
    std::string result = str;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = static_cast<char>(std::tolower(static_cast<unsigned char>(result[i])));
    }
    return result;
}

bool StringUtils::starts_with(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) {
        return false;
    }
    return str.compare(0, prefix.length(), prefix) == 0;
}

bool StringUtils::ends_with(const std::string& str, const std::string& suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }
    return str.compare(str.length() - suffix.length(), suffix.length(), suffix) == 0;
}

