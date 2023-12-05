# create library unialgo::pattern
add_library("pattern" "")
target_sources("pattern" PUBLIC "stringMatching.hpp" "stringMatching.cpp")
add_library(unialgo::pattern ALIAS "pattern")

# tests: 

## match_test
add_executable(
  match_test
  match_test.cc
)
target_link_libraries(
  match_test
  unialgo::pattern
  GTest::gtest_main
)

# places tests in separate dir CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS
set_property(TARGET match_test PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME match_test COMMAND match_test)
