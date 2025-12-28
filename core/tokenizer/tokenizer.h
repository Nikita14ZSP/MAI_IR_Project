#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include "../utils/vector.h"

// TODO: Можно использовать STL для токенизации согласно требованиям

/**
 * Лабораторная работа 3: Токенизация
 * Разбиение текста на слова (токены)
 */
class Tokenizer {
public:
    /**
     * Токенизация текста на слова
     * 
     * @param text исходный текст в UTF-8
     * @return вектор токенов (слов)
     */
    static std::vector<std::string> tokenize(const std::string& text);
    
    /**
     * Токенизация с удалением знаков препинания
     * 
     * @param text исходный текст
     * @param remove_punctuation удалять ли знаки препинания
     * @return вектор токенов
     */
    static std::vector<std::string> tokenize(const std::string& text, bool remove_punctuation);
    
    /**
     * Нормализация токена (приведение к нижнему регистру)
     * 
     * @param token исходный токен
     * @return нормализованный токен
     */
    static std::string normalize(const std::string& token);
    
    /**
     * Проверка, является ли символ буквой (включая UTF-8)
     * 
     * @param c символ
     * @return true если буква
     */
    static bool is_letter(unsigned char c);
    
    /**
     * Проверка, является ли символ знаком препинания
     * 
     * @param c символ
     * @return true если знак препинания
     */
    static bool is_punctuation(unsigned char c);

private:
    // TODO: Добавить вспомогательные методы при необходимости
};

#endif // TOKENIZER_H

