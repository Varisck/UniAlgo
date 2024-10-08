# create library unialgo::pattern
add_library("pattern" "")
target_sources("pattern" PUBLIC "stringMatching.hpp" "wordVecMatching.hpp" "matchingAlgo.hpp" "suffixArray.hpp"
 "stringMatching.cpp"  "wordVecMatching.cpp" "suffixArray.cpp" "bwt.hpp" "bwt.cpp")
add_library(unialgo::pattern ALIAS "pattern")

# tests: 

## match_test
add_executable(
  match_test
  match_test.cc
)
target_link_libraries(
  match_test
  unialgo::utils
  unialgo::pattern
  GTest::gtest_main
)

## match_wv_test
add_executable(
  match_wv_test
  match_wv_test.cc
)
target_link_libraries(
  match_wv_test
  unialgo::utils
  unialgo::pattern
  GTest::gtest_main
)

## test_sa
add_executable(
  test_sa
  test_sa.cc
)
target_link_libraries(
  test_sa
  unialgo::utils
  unialgo::pattern
  GTest::gtest_main
)

# places tests in separate dir CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS
set_property(TARGET match_test PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME match_test COMMAND match_test)

set_property(TARGET match_wv_test PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME match_wv_test COMMAND match_wv_test)

set_property(TARGET test_sa PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME test_sa COMMAND test_sa)
