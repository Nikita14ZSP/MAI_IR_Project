#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include "vector.h"

/**
 * Вспомогательные функции для работы со строками
 */
class StringUtils {
public:
    /**
     * Разделение строки по разделителю
     */
    static Vector<std::string> split(const std::string& str, char delimiter);
    
    /**
     * Объединение строк через разделитель
     */
    static std::string join(const Vector<std::string>& parts, const std::string& delimiter);
    
    /**
     * Удаление пробелов в начале и конце строки
     */
    static std::string trim(const std::string& str);
    
    /**
     * Преобразование в нижний регистр
     */
    static std::string to_lower(const std::string& str);
    
    /**
     * Проверка, начинается ли строка с подстроки
     */
    static bool starts_with(const std::string& str, const std::string& prefix);
    
    /**
     * Проверка, заканчивается ли строка на подстроку
     */
    static bool ends_with(const std::string& str, const std::string& suffix);
};

#endif // STRING_UTILS_H
