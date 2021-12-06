//
// Created by justnik on 01.12.2021.
//

#ifndef LRU_LRU_HPP
#define LRU_LRU_HPP

#include <list>
#include <unordered_map>

#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>


namespace semi_intrusive {
    using LinkMode = boost::intrusive::link_mode<boost::intrusive::normal_link>;
    using LruHook = boost::intrusive::list_base_hook<LinkMode>;

    class LruNode final : public LruHook {
    };

    template<class T>
    class lru {
    private:
        using List = boost::intrusive::list<LruNode, boost::intrusive::constant_time_size<false>>;
        using Map = std::unordered_map<T, LruNode>;
        List list_;
        Map map_;
        std::size_t max_size_;

        const T &getLeastRecentKey();

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
        list_.splice(list_.end(), list_, list_.iterator_to(it->second));
        return true;
    }

    template<class T>
    bool lru<T>::put(const T &key) {
        auto it = map_.find(key);
        if (it != map_.end()) {
            list_.splice(list_.end(), list_, list_.iterator_to(it->second));
            return false;
        }
        if (map_.size() == max_size_) {
            auto node = map_.extract(getLeastRecentKey());
            list_.splice(list_.end(), list_, list_.begin());
            node.key() = key;
            map_.insert(std::move(node));
        } else {
            auto[it, ok] = map_.emplace(key, LruNode{});
            assert(ok);
            list_.insert(list_.end(), it->second);
        }
        return true;
    }

    template<class T>
    const T &lru<T>::getLeastRecentKey() {
        using Pair = typename Map::value_type;
        constexpr auto offset = offsetof(Pair, second) - offsetof(Pair, first);
        return *reinterpret_cast<const T *>(reinterpret_cast<const char *>(&list_.front()) - offset);
    }
}
#endif //LRU_LRU_HPP
