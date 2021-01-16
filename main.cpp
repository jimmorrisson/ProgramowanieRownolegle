#include <climits>
#include <chrono>
#include <cmath>
#include "ExtendedRosenbrockFunction.h"
#include "QuadraticFunction.h"
#include "PowellSingularFunction.h"
#include "TestUtil.h"
#include <memory>
#include <iostream>
#include <vector>
#include <vector.h>
#include <matrix.h>
#ifdef USE_PARALLEL_PROG
#include <mpi.h>
#endif

int main(int argc, char *argv[])
{
    TestUtil testUtilAlg(0.01, INT_MAX);


#ifdef USE_PARALLEL_PROG
    int rank;
    int length;
    char name[BUFSIZ];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(name, &length);
#endif

    if (argc != 2)
    {
        std::cout << "Invalid number of arguments \n"
                  << "Program usage : \n"
                  << "./ParallelProgramming %n \n"
                  << "n - size \n";
        return -1;
    }
    char *p;
    int size = strtol(argv[1], &p, 10);

    std::unique_ptr<IFunction> extendedRosenbrock = std::make_unique<ExtendedRosenbrockFunction>(size);
    std::unique_ptr<IFunction> quadraticFunction = std::make_unique<QuadraticFunction>(size);
    std::unique_ptr<IFunction> powellSingularFunction = std::make_unique<PowellSingularFunction>(size);
#ifdef USE_PARALLEL_PROG
    double centroid[3]; 
#endif

    const std::vector<IFunction *> functions = {
        extendedRosenbrock.get(),
        quadraticFunction.get(),
        powellSingularFunction.get()
    };

#ifdef USE_PARALLEL_PROG
    if (rank == 0)
#endif
    {
        unsigned int destination = 0;
        auto t1 = std::chrono::high_resolution_clock::now();
        for ([[maybe_unused]] const auto function : functions)
        {
            destination++;
#ifdef USE_PARALLEL_PROG
            MPI_Send(&centroid, 3, MPI_DOUBLE, destination, 0, MPI_COMM_WORLD);
#else
            testUtilAlg.runTest(*function);
#endif
        }
#ifdef USE_PARALLEL_PROG
        MPI_Barrier(MPI_COMM_WORLD);
#endif
        auto t2 = std::chrono::high_resolution_clock::now();
        std::cout << "Main execution time: "
                  << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
                  << " ms" << std::endl;
    }
#ifdef USE_PARALLEL_PROG
    else
    {
        MPI_Recv(&centroid, 3, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        testUtilAlg.runTest(*functions[rank - 1]);
        MPI_Barrier(MPI_COMM_WORLD);
    }

    MPI_Finalize();
#endif
    return 0;
}
