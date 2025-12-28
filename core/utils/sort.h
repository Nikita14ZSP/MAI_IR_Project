#ifndef SORT_H
#define SORT_H

#include "vector.h"
#include <functional>

/**
 * Алгоритмы сортировки (без STL)
 */
template<typename T>
class Sort {
public:
    /**
     * Быстрая сортировка (Quick Sort)
     */
    static void quicksort(Vector<T>& arr, int left, int right, 
                         bool (*compare)(const T&, const T&)) {
        if (left < right) {
            int pivot = partition(arr, left, right, compare);
            quicksort(arr, left, pivot - 1, compare);
            quicksort(arr, pivot + 1, right, compare);
        }
    }
    
    static void quicksort(Vector<T>& arr, bool (*compare)(const T&, const T&)) {
        if (arr.size() > 0) {
            quicksort(arr, 0, static_cast<int>(arr.size()) - 1, compare);
        }
    }
    
private:
    static int partition(Vector<T>& arr, int left, int right, 
                        bool (*compare)(const T&, const T&)) {
        T pivot = arr[right];
        int i = left - 1;
        
        for (int j = left; j < right; ++j) {
            if (compare(arr[j], pivot)) {
                ++i;
                T temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
        
        T temp = arr[i + 1];
        arr[i + 1] = arr[right];
        arr[right] = temp;
        
        return i + 1;
    }
};

#endif // SORT_H

