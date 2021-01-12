using AK7_CSHARP.Algebra;
using System;

namespace AK7_CSHARP.Functions
{
    public class ExtendedRosenbrockFunction : IFunction
    {
        public ExtendedRosenbrockFunction(int size) : base(size, "Extended Rosenbrock function", PrepareInitialVector(size))
        { }

        private static Vector PrepareInitialVector(int size)
        {
            Vector ret = new Vector(size);

            for (int i = 1; i < size; i += 2)
            {
                ret[i - 1] = -1.2;
                ret[i] = 1.0;
            }

            return ret;
        }

        public override double Func(Vector x, ref Vector grad)
        {
            double fx = 0.0;

            for (int i = 1; i < Size; i += 2)
            {
                double a = x[i];
                double b = x[i - 1];

                fx += 100.0 * DMath.Pow2(a - DMath.Pow2(b)) + DMath.Pow2(1.0 - b);
                grad[i] = 200.0 * (a - DMath.Pow2(b));
                grad[i - 1] = 2.0 * (-200.0 * a * b + 200 * DMath.Pow3(b) + b - 1);
            }
            return fx;
        }
    }
}
