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
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for shared(flag)
#endif
    for (int iter = 1; iter <= max_iterations; iter++)
    {
        if (flag)
            continue;
        #pragma omp atomic write
        fx = func(Xk, gradXk);

        double n = gradXk.norm();
        #pragma omp critical 
        {
            std::cout << "N: " << n << std::endl;
        }

        if (n <= epsilon)
        {
            // Solution found
            #ifndef USE_PARALLEL_PROG
            return iter;
            #else     
            #pragma omp atomic write       
            flag = true;
            #endif
        }

        // Step 2 search direction
        VectorXd Dk = -Hk * gradXk;

        // Step 3 step length using Armijo rule
        VectorXd Xk1 = VectorXd::Zero(size);
        VectorXd gradXk1 = VectorXd::Zero(size);
        // double step{};
// #ifdef USE_PARALLEL_PROG
// #pragma omp atomic write
// #endif
        const auto step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);

        // Step 4 matrix Hk+1
        VectorXd Sk = step * Dk;
        VectorXd Yk = gradXk1 - gradXk;
        const auto Yk_trans = Yk.transpose();
        const auto Sk_trans = Sk.transpose();
        double hC = 1 / (Yk_trans * Sk);
        const auto new_Hk = (I - hC * Sk * Yk_trans) * Hk * (I - hC * Yk * Sk_trans) + hC * Sk * Sk_trans;
        Hk = new_Hk;

        Xk = Xk1;
        // }
    }

    // Iterations limit reached
    return max_iterations;
}
