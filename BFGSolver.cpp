#include "BFGSolver.h"
#include "IFunction.h"
#include <Eigen/Dense>
#include <LBFGS.h>
#include <omp.h>

BFGSolver::BFGSolver(double epsilon, int max_iterations) : epsilon(epsilon), max_iterations(max_iterations)
{
}

double BFGSolver::ArmijoBackTrack(IFunction &func,
                                  double fXk, Eigen::VectorXd &Xk, Eigen::VectorXd &gradXk,
                                  Eigen::VectorXd &Dk, VectorXd &Xk1, Eigen::VectorXd &gradXk1,
                                  double &&alpha, const double alphaCoeff, const double delta) const
{
    const double armijoCoeff = delta * gradXk.transpose() * Dk;
    int i = 0;
    while (++i)
    {
        Xk1 = Xk + alpha * Dk;

        const double fXk1 = func(Xk1, gradXk1);

        const double armijoCond = fXk + alpha * armijoCoeff;

        if (fXk1 <= armijoCond)
        {
            // Armijo rule fulfilled
            return alpha;
        }

        alpha *= alphaCoeff;
    }

    return alpha;
}

int BFGSolver::solve(IFunction &func, VectorXd &Xk, double &fx)
{
    // Step 1 init
    fx = 0;
    int size = func.size;
    MatrixXd I = MatrixXd::Identity(size, size);
    MatrixXd Hk = MatrixXd::Identity(size, size);
    VectorXd gradXk = VectorXd::Zero(size);

    volatile bool flag = false;
#pragma omp parallel for shared(flag)
    for (int iter = 1; iter <= max_iterations; iter++)
    {
        if (flag)
            continue;
#pragma omp critical
        {
            fx = func(Xk, gradXk);

            double n = gradXk.norm();

            if (n <= epsilon)
            {
                // Solution found
                flag = true;
            }

            // Step 2 search direction
            VectorXd Dk = -Hk * gradXk;

            // Step 3 step length using Armijo rule
            VectorXd Xk1 = VectorXd::Zero(size);
            VectorXd gradXk1 = VectorXd::Zero(size);
            // #ifdef PARALLEL_PROG
            double step{};
            step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);

            // #else
            // double step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);
            // Step 4 matrix Hk+1
            VectorXd Sk = step * Dk;
            VectorXd Yk = gradXk1 - gradXk;
            double hC = 1 / (Yk.transpose() * Sk);
            Hk = (I - hC * Sk * Yk.transpose()) * Hk * (I - hC * Yk * Sk.transpose()) + hC * Sk * Sk.transpose();

            Xk = Xk1;
        }
    }

    // Iterations limit reached
    return max_iterations;
}
