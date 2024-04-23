# create library unialgo::pattern
add_library("math" "")
target_sources("math" PUBLIC "math.hpp" "sparseMatrix.hpp" "sparseMatrix.cpp")
add_library(unialgo::math ALIAS "math")

# tests: 

## test_sparseMatrix
add_executable(
  test_sparse
  test_sparse.cc
)
target_link_libraries(
  test_sparse
  unialgo::math
  GTest::gtest_main
)
# places tests in separate dir CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS

set_property(TARGET test_sparse PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME test_sparse COMMAND test_sparse)
