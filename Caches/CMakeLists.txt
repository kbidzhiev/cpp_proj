cmake_minimum_required(VERSION 3.20)

project(Caches LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LRU library (header-only template implementation)
set(LRU_HEADERS
    include/lru.hpp
    include/lru.tpp
)

add_library(caches_lru INTERFACE)
target_sources(caches_lru INTERFACE ${LRU_HEADERS})
target_include_directories(caches_lru INTERFACE include)

# App target
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE caches_lru)
