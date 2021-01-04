#include "matrix.h"
#include <cassert>

namespace math
{
    Matrix::Matrix(const std::size_t rows, const std::size_t cols) : arr{std::make_unique<std::unique_ptr<double[]>[]>(rows)},
                                                                     rows{rows},
                                                                     cols{cols}
    {
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t i = 0; i < rows; i++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
            arr[i] = std::move(oneDimArr);
        }
    }

    Matrix::Matrix(const Matrix &matrix) : arr{std::make_unique<std::unique_ptr<double[]>[]>(matrix.rows)},
                                           rows{matrix.rows},
                                           cols{matrix.cols}

    {
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < rows; row++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
            arr[row] = std::move(oneDimArr);
            for (std::size_t col = 0; col < cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
                at(row, col) = matrix.at_r(row, col);
            }
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

    Matrix operator*(const Matrix &matrix, const double value)
    {
        Matrix ret(matrix);
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            for (std::size_t col = 0; col < matrix.cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
                ret.at(row, col) *= value;
            }
        }
        return Matrix(std::move(matrix));
    }

    Matrix operator*(const double value, const Matrix &matrix)
    {
        return operator*(matrix, value);
    }

    Vector operator*(const Vector &vector, const Matrix &matrix)
    {
        assert(matrix.cols == vector.getSize());
        Vector ret(matrix.rows);
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            for (std::size_t col = 0; col < matrix.cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
                ret.at(row) += (vector.at_r(row) * matrix.at_r(row, col));
            }
        }
        return Vector(std::move(ret));
    }

    Vector operator*(const Matrix &matrix, const Vector &vector)
    {
        return operator*(vector, matrix);
    }

    Matrix operator-(const Matrix &matrix, const double value)
    {
        Matrix ret{matrix};
        auto sub = Matrix::identity(matrix.rows) * value;
        return ret - sub;
    }

    Matrix operator-(const Matrix &lhs, const Matrix &rhs)
    {
        assert(lhs.rows == rhs.rows && lhs.cols == rhs.cols);
        Matrix ret{lhs};
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < lhs.rows; row++)
        {
            for (std::size_t col = 0; col < lhs.cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
                ret.at(row, col) -= rhs.at_r(row, col);
            }
        }
        return Matrix(std::move(ret));
    }

    Matrix operator*(const Matrix &lhs, const Matrix &rhs)
    {
        assert(lhs.cols == rhs.rows);
        Matrix ret{lhs.rows, rhs.cols};
        auto getMultipliedValue = [&ret, &lhs, &rhs](const std::size_t row, const std::size_t col) {
            double val = 0.;
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
            for (std::size_t i = 0; i < lhs.cols; i++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
                val += (lhs.at_r(row, i) * rhs.at_r(i, col));
            }
            return val;
        };
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < ret.rows; row++)
        {
            for (std::size_t col = 0; col < lhs.cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
                ret.at(row, col) = getMultipliedValue(row, col);
            }
        }
        return Matrix(std::move(ret));
    }

    Matrix operator+(const Matrix &matrix, const double value)
    {
        Matrix ret{matrix};
        auto sub = Matrix::identity(matrix.rows) * value;
        return ret + sub;
    }

    Matrix operator+(const Matrix &lhs, const Matrix &rhs)
    {
        assert(lhs.rows == rhs.rows && lhs.cols == rhs.cols);
        Matrix ret{lhs};
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < lhs.rows; row++)
        {
            for (std::size_t col = 0; col < lhs.cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
                ret.at(row, col) += rhs.at_r(row, col);
            }
        }
        return Matrix(std::move(ret));
    }

    std::ostream &operator<<(std::ostream &out, const Matrix &matrix)
    {
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            for (std::size_t col = 0; col < matrix.cols; col++)
            {
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
                {
                    out << matrix.at_r(row, col) << " ";
                }
            }
#ifdef USE_PARALLEL_PROG
#pragma omp critical
#endif
            {
                out << std::endl;
            }
        }
        return out;
    }
} // namespace math
