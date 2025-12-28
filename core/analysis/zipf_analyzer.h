#ifndef ZIPF_ANALYZER_H
#define ZIPF_ANALYZER_H

#include <string>
#include "../utils/vector.h"
#include "../utils/map.h"

// TODO: Использовать собственную структуру данных вместо std::map
// Для соответствия требованиям "без STL"

/**
 * Лабораторная работа 5: Закон Ципфа
 * Анализ частотности слов и построение графика закона Ципфа
 */
class ZipfAnalyzer {
public:
    /**
     * Структура для хранения частоты слова
     */
    struct WordFrequency {
        std::string word;
        int frequency;
        int rank;
        double zipf_value;  // frequency * rank (должно быть примерно константой)
    };
    
    /**
     * Анализ корпуса документов
     * 
     * @param corpus_dir директория с документами
     * @return вектор частот слов, отсортированный по убыванию частоты
     */
    static std::vector<WordFrequency> analyze_corpus(const std::string& corpus_dir);
    
    /**
     * Анализ одного документа
     * 
     * @param filepath путь к файлу
     * @return словарь частот слов
     */
    static Map<std::string, int> analyze_document(const std::string& filepath);
    
    /**
     * Сохранение результатов в CSV для построения графика
     * 
     * @param frequencies вектор частот
     * @param output_path путь к выходному файлу
     */
    static void save_to_csv(const std::vector<WordFrequency>& frequencies, 
                           const std::string& output_path);
    
    /**
     * Вычисление ранга слова по частоте
     */
    static void assign_ranks(std::vector<WordFrequency>& frequencies);
    
    /**
     * Вычисление значения закона Ципфа (frequency * rank)
     */
    static void calculate_zipf_values(std::vector<WordFrequency>& frequencies);

private:
    static void merge_frequencies(Map<std::string, int>& total, 
                                  const Map<std::string, int>& doc_freq);
};

#endif // ZIPF_ANALYZER_H

