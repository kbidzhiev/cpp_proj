
namespace caches
{

    template <typename T, typename KeyT>
    LRUCache_t<T, KeyT>::LRUCache_t(const std::size_t sz) : sz_(sz) {}

    template <typename T, typename KeyT>
    template <typename F>
    bool LRUCache_t<T, KeyT>::lookup_update(KeyT key, F slow_get_page)
    {
        if(sz_ == 0) return false;

        auto it = hash_.find(key);

        if (it == hash_.end())
        {
            if (is_full())
            {
                hash_.erase(data_.back().first);
                data_.pop_back();
            }
            data_.emplace_front(key, slow_get_page(key));
            hash_.emplace(key, data_.begin());
            return false;
        }

        auto it_data = it->second;
        data_.splice(data_.begin(), data_, it_data);

        return true;
    }

    template <typename T, typename KeyT>
    bool LRUCache_t<T, KeyT>::is_full() const
    {
        return hash_.size() == sz_;
    }

} // namespace caches
