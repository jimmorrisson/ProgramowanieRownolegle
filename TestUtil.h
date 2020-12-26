#pragma once
#include <chrono>
#include <LBFGS.h>
#include "IFunction.h"
#include "BFGSolver.h"

class TestUtil
{
private:
	double epsilon;
	int max_iterations;
	bool useLBFGSLib;

	int solveLBFGSLib(IFunction& func, VectorXd& initialVector, double& fx)
	{
		// Create solver and function object
		LBFGSParam<double> param;
		param.epsilon = epsilon;
		param.max_iterations = max_iterations;
		param.linesearch = LBFGS_LINESEARCH_BACKTRACKING_ARMIJO;
		LBFGSSolver<double, LineSearchBracketing> solver(param);
		
		return solver.minimize(func, initialVector, fx);
	}

	int solveBFGSImpl(IFunction& func, VectorXd& initialVector, double& fx)
	{
		BFGSolver solver(epsilon, max_iterations);
		return solver.solve(func, initialVector, fx);
	}

public:
	TestUtil(double epsilon, int max_iterations, bool useLBFGSLib) : 
		epsilon(epsilon), max_iterations(max_iterations), useLBFGSLib(useLBFGSLib) {}

	void runTest(IFunction& func)
	{
		std::cout << "<--------------STARTING TEST-------------->" << std::endl;
		// std::cout << func;

		VectorXd initialVector = func.getInitialVector();
		double fx;

		// Running test
		auto t1 = std::chrono::high_resolution_clock::now();
		int iterationNumber = useLBFGSLib ? solveLBFGSLib(func, initialVector, fx) : solveBFGSImpl(func, initialVector, fx);
		auto t2 = std::chrono::high_resolution_clock::now();
		
		std::cout << iterationNumber << " iterations" << std::endl;
		// std::cout << "x = \n" << initialVector.format(Eigen::IOFormat(Eigen::FullPrecision, 0, " ", ", ", "", "")) << std::endl;
		// std::cout << "f(x) = " << fx << std::endl;
		std::cout << "Execution time: " 
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() 
			<< " ms" << std::endl;
		std::cout << "<--------------TEST FINISHED-------------->" << std::endl;
	}
};