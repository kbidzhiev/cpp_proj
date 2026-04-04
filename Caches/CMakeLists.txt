cmake_minimum_required(VERSION 3.20)

project(Caches LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google test downloading
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


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



### Google test
enable_testing()

add_executable(
  hello_test
  tests/test_lru/test_lru.cpp
)
target_link_libraries(
  hello_test
  PRIVATE
  caches_lru
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

# End-to-end tests: each .dat input must have a matching .ans expected output.
file(GLOB E2E_INPUTS CONFIGURE_DEPENDS tests/test_lru_e2e/*.dat)
foreach(input_file IN LISTS E2E_INPUTS)
  get_filename_component(test_name "${input_file}" NAME_WE)
  set(answer_file "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_lru_e2e/${test_name}.ans")

  if(NOT EXISTS "${answer_file}")
    message(FATAL_ERROR "Missing answer file for ${input_file}: ${answer_file}")
  endif()

  add_test(
    NAME E2E_${test_name}
    COMMAND ${CMAKE_COMMAND}
      -DAPP=$<TARGET_FILE:${PROJECT_NAME}>
      -DINPUT_FILE=${input_file}
      -DANSWER_FILE=${answer_file}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_lru_e2e/run_case.cmake
  )
endforeach()
