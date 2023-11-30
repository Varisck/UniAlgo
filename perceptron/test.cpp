#include <cstddef> //std::size_t
#include <iostream>
#include <random>

#include "UniAlgo/utils/alignedAlloc.h"
#include "UniAlgo/perceptron/perceptron.h"

int main()
{
    unialgo::Perceptron p = unialgo::Perceptron(3);
    p.PrintWeights();

    double *train_set = static_cast<double *>(unialgo::utils::aligned_alloc(sizeof(double), sizeof(double) * 4 * 400));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0f, 0.5f);
    std::uniform_real_distribution<double> dis2(0.5f, 1.0f);
    std::uniform_int_distribution<int> disP(0, 1);

    for (int i = 0; i < 400; ++i)
    {
        if (disP(gen) == 0)
        {
            for (int j = 0; j < 3; ++j)
            {
                train_set[i * 4 + j] = dis(gen);
            }
            train_set[i * 4 + 3] = 0;
        }
        else
        {
            for (int j = 0; j < 3; ++j)
            {
                train_set[i * 4 + j] = dis2(gen);
            }
            train_set[i * 4 + 3] = 1;
        }
    }

    p.Train(train_set, 400);
    unialgo::utils::aligned_free(train_set);
    p.PrintWeights();

    double *test = static_cast<double *>(unialgo::utils::aligned_alloc(sizeof(double), 3 * sizeof(double)));

    auto testVector = [](double *testVec, std::uniform_real_distribution<double> &distribution, std::mt19937 &device, unialgo::Perceptron &p, bool target)
    {
        testVec[0] = distribution(device);
        testVec[1] = distribution(device);
        testVec[2] = distribution(device);
        bool res = p.Test(testVec);
        std::cout << "result of test using vector ("
                  << testVec[0] << ", " << testVec[1] << ", " << testVec[2]
                  << ") is : " << res << " target: " << target << std::endl;
    };

    testVector(test, dis, gen, p, 0);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis, gen, p, 0);
    testVector(test, dis, gen, p, 0);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis, gen, p, 0);

    unialgo::utils::aligned_free(test);

    return 0;
}