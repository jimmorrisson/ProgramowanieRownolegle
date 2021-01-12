using AK7_CSHARP.Algebra;
using System;

namespace AK7_CSHARP.Functions
{
    public class PowellSingularFunction : IFunction
    {
        public PowellSingularFunction(int size) : base(size, "Powell singular function", PrepareInitialVector(size))
        { }

        private static Vector PrepareInitialVector(int size)
        {
            Vector ret = new Vector(size);

            for (int i = 3; i < size; i += 4)
            {
                ret[i - 3] = 3;
                ret[i - 2] = -1;
                ret[i - 1] = 0;
                ret[i] = 1;
            }

            return ret;
        }

        public override double Func(Vector x, ref Vector grad)
        {
            double fx = 0.0;

            for (int i = 3; i < Size; i += 4)
            {
                double a = x[i - 3];
                double b = x[i - 2];
                double c = x[i - 1];
                double d = x[i];

                fx += DMath.Pow2(a + 10.0 * b) + 5.0 * DMath.Pow2(c - d) + DMath.Pow4(b - 2.0 * c) + 10.0 * DMath.Pow4(a - d);
                grad[i - 3] = 40.0 * DMath.Pow3(a - d) + 2.0 * a + 20.0 * b;
                grad[i - 2] = 20.0 * a + 200.0 * b + 4.0 * DMath.Pow3(b - 2.0 * c);
                grad[i - 1] = 10.0 * (c - d) - 8.0 * DMath.Pow3(b - 2.0 * c);
                grad[i] = 10.0 * (-4.0 * DMath.Pow3(a - d) - c + d);
            }

            return fx;
        }
    }
}
