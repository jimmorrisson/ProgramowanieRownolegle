#pragma once
#include "IFunction.h"

class ExtendedRosenbrockFunction : public IFunction
{
private:
    math::Vector prepareInitialVector(int size)
    {
        math::Vector x(size);
        for (int i = 1; i < size; i += 2)
        {
            x.at(i - 1) = -1.2;
            x.at(i) = 1.0;
        }

        return math::Vector(std::move(x));
    }
public:
    ExtendedRosenbrockFunction(int size) 
        : IFunction(size, "Extended Rosenbrock function", prepareInitialVector(size)) {}

    double operator()(const math::Vector& x, math::Vector& grad)
    {
        double fx = 0.0;
        
        for (int i = 1; i < size; i += 2)
        {
            double a = x.at_r(i);
            double b = x.at_r(i - 1);
            fx += 100.0 * pow(a - pow(b, 2), 2) + pow(1.0 - b, 2);
            grad.at(i) = 200.0 * (a - pow(b, 2));
            grad.at(i - 1) = 2.0 * (-200.0 * a * b + 200 * pow(b, 3) + b - 1);                
        }
        return fx;
    }
};
