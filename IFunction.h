#pragma once
#include <iostream>
#include <iomanip>
#include <matrix.h>
#include <vector.h>

class IFunction
{
protected:
	math::Vector initialVector;

	IFunction(int size, math::Vector initialVector);

public:
	const int size;

	virtual ~IFunction() = default;

	math::Vector getInitialVector();

	virtual double operator()(const math::Vector& x, math::Vector& grad) = 0;

	friend std::ostream& operator<<(std::ostream& out, const IFunction& func);
};
