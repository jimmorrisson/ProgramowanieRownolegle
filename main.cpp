#include <math.h>
#include <LBFGS.h>
#include "LBFGSExampleRosenbrock.h"
#include "ExtendedRosenbrockFunction.h"
#include "QuadraticFunction.h"
#include "PowerSingularFunction.h"
#include "TestUtil.h"

int main()
{
    TestUtil testUtilLib(1e-6, 1000, true);
    TestUtil testUtilAlg(1e-6, 1000, false);

    testUtilLib.runTest(LBFGSExampleRosenbrock(60));
    testUtilAlg.runTest(LBFGSExampleRosenbrock(60));
    
    testUtilLib.runTest(ExtendedRosenbrockFunction(60));
    testUtilAlg.runTest(ExtendedRosenbrockFunction(60));

    testUtilLib.runTest(QuadraticFunction(60));
    testUtilAlg.runTest(QuadraticFunction(60));

    testUtilLib.runTest(PowerSingularFunction(60));
    testUtilAlg.runTest(PowerSingularFunction(60));
    return 0;
}
