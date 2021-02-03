//
// Created by justnik on 02.02.2021.
//

#ifndef LRU_LRU_H
#define LRU_LRU_H

#include <cstddef>

template<class T>
class lru {
private:
    std::list<T> list;
    std::unordered_map<T, typename std::list<T>::iterator, Hash, Equal> map;
    std::size_t max_size;
public:
    lru(const std::size_t &maxSize);

    bool has(const T &key);
};

template<class T>
lru<T>::lru(const std::size_t &maxSize):max_size(maxSize) {}

template<class T>
bool lru<T>::has(const T &key) {
    auto it = map.find(key);
    if (it == map.end()) return false;

    auto list_it = it->second;
    list.erase(list_it);
    it->second = list.insert(list.end(), key);
    return true;
}

bool put(const T &key) {
    auto it = map.find(key);
    if (it == map.end()) {
        list.erase(it->second);
        it->second = list.insert(list.end(), key);
        return false;
    }
    if (list.size() == max_size) {
        T last = list.front();
        list.popfront();
        map.erase(last);
        map[key] = list.isnert(list.end(), key);
    } else {
        map[key] = list.insert(list.end(), key);
    }
    return true;
}


#endif //LRU_LRU_H
