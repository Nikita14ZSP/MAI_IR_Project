#include "search_cli.h"
#include "../search/boolean_search.h"
#include <iostream>
#include <string>

SearchCLI::SearchCLI(const BooleanSearch& search_engine) : search_engine_(search_engine) {
}

void SearchCLI::process_query(const std::string& query) {
    if (query.empty()) {
        return;
    }
    
    std::cout << "Поиск: " << query << std::endl;
    
    Vector<int> results = search_engine_.search(query);
    
    print_results(results);
}

void SearchCLI::interactive_mode() {
    std::cout << "Введите поисковый запрос (или 'quit' для выхода):" << std::endl;
    
    std::string query;
    while (std::getline(std::cin, query)) {
        if (query == "quit" || query == "exit" || query == "q") {
            break;
        }
        
        if (!query.empty()) {
            process_query(query);
            std::cout << "\nВведите следующий запрос:" << std::endl;
        }
    }
}

void SearchCLI::print_results(const Vector<int>& doc_ids) {
    std::cout << "Найдено документов: " << doc_ids.size() << std::endl;
    
    if (doc_ids.empty()) {
        std::cout << "Документы не найдены." << std::endl;
        return;
    }
    
    // Вывод первых 10 результатов (как требуется)
    size_t max_results = doc_ids.size() < 10 ? doc_ids.size() : 10;
    
    std::cout << "Первые " << max_results << " результатов:" << std::endl;
    for (size_t i = 0; i < max_results; ++i) {
        std::cout << doc_ids[i];
        if (i < max_results - 1) {
            std::cout << ", ";
        }
    }
    
    if (doc_ids.size() > 10) {
        std::cout << "\n... (всего " << doc_ids.size() << " результатов)" << std::endl;
    } else {
        std::cout << std::endl;
    }
}
