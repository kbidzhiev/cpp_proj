#pragma once
#include <cstddef>
#include <list>
#include <unordered_map>
#include <utility>

namespace caches
{

    template <typename T, typename KeyT = int>
    class LRUCache_t
    {
        using Node = std::pair<KeyT, T>;
        using ListIt = typename std::list<Node>::iterator;

        const std::size_t sz_;
        std::list<Node> data_;
        std::unordered_map<KeyT, ListIt> hash_;

        bool is_full() const;

    public:
        LRUCache_t(const std::size_t sz_);

        template <typename F>
        bool lookup_update(KeyT key, F slow_get_page);
    };

} // namespace caches
#include "lru.tpp"
