#include "BFGSolver.h"
#include "IFunction.h"
#include <atomic>
#include <cstdio>
#include <Eigen/Dense>
#include <LBFGS.h>
#include <matrix.h>
#ifdef USE_PARALLEL_PROG
#include <omp.h>
#endif

BFGSolver::BFGSolver(double epsilon, int max_iterations) : epsilon(epsilon), max_iterations(max_iterations)
{
}

double BFGSolver::ArmijoBackTrack(IFunction &func,
                                  double fXk, Eigen::VectorXd &Xk, Eigen::VectorXd &gradXk,
                                  Eigen::VectorXd &Dk, VectorXd &Xk1, Eigen::VectorXd &gradXk1,
                                  double &&alpha, const double alphaCoeff, const double delta) const
{
    const double armijoCoeff = delta * gradXk.transpose() * Dk;
    std::atomic<bool> go(true);
#ifdef USE_PARALLEL_PROG
#pragma omp parallel shared(Xk1, alpha)
#endif
    {
        int i = 0;
        while (go)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
            {
                Xk1 = Xk + alpha * Dk;
            }

            const double fXk1 = func(Xk1, gradXk1);

            const double armijoCond = fXk + alpha * armijoCoeff;

            if (fXk1 <= armijoCond)
            {
                go = false;
            }

#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            alpha *= alphaCoeff;
        }
        ++i;
    }

    return alpha;
}

double BFGSolver::ArmijoBackTrack(IFunction &func,
                                  double fXk, math::Vector &Xk, math::Vector &gradXk,
                                  math::Vector &Dk, math::Vector &Xk1, math::Vector &gradXk1, double &&alpha,
                                  const double alphaCoeff, const double delta) const
{
    const double armijoCoeff = delta * gradXk.transpose() * Dk;
    std::atomic<bool> go(true);
#ifdef USE_PARALLEL_PROG
#pragma omp parallel shared(Xk1, alpha)
#endif
    {
        int i = 0;
        while (go)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
            {
                Xk1 = Xk + alpha * Dk;
            }

            const double fXk1 = func(Xk1, gradXk1);

            const double armijoCond = fXk + alpha * armijoCoeff;

            if (fXk1 <= armijoCond)
            {
                go = false;
            }

#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            alpha *= alphaCoeff;
        }
        ++i;
    }

    return alpha;
}

void BFGSolver::solve(IFunction &func, VectorXd &Xk, double &fx)
{
    // Step 1 init
    fx = 0;
    int size = func.size;

    std::atomic<bool> go(true);
    uint give = 0;
#ifdef USE_PARALLEL_PROG
#pragma omp parallel private(go) shared(Xk)
#endif
    {
        uint i;
        uint stop;
        MatrixXd I;
        MatrixXd Hk;
        VectorXd gradXk;
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
        {
#ifdef USE_PARALLEL_PROG
            std::cout << "threads=" << omp_get_num_threads() << std::endl;
#endif
            i = give;
#ifdef USE_PARALLEL_PROG
            give += max_iterations / omp_get_num_threads();
#endif
            stop = max_iterations;
            I = MatrixXd::Identity(size, size);
            Hk = MatrixXd::Identity(size, size);
            gradXk = VectorXd::Zero(size);
#ifdef USE_PARALLEL_PROG
            if (omp_get_thread_num() == omp_get_num_threads() - 1)
                stop = max_iterations;
#endif
        }

        while (i < stop && go)
        {
            fx = func(Xk, gradXk);

            double n = gradXk.norm();
            printf("N: %f\n", n);

            if (n <= epsilon)
            {
                go = false;
            }

            // Step 2 search direction
            VectorXd Dk = -Hk * gradXk;

            // Step 3 step length using Armijo rule
            VectorXd Xk1 = VectorXd::Zero(size);
            VectorXd gradXk1 = VectorXd::Zero(size);
            double step{};
#pragma omp atomic write
            step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);

            // Step 4 matrix Hk+1
            VectorXd Sk = step * Dk;
            VectorXd Yk = gradXk1 - gradXk;
            const auto Yk_trans = Yk.transpose();
            const auto Sk_trans = Sk.transpose();
            double hC = 1 / (Yk_trans * Sk);
            const auto new_Hk = (I - hC * Sk * Yk_trans) * Hk * (I - hC * Yk * Sk_trans) + hC * Sk * Sk_trans;
#pragma omp critical
            {
                Hk = new_Hk;
                Xk = Xk1;
            }
            i++;
        }
    }
#pragma omp barrier
}

void BFGSolver::solve(IFunction &func, math::Vector &Xk, double &fx)
{
    // Step 1 init
    fx = 0;
    int size = func.size;

    std::atomic<bool> go(true);
    uint give = 0;
#ifdef USE_PARALLEL_PROG
#pragma omp parallel private(go) shared(Xk)
#endif
    {
        uint i;
        uint stop;
        math::Matrix I;
        math::Matrix Hk;
        math::Vector gradXk;
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
        {
#ifdef USE_PARALLEL_PROG
            std::cout << "threads=" << omp_get_num_threads() << std::endl;
#endif
            i = give;
#ifdef USE_PARALLEL_PROG
            give += max_iterations / omp_get_num_threads();
#endif
            stop = max_iterations;
            I = math::Matrix::identity(size);
            Hk = math::Matrix::identity(size);
            gradXk = math::Vector(size);
#ifdef USE_PARALLEL_PROG
            if (omp_get_thread_num() == omp_get_num_threads() - 1)
                stop = max_iterations;
#endif
        }

        while (i < stop && go)
        {
            fx = func(Xk, gradXk);

            double n = gradXk.norm();
            printf("N: %f\n", n);

            if (n <= epsilon)
            {
                go = false;
            }

            // Step 2 search direction
            math::Vector Dk = (Hk * (-1)) * gradXk;

            // Step 3 step length using Armijo rule
            math::Vector Xk1(size);
            math::Vector gradXk1(size);
            double step{};
#pragma omp atomic write
            step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);

            // Step 4 matrix Hk+1
            auto Sk = step * Dk;
            auto Yk = gradXk1 - gradXk;
            const auto Yk_trans = Yk.transpose();
            const auto Sk_trans = Sk.transpose();
            double hC = 1 / (Yk_trans * Sk);
            const auto new_Hk = (I - hC * Sk * Yk_trans) * Hk * (I - hC * Yk * Sk_trans) + hC * Sk * Sk_trans;
#pragma omp critical
            {
                Hk = new_Hk;
                Xk = Xk1;
            }
            i++;
        }
    }
#pragma omp barrier
}
