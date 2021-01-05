#include "BFGSolver.h"
#include "IFunction.h"
#include <atomic>
#include <cstdio>
#include <matrix.h>
#include <chrono>
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

int BFGSolver::solve(IFunction &func, math::Vector &Xk, double &fx)
{
// TODO: Output somewhere number of threads
// #ifdef USE_PARALLEL_PROG
//         // Only master thread should write to output
//         if (omp_get_thread_num() == 0)
//         {
//             std::cout << "Number of threads = " << omp_get_num_threads() << std::endl;
//         }
// #else
//         std::cout << "Number of threads = " << 1 << std::endl;
// #endif

    // Step 1 init
    fx = 0;
    int size = func.size;
    math::Matrix I = math::Matrix::identity(size);
    math::Matrix Hk = math::Matrix::identity(size);
    math::Vector gradXk(size);

    for (int i = 1; i <= max_iterations; i++)
    {
        std::cout << "iteration = " << i << std::endl;

        auto t1 = std::chrono::high_resolution_clock::now();
        fx = func(Xk, gradXk);
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "grad: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microsec" << std::endl;

        t1 = std::chrono::high_resolution_clock::now();
        double n = gradXk.norm();
        t2 = std::chrono::high_resolution_clock::now();
        std::cout << "norm: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microsec" << std::endl;

        if (n <= epsilon)
        {
            return i;
        }

        // Step 2 search direction
        t1 = std::chrono::high_resolution_clock::now();
        math::Vector Dk = math::Matrix::add_inv(Hk) * gradXk;
        t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Dk: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microsec" << std::endl;

        // Step 3 step length using Armijo rule
        math::Vector Xk1(size);
        math::Vector gradXk1(size);
        t1 = std::chrono::high_resolution_clock::now();
        double step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, Xk1, gradXk1, 1, 0.2, 0.0001);
        t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Armijo: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microsec" << std::endl;

        // Step 4 matrix Hk+1
        const auto Sk = step * Dk;
        auto Yk = gradXk1 - gradXk;
        const auto Yk_trans = math::Matrix::transpose(Yk);
        const auto Sk_trans = math::Matrix::transpose(Sk);
        double hC = 1 / (Yk_trans * Sk).to_scalar();
        
        std::cout << "Hk+1 start" << std::endl;
        t1 = std::chrono::high_resolution_clock::now();
        Hk = ((I - hC * (Sk * Yk_trans).to_scalar()) * Hk * (I - hC * (Yk * Sk_trans).to_scalar()) + hC * (Sk * Sk_trans).to_scalar());
        t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Hk+1 duration: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << " microsec" << std::endl;

        Xk = Xk1;
    }

    return INT32_MAX;
}
