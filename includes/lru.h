//
// Created by justnik on 02.02.2021.
//

#ifndef LRU_LRU_H
#define LRU_LRU_H

#include <cstddef>
#include <list>
#include <unordered_map>

template<class T>
class lru {
private:
    std::list<T> list_{};
    std::unordered_map<T, typename std::list<T>::iterator> map_{};
    std::size_t max_size;
public:
    lru(const std::size_t &maxSize);

    bool has(const T &key);

    bool put(const T &key);
};

template<class T>
lru<T>::lru(const std::size_t &maxSize):max_size(maxSize) {}

template<class T>
bool lru<T>::has(const T &key) {
    auto it = map_.find(key);
    if (it == map_.end()) return false;

    auto list_it = it->second;
    list_.splice(list_.end(), list_, list_it);
    return true;
}

template<class T>
bool lru<T>::put(const T &key) {
    auto it = map_.find(key);
    if (it != map_.end()) {
        auto list_it = it->second;
        list_.splice(list_.end(), list_, list_it);
        return false;
    }
    if (list_.size() == max_size) {
        T last = list_.front();
        list_.pop_front();
        map_.erase(last);
        list_.front() = key;
        list_.splice(list_.end(), list_, list_.begin());
        map_[key] = --list_.end();
    } else {
        map_[key] = list_.insert(list_.end(), key);
    }
    return true;
}


#endif //LRU_LRU_H
