#include "vector.h"
#include <cassert>
#include <iostream>

namespace math
{
    Vector::Vector(const std::size_t size, const double value) : arr{std::make_unique<double[]>(size)}, size{size}
    {
        for (std::size_t i = 0; i < size; i++)
        {
            arr[i] = value;
        }
    }

    Vector::Vector(const Vector &vector) : arr{std::make_unique<double[]>(vector.size)}, size{vector.size}
    {
        for (std::size_t i = 0; i < size; i++)
        {
            arr[i] = vector.arr[i];
        }
    }

    Vector::Vector(Vector &&vector) : arr{std::move(vector.arr)}, size{vector.size}
    {
    }

    Vector::Vector() : arr{nullptr}, size{0}
    {
    }

    Vector operator*(double lhs, const Vector &rhs)
    {
        Vector ret(rhs);
        for (std::size_t i = 0; i < rhs.size; i++)
        {
            ret.at(i) *= lhs;
        }
        return Vector(std::move(ret));
    }

    Vector operator-(const Vector &lhs, const Vector &rhs)
    {
        assert(lhs.size == rhs.size);
        Vector ret(lhs);
        for (std::size_t i = 0; i < ret.size; i++)
        {
            ret.at(i) -= rhs.at_r(i);
        }
        return Vector(std::move(ret));
    }

    Vector operator+(const Vector &lhs, const Vector &rhs)
    {
        assert(lhs.size == rhs.size);
        Vector ret(lhs);
        for (std::size_t i = 0; i < ret.size; i++)
        {
            ret.at(i) += rhs.at_r(i);
        }
        return Vector(std::move(ret));
    }

    double operator*(const Vector &lhs, const Vector &rhs)
    {
        assert(lhs.size == rhs.size);
        double ret = 0.0;
        for (std::size_t i = 0; i < lhs.size; i++)
        {
            ret += (lhs.at_r(i) * rhs.at_r(i));
        }
        return ret;
    }

    std::ostream &operator<<(std::ostream &out, const Vector &vec)
    {
        out << "[";
        if (vec.size > 0)
        {
            out << " " << vec.at_r(0);
        }

        for (std::size_t i = 1; i < vec.size; i++)
        {
            out << ", " << vec.at_r(i);
        }

        out << " ]" << std::endl;

        return out;
    }
} // namespace math
