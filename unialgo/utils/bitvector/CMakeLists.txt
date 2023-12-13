add_library("bitvector" "")
target_sources("bitvector" PUBLIC "bitvector.hpp" "bitMaps.hpp" "bitvector.cpp")
add_library(unialgo::utils::bitvector ALIAS "bitvector")
