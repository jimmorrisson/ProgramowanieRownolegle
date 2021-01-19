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
#include "vector.h"
#include "matrix.h"

#ifdef USE_PARALLEL_PROG
#include <mpi.h>
#include "config.h"

namespace config
{
    int workersNumber;
}

    void stopWorkerProcesses()
    {
        int data = STOP_RUNNING;

        for (int dest = 1; dest <= config::workersNumber; dest++)
        {
            MPI_Send(&data, 1, MPI_INT, dest, FROM_MAIN, MPI_COMM_WORLD);
        }
    }

    void runWorkerJob(int rank)
    { // MPI matrix multiplication, worker process

        std::size_t lCols, lRows, rCols, rRows;
        int offset, rowsNumber;

        MPI_Recv(&lRows, 1, MPI_UNSIGNED_LONG_LONG, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&lCols, 1, MPI_UNSIGNED_LONG_LONG, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&rRows, 1, MPI_UNSIGNED_LONG_LONG, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&rCols, 1, MPI_UNSIGNED_LONG_LONG, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&offset, 1, MPI_INT, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&rowsNumber, 1, MPI_INT, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        auto lhsArr = std::make_unique<double[]>(lRows * lCols);
        auto rhsArr = std::make_unique<double[]>(rRows * rCols);
        auto retArr = std::make_unique<double[]>(lRows * rCols);

        MPI_Recv(lhsArr, lRows * lCols, MPI_DOUBLE, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(rhsArr, rRows * rCols, MPI_DOUBLE, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        for (std::size_t row = offset; row < offset + rowsNumber; row++)
        {
            for (std::size_t col = 0; col < rCols; col++)
            {
                double val = 0.0;

                for (std::size_t i = 0; i < lCols; i++)
                {
                    val += lhsArr[row * lCols + i] * rhsArr[i * rCols + col];
                }

                retArr[row * rCols + col] = val;
            }
        }

        MPI_Send(&offset, 1, MPI_INT, 0, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&rowsNumber, 1, MPI_INT, 0, FROM_WORKER, MPI_COMM_WORLD);
        MPI_Send(&retArr[offset * rCols + 0], rowsNumber * rCols, MPI_DOUBLE, 0, FROM_WORKER, MPI_COMM_WORLD);
    }

    void runWorkerJobCycle(int rank)
    {
        while (true)
        {
            int shouldContinue;

            MPI_Recv(&shouldContinue, 1, MPI_INT, 0, FROM_MAIN, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (shouldContinue == STOP_RUNNING)
            {
                std::cout << "MPI worker process with rank " << rank << " received stop from main." << std::endl;
                break;
            }

            runWorkerJob(rank);
        }
    }
#endif

int main(int argc, char *argv[])
{
#ifdef USE_PARALLEL_PROG
    // MPI Start
    int rank;
    int processesNumber;
    int workersNumber;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processesNumber);
    
    if (processesNumber < 2) 
    {
        std::cout << "ERROR: MPI version requires at least 2 processes to run." << std::endl;
        MPI_Abort(MPI_COMM_WORLD, -1);
        return -1;
    }

    config::workersNumber = processesNumber - 1;

    if (rank == 0)
    {
        // Main process work started
        std::cout << "MPI main process started. " << processesNumber << " processes total." << std::endl;
#endif
        if (argc != 2)
        {
            std::cout << "Invalid number of arguments \n"
                << "Program usage : \n"
                << "./ParallelProgramming %n \n"
                << "n - size \n";
            return -1;
        }

        TestUtil testUtilAlg(0.01, INT_MAX);

        char *p;
        int size = strtol(argv[1], &p, 10);

        std::unique_ptr<IFunction> extendedRosenbrock = std::make_unique<ExtendedRosenbrockFunction>(size);
        testUtilAlg.runTest(*extendedRosenbrock.get());

        std::unique_ptr<IFunction> quadraticFunction = std::make_unique<QuadraticFunction>(size);
        testUtilAlg.runTest(*quadraticFunction.get());

        std::unique_ptr<IFunction> powerSingularFunction = std::make_unique<PowellSingularFunction>(size);
        testUtilAlg.runTest(*powerSingularFunction.get());

#ifdef USE_PARALLEL_PROG
        stopWorkerProcesses();
        std::cout << "MPI main process ended. " << std::endl;
        // Main process work ended
    }

    if (rank > 0)
    {
        std::cout << "MPI worker process with rank " << rank << " started." << std::endl;
        // Worker process work started
        if (argc == 2)
        {
            runWorkerJobCycle(rank);
        }

        std::cout << "MPI worker process with rank " << rank << " ended." << std::endl;
        // Worker process work ended
    }

    MPI_Finalize();
    // MPI End
#endif

    return 0;
}
