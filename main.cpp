#include <math.h>
#include <LBFGS.h>
#include "LBFGSExampleRosenbrock.h"
#include "ExtendedRosenbrockFunction.h"
#include "QuadraticFunction.h"
#include "PowerSingularFunction.h"
#include "TestUtil.h"
#include <memory>
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[])
{
    TestUtil testUtilLib(1e-6, 1000, true);
    TestUtil testUtilAlg(1e-6, 1000, false);


    if (argc != 2) {
        return -1;
    }
    char *p;
    int size = strtol(argv[1], &p, 10);    

    std::unique_ptr<IFunction> exampleRosenbrock = std::make_unique<LBFGSExampleRosenbrock>(size);
    // testUtilLib.runTest(*exampleRosenbrock.get());
    testUtilAlg.runTest(*exampleRosenbrock.get());

    std::unique_ptr<IFunction> extendedRosenbrock = std::make_unique<ExtendedRosenbrockFunction>(size);
    // testUtilLib.runTest(*extendedRosenbrock.get());
    testUtilAlg.runTest(*extendedRosenbrock.get());

    std::unique_ptr<IFunction> quadraticFunction = std::make_unique<QuadraticFunction>(size);
    // testUtilLib.runTest(*quadraticFunction.get());
    testUtilAlg.runTest(*quadraticFunction.get());

    std::unique_ptr<IFunction> powerSingularFunction = std::make_unique<PowerSingularFunction>(size);
    // testUtilLib.runTest(*powerSingularFunction.get());
    testUtilAlg.runTest(*powerSingularFunction.get());

    testUtilAlg.runTest(*exampleRosenbrock.get());
    testUtilAlg.runTest(*extendedRosenbrock.get());
    testUtilAlg.runTest(*quadraticFunction.get());
    testUtilAlg.runTest(*powerSingularFunction.get());

    // #pragma omp parallel num_threads(4)
    // {
    //     #pragma omp critical
    //     std::cout << "tid = " << omp_get_thread_num() << std::endl;
    // }
    return 0;
}
