#include "IFunction.h"

class LBFGSExampleRosenbrock : public IFunction
{
private:
    VectorXd prepareInitialVector(int size)
    {
        return VectorXd::Zero(size);
    }

public:
    LBFGSExampleRosenbrock(int size) :IFunction(size, prepareInitialVector(size)) {}

    double operator()(const VectorXd& x, VectorXd& grad)
    {
        double fx = 0.0;
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
};