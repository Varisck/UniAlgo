#ifndef UNIALGO_DOTPROD_
#define UNIALGO_DOTPROD_

#include <immintrin.h>
#include <cassert>

/**
 *    \file dotProduct.h
 *    \brief Header file implementing dot product using simd
 *
 *    Implementation of dot product for floats and doubles, implemented using avx.
 **/

namespace unialgo
{

    /**
     * @brief Implementation of horizontal sum of __m128 register
     *
     * @param a
     * @return Returns the sum of all 4 precision single values in a
     */
    inline float horizontal_add_ps(__m128 a)
    {
        // add the 4ps in a and place them in the lowest 2 of low2
        const __m128 low2 = _mm_add_ps(a, _mm_movehl_ps(a, a));
        // add the lower 2 elements into the lowest
        const __m128 lowestRes = _mm_add_ss(low2, _mm_movehdup_ps(low2));
        // return the lowest float in register
        return _mm_cvtss_f32(lowestRes);
    }

    /**
     * @brief Implementation of horizontal sum of __m256 register
     *
     * @param a
     * @return Returns the sum of all 8ps values in a
     */
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

    /**
     * @brief Implementation of horizontal sum of __m256d register
     *
     * @param a
     * @return Returns the sum of all 4pd values in a
     */
    inline double horizontal_add_pd(__m256d a)
    {
        // Horizontally add adjacent pairs of dp
        // sum[0] = sum[1] = a[0] + a[1]
        // sum[2] = sum[3] = a[2] + a[3]
        __m256d sum = _mm256_hadd_pd(a, a);
        // extract sum[2] and sum[3]
        __m128d sum_high128d = _mm256_extractf128_pd(sum, 1);
        // extract sum[0] and sum[1]
        __m128d sum_low128d = _mm256_castpd256_pd128(sum);
        __m128d res = _mm_add_pd(sum_high128d, sum_low128d);
        return _mm_cvtsd_f64(res);
    }

    /**
     * @brief Computes dot product between p1 and p2
     *
     * result might be different from non-vectorized way because of float arithmetic
     * it is expected that p1 and p2 are aligned allocated with type, and that the 2 vectors have the same length
     *
     * @param p1 pointer to first vector
     * @param p2 pointer to second vector
     * @param size_p size of vectors
     * @return float
     */
    inline float DotProduct(float *p1, float *p2, size_t size_p)
    {
        constexpr int num_float = 8;                                              // number of floats in __m256
        constexpr int num_counters = 4;                                           // number of counters to use to compute dot product
        constexpr int increment_evry_loop = num_counters * num_float;             // number of values to move pointer forword
        const float *const p1_end = p1 + size_p - (size_p % increment_evry_loop); // end of vectorizable part of p1
        const float *const real_p1_end = p1 + size_p;                             // end of p1

        __m256 c[num_counters];
        __m256 a, b;

        for (int i = 0; i < num_counters; ++i)
        {
            a = _mm256_load_ps(p1 + num_float * i);
            b = _mm256_load_ps(p2 + num_float * i);
            c[i] = _mm256_mul_ps(a, b);
        }
        p1 += increment_evry_loop;
        p2 += increment_evry_loop;

        for (; p1 < p1_end; p1 += increment_evry_loop, p2 += increment_evry_loop)
        {
            for (int i = 0; i < num_counters; ++i)
            {
                a = _mm256_load_ps(p1 + num_float * i);
                b = _mm256_load_ps(p2 + num_float * i);
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

    /**
     * @brief Computes dot product between p1 and p2
     *
     *  Result might be different from non-vectorized way because of double arithmetic
     *  it is expected that p1 and p2 are aligned allocated with type, and that the 2 vectors have the same length
     *
     * @param p1 pointer to first vector
     * @param p2 pointer to second vector
     * @param size_p size of vectors
     * @return double
     */
    inline double DotProduct(double *p1, double *p2, size_t size_p)
    {
        constexpr int num_double = 4;                                              // number of doubles in __m256
        constexpr int num_counters = 4;                                            // number of counters to use to compute dot product
        constexpr int increment_evry_loop = num_counters * num_double;             // number of values to move pointer forword
        const double *const p1_end = p1 + size_p - (size_p % increment_evry_loop); // end of vectorizable part of p1
        const double *const real_p1_end = p1 + size_p;                             // end of p1

        __m256d c[num_counters];
        __m256d a, b;

        for (int i = 0; i < num_counters; ++i)
        {
            a = _mm256_load_pd(p1 + num_double * i);
            b = _mm256_load_pd(p2 + num_double * i);
            c[i] = _mm256_mul_pd(a, b);
        }
        p1 += increment_evry_loop;
        p2 += increment_evry_loop;

        for (; p1 < p1_end; p1 += increment_evry_loop, p2 += increment_evry_loop)
        {
            for (int i = 0; i < num_counters; ++i)
            {
                a = _mm256_load_pd(p1 + num_double * i);
                b = _mm256_load_pd(p2 + num_double * i);
                c[i] = _mm256_fmadd_pd(a, b, c[i]);
            }
        }
        for (int i = 1; i < num_counters; ++i)
        {
            c[0] = _mm256_add_pd(c[0], c[i]);
        }
        double partial = horizontal_add_pd(c[0]);
        double rest = 0.0;
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

#endif // UNIALGO_DOTPROD_