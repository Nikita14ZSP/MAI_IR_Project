#include "tokenizer.h"
#include <cctype>
#include <sstream>
#include <algorithm>

// STL разрешен только для токенизации (sstream, algorithm)

std::vector<std::string> Tokenizer::tokenize(const std::string& text) {
    return tokenize(text, true);
}

std::vector<std::string> Tokenizer::tokenize(const std::string& text, bool remove_punctuation) {
    std::vector<std::string> tokens;
    
    if (text.empty()) {
        return tokens;
    }
    
    std::stringstream ss(text);
    std::string word;
    
    // Используем STL для разбиения на слова (разрешено по требованиям)
    while (ss >> word) {
        if (remove_punctuation) {
            // Удаление знаков препинания в начале и конце
            while (!word.empty() && is_punctuation(static_cast<unsigned char>(word[0]))) {
                word.erase(0, 1);
            }
            while (!word.empty() && is_punctuation(static_cast<unsigned char>(word[word.length() - 1]))) {
                word.erase(word.length() - 1, 1);
            }
        }
        
        if (!word.empty()) {
            std::string normalized = normalize(word);
            if (!normalized.empty()) {
                tokens.push_back(normalized);
            }
        }
    }
    
    return tokens;
}

std::string Tokenizer::normalize(const std::string& token) {
    if (token.empty()) {
        return token;
    }
    
    std::string result;
    result.reserve(token.length());
    
    for (size_t i = 0; i < token.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(token[i]);
        
        // Преобразование к нижнему регистру для латиницы
        if (c >= 'A' && c <= 'Z') {
            result += static_cast<char>(c - 'A' + 'a');
        } else if (c >= 0xC0 && c <= 0xDF) {
            // Русские заглавные буквы (А-Я в UTF-8, начало 2-байтового символа)
            if (i + 1 < token.length()) {
                unsigned char c2 = static_cast<unsigned char>(token[i + 1]);
                // Упрощенная обработка: А (0xD0 0x90) -> а (0xD0 0xB0)
                if (c == 0xD0 && c2 >= 0x90 && c2 <= 0xAF) {
                    result += static_cast<char>(0xD0);
                    result += static_cast<char>(c2 + 0x20);  // Смещение к строчным
                    ++i;
                } else {
                    result += token[i];
                }
            } else {
                result += token[i];
            }
        } else {
            result += token[i];
        }
    }
    
    return result;
}

bool Tokenizer::is_letter(unsigned char c) {
    // Английские буквы
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return true;
    }
    
    // Русские буквы (UTF-8): А-Я, а-я
    // Это начало 2-байтового символа UTF-8
    if (c >= 0xD0 && c <= 0xDF) {
        return true;
    }
    
    return false;
}

bool Tokenizer::is_punctuation(unsigned char c) {
    // Стандартные знаки препинания
    if (std::ispunct(c) != 0) {
        return true;
    }
    
    // Пробелы и переносы строк
    if (c == ' ' || c == '\n' || c == '\t' || c == '\r') {
        return true;
    }
    
    return false;
}
