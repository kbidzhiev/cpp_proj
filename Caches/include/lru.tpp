#include "lru.hpp"

namespace caches
{

    template <typename T, typename KeyT>
    LRUcache_t<T, KeyT>::LRUcache_t(const size_t sz) : sz_(sz) {}

    template <typename T, typename KeyT>
    template <typename F>
    bool LRUcache_t<T, KeyT>::lookup_update(KeyT key, F slow_get_page)
    {
        auto it = hash_.find(key);

        if (it == hash_.end())
        {
            if (is_full())
            {
                hash_.erase(data_.back());
                data_.pop_back();
            }
            data_.push_front(slow_get_page(key));
            hash_[key] = data_.begin();
            return false;
        }

        auto it_data = it->second;
        data_.splice(data_.begin(), data_, it_data, std::next(it_data));

        return true;
    }

    // private part

    template <typename T, typename KeyT>
    bool LRUcache_t<T, KeyT>::is_full() const
    {
        return hash_.size() == sz_;
    }

} // namespace caches