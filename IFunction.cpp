#include "IFunction.h"
#include <iostream>

IFunction::IFunction(int size, VectorXd initialVector) : size(size), initialVector(initialVector) 
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
