#pragma once
#include <list>
#include <unordered_map>

namespace caches
{

    template <typename T, typename KeyT = int>
    class LRUcache_t
    {
    private:
        const size_t sz_;
        std::list<T> data_;
        using ListIt = typename std::list<T>::iterator;
        std::unordered_map<KeyT, ListIt> hash_;

    public:
        LRUcache_t(const size_t sz_);

        template <typename F>
        bool lookup_update(KeyT key, F slow_get_page);

        bool is_full() const;
    };

} // namespace caches



#include"lru.tpp"