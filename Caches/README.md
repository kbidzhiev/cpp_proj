# Caches

C++17 LRU cache project with:

- Header-only library: `include/caches/lru.hpp`, `include/caches/lru.tpp`
- CLI app target: `Caches`
- Tests: GoogleTest unit tests + CTest end-to-end cases

## Quick Start

From the repository root:

```bash
cmake -S . -B build
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Prerequisites

- CMake 3.20+
- A C++17 compiler (`g++`, `clang++`, etc.)
- A build tool supported by CMake (Ninja/Make)

## Build

From the repository root:

```bash
cmake -S . -B build
cmake --build build -j
```

This builds:

- `build/Caches` (application)
- `build/test_lru` (unit test binary)

## Test

Run all tests (unit + E2E):

```bash
ctest --test-dir build --output-on-failure
```

Run only unit tests:

```bash
ctest --test-dir build -R LRUCacheTest --output-on-failure
```

Run only end-to-end tests:

```bash
ctest --test-dir build -R '^E2E_' --output-on-failure
```

List discovered tests:

```bash
ctest --test-dir build -N
```

## Run the App

Input format:

1. cache capacity `m`
2. number of requests `n`
3. `n` keys

Output: number of cache hits.

Example:

```bash
printf "2 5\n1 2 1 3 1\n" | ./build/Caches
```
