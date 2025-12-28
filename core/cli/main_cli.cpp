#include <iostream>
#include <string>
#include "search_cli.h"
#include "../index/boolean_index.h"
#include "../search/boolean_search.h"
#include "../utils/vector.h"

int main(int argc, char* argv[]) {
    // TODO: Обработка аргументов командной строки
    // Использование: ./search_cli <index_path> [query]
    
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <index_path> [query]" << std::endl;
        std::cerr << "  index_path - путь к файлу индекса" << std::endl;
        std::cerr << "  query - поисковый запрос (опционально, если не указан - интерактивный режим)" << std::endl;
        return 1;
    }
    
    std::string index_path = argv[1];
    
    // Загрузка индекса
    BooleanIndex index;
    std::cout << "Загрузка индекса из: " << index_path << std::endl;
    index.load(index_path);
    
    BooleanIndex::IndexStats stats = index.get_stats();
    std::cout << "Индекс загружен:" << std::endl;
    std::cout << "  Уникальных слов: " << stats.total_words << std::endl;
    std::cout << "  Документов: " << stats.total_documents << std::endl;
    std::cout << "  Всего записей: " << stats.total_postings << std::endl;
    
    // Создание поискового движка
    BooleanSearch search_engine(index);
    SearchCLI cli(search_engine);
    
    // Если указан запрос - выполнить поиск, иначе - интерактивный режим
    if (argc >= 3) {
        std::string query = argv[2];
        for (int i = 3; i < argc; ++i) {
            query += " " + std::string(argv[i]);
        }
        cli.process_query(query);
    } else {
        cli.interactive_mode();
    }
    
    return 0;
}

