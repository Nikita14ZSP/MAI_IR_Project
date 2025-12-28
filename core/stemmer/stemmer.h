#ifndef STEMMER_H
#define STEMMER_H

#include <string>

/**
 * Лабораторная работа 4: Стемминг
 * Упрощенный стеммер для русского и английского языков
 * Удаление окончаний слов для приведения к базовой форме
 */
class Stemmer {
public:
    /**
     * Стемминг слова (приведение к основе)
     * 
     * @param word исходное слово
     * @return основа слова
     */
    static std::string stem(const std::string& word);
    
    /**
     * Определение языка слова (русский/английский)
     * 
     * @param word слово
     * @return 'r' для русского, 'e' для английского, 'u' для неизвестного
     */
    static char detect_language(const std::string& word);

private:
    /**
     * Стемминг русского слова
     */
    static std::string stem_russian(const std::string& word);
    
    /**
     * Стемминг английского слова (упрощенный алгоритм Портера)
     */
    static std::string stem_english(const std::string& word);
    
    /**
     * Проверка, заканчивается ли слово на суффикс
     */
    static bool ends_with(const std::string& word, const std::string& suffix);
    
    /**
     * Удаление суффикса из слова
     */
    static std::string remove_suffix(const std::string& word, const std::string& suffix);
    
    // TODO: Добавить методы для обработки различных окончаний
};

#endif // STEMMER_H

