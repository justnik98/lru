//
// Created by justnik on 01.12.2021.
//

#ifndef LRU_LRU_HPP
#define LRU_LRU_HPP

#include <list>
#include <unordered_map>

template<class T>
class lru {
private:
    std::list<T> list_;
    std::unordered_map<T, typename std::list<T>::iterator> map_;
    std::size_t max_size_;
public:
    explicit lru(size_t maxSize) : max_size_(maxSize) {};

    bool has(const T &key);

    bool put(const T &key);
};

template<class T>
bool lru<T>::has(const T &key) {
    auto it = map_.find(key);
    if (it == map_.end()) {
        return false;
    }
    auto list_it = it->second;
    list_.splice(list_.end(), list_, list_it);
    return true;
}

template<class T>
bool lru<T>::put(const T &key) {
    auto it = map_.find(key);
    if (it != map_.end()) {
        auto list_it = it->second;
        list_.erase(list_it);
        it->second = list_.insert(list_.end(), key);
        return false;
    }
    if (list_.size() == max_size_) {
        T last = list_.front();
        auto node = map_.extract(last);
        list_.front() = key;
        list_.splice(list_.end(), list_, list_.begin());
        node.key() = key;
        node.mapped() = --list_.end();
        map_.insert(std::move(node));
    } else {
        map_[key] = list_.insert(list_.end(), key);
    }
}


#endif //LRU_LRU_HPP
