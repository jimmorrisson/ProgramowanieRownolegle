#pragma once
#include <cassert>
#include <cmath>
#include <memory>

namespace math
{
    class Vector
    {
    public:
        explicit Vector(const std::size_t size, const double value = 0.);
        explicit Vector(const Vector &vector);
        explicit Vector(Vector &&vector);
        Vector();

        Vector &operator=(const Vector &vector)
        {
            arr = std::make_unique<double[]>(vector.size);
            size = vector.size;
            for (std::size_t i = 0; i < size; i++)
            {
                at(i) = vector.at_r(i);
            }
            return *this;
        }
        Vector &operator=(Vector &&vector)
        {
            arr = std::move(vector.arr);
            return *this;
        }

        Vector &operator*=(double value)
        {
            for (std::size_t i = 0; i < size; i++)
            {
                arr[i] *= value;
            }
            return *this;
        }

        Vector &operator-()
        {
            for (std::size_t i = 0; i < size; i++)
            {
                arr[i] *= -1;
            }
            return *this;
        }

        double &at(const std::size_t i)
        {
            return arr[i];
        }

        double at_r(const std::size_t i) const noexcept
        {
            return arr[i];
        }

        auto getSize() const noexcept
        {
            return size;
        }

        double norm() const noexcept
        {
            double ret = 0.;
            for (std::size_t i = 0; i < size; i++)
            {
                ret += std::pow(arr[i], 2);
            }
            return std::sqrt(ret);
        }

        double to_scalar() const
        {
            assert(size == 1);
            return at_r(0);
        }

        friend std::ostream &operator<<(std::ostream &out, const Vector &vec);
        friend Vector operator*(double lhs, const Vector &rhs);
        friend Vector operator-(const Vector &lhs, const Vector &rhs);
        friend Vector operator+(const Vector &lhs, const Vector &rhs);
        friend double operator*(const Vector &lhs, const Vector &rhs);

    private:
        std::unique_ptr<double[]> arr;
        std::size_t size;
    };
} // namespace math
