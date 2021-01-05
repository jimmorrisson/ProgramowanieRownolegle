#include "IFunction.h"
#include <iostream>

IFunction::IFunction(int size, const std::string name, math::Vector initialVector) 
    : initialVector(initialVector), size(size), name(name)
{
}

std::ostream &operator<<(std::ostream &out, const IFunction &func)
{
    out << func.name << std::endl
        << "Size: " << func.size << std::endl
        << "Initial vector: " << func.initialVector;

    return out;
}

math::Vector IFunction::getInitialVector()
{
    return math::Vector(initialVector);
}
