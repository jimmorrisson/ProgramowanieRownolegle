#include "PowerSingularFunction.h"

PowerSingularFunction::PowerSingularFunction(int size) : IFunction(size, prepareInitialVector(size))
{
}

VectorXd PowerSingularFunction::prepareInitialVector(int size)
{
    VectorXd x = VectorXd::Zero(size);
    for (int i = 3; i < size; i += 4)
    {
        x[i - 3] = 3;
        x[i - 2] = -1;
        x[i - 1] = 0;
        x[i] = 1;
    }

    return x;
}

double PowerSingularFunction::operator()(const VectorXd &x, VectorXd &grad)
{
    double fx = 0.0;
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
    for (int i = 3; i < size; i += 4)
    {
        double a = x[i - 3];
        double b = x[i - 2];
        double c = x[i - 1];
        double d = x[i];
        // #ifdef USE_PARALLEL_PROG
        // #pragma omp critical
        // #endif
        // {
        fx += pow(a + 10.0 * b, 2) + 5.0 * pow(c - d, 2) + pow(b - 2.0 * c, 4) + 10.0 * pow(a - d, 4);
        grad[i - 3] = 40.0 * pow(a - d, 3) + 2.0 * a + 20.0 * b;
        grad[i - 2] = 20.0 * a + 200.0 * b + 4.0 * pow(b - 2.0 * c, 3);
        grad[i - 1] = 10.0 * (c - d) - 8.0 * pow(b - 2.0 * c, 3);
        grad[i] = 10.0 * (-4.0 * pow(a - d, 3) - c + d);
        // }
    }
    return fx;
}
