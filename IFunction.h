#pragma once
#include <iostream>
#include <iomanip>
#include <Eigen/Core>
using Eigen::VectorXd;
using namespace LBFGSpp;

class IFunction
{
protected:
	const int size;

	VectorXd initialVector;

	IFunction(int size, VectorXd initialVector) : size(size), initialVector(initialVector) {}

public:

	VectorXd getInitialVector()
	{
		return VectorXd(initialVector);
	}

	virtual double operator()(const VectorXd& x, VectorXd& grad) = 0;

	friend std::ostream& operator<<(std::ostream& out, const IFunction& func);
};

std::ostream& operator<<(std::ostream& out, const IFunction& func)
{
	out << typeid(func).name() << std::endl
		<< "Size: " << func.size << std::endl
		<< "Initial vector: " << std::endl
		<< func.initialVector.format(Eigen::IOFormat(Eigen::FullPrecision, 0, " ", ", ", "", ""))
		<< std::endl;

	return out;
}