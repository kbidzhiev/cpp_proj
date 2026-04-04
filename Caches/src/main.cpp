#include <iostream>
#include <cassert>
#include "lru.hpp"


int slow_get_page(int id){return id;}


int main() {
  int hits = 0;
  int n;
  size_t m;

  std::cin >> m >> n;
  assert(std::cin.good());
  caches::LRUCache_t<int> c{m};

  for (int i = 0; i < n; ++i) {
    //std::cout << "step: " << i << std::endl;
    int q;
    std::cin >> q;
    assert(std::cin.good() || std::cin.eof());
    if (c.lookup_update(q, slow_get_page))
      hits += 1;
  }

  std::cout << hits << std::endl;
}
