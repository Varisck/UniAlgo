#include "unialgo/utils/alignedAlloc.h"

#if defined(__linux__) || defined(__APPLE__)
/// definition for linux and apple compilers

#include <cstdlib>

void *unialgo::utils::aligned_alloc(std::size_t alignment, std::size_t size) {
  return std::aligned_alloc(alignment, size);
}

void unialgo::utils::aligned_free(void *ptr) { std::free(ptr); }

#elif defined(_WIN32)
/// definition for windows compilers

#include <malloc.h>

void *unialgo::utils::aligned_alloc(std::size_t alignment, std::size_t size) {
  return _aligned_malloc(size, alignment);
}

void unialgo::utils::aligned_free(void *memblock) { _aligned_free(memblock); }

#else
/// fallout

// to implement (?)

#endif
