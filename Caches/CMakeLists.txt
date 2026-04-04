cmake_minimum_required(VERSION 3.20)

project(Caches LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# LRU cache library (header-only)
add_library(caches_lru INTERFACE)
target_include_directories(caches_lru INTERFACE include)

# App
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE caches_lru)

enable_testing()

# Download GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

# Unit tests
add_executable(
  test_lru_unit
  tests/test_lru/test_lru.cpp
)
target_link_libraries(
  test_lru_unit
  PRIVATE
  caches_lru
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_lru_unit)

# End-to-end tests (.dat input, .ans expected output)
file(GLOB E2E_INPUTS CONFIGURE_DEPENDS tests/test_lru/e2e/*.dat)
foreach(input_file IN LISTS E2E_INPUTS)
  get_filename_component(test_name "${input_file}" NAME_WE)
  string(REPLACE ".dat" ".ans" answer_file "${input_file}")

  if(NOT EXISTS "${answer_file}")
    message(FATAL_ERROR "Missing answer file for ${input_file}: ${answer_file}")
  endif()

  add_test(
    NAME E2E_${test_name}
    COMMAND ${CMAKE_COMMAND}
      -DAPP=$<TARGET_FILE:${PROJECT_NAME}>
      -DINPUT_FILE=${input_file}
      -DANSWER_FILE=${answer_file}
      -P ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_lru/e2e/run_case.cmake
  )
endforeach()
