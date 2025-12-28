#include "file_utils.h"
#include <fstream>
#include <sstream>
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>

// Для токенизации разрешен STL
#include <sstream>
#include <algorithm>

std::string FileUtils::read_file(const std::string& filepath) {
    // TODO: Чтение файла в UTF-8
    std::ifstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return "";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool FileUtils::write_file(const std::string& filepath, const std::string& content) {
    // TODO: Запись файла в UTF-8
    std::ofstream file(filepath, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }
    
    file << content;
    file.close();
    return true;
}

Vector<std::string> FileUtils::list_files(const std::string& dir_path) {
    Vector<std::string> files;
    
    DIR* dir = opendir(dir_path.c_str());
    if (!dir) {
        return files;
    }
    
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        if (entry->d_name[0] == '.') {
            continue;  // Пропустить скрытые файлы
        }
        
        std::string filepath = dir_path + "/" + std::string(entry->d_name);
        
        // Проверить, что это файл
        struct stat file_stat;
        if (stat(filepath.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            files.push_back(filepath);
        }
    }
    
    closedir(dir);
    return files;
}

bool FileUtils::file_exists(const std::string& filepath) {
    struct stat buffer;
    return (stat(filepath.c_str(), &buffer) == 0);
}

std::string FileUtils::get_filename(const std::string& filepath) {
    size_t pos = filepath.find_last_of("/\\");
    if (pos != std::string::npos) {
        return filepath.substr(pos + 1);
    }
    return filepath;
}

