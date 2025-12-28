#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <string>
#include "../utils/vector.h"

/**
 * Вспомогательные функции для работы с файлами
 */
class FileUtils {
public:
    /**
     * Чтение файла в строку (UTF-8)
     */
    static std::string read_file(const std::string& filepath);
    
    /**
     * Запись строки в файл (UTF-8)
     */
    static bool write_file(const std::string& filepath, const std::string& content);
    
    /**
     * Получение списка всех файлов в директории
     */
    static Vector<std::string> list_files(const std::string& dir_path);
    
    /**
     * Проверка существования файла
     */
    static bool file_exists(const std::string& filepath);
    
    /**
     * Получение имени файла без пути
     */
    static std::string get_filename(const std::string& filepath);
};

#endif // FILE_UTILS_H

