#include <random>
#include <iostream>

#include "UniAlgo/perceptron/perceptron.h"
#include "UniAlgo/utils/alignedAlloc.h"
#include "UniAlgo/dotProduct/dotProduct.h"

namespace unialgo
{

    Perceptron::Perceptron(std::size_t inputSize)
    {
        input_size_ = inputSize;
        weight_vec_size_ = inputSize + 1;
        weights_ = static_cast<weightType *>(utils::aligned_alloc(weightSize, weightSize * weight_vec_size_));
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

    Perceptron::~Perceptron()
    {
        if (weights_ != nullptr)
        {
            utils::aligned_free(weights_);
            weights_ = nullptr;
        }
    }

    void Perceptron::PrintWeights()
    {
        std::cout << "Weights vector size: " << weight_vec_size_ << std::endl;
        for (std::size_t i = 0; i < weight_vec_size_; ++i)
        {
            std::cout << weights_[i] << " ";
        }
        std::cout << std::endl;
    }

    void Perceptron::Train(double *train_set, std::size_t train_set_size)
    {
        const std::size_t train_set_vec_size = input_size_ + 1;
        double *train_set_end = train_set + (train_set_size * train_set_vec_size);
        for (; train_set < train_set_end; train_set += train_set_vec_size)
        {
            double target = *(train_set + train_set_vec_size - 1);
            *(train_set + train_set_vec_size - 1) = -1;

            std::cout << "target: " << target << std::endl;
            for (int i = 0; i < train_set_vec_size; ++i)
            {
                std::cout << *(train_set + i) << " ";
            }
            std::cout << std::endl;

            double res = DotProduct(train_set, weights_, train_set_vec_size);
            std::cout << "dotProd res: " << res << std::endl;

            if (res >= 0 && target != 1)
            {
                /// t = 0 and y = 1
                /// w' = w - x
            }
            else if (res < 0 && target == 1)
            {
                /// t = 1 and y = 0
                /// w' = w + x
            }
        }
    }

} // end namespace unialgo