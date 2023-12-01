#include <random>
#include <iostream>

#include "UniAlgo/perceptron/perceptron.h"
#include "UniAlgo/utils/alignedAlloc.h"

namespace unialgo
{

    Perceptron::Perceptron(std::size_t inputSize)
    {
        input_size_ = inputSize;
        weight_vec_size_ = inputSize + 1;
        weights_ = static_cast<weightType *>(utils::aligned_alloc(weightSize, weightSize * weight_vec_size_));
        (*this).InitializeRandomWeights();
    }

    Perceptron::~Perceptron()
    {
        if (weights_ != nullptr)
        {
            utils::aligned_free(weights_);
            weights_ = nullptr;
        }
    }

    void Perceptron::PrintWeights(std::ostream &os)
    {
        os << "Weights vector size: " << weight_vec_size_ << std::endl;
        for (std::size_t i = 0; i < weight_vec_size_; ++i)
        {
            os << weights_[i] << " ";
        }
        os << std::endl;
    }

    void Perceptron::InitializeRandomWeights()
    {
        if (weights_ != nullptr)
        {
            /// weights initialization:
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<weightType> dis(0.0f, 1.0f);
            for (size_t i = 0; i < weight_vec_size_; ++i)
            {
                weights_[i] = dis(gen);
            }
        }
    }

} // end namespace unialgo