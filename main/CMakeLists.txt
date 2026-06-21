add_executable(main)
target_sources(main
  PRIVATE
    main.cpp
)

target_compile_options(main
  PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<CXX_COMPILER_ID:GNU,Clang>:-Wall -Wextra -Werror>
)

target_link_libraries(main
  PRIVATE
    LRU 
)
