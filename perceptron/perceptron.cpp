#include "UniAlgo/perceptron/perceptron.h"
#include "UniAlgo/utils/alignedAlloc.h"

namespace unialgo
{

    Perceptron::Perceptron(std::size_t inputSize)
    {
        inputSize_ = inputSize + 1;
        weightType *weights_ = static_cast<weightType *>(utils::aligned_alloc(weightSize, weightSize * inputSize_));
    }

    Perceptron::~Perceptron()
    {
        if (weights_ != nullptr)
        {
            utils::aligned_free(weights_);
            weights_ = nullptr;
        }
    }

} // end namespace unialgo