#ifndef MAP_H
#define MAP_H

#include "vector.h"
#include <cstring>
#include <string>

/**
 * Собственная реализация ассоциативного массива (аналог std::map)
 * Использует простую хеш-таблицу с разрешением коллизий методом цепочек
 */
template<typename Key, typename Value>
class Map {
private:
    struct Node {
        Key key;
        Value value;
        Node* next;
        
        Node(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
    };
    
    static const size_t INITIAL_BUCKETS = 16;
    Node** buckets_;
    size_t bucket_count_;
    size_t size_;
    
    size_t hash(const Key& key) const {
        // Хеш-функция для строк
        return hash_string(key);
    }
    
    size_t hash_string(const std::string& key) const {
        size_t h = 0;
        for (size_t i = 0; i < key.length(); ++i) {
            h = h * 31 + static_cast<unsigned char>(key[i]);
        }
        return h % bucket_count_;
    }
    
    void rehash() {
        size_t old_bucket_count = bucket_count_;
        bucket_count_ *= 2;
        Node** new_buckets = new Node*[bucket_count_]();
        
        for (size_t i = 0; i < old_bucket_count; ++i) {
            Node* node = buckets_[i];
            while (node) {
                Node* next = node->next;
                size_t new_bucket = hash(node->key);
                node->next = new_buckets[new_bucket];
                new_buckets[new_bucket] = node;
                node = next;
            }
        }
        
        delete[] buckets_;
        buckets_ = new_buckets;
    }
    
public:
    Map() : bucket_count_(INITIAL_BUCKETS), size_(0) {
        buckets_ = new Node*[bucket_count_]();
        for (size_t i = 0; i < bucket_count_; ++i) {
            buckets_[i] = nullptr;
        }
    }
    
    ~Map() {
        clear();
        delete[] buckets_;
    }
    
    Map(const Map& other) : bucket_count_(other.bucket_count_), size_(0) {
        buckets_ = new Node*[bucket_count_]();
        for (size_t i = 0; i < bucket_count_; ++i) {
            buckets_[i] = nullptr;
        }
        for (size_t i = 0; i < bucket_count_; ++i) {
            Node* node = other.buckets_[i];
            while (node) {
                insert(node->key, node->value);
                node = node->next;
            }
        }
    }
    
    void clear() {
        for (size_t i = 0; i < bucket_count_; ++i) {
            Node* node = buckets_[i];
            while (node) {
                Node* next = node->next;
                delete node;
                node = next;
            }
            buckets_[i] = nullptr;
        }
        size_ = 0;
    }
    
    void insert(const Key& key, const Value& value) {
        if (size_ >= bucket_count_ * 2) {
            rehash();
        }
        
        size_t bucket = hash(key);
        Node* node = buckets_[bucket];
        
        while (node) {
            if (node->key == key) {
                node->value = value;
                return;
            }
            node = node->next;
        }
        
        Node* new_node = new Node(key, value);
        new_node->next = buckets_[bucket];
        buckets_[bucket] = new_node;
        ++size_;
    }
    
    bool find(const Key& key, Value& value) const {
        size_t bucket = hash(key);
        Node* node = buckets_[bucket];
        
        while (node) {
            if (node->key == key) {
                value = node->value;
                return true;
            }
            node = node->next;
        }
        
        value = Value{};  // Инициализировать значением по умолчанию
        return false;
    }
    
    Value& operator[](const Key& key) {
        size_t bucket = hash(key);
        Node* node = buckets_[bucket];
        
        while (node) {
            if (node->key == key) {
                return node->value;
            }
            node = node->next;
        }
        
        // Создать новый узел
        Node* new_node = new Node(key, Value{});
        new_node->next = buckets_[bucket];
        buckets_[bucket] = new_node;
        ++size_;
        
        return new_node->value;
    }
    
    bool contains(const Key& key) const {
        size_t bucket = hash(key);
        Node* node = buckets_[bucket];
        
        while (node) {
            if (node->key == key) {
                return true;
            }
            node = node->next;
        }
        
        return false;
    }
    
    size_t get_size() const {
        return size_;
    }
    
    bool empty() const {
        return size_ == 0;
    }
    
    void get_keys(Vector<Key>& keys) const {
        keys.clear();
        for (size_t i = 0; i < bucket_count_; ++i) {
            Node* node = buckets_[i];
            while (node) {
                keys.push_back(node->key);
                node = node->next;
            }
        }
    }
};

#endif // MAP_H
