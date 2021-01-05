#pragma once
#include "IFunction.h"

class ExtendedRosenbrockFunction : public IFunction
{
private:
public:
    ExtendedRosenbrockFunction(int size) :IFunction(size, math::Vector(size)) {}

    double operator()(const math::Vector& x, math::Vector& grad)
    {
        double fx = 0.0;
        #ifdef USE_PARALLEL_PROG
        #pragma omp parallel for
        #endif
        for (int i = 1; i < size; i += 2)
        {
            double a = x.at_r(i);
            double b = x.at_r(i - 1);
            #ifdef USE_PARALLEL_PROG
            #pragma omp  atomic update
            #endif
            fx += 100.0 * pow(a - pow(b, 2), 2) + pow(1.0 - b, 2);
            #ifdef USE_PARALLEL_PROG
            #pragma omp  atomic write
            #endif
            grad.at(i) = 200.0 * (a - pow(b, 2));
            #ifdef USE_PARALLEL_PROG
            #pragma omp  atomic write
            #endif
            grad.at(i - 1) = 2.0 * (-200.0 * a * b + 200 * pow(b, 3) + b - 1);                
        }
        return fx;
    }
};
