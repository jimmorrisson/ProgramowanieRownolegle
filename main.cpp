#include <climits>
#include <cmath>
#include "ExtendedRosenbrockFunction.h"
#include "QuadraticFunction.h"
#include "PowellSingularFunction.h"
#include "TestUtil.h"
#include <memory>
#include <iostream>
#include <vector.h>
#include <matrix.h>

int main(int argc, char* argv[])
{
    TestUtil testUtilAlg(0.01, INT_MAX);


    if (argc != 2) {
        std::cout << "Invalid number of arguments \n"
                  << "Program usage : \n"
                  << "./ParallelProgramming %n \n"
                  << "n - size \n";
        return -1;
    }
    char *p;
    int size = strtol(argv[1], &p, 10);

    // std::unique_ptr<IFunction> extendedRosenbrock = std::make_unique<ExtendedRosenbrockFunction>(size);
    // testUtilAlg.runTest(*extendedRosenbrock.get());

    std::unique_ptr<IFunction> quadraticFunction = std::make_unique<QuadraticFunction>(size);
    testUtilAlg.runTest(*quadraticFunction.get());

    // std::unique_ptr<IFunction> powerSingularFunction = std::make_unique<PowellSingularFunction>(size);
    // testUtilAlg.runTest(*powerSingularFunction.get());

    return 0;
}
