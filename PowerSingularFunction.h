#pragma once
#include "IFunction.h"

class PowerSingularFunction final : public IFunction
{
private:
    math::Vector prepareInitialVector(int size);

public:
    PowerSingularFunction(int size);
    double operator()(const math::Vector& x, math::Vector& grad) final;
};