#include "matrix.h"
#include <cassert>

namespace math
{
    Matrix::Matrix(const std::size_t rows, const std::size_t cols) :
        arr{std::make_unique<std::unique_ptr<double[]>[]>(rows)},
        rows{rows},
        cols{cols}
    {
        for(std::size_t i = 0; i < rows; i++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
            arr[i] = std::move(oneDimArr);
        }
    }

    Matrix::Matrix(const Matrix &matrix) : 
        arr{std::make_unique<std::unique_ptr<double[]>[]>(matrix.rows)},
        rows{matrix.rows},
        cols{matrix.cols}

    {
        for(std::size_t row = 0; row < rows; row++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
            arr[row] = std::move(oneDimArr);
            for (std::size_t col = 0; col < cols; col++)
            {
                this->at(row, col) = matrix.at_r(row, col);
            }
        }
    }

    Matrix::Matrix(Matrix &&matrix) :
        arr{std::move(matrix.arr)},
        rows{matrix.rows},
        cols{matrix.cols}
    {
    }

    Matrix::Matrix() :
        arr{nullptr},
        rows{0},
        cols{0}
    {
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
        for (std::size_t row = 0; row < matrix.rows; row++)
        {
            for (std::size_t col = 0; col < matrix.cols; col++)
            {
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
        for (std::size_t row = 0; row < lhs.rows; row++)
        {
            for (std::size_t col = 0; col < lhs.cols; col++)
            {
                ret.at(row, col) -= rhs.at_r(row, col);
            }
        }
        return Matrix(std::move(ret));
    }
    
    // Matrix operator*(const Matrix &lhs, const Matrix &rhs)
    // {
    //     assert(lhs.cols == rhs.rows);
    //     Matrix ret{lhs.rows, rhs.cols};
    //     for (std::size_t row = 0; row < ret.rows; row++)
    //     {
    //         for (std::size_t col = 0; col < lhs.cols; col++)
    //         {
    //         }
    //     }
    // }
} // namespace math
