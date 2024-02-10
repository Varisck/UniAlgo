#ifndef UNIALGO_ALIGNED_ALLOC_
#define UNIALGO_ALIGNED_ALLOC_

/**
 *  \file alignedAlloc.h
 *  @brief Header file defining function aligned_alloc
 *
 *  This allows aligned_alloc, aliged_free usage on all systems
 *
 */

#include <cstddef>  //std::size_t

namespace unialgo {
namespace utils {

void *aligned_alloc(std::size_t alignment, std::size_t size);

void aligned_free(void *memblock);

}  // end namespace utils
}  // end namespace unialgo

#endif  // UNIALGO_ALIGNED_ALLOC_