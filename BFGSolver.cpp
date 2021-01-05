#include "BFGSolver.h"
#include "IFunction.h"
#include <atomic>
#include <cstdio>
#include <matrix.h>
#ifdef USE_PARALLEL_PROG
#include <omp.h>
#endif

BFGSolver::BFGSolver(double epsilon, int max_iterations) : epsilon(epsilon), max_iterations(max_iterations)
{
}

double BFGSolver::ArmijoBackTrack(IFunction &func,
                                  double fXk, math::Vector &Xk, math::Vector &gradXk,
                                  math::Vector &Dk, math::Vector &Xk1, math::Vector &gradXk1, double &&alpha,
                                  const double alphaCoeff, const double delta) const
{
    const double armijoCoeff = delta * (math::Matrix::transpose(gradXk) * Dk).to_scalar();
    while (true)
    {
        Xk1 = Xk + alpha * Dk;
        const double fXk1 = func(Xk1, gradXk1);
        const double armijoCond = fXk + alpha * armijoCoeff;

        if (fXk1 <= armijoCond)
        {
            return alpha;
        }

        alpha *= alphaCoeff;
    }

    return alpha;
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
            go = true;
#endif
            stop = max_iterations;
            const auto mIdentity = math::Matrix::identity(size);
            const auto vectorZero = math::Vector(size);
            I = mIdentity;
            Hk = mIdentity;
            gradXk = vectorZero;
#ifdef USE_PARALLEL_PROG
            if (omp_get_thread_num() == omp_get_num_threads() - 1)
                stop = max_iterations;
#endif
        }

        while (i < stop && go)
        {
            fx = func(Xk, gradXk);

            double n = gradXk.norm();
            // printf("N: %f\n", n);

            if (n <= epsilon)
            {
                go = false;
            }

            // Step 2 search direction
            math::Vector Dk = math::Matrix::add_inv(Hk) * gradXk;

            // Step 3 step length using Armijo rule
            math::Vector Xk1(size);
            math::Vector gradXk1(size);
            double step{};
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
            step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);

            // Step 4 matrix Hk+1
            const auto Sk = step * Dk;
            auto Yk = gradXk1 - gradXk;
            const auto Yk_trans = math::Matrix::transpose(Yk);
            const auto Sk_trans = math::Matrix::transpose(Sk);
            double hC = 1 / (Yk_trans * Sk).to_scalar();
            const auto new_Hk = ((I - hC * (Sk * Yk_trans).to_scalar()) * Hk * (I - hC * (Yk * Sk_trans).to_scalar()) + hC * (Sk * Sk_trans).to_scalar());
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
            {
                Hk = new_Hk;
                Xk = Xk1;
            }
            i++;
        }
    }
#ifdef USE_PARALLEL_PROG
#pragma omp barrier
#endif
}
