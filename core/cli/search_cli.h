#ifndef SEARCH_CLI_H
#define SEARCH_CLI_H

#include <string>
#include "../utils/vector.h"

// Forward declaration
class BooleanSearch;

/**
 * CLI интерфейс для поиска
 */
class SearchCLI {
public:
    /**
     * Конструктор
     */
    SearchCLI(const BooleanSearch& search_engine);
    
    /**
     * Обработка запроса и вывод результатов
     */
    void process_query(const std::string& query);
    
    /**
     * Интерактивный режим (чтение из stdin)
     */
    void interactive_mode();
    
private:
    /**
     * Форматированный вывод результатов
     */
    void print_results(const Vector<int>& doc_ids);
    
    const BooleanSearch& search_engine_;
};

#endif // SEARCH_CLI_H
