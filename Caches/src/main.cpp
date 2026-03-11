#include <iostream>
#include <cassert>
#include "lru.hpp"

struct page_t{
  int index;
  int *data;
};

int slow_get_page(int id){return id;}


int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;
  assert(std::cin.good());
  caches::LRUcache_t<int> c{m};

  for (int i = 0; i < n; ++i) {
    int q;
    std::cin >> q;
    assert(std::cin.good());
    if (c.lookup_update(q, slow_get_page))
      hits += 1;
  }

  std::cout << "hits "<< hits << std::endl;
}