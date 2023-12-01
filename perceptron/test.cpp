#include <cstddef> //std::size_t
#include <iostream>
#include <random>
#include <cassert>

#include "UniAlgo/utils/alignedAlloc.h"
#include "UniAlgo/perceptron/perceptron.h"

template <typename T>
void TestPerceptronReal(std::size_t train_set_size, std::uniform_real_distribution<T> dis1, std::uniform_real_distribution<T> dis2)
{

    unialgo::Perceptron p = unialgo::Perceptron(3);
    p.PrintWeights();

    T *train_set = static_cast<T *>(unialgo::utils::aligned_alloc(sizeof(T), sizeof(T) * 4 * train_set_size));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> disP(0, 1);

    int dist_c1 = 0, dist_c2 = 0;
    for (int i = 0; i < train_set_size; ++i)
    {
        if (disP(gen) == 0)
        {
            for (int j = 0; j < 3; ++j)
            {
                train_set[i * 4 + j] = static_cast<T>(dis1(gen));
            }
            train_set[i * 4 + 3] = 0;
            dist_c1 += 1;
        }
        else
        {
            for (int j = 0; j < 3; ++j)
            {
                train_set[i * 4 + j] = static_cast<T>(dis2(gen));
            }
            train_set[i * 4 + 3] = 1;
            dist_c2 += 1;
        }
    }
    std::cout << "Train set generated with " << dist_c1
              << " instances of class 0 and " << dist_c2
              << " instances of class 1" << std::endl;

    assert(dist_c1 + dist_c2 == train_set_size);

    p.Train<T>(train_set, train_set_size);
    unialgo::utils::aligned_free(train_set);
    p.PrintWeights();

    T *test = static_cast<T *>(unialgo::utils::aligned_alloc(sizeof(T), 3 * sizeof(T)));

    auto testVector = [](T *testVec, std::uniform_real_distribution<T> &distribution, std::mt19937 &device, unialgo::Perceptron &p, bool target)
    {
        testVec[0] = static_cast<T>(distribution(device));
        testVec[1] = static_cast<T>(distribution(device));
        testVec[2] = static_cast<T>(distribution(device));
        bool res = p.Test(testVec);
        std::cout << "result of test using vector ("
                  << testVec[0] << ", " << testVec[1] << ", " << testVec[2]
                  << ") is : " << res << " target: " << target << std::endl;
    };

    testVector(test, dis1, gen, p, 0);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis1, gen, p, 0);
    testVector(test, dis1, gen, p, 0);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis1, gen, p, 0);

    unialgo::utils::aligned_free(test);
}

template <typename T>
void TestPerceptron(std::size_t train_set_size, std::uniform_int_distribution<T> dis1, std::uniform_int_distribution<T> dis2)
{

    unialgo::Perceptron p = unialgo::Perceptron(3);
    p.PrintWeights();

    T *train_set = static_cast<T *>(unialgo::utils::aligned_alloc(sizeof(T), sizeof(T) * 4 * train_set_size));

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> disP(0, 1);

    int dist_c1 = 0, dist_c2 = 0;
    for (int i = 0; i < train_set_size; ++i)
    {
        if (disP(gen) == 0)
        {
            for (int j = 0; j < 3; ++j)
            {
                train_set[i * 4 + j] = static_cast<T>(dis1(gen));
            }
            train_set[i * 4 + 3] = 0;
            dist_c1 += 1;
        }
        else
        {
            for (int j = 0; j < 3; ++j)
            {
                train_set[i * 4 + j] = static_cast<T>(dis2(gen));
            }
            train_set[i * 4 + 3] = 1;
            dist_c2 += 1;
        }
    }
    std::cout << "Train set generated with " << dist_c1
              << " instances of class 0 and " << dist_c2
              << " instances of class 1" << std::endl;

    assert(dist_c1 + dist_c2 == train_set_size);

    p.Train<T>(train_set, train_set_size);
    unialgo::utils::aligned_free(train_set);
    p.PrintWeights();

    T *test = static_cast<T *>(unialgo::utils::aligned_alloc(sizeof(T), 3 * sizeof(T)));

    auto testVector = [](T *testVec, std::uniform_int_distribution<T> &distribution, std::mt19937 &device, unialgo::Perceptron &p, bool target)
    {
        testVec[0] = static_cast<T>(distribution(device));
        testVec[1] = static_cast<T>(distribution(device));
        testVec[2] = static_cast<T>(distribution(device));
        bool res = p.Test(testVec);
        std::cout << "result of test using vector ("
                  << testVec[0] << ", " << testVec[1] << ", " << testVec[2]
                  << ") is : " << res << " target: " << target << std::endl;
    };

    testVector(test, dis1, gen, p, 0);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis1, gen, p, 0);
    testVector(test, dis1, gen, p, 0);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis2, gen, p, 1);
    testVector(test, dis1, gen, p, 0);

    unialgo::utils::aligned_free(test);
}

int main()
{
    TestPerceptronReal<double>(600, std::uniform_real_distribution<double>(0.0, 2.0),
                               std::uniform_real_distribution<double>(6.0, 9.0));
    TestPerceptron<int>(600, std::uniform_int_distribution<int>(0, 20),
                        std::uniform_int_distribution<int>(40, 60));
    return 0;
}