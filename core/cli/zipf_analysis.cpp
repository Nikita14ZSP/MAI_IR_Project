#include <iostream>
#include <string>
#include "../analysis/zipf_analyzer.h"

int main(int argc, char* argv[]) {
    // TODO: Утилита для анализа закона Ципфа
    // Использование: ./zipf_analysis <corpus_dir> <output_csv>
    
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <corpus_dir> <output_csv>" << std::endl;
        std::cerr << "  corpus_dir - директория с документами корпуса" << std::endl;
        std::cerr << "  output_csv - путь к выходному CSV файлу с результатами" << std::endl;
        return 1;
    }
    
    std::string corpus_dir = argv[1];
    std::string output_csv = argv[2];
    
    std::cout << "Анализ закона Ципфа для корпуса: " << corpus_dir << std::endl;
    std::cout << "Выходной файл: " << output_csv << std::endl;
    std::cout << std::endl;
    
    // Анализ корпуса
    std::vector<ZipfAnalyzer::WordFrequency> frequencies = 
        ZipfAnalyzer::analyze_corpus(corpus_dir);
    
    // Сохранение результатов
    std::cout << "Сохранение результатов..." << std::endl;
    ZipfAnalyzer::save_to_csv(frequencies, output_csv);
    
    // Вывод топ-10 слов
    std::cout << std::endl;
    std::cout << "Топ-10 самых частых слов:" << std::endl;
    size_t top_n = std::min(static_cast<size_t>(10), frequencies.size());
    for (size_t i = 0; i < top_n; ++i) {
        const auto& wf = frequencies[i];
        std::cout << "  " << (i + 1) << ". " << wf.word 
                  << " (частота: " << wf.frequency 
                  << ", ранг: " << wf.rank
                  << ", Ципф: " << wf.zipf_value << ")" << std::endl;
    }
    
    std::cout << std::endl;
    std::cout << "Всего уникальных слов: " << frequencies.size() << std::endl;
    std::cout << "Результаты сохранены в: " << output_csv << std::endl;
    
    return 0;
}

