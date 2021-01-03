#include <math.h>
#include <LBFGS.h>
#include "LBFGSExampleRosenbrock.h"
#include "ExtendedRosenbrockFunction.h"
#include "QuadraticFunction.h"
#include "PowerSingularFunction.h"
#include "TestUtil.h"
#include <memory>
#include <iostream>
#include <vector.h>
#include <matrix.h>

int main(int argc, char* argv[])
{
    const auto s = 4;
    math::Vector vec{s, 1};
    auto asdf = 4 * vec;
    for (int i = 0; i < s; i++)
    {
        std::cout << asdf.at(i) << "\n";
    }
    auto mat = math::Matrix::identity(s);
    for (int i = 0; i < s; i++)
    {
        for (int j = 0; j < s; j++)
        {
            std::cout << mat.at(i, j) << " ";
        }
        std::cout << "\n";
    }

    const auto vec_2 = asdf * mat;
    for (int i = 0; i < vec_2.getSize(); i++)
    {
        std::cout << vec_2.at_r(i) << "\n";
    }
    TestUtil testUtilLib(1e-6, INT_MAX, true);
    TestUtil testUtilAlg(0.01, INT_MAX, false);


    if (argc != 2) {
        std::cout << "Invalid number of arguments \n"
                  << "Program usage : \n"
                  << "./ParallelProgramming %n \n"
                  << "n - size \n";
        return -1;
    }
    char *p;
    int size = strtol(argv[1], &p, 10);    
    std::unique_ptr<IFunction> exampleRosenbrock = std::make_unique<LBFGSExampleRosenbrock>(size);
    testUtilAlg.runTest(*exampleRosenbrock.get());

    std::unique_ptr<IFunction> extendedRosenbrock = std::make_unique<ExtendedRosenbrockFunction>(size);
    testUtilAlg.runTest(*extendedRosenbrock.get());

    std::unique_ptr<IFunction> quadraticFunction = std::make_unique<QuadraticFunction>(size);
    testUtilAlg.runTest(*quadraticFunction.get());

    std::unique_ptr<IFunction> powerSingularFunction = std::make_unique<PowerSingularFunction>(size);
    testUtilAlg.runTest(*powerSingularFunction.get());

    return 0;
}
