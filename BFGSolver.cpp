#include "BFGSolver.h"
#include "IFunction.h"
#include <atomic>
#include <cstdio>
#include <matrix.h>

typedef unsigned int uint;

BFGSolver::BFGSolver(double epsilon, int max_iterations) : epsilon(epsilon), max_iterations(max_iterations)
{
}

double BFGSolver::ArmijoBackTrack(IFunction &func,
                                  double fXk, math::Vector &Xk, math::Vector &gradXk,
                                  math::Vector &Dk, math::Vector &Xk1, math::Vector &gradXk1, double &&alpha,
                                  const double alphaCoeff, const double c1, const double c2) const
{
	math::Matrix Dk_trans = math::Matrix::transpose(Dk);
	double condCoeff = (Dk_trans * gradXk).to_scalar();
	double armijoCoeff = c1 * condCoeff;
	double curvatureCoeff = -c2 * condCoeff;

    while (true)
    {
        Xk1 = Xk + alpha * Dk;
        double fXk1 = func(Xk1, gradXk1);

        double armijoCond = fXk + alpha * armijoCoeff;
		double curvatureCond = -((Dk_trans * gradXk1).to_scalar());

		if (fXk1 <= armijoCond && curvatureCond <= curvatureCoeff)
        {
            return alpha;
        }

        alpha *= alphaCoeff;
    }

    return alpha;
}

void BFGSolver::solve(IFunction& func, math::Vector& Xk, double& fx)
{
	// Step 1 init
	fx = 0;
	int size = func.size;

	math::Matrix I = math::Matrix::identity(size);
	math::Matrix Hk = math::Matrix::identity(size);
	math::Vector gradXk = math::Vector(size);

	for (int i = 1; i <= max_iterations; i++)
	{
		fx = func(Xk, gradXk);

		double n = gradXk.norm();

		if (n <= epsilon)
		{
			std::cout << "Number of iterations: " << i << std::endl;
			break;
		}

		// Step 2 search direction
		math::Vector Dk = -(Hk * gradXk);

		// Step 3 step length using Armijo rule
		math::Vector Xk1(size);
		math::Vector gradXk1(size);
		double step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.8, 0.001, 0.9);

		// Step 4 matrix Hk+1
		math::Vector Sk = step * Dk;
		math::Vector Yk = gradXk1 - gradXk;
		math::Matrix Sk_m = math::Matrix(Sk);
		math::Matrix Yk_m = math::Matrix(Yk);
		math::Matrix Sk_trans = math::Matrix::transpose(Sk);
		math::Matrix Yk_trans = math::Matrix::transpose(Yk);
		double hC = 1.0 / (Yk_trans * Sk).to_scalar();

		Hk = (I - hC * Sk_m * Yk_trans) * Hk * (I - hC * Yk_m * Sk_trans) + hC * Sk_m * Sk_trans;

		Xk = Xk1;
	}
}
