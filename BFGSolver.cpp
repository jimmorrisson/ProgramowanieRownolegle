#include "BFGSolver.h"
#include "IFunction.h"
#include <atomic>
#include <cstdio>
#include "matrix.h"
#ifdef USE_PARALLEL_PROG
#include <mpi.h>
#endif

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

#ifdef USE_PARALLEL_PROG
    int rank;
    int length;
    char name[BUFSIZ];

    MPI_Init(nullptr, nullptr);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &length);
#endif

	for (int i = 1; i <= max_iterations; i++)
	{
		fx = func(Xk, gradXk);

		double n = gradXk.norm();

		// std::cout << "N: " << n << std::endl;

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
#ifdef USE_PARALLEL_PROG
		math::Matrix ret1{Sk_m.getRows(), Yk_trans.getCols()};
		math::Matrix ret2{Yk_m.getRows(), Sk_trans.getCols()};
		if (rank == 0)
		{
			MPI_Send(&Sk_m.arr[0][0], Sk_m.getRows() * Sk_m.getCols(),
					 MPI_DOUBLE, 1, 1, MPI_COMM_WORLD);
			MPI_Send(&Yk_trans.arr[0][0], Yk_trans.getRows() * Yk_trans.getCols(),
					 MPI_DOUBLE, 1, 2, MPI_COMM_WORLD);

			MPI_Send(&Yk_m.arr[0][0], Yk_m.getRows() * Yk_m.getCols(),
					 MPI_DOUBLE, 2, 3, MPI_COMM_WORLD);
			MPI_Send(&Sk_trans.arr[0][0], Sk_trans.getRows() * Sk_trans.getCols(),
					 MPI_DOUBLE, 2, 4, MPI_COMM_WORLD);

			MPI_Recv(&ret1.arr[0][0], ret1.getRows() * ret1.getCols(),
					 MPI_DOUBLE, 1, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&ret2.arr[0][0], ret2.getRows() * ret2.getCols(),
					 MPI_DOUBLE, 2, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Barrier(MPI_COMM_WORLD);
			std::cout << "TEST";
			std::cout << ret1 << ret2 << std::endl;
			Hk = (I - hC * ret1) * Hk * (I - hC * ret2) + hC * Sk_m * Sk_trans;
			Xk = Xk1;
		}
		else if (rank == 1)
		{
			MPI_Recv(&Sk_m.arr[0][0], Sk_m.getRows() * Sk_m.getCols(),
					 MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&Yk_trans.arr[0][0], Yk_trans.getRows() * Yk_trans.getCols(),
					 MPI_DOUBLE, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			assert(Sk_m.getCols() == Yk_trans.getRows());

			auto getMultipliedValue = [&Sk_m, &Yk_trans](const std::size_t row, const std::size_t col) {
				double val = 0.0;

				for (std::size_t i = 0; i < Sk_m.getCols(); i++)
				{
					val += (Sk_m.at_r(row, i) * Yk_trans.at_r(i, col));
				}
				return val;
			};

			for (std::size_t row = 0; row < Sk_m.getRows(); row++)
			{
				for (std::size_t col = 0; col < Yk_trans.getCols(); col++)
				{
					ret1.at(row, col) = getMultipliedValue(row, col);
				}
			}
			MPI_Send(&ret1.arr[0][0], ret1.getRows() * ret1.getCols(),
					 MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
		}
		else if (rank == 2)
		{
			MPI_Recv(&Yk_m.arr[0][0], Yk_m.getRows() * Yk_m.getCols(),
					 MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			MPI_Recv(&Sk_trans.arr[0][0], Sk_trans.getRows() * Sk_trans.getCols(),
					 MPI_DOUBLE, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			assert(Yk_m.getCols() == Sk_trans.getRows());
			math::Matrix ret{Yk_m.getRows(), Sk_trans.getCols()};

			auto getMultipliedValue = [&Yk_m, &Sk_trans](const std::size_t row, const std::size_t col) {
				double val = 0.0;

				for (std::size_t i = 0; i < Yk_m.getCols(); i++)
				{
					val += (Yk_m.at_r(row, i) * Sk_trans.at_r(i, col));
				}
				return val;
			};

			for (std::size_t row = 0; row < Yk_m.getRows(); row++)
			{
				for (std::size_t col = 0; col < Sk_trans.getCols(); col++)
				{
					ret2.at(row, col) = getMultipliedValue(row, col);
				}
			}
			MPI_Send(&ret2.arr[0][0], ret2.getRows() * ret2.getCols(),
					 MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);
			MPI_Barrier(MPI_COMM_WORLD);
		}
#else
		Hk = (I - hC * Sk_m * Yk_trans) * Hk * (I - hC * Yk_m * Sk_trans) + hC * Sk_m * Sk_trans;
		Xk = Xk1;
#endif
	}
	MPI_Finalize();
}
