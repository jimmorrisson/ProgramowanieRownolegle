#pragma once
#include <Eigen/Core>

class IFunction;

class BFGSolver
{
private:
	double epsilon;
	int max_iterations;
	
	double ArmijoBackTrack(IFunction& func, 
		double fXk, Eigen::VectorXd& Xk, Eigen::VectorXd& gradXk, 
		Eigen::VectorXd& Dk, Eigen::VectorXd& Xk1, Eigen::VectorXd& gradXk1, double &&alpha, 
		const double alphaCoeff, const double delta) const;
public:
	BFGSolver(double epsilon, int max_iterations);

	int solve(IFunction& func, Eigen::VectorXd& Xk, double& fx);
};