#pragma once
#include "IFunction.h"

class LBFGSExampleRosenbrock final : public IFunction
{
private:

public:
    LBFGSExampleRosenbrock(int size);

    double operator()(const math::Vector& x, math::Vector& grad) final;
    double operator()(const VectorXd& x, VectorXd& grad) final;
};