#ifndef BOOLEAN_SEARCH_H
#define BOOLEAN_SEARCH_H

#include <string>
#include "../index/boolean_index.h"
#include "../utils/vector.h"

/**
 * Лабораторная работа 7: Булев поиск
 * Поиск документов по булевым запросам (AND, OR, NOT)
 */
class BooleanSearch {
public:
    /**
     * Конструктор
     * 
     * @param index ссылка на булев индекс
     */
    BooleanSearch(const BooleanIndex& index);
    
    /**
     * Поиск по запросу
     * 
     * @param query поисковый запрос (например: "word1 AND word2 OR word3 NOT word4")
     * @return список ID документов, соответствующих запросу
     */
    Vector<int> search(const std::string& query) const;
    
    /**
     * Парсинг запроса и выполнение поиска
     * 
     * Поддерживаемые операторы:
     * - AND (и)
     * - OR (или)
     * - NOT (не)
     * 
     * @param query строка запроса
     * @return список ID документов
     */
    Vector<int> parse_and_search(const std::string& query) const;

private:
    const BooleanIndex& index_;
    
    /**
     * Объединение двух списков (OR)
     */
    Vector<int> union_lists(const Vector<int>& list1, 
                           const Vector<int>& list2) const;
    
    /**
     * Пересечение двух списков (AND)
     */
    Vector<int> intersect_lists(const Vector<int>& list1, 
                               const Vector<int>& list2) const;
    
    /**
     * Разность списков (NOT)
     */
    Vector<int> difference_lists(const Vector<int>& list1, 
                                const Vector<int>& list2) const;
    
    /**
     * Сортировка и удаление дубликатов из списка
     */
    Vector<int> normalize_list(const Vector<int>& list) const;
    
    /**
     * Парсинг запроса на токены
     */
    Vector<std::string> tokenize_query(const std::string& query) const;
};

#endif // BOOLEAN_SEARCH_H

