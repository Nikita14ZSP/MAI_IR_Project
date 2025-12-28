#include <iostream>
#include <string>
#include "../index/boolean_index.h"

int main(int argc, char* argv[]) {
    // TODO: Утилита для построения индекса
    // Использование: ./build_index <corpus_dir> <index_path>
    
    if (argc < 3) {
        std::cerr << "Использование: " << argv[0] << " <corpus_dir> <index_path>" << std::endl;
        std::cerr << "  corpus_dir - директория с документами корпуса" << std::endl;
        std::cerr << "  index_path - путь к выходному файлу индекса" << std::endl;
        return 1;
    }
    
    std::string corpus_dir = argv[1];
    std::string index_path = argv[2];
    
    // Создать директорию для индекса если нужно
    size_t last_slash = index_path.find_last_of("/\\");
    if (last_slash != std::string::npos) {
        std::string index_dir = index_path.substr(0, last_slash);
        // Создать директорию (упрощенная версия)
        system(("mkdir -p " + index_dir).c_str());
    }
    
    BooleanIndex index;
    
    std::cout << "Построение индекса из корпуса: " << corpus_dir << std::endl;
    std::cout << "Выходной файл: " << index_path << std::endl;
    std::cout << std::endl;
    
    // Построение индекса
    index.build(corpus_dir);
    
    // Сохранение индекса
    std::cout << "Сохранение индекса..." << std::endl;
    index.save(index_path);
    
    // Вывод статистики
    BooleanIndex::IndexStats stats = index.get_stats();
    std::cout << std::endl;
    std::cout << "Индекс построен:" << std::endl;
    std::cout << "  Уникальных слов: " << stats.total_words << std::endl;
    std::cout << "  Документов: " << stats.total_documents << std::endl;
    std::cout << "  Всего записей: " << stats.total_postings << std::endl;
    std::cout << "  Сохранен в: " << index_path << std::endl;
    
    return 0;
}

