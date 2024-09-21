add_library("graph" INTERFACE)
target_sources("graph" PUBLIC "node.hpp" "graph.hpp")
add_library(unialgo::graph ALIAS "graph")

# tests: 

## test_graph
add_executable(
  test_graph
  test_graph.cc
)
target_link_libraries(
  test_graph
  unialgo::graph
  GTest::gtest_main
)
# places tests in separate dir CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS

set_property(TARGET test_graph PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME test_graph COMMAND test_graph)
