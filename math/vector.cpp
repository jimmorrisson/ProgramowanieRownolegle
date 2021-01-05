#include "vector.h"
#include <cassert>
#include <iostream>

namespace math
{
    Vector::Vector(const std::size_t size, const double value) : arr{std::make_unique<double[]>(size)}, size{size}
    {
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t i = 0; i < size; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
            arr[i] = value;
        }
    }

    Vector::Vector(const Vector &vector) : arr{std::make_unique<double[]>(vector.size)}, size{vector.size}
    {
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t i = 0; i < size; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic write
#endif
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
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t i = 0; i < rhs.size; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            ret.at(i) *= lhs;
        }
        return Vector(std::move(ret));
    }

    Vector operator-(const Vector &lhs, const Vector &rhs)
    {
        assert(lhs.size == rhs.size);
        Vector ret(lhs);
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t i = 0; i < ret.size; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            ret.at(i) -= rhs.at_r(i);
        }
        return Vector(std::move(ret));
    }

    Vector operator+(const Vector &lhs, const Vector &rhs)
    {
        assert(lhs.size == rhs.size);
        Vector ret(lhs);
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif
        for (std::size_t i = 0; i < ret.size; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            ret.at(i) += rhs.at_r(i);
        }
        return Vector(std::move(ret));
    }

    double operator*(const Vector &lhs, const Vector &rhs)
    {
        assert(lhs.size == rhs.size);
        double ret = 0.;
#ifdef USE_PARALLEL_PROG
#pragma omp parallel for
#endif

        for (std::size_t i = 0; i < lhs.size; i++)
        {
#ifdef USE_PARALLEL_PROG
#pragma omp atomic update
#endif
            ret += (lhs.at_r(i) * rhs.at_r(i));
        }
        return ret;
    }

    std::ostream &operator<<(std::ostream &out, const Vector &vec)
    {
        for (std::size_t i = 0; i < vec.size; i++)
        {
            out << vec.at_r(i) << std::endl;
        }

        return out;
    }
} // namespace math
