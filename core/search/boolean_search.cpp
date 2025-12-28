#include "boolean_search.h"
#include "../utils/sort.h"
#include <sstream>
#include <cctype>

static bool compare_int(const int& a, const int& b) {
    return a < b;
}

BooleanSearch::BooleanSearch(const BooleanIndex& index) : index_(index) {
}

Vector<int> BooleanSearch::search(const std::string& query) const {
    return parse_and_search(query);
}

Vector<int> BooleanSearch::parse_and_search(const std::string& query) const {
    // Токенизация запроса
    Vector<std::string> tokens = tokenize_query(query);
    
    if (tokens.empty()) {
        return Vector<int>();
    }
    
    // Обработка простого случая (без операторов)
    if (tokens.size() == 1) {
        return index_.get_documents(tokens[0]);
    }
    
    // Упрощенная версия: обработка последовательно слева направо
    Vector<int> result;
    bool is_first = true;
    std::string current_op = "OR";  // По умолчанию OR
    
    for (size_t i = 0; i < tokens.size(); ++i) {
        const std::string& token = tokens[i];
        
        // Проверка на оператор
        if (token == "AND" || token == "OR" || token == "NOT") {
            current_op = token;
            continue;
        }
        
        // Получить документы для слова
        Vector<int> word_docs = index_.get_documents(token);
        
        if (is_first) {
            result = word_docs;
            is_first = false;
        } else {
            // Применить оператор
            if (current_op == "AND") {
                result = intersect_lists(result, word_docs);
            } else if (current_op == "OR") {
                result = union_lists(result, word_docs);
            } else if (current_op == "NOT") {
                result = difference_lists(result, word_docs);
            }
        }
    }
    
    return normalize_list(result);
}

Vector<int> BooleanSearch::union_lists(const Vector<int>& list1, 
                                       const Vector<int>& list2) const {
    Vector<int> result = list1;
    
    // Добавить элементы из list2, которых нет в result
    for (size_t i = 0; i < list2.size(); ++i) {
        int doc_id = list2[i];
        bool found = false;
        
        for (size_t j = 0; j < result.size(); ++j) {
            if (result[j] == doc_id) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            result.push_back(doc_id);
        }
    }
    
    Sort<int>::quicksort(result, compare_int);
    return result;
}

Vector<int> BooleanSearch::intersect_lists(const Vector<int>& list1, 
                                           const Vector<int>& list2) const {
    Vector<int> result;
    
    for (size_t i = 0; i < list1.size(); ++i) {
        int doc_id = list1[i];
        
        for (size_t j = 0; j < list2.size(); ++j) {
            if (list2[j] == doc_id) {
                result.push_back(doc_id);
                break;
            }
        }
    }
    
    Sort<int>::quicksort(result, compare_int);
    return result;
}

Vector<int> BooleanSearch::difference_lists(const Vector<int>& list1, 
                                            const Vector<int>& list2) const {
    Vector<int> result;
    
    for (size_t i = 0; i < list1.size(); ++i) {
        int doc_id = list1[i];
        bool found = false;
        
        for (size_t j = 0; j < list2.size(); ++j) {
            if (list2[j] == doc_id) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            result.push_back(doc_id);
        }
    }
    
    Sort<int>::quicksort(result, compare_int);
    return result;
}

Vector<int> BooleanSearch::normalize_list(const Vector<int>& list) const {
    if (list.empty()) {
        return list;
    }
    
    // Удаление дубликатов
    Vector<int> result;
    for (size_t i = 0; i < list.size(); ++i) {
        bool found = false;
        for (size_t j = 0; j < result.size(); ++j) {
            if (result[j] == list[i]) {
                found = true;
                break;
            }
        }
        if (!found) {
            result.push_back(list[i]);
        }
    }
    
    Sort<int>::quicksort(result, compare_int);
    return result;
}

Vector<std::string> BooleanSearch::tokenize_query(const std::string& query) const {
    Vector<std::string> tokens;
    std::stringstream ss(query);
    std::string token;
    
    while (ss >> token) {
        // Преобразовать к верхнему регистру для операторов
        std::string upper_token = token;
        for (size_t i = 0; i < upper_token.length(); ++i) {
            upper_token[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(upper_token[i])));
        }
        
        if (upper_token == "AND" || upper_token == "OR" || upper_token == "NOT") {
            tokens.push_back(upper_token);
        } else {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}
