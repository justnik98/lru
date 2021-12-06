//
// Created by justnik on 01.12.2021.
//

#ifndef LRU_LRU_HPP
#define LRU_LRU_HPP

#include <list>
#include <memory>
#include <unordered_map>

#include <boost/intrusive/link_mode.hpp>
#include <boost/intrusive/list.hpp>
#include <boost/intrusive/list_hook.hpp>

#include <boost/intrusive/unordered_set.hpp>
#include <boost/intrusive/unordered_set_hook.hpp>

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


namespace fully_intrusive {
    using LinkMode = boost::intrusive::link_mode<boost::intrusive::normal_link>;
    using LruListHook = boost::intrusive::list_base_hook<LinkMode>;
    using LruHashSetHook = boost::intrusive::unordered_set_base_hook<LinkMode>;

    template <class Key>
    class LruNode final : public LruListHook, public LruHashSetHook {
    public:
        explicit LruNode(Key&& key) : key_(std::move(key)) {}

        const Key& getKey() const noexcept { return key_; }
        void setKey(Key key) { key_ = std::move(key); }

    private:
        Key key_;
    };

    template<class Key>
    const Key &getKey(const LruNode<Key> &node) noexcept {
        return node.getKey();
    }

    template<class T>
    const T &getKey(const T &key) noexcept {
        return key;
    }

    template<typename T, typename Hash = std::hash<T>,
            typename Equal = std::equal_to<T>>
    class lru final {
    public:
        explicit lru(size_t max_size) : buckets_(max_size ? max_size : 1),
                                        map_(BucketTraits(buckets_.data(), buckets_.size())) {}

        ~lru() {
            while (!list_.empty()) {
                extractNode(list_.begin());
            }
        }

        lru(lru &&lru) noexcept = default;

        lru(const lru &lru) = delete;

        lru &operator=(lru &&lru) noexcept = default;

        lru &operator=(const lru &lru) = delete;

        bool put(const T &key) {
            auto it = map_.find(key, map_.hash_function(), map_.key_eq());
            if (it != map_.end()) {
                list_.splice(list_.end(), list_, list_.iterator_to(*it));
                return false;
            }

            if (map_.size() == buckets_.size()) {
                auto node = extractNode(list_.begin());
                node->setKey(key);
                insertNode(std::move(node));
            } else {
                auto node = std::make_unique<LruNode>(T(key));
                insertNode(std::move(node));
            }

            return true;
        }

        bool has(const T &key) {
            auto it = map_.find(key, map_.hash_function(), map_.key_eq());
            if (it == map_.end()) return false;

            list_.splice(list_.end(), list_, list_.iterator_to(*it));
            return true;
        }

    private:
        using LruNode = fully_intrusive::LruNode<T>;
        using List = boost::intrusive::list<
                LruNode,
                boost::intrusive::constant_time_size<false>  // Beware!
        >;

        struct LruNodeHash : Hash {
            template<class NodeOrKey>
            auto operator()(const NodeOrKey &x) const {
                return Hash::operator()(getKey(x));
            }
        };

        struct LruNodeEqual : Equal {
            template<class NodeOrKey1, class NodeOrKey2>
            auto operator()(const NodeOrKey1 &x, const NodeOrKey2 &y) const {
                return Equal::operator()(getKey(x), getKey(y));
            }
        };

        using Map = boost::intrusive::unordered_set<
                LruNode, boost::intrusive::constant_time_size<true>,
                boost::intrusive::hash<LruNodeHash>,
                boost::intrusive::equal<LruNodeEqual>>;

        using BucketTraits = typename Map::bucket_traits;
        using BucketType = typename Map::bucket_type;

        std::unique_ptr<LruNode> extractNode(typename List::iterator it) noexcept {
            std::unique_ptr<LruNode> ret(&*it);
            map_.erase(map_.iterator_to(*it));
            list_.erase(it);
            return ret;
        }

        void insertNode(std::unique_ptr<LruNode> node) noexcept {
            if (!node) return;

            map_.insert(*node);                // noexcept
            list_.insert(list_.end(), *node);  // noexcept

            [[maybe_unused]] auto ignore = node.release();
        }

        std::vector<BucketType> buckets_;
        Map map_;
        List list_;
    };
}
#endif //LRU_LRU_HPP
