#include "zipf_analyzer.h"
#include "../tokenizer/tokenizer.h"
#include "../stemmer/stemmer.h"
#include "../utils/file_utils.h"
#include "../utils/sort.h"
#include <algorithm> // для std::sort
#include <fstream>
#include <iostream>

struct WordFreqPair {
    std::string word;
    int frequency;
    
    WordFreqPair() : word(""), frequency(0) {}  // Добавляем конструктор по умолчанию
    WordFreqPair(const std::string& w, int f) : word(w), frequency(f) {}
};

static bool compare_by_frequency_desc(const WordFreqPair& a, const WordFreqPair& b) {
    return a.frequency > b.frequency;
}

std::vector<ZipfAnalyzer::WordFrequency> ZipfAnalyzer::analyze_corpus(const std::string& corpus_dir) {
    Map<std::string, int> total_frequencies;
    
    Vector<std::string> files = FileUtils::list_files(corpus_dir);
    
    std::cout << "Анализ корпуса: " << files.size() << " файлов" << std::endl;
    
    // Анализ каждого документа
    for (size_t i = 0; i < files.size(); ++i) {
        if ((i + 1) % 100 == 0) {
            std::cout << "Обработано файлов: " << (i + 1) << std::endl;
        }
        
        Map<std::string, int> doc_freq = analyze_document(files[i]);
        merge_frequencies(total_frequencies, doc_freq);
    }
    
    // Преобразование в вектор
    std::vector<WordFreqPair> pairs;
    Vector<std::string> keys;
    total_frequencies.get_keys(keys);
    
    for (size_t i = 0; i < keys.size(); ++i) {
        int freq = 0;
        total_frequencies.find(keys[i], freq);
        pairs.push_back(WordFreqPair(keys[i], freq));
    }
    
    // Сортировка по убыванию частоты
    std::sort(pairs.begin(), pairs.end(), 
              [](const WordFreqPair& a, const WordFreqPair& b) { 
                  return a.frequency > b.frequency; 
              });
    
    // Преобразование в WordFrequency
    std::vector<WordFrequency> frequencies;
    frequencies.resize(pairs.size());
    
    for (size_t i = 0; i < pairs.size(); ++i) {
        frequencies[i].word = pairs[i].word;
        frequencies[i].frequency = pairs[i].frequency;
        frequencies[i].rank = static_cast<int>(i + 1);
        frequencies[i].zipf_value = static_cast<double>(frequencies[i].frequency) * 
                                    static_cast<double>(frequencies[i].rank);
    }
    
    return frequencies;
}

Map<std::string, int> ZipfAnalyzer::analyze_document(const std::string& filepath) {
    Map<std::string, int> frequencies;
    
    std::string content = FileUtils::read_file(filepath);
    if (content.empty()) {
        return frequencies;
    }
    
    // Токенизация
    std::vector<std::string> tokens = Tokenizer::tokenize(content);
    
    // Подсчет частот (со стеммингом)
    for (size_t i = 0; i < tokens.size(); ++i) {
        std::string stemmed = Stemmer::stem(tokens[i]);
        if (!stemmed.empty()) {
            int count = 0;
            frequencies.find(stemmed, count);
            frequencies.insert(stemmed, count + 1);
        }
    }
    
    return frequencies;
}

void ZipfAnalyzer::save_to_csv(const std::vector<WordFrequency>& frequencies, 
                               const std::string& output_path) {
    std::ofstream out(output_path);
    if (!out.is_open()) {
        std::cerr << "Ошибка открытия файла: " << output_path << std::endl;
        return;
    }
    
    // Заголовок
    out << "rank,frequency,word,zipf_value\n";
    
    // Данные
    for (size_t i = 0; i < frequencies.size(); ++i) {
        const WordFrequency& wf = frequencies[i];
        out << wf.rank << "," << wf.frequency << ",\"" << wf.word << "\"," 
            << wf.zipf_value << "\n";
    }
    
    out.close();
}

void ZipfAnalyzer::assign_ranks(std::vector<WordFrequency>& frequencies) {
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequencies[i].rank = static_cast<int>(i + 1);
    }
}

void ZipfAnalyzer::calculate_zipf_values(std::vector<WordFrequency>& frequencies) {
    for (size_t i = 0; i < frequencies.size(); ++i) {
        frequencies[i].zipf_value = static_cast<double>(frequencies[i].frequency) * 
                                   static_cast<double>(frequencies[i].rank);
    }
}

void ZipfAnalyzer::merge_frequencies(Map<std::string, int>& total, 
                                     const Map<std::string, int>& doc_freq) {
    Vector<std::string> keys;
    doc_freq.get_keys(keys);
    
    for (size_t i = 0; i < keys.size(); ++i) {
        int freq = 0;
        doc_freq.find(keys[i], freq);
        int total_freq = 0;
        total.find(keys[i], total_freq);
        total.insert(keys[i], total_freq + freq);
    }
}
