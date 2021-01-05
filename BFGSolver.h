#pragma once
#include <vector.h>
class IFunction;

class BFGSolver
{
private:
	double epsilon;
	int max_iterations;
	
	double ArmijoBackTrack(IFunction& func, 
		double fXk, math::Vector& Xk, math::Vector& gradXk, 
		math::Vector& Dk, math::Vector& Xk1, math::Vector& gradXk1, double &&alpha, 
		const double alphaCoeff, const double delta) const;
public:
	BFGSolver(double epsilon, int max_iterations);

	int solve(IFunction &func, math::Vector &Xk, double &fx);
};