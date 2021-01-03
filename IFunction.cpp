#include "IFunction.h"
#include <iostream>

IFunction::IFunction(int size, math::Vector initialVector) : initialVector(initialVector), size(size)
{
}

std::ostream &operator<<(std::ostream &out, const IFunction &func)
{
    out << typeid(func).name() << std::endl
        << "Size: " << func.size << std::endl
        << "Initial vector: " << std::endl
        << std::endl;

    return out;
}

math::Vector IFunction::getInitialVector()
{
    return math::Vector(initialVector);
}
