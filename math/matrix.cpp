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
    
    Vector operator-(Vector vector)
    {
        for (std::size_t i = 0; i < vector.getSize(); i++)
        {
            vector.at(i) *= -1;
        }
        return vector;
    }

} // namespace math
