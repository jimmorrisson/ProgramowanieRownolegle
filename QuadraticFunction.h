#pragma once
#include "IFunction.h"

class QuadraticFunction : public IFunction
{
private:
    VectorXd prepareInitialVector(int size)
    {
        return VectorXd::Constant(size, 3);
    }

public:
    QuadraticFunction(int size) :IFunction(size, prepareInitialVector(size)) {}

    double operator()(const VectorXd& x, VectorXd& grad)
    {
        double fx = pow(x[0], 2);
        grad[0] = 2 * x[0];

        fx += 101 * pow(x[1], 2);
        grad[1] = 202 * x[1];

        for (int i = 2; i <= size - 3; i++)
        {
            fx += 201 * pow(x[i], 2);
            grad[i] = 402 * x[i];
        }

        fx += 200 * pow(x[size - 2], 2);
        grad[size - 2] = 400 * x[size - 2];

        fx += 100 * pow(x[size - 1], 2);
        grad[size - 1] = 200 * x[size - 1];

        return fx;
    }
};