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

#include "UniAlgo/dotProduct/dotProduct.h"

namespace unialgo
{

    class Perceptron
    {
    public:
        using weightType = double;
        const std::size_t weightSize = sizeof(weightType);

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

    private:
        std::size_t inputSize_; /// size of weights vector
        // std::unique_ptr<weightType> _weights; /// weights pointer (need to be aligned allocated for avx)
        weightType *weights_; /// weights pointer (need to be aligned allocated for avx)

    }; // end class Perceptron

} // end namespace unialgo

#endif // UNIALGO_PERCEPTRON_