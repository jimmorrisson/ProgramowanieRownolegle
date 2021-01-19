#pragma once
#include <cassert>
#include <memory>
#include "vector.h"
#include <ostream>

namespace math
{
    class Matrix
    {
    public:
        static int workersNumber;

        std::unique_ptr<std::unique_ptr<double[]>[]> arr;
        explicit Matrix(const std::size_t rows, const std::size_t cols);
        explicit Matrix(const Matrix &matrix);
        explicit Matrix(const Vector& vector);
        explicit Matrix(Matrix &&matrix);
        Matrix();

        double* toArray() const;

        constexpr auto getRows()
        {
            return rows;
        }

        constexpr auto getCols()
        {
            return cols;
        }

        static Matrix identity(const std::size_t size)
        {
            Matrix ret(size, size);
            for (std::size_t row = 0; row < size; row++)
            {
                ret.at(row, row) = 1.0;
            }
            return Matrix(std::move(ret));
        }

        Matrix &operator=(const Matrix &matrix)
        {
            arr = std::make_unique<std::unique_ptr<double[]>[]>(matrix.rows);
            rows = matrix.rows;
            cols = matrix.cols;
            for (std::size_t row = 0; row < rows; row++)
            {
                auto oneDimArr = std::make_unique<double[]>(cols);
                arr[row] = std::move(oneDimArr);
                for (std::size_t col = 0; col < cols; col++)
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
            for (std::size_t i = 0; i < vector.getSize(); i++)
            {
                ret.at(0, i) = vector.at_r(i);
            }
            return Matrix(std::move(ret));
        }

        friend Matrix operator*(const Matrix &matrix, const double value);
        friend Matrix operator*(const double value, const Matrix &matrix);
        friend Vector operator*(const Matrix &matrix, const Vector &vector);
        friend Matrix operator+(const Matrix& lhs, const Matrix& rhs);
        friend Matrix operator-(const Matrix &lhs, const Matrix &rhs);
        friend Matrix operator*(const Matrix& lhs, const Matrix& rhs);
        friend std::ostream &operator<<(std::ostream &out, const Matrix &matrix);

    private:
        std::size_t rows;
        std::size_t cols;
    };
} // namespace math
