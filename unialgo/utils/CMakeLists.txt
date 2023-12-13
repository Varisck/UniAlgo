add_library("utils" "")
target_sources("utils" PUBLIC "alignedAlloc.h" "alignedAlloc.cpp")
add_library(unialgo::utils ALIAS "utils")

add_subdirectory(bitvector)
target_link_libraries("utils" PUBLIC bitvector)
