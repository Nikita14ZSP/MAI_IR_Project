#ifndef VECTOR_H
#define VECTOR_H

#include <cstdlib>
#include <cstring>

/**
 * Собственная реализация динамического массива (аналог std::vector)
 * Без использования STL
 */
template<typename T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    void expand() {
        if (capacity_ == 0) {
            capacity_ = 1;
        }
        size_t new_capacity = capacity_ * 2;
        T* new_data = new T[new_capacity];
        
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data_[i];
        }
        
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}
    
    ~Vector() {
        delete[] data_;
    }
    
    Vector(const Vector& other) : data_(nullptr), size_(0), capacity_(0) {
        reserve(other.size_);
        size_ = other.size_;
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }
    
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            data_ = nullptr;
            size_ = 0;
            capacity_ = 0;
            reserve(other.size_);
            size_ = other.size_;
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }
    
    void push_back(const T& value) {
        if (size_ >= capacity_) {
            expand();
        }
        data_[size_++] = value;
    }
    
    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }
    
    T& operator[](size_t index) {
        return data_[index];
    }
    
    const T& operator[](size_t index) const {
        return data_[index];
    }
    
    T& back() {
        // Предполагается, что вызывается только когда size_ > 0
        return data_[size_ - 1];
    }
    
    const T& back() const {
        return data_[size_ - 1];
    }
    
    size_t size() const {
        return size_;
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    void clear() {
        size_ = 0;
    }
    
    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            T* new_data = new T[new_capacity];
            for (size_t i = 0; i < size_; ++i) {
                new_data[i] = data_[i];
            }
            delete[] data_;
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }
    
    void resize(size_t new_size) {
        if (new_size > capacity_) {
            reserve(new_size);
        }
        size_ = new_size;
    }
    
    T* begin() {
        return data_;
    }
    
    const T* begin() const {
        return data_;
    }
    
    T* end() {
        return data_ + size_;
    }
    
    const T* end() const {
        return data_ + size_;
    }
};

#endif // VECTOR_H

