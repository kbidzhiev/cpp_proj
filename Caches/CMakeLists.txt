cmake_minimum_required(VERSION 3.20)

project(Caches LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
set(SOURCES
    src/main.cpp
)

# Header files
set(HEADERS
    include/lru.hpp
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Let the compiler find headers in the `include/` folder
# allows to use
# #include "MyClass.h"
target_include_directories(${PROJECT_NAME} PRIVATE include)