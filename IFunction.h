#pragma once
#include <iostream>
#include <iomanip>
#include <Eigen/Core>
#include <Eigen/Dense>
#include <LBFGS.h>
#include <matrix.h>
#include <vector.h>
using Eigen::VectorXd;
using Eigen::MatrixXd;
using namespace LBFGSpp;

class IFunction
{
protected:
	VectorXd initialVector;

	IFunction(int size, VectorXd initialVector);

public:
	const int size;

	virtual ~IFunction() = default;

	VectorXd getInitialVector();

	virtual double operator()(const math::Vector& x, math::Vector& grad) = 0;

	virtual double operator()(const VectorXd& x, VectorXd& grad) = 0;

	friend std::ostream& operator<<(std::ostream& out, const IFunction& func);
};
