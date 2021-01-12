using AK7_CSHARP.Algebra;
using System;

namespace AK7_CSHARP.Functions
{
    public class QuadraticFunction : IFunction
    {
        public QuadraticFunction(int size) : base(size, "Quadratic function", new Vector(size, 3.0))
        { }

        public override double Func(Vector x, ref Vector grad)
        {
            double fx = DMath.Pow2(x[0]);
            grad[0] = 2 * x[0];

            fx += 101 * DMath.Pow2(x[1]);
            grad[1] = 202 * x[1];

            for (int i = 2; i <= Size - 3; i++)
            {
                fx += 201 * DMath.Pow2(x[i]);
                grad[i] = 402 * x[i];
            }

            fx += 200 * DMath.Pow2(x[Size - 2]);
            grad[Size - 2] = 400 * x[Size - 2];

            fx += 100 * DMath.Pow2(x[Size - 1]);
            grad[Size - 1] = 200 * x[Size - 1];

            return fx;
        }
    }
}
