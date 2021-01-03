#include "LBFGSExampleRosenbrock.h"

VectorXd LBFGSExampleRosenbrock::prepareInitialVector(int size)
{
    return VectorXd::Zero(size);
}

LBFGSExampleRosenbrock::LBFGSExampleRosenbrock(int size) : IFunction(size, math::Vector(size)) 
{
}

double LBFGSExampleRosenbrock::operator()(const math::Vector& x, math::Vector& grad)
{
    double fx = 0.0;
    #ifdef USE_PARALLEL_PROG
    #pragma omp parallel for
    #endif
    for (int i = 0; i < size; i += 2)
    {
        double t1 = 1.0 - x.at_r(i);
        double t2 = 10 * (x.at_r(i + 1) - x.at_r(i) * x.at_r(i));
        grad.at(i + 1) = 20 * t2;
        grad.at(i) = -2.0 * (x.at_r(i) * grad.at_r(i + 1) + t1);
        fx += t1 * t1 + t2 * t2;
    }
    return fx;
}

double LBFGSExampleRosenbrock::operator()(const VectorXd &x, VectorXd &grad)
{
    double fx = 0.0;
    #ifdef USE_PARALLEL_PROG
    #pragma omp parallel for
    #endif
    for (int i = 0; i < size; i += 2)
    {
        double t1 = 1.0 - x[i];
        double t2 = 10 * (x[i + 1] - x[i] * x[i]);
        grad[i + 1] = 20 * t2;
        grad[i] = -2.0 * (x[i] * grad[i + 1] + t1);
        fx += t1 * t1 + t2 * t2;
    }
    return fx;
}
