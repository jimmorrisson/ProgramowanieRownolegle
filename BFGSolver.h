#pragma once
#include <Eigen/Core>
#include <vector.h>
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

	double ArmijoBackTrack(IFunction& func, 
		double fXk, math::Vector& Xk, math::Vector& gradXk, 
		math::Vector& Dk, math::Vector& Xk1, math::Vector& gradXk1, double &&alpha, 
		const double alphaCoeff, const double delta) const;
public:
	BFGSolver(double epsilon, int max_iterations);

	void solve(IFunction& func, Eigen::VectorXd& Xk, double& fx);
	void solve(IFunction &func, math::Vector &Xk, double &fx);
};