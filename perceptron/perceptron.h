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
         */
        ~Perceptron();

        /**
         * @brief Prints the perceptron weights
         *
         * @param os Output stream to print the weights default = std::cout
         */
        void PrintWeights(std::ostream &os = std::cout);

        /**
         * @brief Train function for perceptron
         *
         * Number of columns in training data has to be input_size_ + 1
         * position input_size_ = target value (0 or 1)
         * train_set[i] = [0, ..., input_size_ - 1, T] T = target.
         * To correct the error this train function uses w' = w + (t - y)x
         *
         * @param train_set Pointer to matrix (contiguous memory) of training data
         * @param train_set_size Number of rows in training data
         */
        void Train(double *train_set, std::size_t train_set_size);

        /**
         * @brief Function to test one input vector
         *
         * Return value is <w, x> >= 0
         *
         * @param test_vec Input vector to test size: input_size_
         * @return true T = 1
         * @return false T = 0
         */
        bool Test(double *test_vec);

    private:
        std::size_t input_size_;      /// size of input vector
        std::size_t weight_vec_size_; /// size of weights vector
        weightType *weights_;         /// weights pointer (need to be aligned allocated for avx)
        // std::unique_ptr<weightType> _weights; /// weights pointer (need to be aligned allocated for avx)

        /**
         * @brief This function initialized the weights_ vector to random 64-bit precision values
         */
        void InitializeRandomWeights();

    }; // end class Perceptron

} // end namespace unialgo

#endif // UNIALGO_PERCEPTRON_