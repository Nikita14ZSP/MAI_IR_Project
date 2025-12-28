#ifndef BOOLEAN_INDEX_H
#define BOOLEAN_INDEX_H

#include <string>
#include "../utils/vector.h"
#include "../utils/map.h"
#include "../utils/set.h"

// TODO: Заменить std::map и std::vector на собственные структуры данных

/**
 * Лабораторная работа 6: Булев индекс
 * Инвертированный индекс для булева поиска
 * 
 * Структура: слово -> список ID документов, содержащих это слово
 */
class BooleanIndex {
public:
    /**
     * Построение индекса из корпуса документов
     * 
     * @param corpus_dir директория с документами
     */
    void build(const std::string& corpus_dir);
    
    /**
     * Добавление документа в индекс
     * 
     * @param doc_id идентификатор документа
     * @param content содержимое документа
     */
    void add_document(int doc_id, const std::string& content);
    
    /**
     * Получение списка ID документов для слова
     * 
     * @param word слово
     * @return список ID документов (пустой, если слова нет в индексе)
     */
    Vector<int> get_documents(const std::string& word) const;
    
    /**
     * Сохранение индекса в файл
     * 
     * @param filepath путь к файлу
     */
    void save(const std::string& filepath) const;
    
    /**
     * Загрузка индекса из файла
     * 
     * @param filepath путь к файлу
     */
    void load(const std::string& filepath);
    
    /**
     * Получение статистики индекса
     */
    struct IndexStats {
        size_t total_words;      // Количество уникальных слов
        size_t total_documents;  // Количество документов
        size_t total_postings;   // Общее количество записей
    };
    
    IndexStats get_stats() const;
    
    /**
     * Получение всех слов в индексе
     */
    Vector<std::string> get_all_words() const;

private:
    // Инвертированный индекс: слово -> список ID документов
    Map<std::string, Vector<int>> index_;
    
    // Множество ID документов (для статистики)
    Set<int> document_ids_;
};

#endif // BOOLEAN_INDEX_H

