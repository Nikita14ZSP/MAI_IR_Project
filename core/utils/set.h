#ifndef SET_H
#define SET_H

#include "vector.h"
#include <cstring>

/**
 * Собственная реализация множества (аналог std::set)
 * Использует простой массив с проверкой уникальности
 */
template<typename T>
class Set {
private:
    Vector<T> data_;
    
    int find_index(const T& value) const {
        for (size_t i = 0; i < data_.size(); ++i) {
            if (data_[i] == value) {
                return static_cast<int>(i);
            }
        }
        return -1;
    }

public:
    Set() {}
    
    void insert(const T& value) {
        if (find_index(value) == -1) {
            data_.push_back(value);
        }
    }
    
    bool contains(const T& value) const {
        return find_index(value) != -1;
    }
    
    void erase(const T& value) {
        int index = find_index(value);
        if (index >= 0) {
            for (size_t i = static_cast<size_t>(index); i < data_.size() - 1; ++i) {
                data_[i] = data_[i + 1];
            }
            data_.pop_back();
        }
    }
    
    size_t size() const {
        return data_.size();
    }
    
    bool empty() const {
        return data_.empty();
    }
    
    void clear() {
        data_.clear();
    }
    
    void to_vector(Vector<T>& vec) const {
        vec = data_;
    }
    
    const Vector<T>& get_data() const {
        return data_;
    }
};

#endif // SET_H

