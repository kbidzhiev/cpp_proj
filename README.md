# cpp_proj





# Tips

## CMAKE


```bash
cmake -S . -B build -G Ninja \
  -DCMAKE_C_COMPILER=gcc-14 \
  -DCMAKE_CXX_COMPILER=g++-14

cmake --build build
ctest --test-dir build --output-on-failure
```
