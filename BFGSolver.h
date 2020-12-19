#include "IFunction.h"

class BFGSolver
{
private:
	double epsilon;
	int max_iterations;

public:
	BFGSolver(double epsilon, int max_iterations) : epsilon(epsilon), max_iterations(max_iterations) {}

	int solve(IFunction& func, VectorXd& initialVector, double& fx)
	{
		// TODO
		fx = 0;
		return 0;
	}
};