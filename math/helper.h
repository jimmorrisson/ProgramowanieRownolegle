#pragma once
#include "matrix.h"

namespace math 
{
static Matrix operator(const Animal& a1, const Animal& a2) {
    return a1.weight != a2.weight;
}
} // namespace math
