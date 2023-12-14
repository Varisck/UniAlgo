add_library("bitvector" "")
target_sources("bitvector" PUBLIC "bitvector.hpp" "bitMaps.hpp" "bitvector.cpp")
add_library(unialgo::utils::bitvector ALIAS "bitvector")

# tests: 

## match_test
add_executable(
  Bitvector_test
  test.cc
)
target_link_libraries(
  Bitvector_test
  unialgo::utils::bitvector
  GTest::gtest_main
)

# places tests in separate dir CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS
set_property(TARGET Bitvector_test PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME Bitvector_test COMMAND Bitvector_test)

