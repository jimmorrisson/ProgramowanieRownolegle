#pragma once
#include "IFunction.h"

class ExtendedRosenbrockFunction : public IFunction
{
private:
	VectorXd prepareInitialVector(int size)
	{
        VectorXd vect = VectorXd::Zero(size);
        bool even = true;
        for (int i = 0; i < size; i++)
        {
            vect[i] = even ? -1.2 : 1.0;
            even = !even;
        }

        return vect;
	}

public:
    ExtendedRosenbrockFunction(int size) :IFunction(size, prepareInitialVector(size)) {}

    double operator()(const VectorXd& x, VectorXd& grad)
    {
        double fx = 0.0;
        for (int i = 1; i < size; i += 2)
        {
            double a = x[i];
            double b = x[i - 1];
            fx += 100.0 * pow(a - pow(b, 2), 2) + pow(1.0 - b, 2);
            grad[i] = 200.0 * (a - pow(b, 2));
            grad[i - 1] = 2.0 * (-200.0 * a * b + 200 * pow(b, 3) + b - 1);
        }
        return fx;
    }
};