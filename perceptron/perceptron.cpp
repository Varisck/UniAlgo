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

    void Perceptron::PrintWeights(std::ostream &os = std::cout)
    {
        os << "Weights vector size: " << weight_vec_size_ << std::endl;
        for (std::size_t i = 0; i < weight_vec_size_; ++i)
        {
            os << weights_[i] << " ";
        }
        os << std::endl;
    }

    void Perceptron::Train(double *train_set, std::size_t train_set_size)
    {
        (*this).InitializeRandomWeights();
        const std::size_t train_set_vec_size = input_size_ + 1;
        double *train_set_end = train_set + (train_set_size * train_set_vec_size);
        for (; train_set < train_set_end; train_set += train_set_vec_size)
        {
            double target = *(train_set + train_set_vec_size - 1);
            *(train_set + train_set_vec_size - 1) = -1;

            double res = DotProduct(train_set, weights_, train_set_vec_size);

            // vectorize operation
            if (res >= 0 && target == 0)
            {
                /// t = 0 and y = 1
                /// w' = w - x
                for (std::size_t i = 0; i < weight_vec_size_; ++i)
                {
                    weights_[i] -= train_set[i];
                }
            }
            else if (res < 0 && target == 1)
            {
                /// t = 1 and y = 0
                /// w' = w + x
                for (std::size_t i = 0; i < weight_vec_size_; ++i)
                {
                    weights_[i] += train_set[i];
                }
            }
        }
    }

    bool Perceptron::Test(double *test_vec)
    {
        // use memcopy
        double *test_vec_with_delta = static_cast<double *>(utils::aligned_alloc(weightSize, (input_size_ + 1) * weightSize));
        for (int i = 0; i < input_size_; ++i)
        {
            test_vec_with_delta[i] = test_vec[i];
        }
        test_vec_with_delta[input_size_] = -1;
        double res = DotProduct(test_vec_with_delta, weights_, input_size_ + 1);
        unialgo::utils::aligned_free(test_vec_with_delta);
        return res >= 0;
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