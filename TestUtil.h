#pragma once
#include <chrono>
#include "IFunction.h"
#include "BFGSolver.h"

class TestUtil
{
private:
	double epsilon;
	int max_iterations;

	void solveBFGSImpl(IFunction& func, math::Vector& initialVector, double& fx)
	{
		BFGSolver solver(epsilon, max_iterations);
		return solver.solve(func, initialVector, fx);
	}

public:
	TestUtil(double epsilon, int max_iterations) : 
		epsilon(epsilon), max_iterations(max_iterations) {}

	void runTest(IFunction& func)
	{
		std::cout << "<--------------STARTING TEST-------------->" << std::endl;
		// std::cout << func;

		auto initialVector = func.getInitialVector();
		double fx;

		// Running test
		auto t1 = std::chrono::high_resolution_clock::now();
		solveBFGSImpl(func, initialVector, fx);
		auto t2 = std::chrono::high_resolution_clock::now();
		
		// std::cout << "x = \n" << initialVector.format(Eigen::IOFormat(Eigen::FullPrecision, 0, " ", ", ", "", "")) << std::endl;
		// std::cout << "f(x) = " << fx << std::endl;
		std::cout << "Execution time: " 
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() 
			<< " ms" << std::endl;
		std::cout << "<--------------TEST FINISHED-------------->" << std::endl;
	}
};