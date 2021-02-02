//
// Created by justnik on 02.02.2021.
//

#ifndef LRU_LRU_H
#define LRU_LRU_H

#include <cstddef>

template<class T>
class lru {
private:
    std::list<T> list_;
    std::unordered_map<T, typename std::list<T>::iterator, Hash, Equal> map_;
    std::size_t max_size;
public:
    lru(const std::size_t &maxSize);

    bool has(const T &key);

    bool put
};

template<class T>
lru<T>::lru(const std::size_t &maxSize):max_size(maxSize) {}




#endif //LRU_LRU_H
