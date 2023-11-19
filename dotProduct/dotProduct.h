#ifndef UNIALGO_DOTPROD_
#define UNIALGO_DOTPROD_

#include <immintrin.h>
#include <cassert>

namespace unialgo
{

    // returns the sum of all 4ps values in a
    inline float horizontal_add_ps(__m128 a)
    {
        // add the 4ps in a and place them in the lowest 2 of low2
        const __m128 low2 = _mm_add_ps(a, _mm_movehl_ps(a, a));
        // add the lower 2 elements into the lowest
        const __m128 lowestRes = _mm_add_ss(low2, _mm_movehdup_ps(low2));
        // return the lowest float in register
        return _mm_cvtss_f32(lowestRes);
    }

    // returns the sum of all 8ps values in a
    inline float horizontal_add_ps(__m256 a)
    {
        // divide 256ps register in 2 128ps registers
        const __m128 upper128 = _mm256_extractf128_ps(a, 1);
        const __m128 lower128 = _mm256_castps256_ps128(a);
        // add the 2 128ps registers and return the horizontal add
        return horizontal_add_ps(_mm_add_ps(lower128, upper128));
    }

    // returns the sum of all 16ps values in a
    inline float horizontal_add_ps(__m512 a)
    {
        return _mm512_reduce_add_ps(a);
    }

    // computes dot product between p1 and p2
    // result might be different from non-vectorized way because of float arithmetic
    // it is expected that p1 and p2 are aligned allocated with type and both vectors are same length
    inline float DotProduct(float *p1, float *p2, size_t size_p)
    {
        constexpr int num_float = 8;                                              // number of floats in __m256
        constexpr int num_counters = 4;                                           // number of counters to use for dot product
        constexpr int increment_evry_loop = num_counters * num_float;             // number of values to move pointer forword
        const float *const p1_end = p1 + size_p - (size_p % increment_evry_loop); // end of vectorizable part of p1
        const float *const real_p1_end = p1 + size_p;                             // end of p1

        __m256 c[num_counters];
        __m256 a, b;

        for (int i = 0; i < num_counters; ++i)
        {
            a = _mm256_load_ps(p1 + 8 * i);
            b = _mm256_load_ps(p2 + 8 * i);
            c[i] = _mm256_mul_ps(a, b);
        }
        p1 += increment_evry_loop;
        p2 += increment_evry_loop;

        for (; p1 < p1_end; p1 += increment_evry_loop, p2 += increment_evry_loop)
        {
            for (int i = 0; i < num_counters; ++i)
            {
                a = _mm256_load_ps(p1 + 8 * i);
                b = _mm256_load_ps(p2 + 8 * i);
                c[i] = _mm256_fmadd_ps(a, b, c[i]);
            }
        }

        for (int i = 1; i < num_counters; ++i)
        {
            c[0] = _mm256_add_ps(c[0], c[i]);
        }
        float partial = horizontal_add_ps(c[0]);
        float rest = 0.0f;
        if (size_p % increment_evry_loop != 0)
        {
            for (; p1 < real_p1_end; ++p1, ++p2)
            {
                rest += (*p1) * (*p2);
            }
        }
        return partial + rest;
    }

} // end namespace unialgo

#endif