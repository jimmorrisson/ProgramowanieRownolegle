#pragma once
#include "IFunction.h"

class LBFGSExampleRosenbrock final : public IFunction
{
private:
    VectorXd prepareInitialVector(int size);

public:
    LBFGSExampleRosenbrock(int size);

    double operator()(const VectorXd& x, VectorXd& grad) final;
};