# create library unialgo::pattern
add_library("pattern")
target_sources("pattern" PRIVATE "stringMatching.hpp" "stringMatching.cpp")
add_library(unialgo::pattern ALIAS "pattern")


# tests 
enable_testing()

# match_test
add_executable(
  match_test
  match_test.cc
)
target_link_libraries(
  match_test
  unialgo::pattern
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(match_test)