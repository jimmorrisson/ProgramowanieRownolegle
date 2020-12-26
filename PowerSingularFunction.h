#pragma once
#include "IFunction.h"

class PowerSingularFunction final : public IFunction
{
private:
    VectorXd prepareInitialVector(int size);

public:
    PowerSingularFunction(int size);
    double operator()(const VectorXd& x, VectorXd& grad) final;
};