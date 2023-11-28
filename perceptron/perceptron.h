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
         * @param inputSize size of input connection vector of perceptron
         */
        explicit Perceptron(std::size_t inputSize);

        /**
         * @brief Destroy the Perceptron object
         *
         */
        ~Perceptron();

        // print the perceptron weithgs vector
        void PrintWeights();

        /// @brief test function to train perceptron
        /// train_set_size is number of input_size_ array with data to train on
        /// train_set = (train_set_size * (input_size_ + 1))
        /// train_set[i] = [0, ..., input_size_ - 1, T] T = target
        void Train(double *train_set, std::size_t train_set_size);

        /// @brief test vector test_vec of size input_size_
        bool test(double *test_vec);

    private:
        std::size_t input_size_;      /// size of input vector
        std::size_t weight_vec_size_; /// size of weights vector
        weightType *weights_;         /// weights pointer (need to be aligned allocated for avx)
        // std::unique_ptr<weightType> _weights; /// weights pointer (need to be aligned allocated for avx)

    }; // end class Perceptron

} // end namespace unialgo

#endif // UNIALGO_PERCEPTRON_