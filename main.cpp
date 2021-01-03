#include <climits>
#include <cmath>
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
