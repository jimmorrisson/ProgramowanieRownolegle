using System;
using AK7_CSHARP.Algebra;
using AK7_CSHARP.Functions;

namespace AK7_CSHARP
{
    public class BFGSolver
    {
        public double Epsilon { get; }

        public int MaxIterations { get; }

        public BFGSolver(double epsilon, int maxIterations)
        {
            Epsilon = epsilon;
            MaxIterations = maxIterations;
        }

        private double ArmijoBackTrack(IFunction func, double fXk, Vector Xk, Vector gradXk, Vector Dk, ref Vector Xk1, ref Vector gradXk1, double alpha, double alphaCoeff, double delta)
        {
            double armijoCoeff = delta * (Matrix.Transpose(gradXk) * Dk).Scalar();
            while (true)
            {
                Xk1 = Xk + alpha * Dk;
                double fXk1 = func.Func(Xk1, ref gradXk1);
                double armijoCond = fXk + alpha * armijoCoeff;

                if (fXk1 <= armijoCond)
                {
                    return alpha;
                }

                alpha *= alphaCoeff;
            }
        }

        public double Solve(IFunction func, Vector initialVector, out Vector Xk)
        {
            double fx = 0;
            int size = func.Size;
            Matrix I = Matrix.Identity(size);
            Matrix Hk = Matrix.Identity(size);
            Vector gradXk = new Vector(size);
            Xk = new Vector(initialVector);

            int i = 1;
            bool stopped = false;

            while (i < MaxIterations && !stopped)
            {
                fx = func.Func(Xk, ref gradXk);

                double n = gradXk.Norm();

                if (n <= Epsilon)
                {
                    Console.WriteLine($"Number of iterations = {i}");
                    stopped = true;
                    break;
                }

                // Step 2 search direction
                Vector Dk = -(Hk * gradXk);

                // Step 3 step length using Armijo rule
                Vector Xk1 = new Vector(Xk.Size);
                Vector gradXk1 = new Vector(gradXk.Size);
                double step = ArmijoBackTrack(func, fx, Xk, gradXk, Dk, ref Xk1, ref gradXk1, 1, 0.2, 0.0001);

                // Step 4 matrix Hk+1
                Vector Sk = step * Dk;
                Vector Yk = gradXk1 - gradXk;
                Matrix Sk_m = new Matrix(Sk);
                Matrix Yk_m = new Matrix(Yk);
                Matrix Yk_trans = Matrix.Transpose(Yk);
                Matrix Sk_trans = Matrix.Transpose(Sk);
                double hC = 1.0 / (Yk_trans * Sk).Scalar();
                Hk = ((I - hC * Sk_m * Yk_trans) * Hk * (I - hC * Yk_m * Sk_trans) + hC * Sk_m * Sk_trans);

                Xk = Xk1;

                i++;
            }

            return fx;
        }
    }
}
