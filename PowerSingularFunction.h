#pragma once
#include "IFunction.h"

class PowerSingularFunction : public IFunction
{
private:
    VectorXd prepareInitialVector(int size);

public:
    PowerSingularFunction(int size) :IFunction(size, prepareInitialVector(size)) {}
    double operator()(const VectorXd& x, VectorXd& grad);
};