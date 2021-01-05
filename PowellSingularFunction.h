#pragma once
#include "IFunction.h"

class PowellSingularFunction final : public IFunction
{
private:
    math::Vector prepareInitialVector(int size);

public:
    PowellSingularFunction(int size);
    double operator()(const math::Vector& x, math::Vector& grad) final;
};