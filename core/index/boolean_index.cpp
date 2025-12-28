#include "boolean_index.h"
#include "../tokenizer/tokenizer.h"
#include "../stemmer/stemmer.h"
#include "../utils/file_utils.h"
#include "../utils/sort.h"
#include <fstream>
#include <iostream>
#include <sstream>

static bool compare_int(const int& a, const int& b) {
    return a < b;
}

void BooleanIndex::build(const std::string& corpus_dir) {
    Vector<std::string> files = FileUtils::list_files(corpus_dir);
    
    std::cout << "Построение индекса из " << files.size() << " документов" << std::endl;
    
    // Обработка каждого документа
    for (size_t i = 0; i < files.size(); ++i) {
        int doc_id = static_cast<int>(i + 1);
        
        if ((i + 1) % 100 == 0) {
            std::cout << "Индексировано документов: " << (i + 1) << std::endl;
        }
        
        std::string content = FileUtils::read_file(files[i]);
        if (content.empty()) {
            continue;
        }
        
        add_document(doc_id, content);
    }
    
    std::cout << "Индекс построен. Уникальных слов: " << index_.get_size() << std::endl;
}

void BooleanIndex::add_document(int doc_id, const std::string& content) {
    // Токенизация текста
    std::vector<std::string> tokens = Tokenizer::tokenize(content);
    
    // Множество уникальных слов в документе
    Set<std::string> unique_words;
    
    // Обработка каждого токена
    for (size_t i = 0; i < tokens.size(); ++i) {
        // Стемминг
        std::string stemmed = Stemmer::stem(tokens[i]);
        if (!stemmed.empty()) {
            unique_words.insert(stemmed);
        }
    }
    
    // Добавить слова в индекс
    Vector<std::string> words;
    unique_words.to_vector(words);
    
    for (size_t i = 0; i < words.size(); ++i) {
        const std::string& word = words[i];
        
        // Проверить, есть ли уже это слово в индексе
        Vector<int> doc_list;
        index_.find(word, doc_list);
        
        // Добавить doc_id в список (если его еще нет)
        bool found = false;
        for (size_t j = 0; j < doc_list.size(); ++j) {
            if (doc_list[j] == doc_id) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            doc_list.push_back(doc_id);
            index_.insert(word, doc_list);
        } else {
            // Уже есть, обновить список (хотя он не изменился)
            index_.insert(word, doc_list);
        }
    }
    
    // Записать doc_id в множество документов
    document_ids_.insert(doc_id);
}

Vector<int> BooleanIndex::get_documents(const std::string& word) const {
    // Применить стемминг к слову
    std::string stemmed = Stemmer::stem(word);
    
    Vector<int> doc_list;
    index_.find(stemmed, doc_list);  // Получить список (может быть пустым)
    
    // Сортировка списка
    if (doc_list.size() > 0) {
        Sort<int>::quicksort(doc_list, compare_int);
    }
    
    return doc_list;
}

void BooleanIndex::save(const std::string& filepath) const {
    std::ofstream out(filepath);
    if (!out.is_open()) {
        std::cerr << "Ошибка открытия файла для записи: " << filepath << std::endl;
        return;
    }
    
    Vector<std::string> keys;
    index_.get_keys(keys);
    
    for (size_t i = 0; i < keys.size(); ++i) {
        const std::string& word = keys[i];
        Vector<int> doc_list;
        
        index_.find(word, doc_list);  // Получить список
        
        out << word << "\t";
        
        for (size_t j = 0; j < doc_list.size(); ++j) {
            if (j > 0) out << ",";
            out << doc_list[j];
        }
        out << "\n";
    }
    
    out.close();
}

void BooleanIndex::load(const std::string& filepath) {
    std::ifstream in(filepath);
    if (!in.is_open()) {
        std::cerr << "Ошибка открытия файла для чтения: " << filepath << std::endl;
        return;
    }
    
    index_.clear();
    document_ids_.clear();
    
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        
        // Разделить по табуляции
        size_t tab_pos = line.find('\t');
        if (tab_pos == std::string::npos) continue;
        
        std::string word = line.substr(0, tab_pos);
        std::string doc_list_str = line.substr(tab_pos + 1);
        
        // Разобрать список ID
        Vector<int> doc_list;
        std::stringstream ss(doc_list_str);
        std::string id_str;
        
        while (std::getline(ss, id_str, ',')) {
            if (!id_str.empty()) {
                int doc_id = std::stoi(id_str);
                doc_list.push_back(doc_id);
                document_ids_.insert(doc_id);
            }
        }
        
        index_.insert(word, doc_list);
    }
    
    in.close();
}

BooleanIndex::IndexStats BooleanIndex::get_stats() const {
    IndexStats stats;
    stats.total_words = index_.get_size();
    stats.total_documents = document_ids_.size();
    
    size_t total_postings = 0;
    Vector<std::string> keys;
    index_.get_keys(keys);
    
    for (size_t i = 0; i < keys.size(); ++i) {
        Vector<int> doc_list;
        index_.find(keys[i], doc_list);
        total_postings += doc_list.size();
    }
    
    stats.total_postings = total_postings;
    
    return stats;
}

Vector<std::string> BooleanIndex::get_all_words() const {
    Vector<std::string> words;
    index_.get_keys(words);
    return words;
}
