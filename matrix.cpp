#include "matrix.h"
#include <cassert>

#ifdef USE_PARALLEL_PROG
#include <mpi.h>
#include "config.h"
#endif
#include <iostream>

namespace math
{
    Matrix::Matrix(const std::size_t rows, const std::size_t cols) : arr{std::make_unique<std::unique_ptr<double[]>[]>(rows)},
                                                                     rows{rows},
                                                                     cols{cols}
    {
        for (std::size_t i = 0; i < rows; i++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
            arr[i] = std::move(oneDimArr);
        }
    }

    Matrix::Matrix(const Matrix &matrix) : arr{std::make_unique<std::unique_ptr<double[]>[]>(matrix.rows)},
                                           rows{matrix.rows},
                                           cols{matrix.cols}
    {
        for (std::size_t row = 0; row < rows; row++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
            arr[row] = std::move(oneDimArr);
            for (std::size_t col = 0; col < cols; col++)
            {
                at(row, col) = matrix.at_r(row, col);
            }
        }
    }

	Matrix::Matrix(const Vector& vector) : arr{ std::make_unique<std::unique_ptr<double[]>[]>(vector.getSize()) },
		                                   rows{ vector.getSize() },
		                                   cols{ 1 }
	{
        for (std::size_t row = 0; row < rows; row++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
            arr[row] = std::move(oneDimArr);
            at(row, 0) = vector.at_r(row);
        }
	}

    Matrix::Matrix(Matrix &&matrix) : arr{std::move(matrix.arr)},
                                      rows{matrix.rows},
                                      cols{matrix.cols}
    {
    }

    Matrix::Matrix() : arr{nullptr},
                       rows{0},
                       cols{0}
    {
    }

    double* Matrix::toArray() const
    {
        double* ret = new double[rows * cols];

        for (std::size_t row = 0; row < rows; row++)
        {
            for (std::size_t col = 0; col < cols; col++)
            {
                ret[row * cols + col] = arr[row][col];
            }
        }

        return ret;
    }

    Matrix operator*(const Matrix &matrix, const double value)
    {
        Matrix ret(matrix);

        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            for (std::size_t col = 0; col < matrix.cols; col++)
            {
                ret.at(row, col) *= value;
            }
        }

        return Matrix(std::move(ret));
    }

    Matrix operator*(const double value, const Matrix &matrix)
    {
        return operator*(matrix, value);
    }

    Vector operator*(const Matrix &matrix, const Vector &vector)
    {
        assert(matrix.cols == vector.getSize());
        Vector ret(matrix.rows);

        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            for (std::size_t col = 0; col < matrix.cols; col++)
            {
                ret.at(row) += vector.at_r(col) * matrix.at_r(row, col);
        }
    }
        return Vector(std::move(ret));
    }

    Matrix operator+(const Matrix& lhs, const Matrix& rhs)
    {
        assert(lhs.rows == rhs.rows && lhs.cols == rhs.cols);
        Matrix ret{ lhs };

        for (std::size_t row = 0; row < lhs.rows; row++)
        {
            for (std::size_t col = 0; col < lhs.cols; col++)
            {
                ret.at(row, col) += rhs.at_r(row, col);
            }
        }

        return Matrix(std::move(ret));
    }

    Matrix operator-(const Matrix &lhs, const Matrix &rhs)
    {
        assert(lhs.rows == rhs.rows && lhs.cols == rhs.cols);
        Matrix ret{lhs};

        for (std::size_t row = 0; row < lhs.rows; row++)
        {
            for (std::size_t col = 0; col < lhs.cols; col++)
            {
                ret.at(row, col) -= rhs.at_r(row, col);
            }
        }

        return Matrix(std::move(ret));
    }

#ifdef USE_PARALLEL_PROG
    // MPI matrix multiplication, main process
    Matrix operator*(const Matrix &lhs, const Matrix &rhs)
    {
        assert(lhs.cols == rhs.rows);
        Matrix ret{lhs.rows, rhs.cols};

        int shouldContinue = CONTINUE_RUNNING;
        int workersNumber = config::workersNumber;
        int averageRows = lhs.rows / workersNumber;
        int extraRows = lhs.rows % workersNumber;
        int rowsNumber = averageRows;
        int offset = 0;

        double* lhsArr = lhs.toArray();
        double* rhsArr = rhs.toArray();
        double* retArr = new double[lhs.rows * rhs.cols];

        for (int dest = 1; dest <= workersNumber; dest++)
        {
            if (dest == workersNumber)
            {
                rowsNumber += extraRows;
            }
            MPI_Send(&shouldContinue, 1, MPI_INT, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(&lhs.rows, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(&lhs.cols, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(&rhs.rows, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(&rhs.cols, 1, MPI_UNSIGNED_LONG_LONG, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(&offset, 1, MPI_INT, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(&rowsNumber, 1, MPI_INT, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(lhsArr, lhs.rows * lhs.cols, MPI_DOUBLE, dest, FROM_MAIN, MPI_COMM_WORLD);
            MPI_Send(rhsArr, rhs.rows * rhs.cols, MPI_DOUBLE, dest, FROM_MAIN, MPI_COMM_WORLD);

            offset += rowsNumber;
        }

        for (int source = 1; source <= workersNumber; source++)
        {
            MPI_Recv(&offset, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&rowsNumber, 1, MPI_INT, source, FROM_WORKER, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(&retArr[offset * rhs.cols], rowsNumber * rhs.cols, MPI_DOUBLE, source, FROM_WORKER, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        for (std::size_t row = 0; row < ret.rows; row++)
        {
            for (std::size_t col = 0; col < ret.cols; col++)
            {
                ret.arr[row][col] = retArr[row * rhs.cols + col];
            }
        }

        delete[] lhsArr;
        delete[] rhsArr;
        delete[] retArr;

        return Matrix(std::move(ret));
    }
#else
    // Sequential matrix multiplication
    Matrix operator*(const Matrix& lhs, const Matrix& rhs)
    {
        assert(lhs.cols == rhs.rows);
        Matrix ret{ lhs.rows, rhs.cols };

        auto getMultipliedValue = [&lhs, &rhs](const std::size_t row, const std::size_t col) {
            double val = 0.0;

            for (std::size_t i = 0; i < lhs.cols; i++)
            {
                val += (lhs.at_r(row, i) * rhs.at_r(i, col));
            }
            return val;
        };

        for (std::size_t row = 0; row < lhs.rows; row++)
        {
            for (std::size_t col = 0; col < rhs.cols; col++)
            {
                ret.at(row, col) = getMultipliedValue(row, col);
            }
        }
        return Matrix(std::move(ret));
    }
#endif

    std::ostream &operator<<(std::ostream &out, const Matrix &matrix)
    {
        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            out << "[";

            if (matrix.cols > 0)
            {
                out << " " << matrix.at_r(row, 0);
            }

            for (std::size_t col = 1; col < matrix.cols; col++)
            {
                out << ", " << matrix.at_r(row, col);
            }

            out << " ]" << std::endl;
        }
        return out;
    }
} // namespace math
