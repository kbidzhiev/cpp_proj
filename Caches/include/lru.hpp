#pragma once
#include <cstddef>
#include <iterator>
#include <list>
#include <unordered_map>

namespace caches
{

    template <typename T, typename KeyT = int>
    class LRUcache_t
    {
    private:
        const std::size_t sz_;
        std::list<T> data_;
        using ListIt = typename std::list<T>::iterator;
        std::unordered_map<KeyT, ListIt> hash_;

        bool is_full() const;

    public:
        LRUcache_t(const std::size_t sz_);

        template <typename F>
        bool lookup_update(KeyT key, F slow_get_page);
    };

} // namespace caches
#include "lru.tpp"
