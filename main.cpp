#include <math.h>
#include <LBFGS.h>
#include "LBFGSExampleRosenbrock.h"
#include "ExtendedRosenbrockFunction.h"
#include "QuadraticFunction.h"
#include "PowerSingularFunction.h"
#include "TestUtil.h"

int main()
{
    TestUtil testUtil(1e-6, 100, LBFGS_LINESEARCH_BACKTRACKING_ARMIJO);

    testUtil.runTest(LBFGSExampleRosenbrock(60));
    testUtil.runTest(ExtendedRosenbrockFunction(60));
    testUtil.runTest(QuadraticFunction(60));
    testUtil.runTest(PowerSingularFunction(60));
    return 0;
}
