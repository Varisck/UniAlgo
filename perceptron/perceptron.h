#ifndef UNIALGO_PERCEPTRON_
#define UNIALGO_PERCEPTRON_

/**
 *   \file perceptron.h
 *   @brief Header file defining class perceptron
 *
 *  Simple implementation of a perceptron. Uses implementation of vectorized operation
 *  in dotProduct.h.
 */

#include <memory>
#include <iostream>

#include "UniAlgo/utils/alignedAlloc.h"

namespace unialgo
{

    class Perceptron
    {
    public:
        using weightType = double;                         // type of weights
        const std::size_t weightSize = sizeof(weightType); // byte size of weightType

        /**
         * @brief Construct a new Perceptron object
         *
         * @param inputSize size of input vector of perceptron
         */
        explicit Perceptron(std::size_t inputSize);

        /**
         * @brief Destroy the Perceptron object
         */
        ~Perceptron();

        /**
         * @brief Prints the perceptron weights
         *
         * @param os Output stream to print the weights default = std::cout
         */
        void PrintWeights(std::ostream &os = std::cout);

        /**
         * @brief Train function using Gradiant Descent
         *
         * @details Number of columns in training data has to be input_size_ + 1
         * last position [input_size_] = targetValue (0 or 1)
         * Ex: train_set[i] = [0, ..., input_size_ - 1, T] T = target.
         * To correct the error this function uses Gradiant Descent w' = w + (t - y)x
         *
         * @tparam T type of train_set
         * @param train_set Pointer to matrix (contiguous memory) of training data
         * @param train_set_size Number of rows in training data Delta_w[i] = mu(sum_{data in dataset}(t - y)(x[i]))
         * @param mu Learning rate
         *
         * @todo implement choose option to implement (batch, mini-batch, single)
         *
         */
        template <typename T>
        void TrainGD(T *train_set, std::size_t train_set_size) {}

        /**
         * @brief Train function for perceptron
         *
         * @details Number of columns in training data has to be input_size_ + 1
         * last position [input_size_] = targetValue (0 or 1)
         * Ex: train_set[i] = [0, ..., input_size_ - 1, T] T = target.
         * To correct the error this train function uses w' = w + (t - y)x
         *
         * @tparam T type of train_set
         * @param train_set Pointer to matrix (contiguous memory) of training data
         * @param train_set_size Number of rows in training data
         *
         */
        template <typename T>
        void Train(T *train_set, std::size_t train_set_size)
        {
            (*this).InitializeRandomWeights();

            const std::size_t train_set_vec_size = input_size_ + 1;
            const T *train_set_end = train_set + (train_set_size * train_set_vec_size);

            for (; train_set < train_set_end; train_set += train_set_vec_size)
            {
                // getting target value of training case and setting delta inputValue to -1
                T target = *(train_set + train_set_vec_size - 1);
                *(train_set + train_set_vec_size - 1) = -1;

                // dot product <train_set, weights_> to get res from perceptron
                double res = DotProduct(train_set);

                // Fixing error in response
                FixErrorInTraining(train_set, res, target);
            }
        }

        /**
         * @brief Function to test one input vector
         *
         * Return value is equation: <w, x> >= 0
         *
         * @tparam T type of input_vec
         * @param test_vec Input vector to test size: input_size_
         * @return true T = 1
         * @return false T = 0
         *
         */
        template <typename T>
        bool Test(T *test_vec)
        {
            {
                // use memcopy create vector with inputValue for delta
                T *test_vec_with_delta = static_cast<T *>(utils::aligned_alloc(sizeof(T), (input_size_ + 1) * sizeof(T)));
                for (int i = 0; i < input_size_; ++i)
                {
                    test_vec_with_delta[i] = test_vec[i];
                }
                // set inputValue of delta to -1
                test_vec_with_delta[input_size_] = -1;
                double res = DotProduct(test_vec_with_delta);

                unialgo::utils::aligned_free(test_vec_with_delta);
                return res >= 0;
            }
        }

// define __UNIALGO_USE_AVX to have Train function with AVX
#ifdef __UNIALGO_USE_AVX
#include "UniAlgo/dotProduct/dotProduct.h"

        /**
         * @brief Train function for perceptron with AVX
         *
         * Number of columns in training data has to be input_size_ + 1
         * position input_size_ = target value (0 or 1)
         * train_set[i] = [0, ..., input_size_ - 1, T] T = target.
         * To correct the error this train function uses w' = w + (t - y)x
         *
         * @param train_set Pointer to matrix (contiguous memory) of training data
         * @param train_set_size Number of rows in training data
         */
        void Train(double *train_set, std::size_t train_set_size)
        {
            (*this).InitializeRandomWeights();

            const std::size_t train_set_vec_size = input_size_ + 1;
            double *train_set_end = train_set + (train_set_size * train_set_vec_size);

            for (; train_set < train_set_end; train_set += train_set_vec_size)
            {
                // getting target value of training case and setting delta inputValue to -1
                double target = *(train_set + train_set_vec_size - 1);
                *(train_set + train_set_vec_size - 1) = -1;

                double res = math::DotProduct(train_set, weights_, train_set_vec_size);
                // Fixing error in response
                FixErrorInTraining(train_set, res, target);
            }
        }
#endif // __UNIALGO_USE_AVX

    private:
        std::size_t input_size_;      /// size of input vector
        std::size_t weight_vec_size_; /// size of weights vector
        weightType *weights_;         /// weights pointer (need to be aligned allocated for avx)

        /**
         * @brief This function initialized the weights_ vector to random 64-bit precision values
         */
        void InitializeRandomWeights();

        template <typename T>
        void FixErrorInTraining(T *train_set, double perceptronRes, T target)
        {
            if (perceptronRes >= 0 && target == 0)
            {
                /// t = 0 and y = 1 ===> w' = w - x
                for (std::size_t i = 0; i < weight_vec_size_; ++i)
                {
                    weights_[i] -= train_set[i];
                }
            }
            else if (perceptronRes < 0 && target == 1)
            {
                /// t = 1 and y = 0 ===> w' = w + x
                for (std::size_t i = 0; i < weight_vec_size_; ++i)
                {
                    weights_[i] += train_set[i];
                }
            }
        }

        template <typename T>
        inline double DotProduct(T *vec)
        {
            double res = 0.0;
            for (std::size_t i = 0; i < weight_vec_size_; ++i)
            {
                res += vec[i] * weights_[i];
            }
            return res;
        }

    }; // end class Perceptron

} // end namespace unialgo

#endif // UNIALGO_PERCEPTRON_