#include "PowellSingularFunction.h"
#include <atomic>

PowellSingularFunction::PowellSingularFunction(int size) : IFunction(size, prepareInitialVector(size))
{
}

math::Vector PowellSingularFunction::prepareInitialVector(int size)
{
    math::Vector x(size);
    for (int i = 3; i < size; i += 4)
    {
        x.at(i - 3) = 3;
        x.at(i - 2) = -1;
        x.at(i - 1) = 0;
        x.at(i) = 1;
    }

    return math::Vector(std::move(x));
}

double PowellSingularFunction::operator()(const math::Vector& x, math::Vector& grad)
{
    double fx = 0.0;
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
    for (int i = 3; i < size; i += 4)
    {
        double a = x.at_r(i - 3);
        double b = x.at_r(i - 2);
        double c = x.at_r(i - 1);
        double d = x.at_r(i);

#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
        fx += pow(a + 10.0 * b, 2) + 5.0 * pow(c - d, 2) + pow(b - 2.0 * c, 4) + 10.0 * pow(a - d, 4);
        grad.at(i - 3) = 40.0 * pow(a - d, 3) + 2.0 * a + 20.0 * b;
        grad.at(i - 2) = 20.0 * a + 200.0 * b + 4.0 * pow(b - 2.0 * c, 3);
        grad.at(i - 1) = 10.0 * (c - d) - 8.0 * pow(b - 2.0 * c, 3);
        grad.at(i) = 10.0 * (-4.0 * pow(a - d, 3) - c + d);
    }
    return fx;
}
