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

int main()
{
    TestUtil testUtilLib(1e-6, 1000, true);
    TestUtil testUtilAlg(1e-6, 1000, false);

    std::unique_ptr<IFunction> exampleRosenbrock = std::make_unique<LBFGSExampleRosenbrock>(60);
    testUtilLib.runTest(*exampleRosenbrock.get());
    testUtilAlg.runTest(*exampleRosenbrock.get());

    std::unique_ptr<IFunction> extendedRosenbrock = std::make_unique<ExtendedRosenbrockFunction>(60);
    testUtilLib.runTest(*extendedRosenbrock.get());
    testUtilAlg.runTest(*extendedRosenbrock.get());

    std::unique_ptr<IFunction> quadraticFunction = std::make_unique<QuadraticFunction>(60);
    testUtilLib.runTest(*quadraticFunction.get());
    testUtilAlg.runTest(*quadraticFunction.get());

    std::unique_ptr<IFunction> powerSingularFunction = std::make_unique<PowerSingularFunction>(60);
    testUtilLib.runTest(*powerSingularFunction.get());
    testUtilAlg.runTest(*powerSingularFunction.get());

    #pragma omp parallel num_threads(4)
    {
        #pragma omp critical
        std::cout << "tid = " << omp_get_thread_num() << std::endl;
    }
    return 0;
}
