#pragma once
#include <memory>
#include "vector.h"

namespace math
{
class Matrix
{
public:
    explicit Matrix(const std::size_t rows, const std::size_t cols);
    explicit Matrix(const Matrix &matrix);
    explicit Matrix(Matrix &&matrix);
    Matrix();

    static Matrix identity(const std::size_t size)
    {
        Matrix matrix(size, size);
        for (std::size_t row = 0; row < size; row++)
        {
            for (std::size_t col = 0; col < size; col++)
            {
                if (row == col)
                {
                    matrix.at(row, col) = 1.0;
                }
            }
        }
        return Matrix(std::move(matrix));
    }

    Matrix &operator=(const Matrix &matrix)
    {
        arr = std::make_unique<std::unique_ptr<double[]>[]>(matrix.rows);
        rows = matrix.rows;
        cols = matrix.cols;
        for(std::size_t row = 0; row < rows; row++)
        {
            auto oneDimArr = std::make_unique<double[]>(cols);
            arr[row] = std::move(oneDimArr);
            for(std::size_t col = 0; col < cols; col++)
            {
                at(row, col) = matrix.at_r(row, col);
            }
        }

        return *this;
    }
    Matrix &operator=(Matrix &&matrix)
    {
        arr = std::move(matrix.arr);
        return *this;
    }

    double &at(const std::size_t row, const std::size_t col)
    {
        return arr[row][col];
    }

    double at_r(const std::size_t row, const std::size_t col) const noexcept
    {
        return arr[row][col];
    }

    double to_scalar() const 
    {   
        assert(rows == 1 && cols == 1);
        return at_r(0, 0);
    }

    static Matrix transpose(const Vector &vector)
    {
        Matrix ret{1, vector.getSize()};
        return Matrix(std::move(ret));
    }

    friend Matrix operator*(const Matrix &matrix, const double value);
    friend Matrix operator*(const double value, const Matrix &matrix);
    friend Vector operator*(const Vector &vector, const Matrix &matrix);
    friend Vector operator*(const Matrix &matrix, const Vector &vector);
    friend Matrix operator-(const Matrix &matrix, const double value);
    friend Matrix operator-(const Matrix &lhs, const Matrix &rhs);
    friend Matrix operator*(const Matrix &lhs, const Matrix &rhs);
    friend Matrix operator+(const Matrix &matrix, const double value);
    friend Matrix operator+(const Matrix &lhs, const Matrix &rhs);

private:
    std::unique_ptr<std::unique_ptr<double[]>[]> arr;
    std::size_t rows;
    std::size_t cols;
};
} // namespace math
