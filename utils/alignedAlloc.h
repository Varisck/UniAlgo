#ifndef UNIALGO_ALIGNED_ALLOC_
#define UNIALGO_ALIGNED_ALLOC_

/**
 *  \file alignedAlloc.h
 *  @brief Header file defining function aligned_alloc
 *
 *  This file was done due to not having aligned_alloc on windows
 *
 */

#include <cstddef> //std::size_t

namespace unialgo
{
    namespace utils
    {

        void *aligned_alloc(std::size_t alignment, std::size_t size);

        void aligned_free(void *memblock);

    }
}

#endif // UNIALGO_ALIGNED_ALLOC_