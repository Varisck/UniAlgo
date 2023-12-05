
add_library("pattern")
target_sources("pattern" PRIVATE "stringMatching.hpp" "stringMatching.cpp")

enable_testing()
add_executable(
  match_test
  stringMatching.hpp
  stringMatching.cpp
  match_test.cc
)

target_link_libraries(
  match_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(match_test)