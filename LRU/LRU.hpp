#pragma once

#include <list>
#include <unordered_map>

namespace cache {

template <typename T, typename KeyT = int> class LRU_t {
  size_t sz_;
  std::list<std::pair<KeyT, T>> cache_;

  using ListIt = typename decltype(cache_)::iterator;
  std::unordered_map<KeyT, ListIt> hash_;

public:
  explicit LRU_t(size_t sz) : sz_{sz} {}

  bool full() const { return cache_.size() == sz_; };

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    auto hit = hash_.find(key);
    if (hit == hash_.end()) { // not found
      if (full()) {
        hash_.erase(cache_.back().first);
        cache_.pop_back();
      }
      cache_.emplace_front(key, slow_get_page(key));
      hash_.emplace(key, cache_.begin());
      return false;
    }

    auto eltit = hit->second;
    cache_.splice(cache_.begin(), cache_, eltit);
    return true;
  }
};

} // namespace cache
