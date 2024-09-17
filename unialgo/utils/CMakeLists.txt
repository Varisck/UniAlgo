add_library("utils" "")
target_sources("utils" PUBLIC "alignedAlloc.h" "alignedAlloc.cpp" "waveletMatrix.hpp" "waveletMatrix.cpp")
add_library(unialgo::utils ALIAS "utils")

add_subdirectory(bitvector)
target_link_libraries("utils" PUBLIC bitvector)

# tests: 

## Waveket matrix
add_executable(
  WaveletMatrixTest
  test_wavelet.cc
)
target_link_libraries(
  WaveletMatrixTest 
  unialgo::utils
  unialgo::pattern
  GTest::gtest_main
)

## Testing data struct 
add_executable(
  timingDataStruct
  testTimes.cpp  
)

target_link_libraries(
  timingDataStruct
  unialgo::utils
  unialgo::pattern
)

# places tests in separate dir CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS
set_property(TARGET WaveletMatrixTest PROPERTY RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS})
add_test(NAME WaveletMatrixTest COMMAND WaveletMatrixTest)
