#include "stemmer.h"
#include <cstring>

std::string Stemmer::stem(const std::string& word) {
    if (word.empty()) {
        return word;
    }
    
    char lang = detect_language(word);
    
    if (lang == 'r') {
        return stem_russian(word);
    } else if (lang == 'e') {
        return stem_english(word);
    }
    
    return word;
}

char Stemmer::detect_language(const std::string& word) {
    bool has_cyrillic = false;
    bool has_latin = false;
    
    for (size_t i = 0; i < word.length(); ++i) {
        unsigned char c = static_cast<unsigned char>(word[i]);
        
        // Проверка на кириллицу (UTF-8: 0xD0-0xD1 для русских букв)
        if ((c >= 0xD0 && c <= 0xD1) || (c >= 0xD2 && c <= 0xD3)) {
            has_cyrillic = true;
        } else if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
            has_latin = true;
        }
    }
    
    if (has_cyrillic) {
        return 'r';
    } else if (has_latin) {
        return 'e';
    }
    
    return 'u';
}

std::string Stemmer::stem_russian(const std::string& word) {
    if (word.length() < 4) {
        return word;
    }
    
    std::string result = word;
    size_t len = result.length();
    
    // Список окончаний (от более длинных к коротким)
    // Обработка UTF-8 строк требует аккуратности
    const char* suffixes[] = {
        "\xD0\xB0\xD0\xBC\xD0\xB8",  // "ами" (3 байта UTF-8)
        "\xD1\x8F\xD0\xBC\xD0\xB8",  // "ями"
        "\xD0\xBE\xD0\xB2",          // "ов"
        "\xD0\xB5\xD0\xB2",          // "ев"
        "\xD0\xB5\xD0\xB9",          // "ей"
        "\xD0\xBE\xD0\xB9",          // "ой"
        "\xD0\xBE\xD0\xBC",          // "ом"
        "\xD0\xB5\xD0\xBC",          // "ем"
        "\xD0\xB0\xD1\x8F",          // "ая"
        "\xD0\xBE\xD0\xB9",          // "ой"
        "\xD0\xBE\xD0\xB5",          // "ое"
        "\xD1\x8B\xD0\xB5",          // "ые"
        "\xD0\xB8\xD0\xB5",          // "ие"
    };
    
    size_t suffix_lengths[] = {6, 6, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4};
    size_t num_suffixes = sizeof(suffixes) / sizeof(suffixes[0]);
    
    // Попытка удалить окончания
    for (size_t i = 0; i < num_suffixes; ++i) {
        size_t suffix_len = suffix_lengths[i];
        if (len > suffix_len + 2) {
            // Проверка на совпадение окончания
            bool matches = true;
            for (size_t j = 0; j < suffix_len; ++j) {
                if (result[len - suffix_len + j] != suffixes[i][j]) {
                    matches = false;
                    break;
                }
            }
            
            if (matches) {
                result.erase(len - suffix_len);
                break;
            }
        }
    }
    
    return result;
}

std::string Stemmer::stem_english(const std::string& word) {
    if (word.length() < 4) {
        return word;
    }
    
    std::string result = word;
    size_t len = result.length();
    
    // Список окончаний (от более длинных к коротким)
    const char* suffixes[] = {
        "ization", "ation", "sion", "tion", "ness", "ment",
        "ing", "ed", "er", "est", "ly", "s", "es"
    };
    
    size_t suffix_lengths[] = {7, 5, 4, 4, 4, 4, 3, 2, 2, 3, 2, 1, 2};
    size_t num_suffixes = sizeof(suffixes) / sizeof(suffixes[0]);
    
    // Попытка удалить окончания
    for (size_t i = 0; i < num_suffixes; ++i) {
        size_t suffix_len = suffix_lengths[i];
        if (len > suffix_len + 2) {
            if (ends_with(result, suffixes[i])) {
                result.erase(len - suffix_len);
                break;
            }
        }
    }
    
    return result;
}

bool Stemmer::ends_with(const std::string& word, const std::string& suffix) {
    if (word.length() < suffix.length()) {
        return false;
    }
    
    size_t start = word.length() - suffix.length();
    for (size_t i = 0; i < suffix.length(); ++i) {
        if (word[start + i] != suffix[i]) {
            return false;
        }
    }
    
    return true;
}

std::string Stemmer::remove_suffix(const std::string& word, const std::string& suffix) {
    if (!ends_with(word, suffix)) {
        return word;
    }
    
    return word.substr(0, word.length() - suffix.length());
}
