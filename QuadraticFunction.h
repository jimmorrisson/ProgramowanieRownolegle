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
    QuadraticFunction(int size) :IFunction(size, math::Vector(size)) {}

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
            fx += 201 * pow(x.at_r(i), 2);
            grad.at(i) = 402 * x.at_r(i);
        }

        fx += 200 * pow(x.at_r(size - 2), 2);
        grad.at(size - 2) = 400 * x.at_r(size - 2);

        fx += 100 * pow(x.at_r(size - 1), 2);
        grad.at(size - 1) = 200 * x.at_r(size - 1);

        return fx;
    }

    double operator()(const VectorXd& x, VectorXd& grad)
    {
        double fx = pow(x[0], 2);
        grad[0] = 2 * x[0];

        fx += 101 * pow(x[1], 2);
        grad[1] = 202 * x[1];

        #ifdef USE_PARALLEL_PROG
        #pragma omp parallel for
        #endif
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