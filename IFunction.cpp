#include "IFunction.h"
#include <iostream>

IFunction::IFunction(int size, VectorXd initialVector) : initialVector(initialVector), size(size)
{
}

IFunction::IFunction(int size, math::Vector initialVector) : initialVector_v2(initialVector), size(size)
{
}

std::ostream &operator<<(std::ostream &out, const IFunction &func)
{
    out << typeid(func).name() << std::endl
        << "Size: " << func.size << std::endl
        << "Initial vector: " << std::endl
        << func.initialVector.format(Eigen::IOFormat(Eigen::FullPrecision, 0, " ", ", ", "", ""))
        << std::endl;

    return out;
}

VectorXd IFunction::getInitialVector()
{
    return VectorXd(initialVector);
}
math::Vector IFunction::getInitialVector_v2()
{
    return math::Vector(initialVector_v2);
}
