#include <gtest/gtest.h>
#include <string>
#include "lru.hpp"

namespace {
int SlowGetPage(int key) { return key * 10; }
} // namespace

TEST(LRUCacheTest, MissThenHitForSameKey) {
  caches::LRUCache_t<int> cache{2};
  EXPECT_FALSE(cache.lookup_update(7, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(7, SlowGetPage));
}

TEST(LRUCacheTest, ZeroCapacityAlwaysMiss) {
  caches::LRUCache_t<int> cache{0};
  EXPECT_FALSE(cache.lookup_update(7, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(7, SlowGetPage));
}

TEST(LRUCacheTest, EvictsLeastRecentlyUsedOnMiss) {
  caches::LRUCache_t<int> cache{2};
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(2, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(3, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(2, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(3, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));
}

TEST(LRUCacheTest, HitUpdatesRecency) {
  caches::LRUCache_t<int> cache{2};
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(2, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(1, SlowGetPage));   // 1 becomes most-recent
  EXPECT_FALSE(cache.lookup_update(3, SlowGetPage));  // should evict 2
  EXPECT_TRUE(cache.lookup_update(1, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(2, SlowGetPage));  // 2 was evicted
}

TEST(LRUCacheTest, SlowLoaderCalledOnlyOnMiss) {
  caches::LRUCache_t<int> cache{2};
  int calls = 0;
  auto counted_loader = [&calls](int key) { ++calls; return SlowGetPage(key); };
  EXPECT_FALSE(cache.lookup_update(1, counted_loader));
  EXPECT_EQ(calls, 1);
  EXPECT_TRUE(cache.lookup_update(1, counted_loader));
  EXPECT_EQ(calls, 1);
  EXPECT_FALSE(cache.lookup_update(2, counted_loader));
  EXPECT_EQ(calls, 2);
  EXPECT_TRUE(cache.lookup_update(2, counted_loader));
  EXPECT_EQ(calls, 2);
  EXPECT_TRUE(cache.lookup_update(1, counted_loader));
  EXPECT_EQ(calls, 2);
}

TEST(LRUCacheTest, CapacityOneEvictsPreviousKey) {
  caches::LRUCache_t<int> cache{1};
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(1, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(2, SlowGetPage));  // evicts 1
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));  // 1 is gone
}

TEST(LRUCacheTest, NonIntKeyType) {
  caches::LRUCache_t<int, std::string> cache{2};
  auto slow_get_page = [](const std::string &key) {
    return static_cast<int>(key.size());
  };

  EXPECT_FALSE(cache.lookup_update(std::string("alpha"), slow_get_page));
  EXPECT_TRUE(cache.lookup_update(std::string("alpha"), slow_get_page));
}

TEST(LRUCacheTest, NonIntValueType) {
  caches::LRUCache_t<std::string, int> cache{2};
  int calls = 0;
  auto slow_get_page = [&calls](int key) {
    ++calls;
    return std::to_string(key * 10);
  };

  EXPECT_FALSE(cache.lookup_update(7, slow_get_page));
  EXPECT_TRUE(cache.lookup_update(7, slow_get_page));
  EXPECT_EQ(calls, 1);
}

TEST(LRUCacheTest, RecencyAfterMultipleHits) {
  caches::LRUCache_t<int> cache{3};
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(2, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(3, SlowGetPage));

  EXPECT_TRUE(cache.lookup_update(1, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(2, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(4, SlowGetPage));  // should evict 3

  EXPECT_TRUE(cache.lookup_update(2, SlowGetPage));
  EXPECT_TRUE(cache.lookup_update(4, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(3, SlowGetPage));  // evicts 1
}

TEST(LRUCacheTest, ReinsertEvictedKey) {
  caches::LRUCache_t<int> cache{2};
  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(2, SlowGetPage));
  EXPECT_FALSE(cache.lookup_update(3, SlowGetPage));  // evicts 1

  EXPECT_FALSE(cache.lookup_update(1, SlowGetPage));  // reinsert, evict 2
  EXPECT_TRUE(cache.lookup_update(1, SlowGetPage));   // now hit
}

TEST(LRUCacheTest, LoaderNotCalledOnHitAfterRecencyMoves) {
  caches::LRUCache_t<int> cache{3};
  int calls = 0;
  auto counted_loader = [&calls](int key) {
    ++calls;
    return key * 10;
  };

  EXPECT_FALSE(cache.lookup_update(1, counted_loader));
  EXPECT_EQ(calls, 1);
  EXPECT_FALSE(cache.lookup_update(2, counted_loader));
  EXPECT_EQ(calls, 2);
  EXPECT_FALSE(cache.lookup_update(3, counted_loader));
  EXPECT_EQ(calls, 3);

  EXPECT_TRUE(cache.lookup_update(2, counted_loader));
  EXPECT_EQ(calls, 3);
  EXPECT_TRUE(cache.lookup_update(1, counted_loader));
  EXPECT_EQ(calls, 3);
  EXPECT_TRUE(cache.lookup_update(2, counted_loader));
  EXPECT_EQ(calls, 3);
  EXPECT_TRUE(cache.lookup_update(3, counted_loader));
  EXPECT_EQ(calls, 3);

  EXPECT_FALSE(cache.lookup_update(4, counted_loader));
  EXPECT_EQ(calls, 4);

  EXPECT_TRUE(cache.lookup_update(2, counted_loader));
  EXPECT_EQ(calls, 4);
}
