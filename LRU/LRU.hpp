#pragma once

#include <list>
#include <unordered_map>

namespace cache {

template <typename T, typename KeyT = int> class LRU_t {
  std::list<T> cache_;

  using ListIt = typename std::list<T>::iterator;
  std::unordered_map<T, ListIt> hash_;

public:
  LRU_t(size_t sz) : hash_{sz} {}

  bool full() const { return cache_.size() == hash_.size(); };

  template <typename F> bool lookup_update(KeyT key, F slow_get_page) {
    auto hit = hash_.find(key);
    if (hit == hash_.end()) { // not found
      if (full()) {
        hash_.erase(cache_.back().id);
        cache_.pop_back();
      }
      cache_push_front(slow_get_page(key));
      hash_[key] = cache_.begin();
      return false;
    }

    auto eltit = hit->second;
    if (eltit != cache_.begin()) {
      cache_.splice(cache_.begin(), cache_, eltit);
    }
    return true;
  }
};

} // namespace cache
