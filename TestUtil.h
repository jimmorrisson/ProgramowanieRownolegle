#include <chrono>
#include <LBFGS.h>
#include "IFunction.h"

class TestUtil
{
private:
	LBFGSParam<double> param;
public:
	TestUtil(double epsilon, int max_iterations, LINE_SEARCH_TERMINATION_CONDITION condition)
	{
		// Set up parameters
		param.epsilon = epsilon;
		param.max_iterations = max_iterations;
		param.linesearch = condition;
	}

	void runTest(IFunction &func) 
	{
		std::cout << "<--------------STARTING TEST-------------->" << std::endl;
		std::cout << func;

		// Create solver and function object
		LBFGSSolver<double, LineSearchBracketing> solver(param);
		
		double fx;

		// Running test
		auto t1 = std::chrono::high_resolution_clock::now();
		int niter = solver.minimize(func, func.initialVector, fx);
		auto t2 = std::chrono::high_resolution_clock::now();
		
		std::cout << niter << " iterations" << std::endl;
		std::cout << "x = \n" << func.initialVector.transpose() << std::endl;
		std::cout << "f(x) = " << fx << std::endl;
		std::cout << "Execution time: " 
			<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() 
			<< " ms" << std::endl;
		std::cout << "<--------------TEST FINISHED-------------->" << std::endl;
	}
};