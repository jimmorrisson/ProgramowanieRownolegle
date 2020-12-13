#include <iostream>
#include <math.h>
#include <Eigen/Core>
#include <LBFGS.h>
 
using Eigen::VectorXd;
using namespace LBFGSpp;
 
class Rosenbrock
{
private:
    int n;
public:
    Rosenbrock(int n_) : n(n_) {}
    double operator()(const VectorXd& x, VectorXd& grad)
    {
        double fx = 0.0;
        for(int i = 0; i < n; i += 2)
        {
            double t1 = 1.0 - x[i];
            double t2 = 10 * (x[i + 1] - x[i] * x[i]);
            grad[i + 1] = 20 * t2;
            grad[i]     = -2.0 * (x[i] * grad[i + 1] + t1);
            fx += t1 * t1 + t2 * t2;
        }
        return fx;
    }
};

class ExtendedRosenbrock
{
private:
	int n;
public:
	ExtendedRosenbrock(int n_) : n(n_) {}
	double operator()(const VectorXd& x, VectorXd& grad)
	{
		double fx = 0.0;
		for (int i = 0; i < n; i += 2)
		{
			double a = x[i + 1];
			double b = x[i];
			fx += 100.0 * pow(a - pow(b, 2), 2) + pow(1.0 - b, 2);
            grad[i + 1] = 200.0 * (a - pow(b, 2));
            grad[i] = -400.0 * a * b + 400 * pow(b, 3) + 2 * b - 2;
		}
		return fx;
	}
};

void TestRosenbrock(int n)
{
    std::cout << "Rosenbrock" << std::endl;

    // Set up parameters
    LBFGSParam<double> param;
    param.epsilon = 1e-6;
    param.max_iterations = 100;

    // Create solver and function object
    LBFGSSolver<double, LineSearchBracketing> solver(param);
    Rosenbrock fun(n);

    // Initial guess
    VectorXd x = VectorXd::Zero(n);
    // x will be overwritten to be the best point found
    double fx;
    int niter = solver.minimize(fun, x, fx);

    std::cout << niter << " iterations" << std::endl;
    std::cout << "x = \n" << x.transpose() << std::endl;
    std::cout << "f(x) = " << fx << std::endl;
}

void TestExtendedRosenbrock(int n)
{
    std::cout << "ExtendedRosenbrock" << std::endl;

    // Set up parameters
    LBFGSParam<double> param;
    param.epsilon = 1e-6;
    param.max_iterations = 100;

    // Create solver and function object
    LBFGSSolver<double, LineSearchBracketing> solver(param);
    ExtendedRosenbrock fun(n);

    // Initial guess
    VectorXd x = VectorXd::Zero(n);
    bool even = true;
    for (int i = 0; i < n; i++)
    {
        x[i] = even ? -1.2 : 1.0;
        even = !even;
    }

    // x will be overwritten to be the best point found
    double fx;
    int niter = solver.minimize(fun, x, fx);

    std::cout << niter << " iterations" << std::endl;
    std::cout << "x = \n" << x.transpose() << std::endl;
    std::cout << "f(x) = " << fx << std::endl;
}

int main()
{
    TestRosenbrock(10);
    TestExtendedRosenbrock(10);
 
    return 0;
}
