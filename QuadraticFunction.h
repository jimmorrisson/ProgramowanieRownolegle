#pragma once
#include "IFunction.h"

class QuadraticFunction : public IFunction
{
private:

public:
    QuadraticFunction(int size) 
        : IFunction(size, "Quadratic function", math::Vector(size, 3)) {}

    double operator()(const math::Vector& x, math::Vector& grad)
    {
        double fx = pow(x.at_r(0), 2);
        grad.at(0) = 2 * x.at_r(0);

        fx += 101 * pow(x.at_r(1), 2);
        grad.at(1) = 202 * x.at_r(1);

#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (int i = 2; i <= size - 3; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            fx += 201 * pow(x.at_r(i), 2);
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
            grad.at(i) = 402 * x.at_r(i);
        }

        fx += 200 * pow(x.at_r(size - 2), 2);
        grad.at(size - 2) = 400 * x.at_r(size - 2);

        fx += 100 * pow(x.at_r(size - 1), 2);
        grad.at(size - 1) = 200 * x.at_r(size - 1);

        return fx;
    }
};