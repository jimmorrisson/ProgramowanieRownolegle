using System;
using AK7_CSHARP.Algebra;
using AK7_CSHARP.Functions;

namespace AK7_CSHARP
{
    public class TestUtil
    {
        double Epsilon { get; }

        int MaxIterations { get; }

        public TestUtil(double epsilon, int maxIterations)
        {
            Epsilon = epsilon;
            MaxIterations = maxIterations;
        }

        private double SolveBFGSImpl(IFunction func, Vector initialVector, out Vector Xk)
        {
            BFGSolver solver = new BFGSolver(Epsilon, MaxIterations);
            return solver.Solve(func, initialVector, out Xk);
        }

        public void RunTest(IFunction func)
        {
            Console.WriteLine("<--------------STARTING TEST-------------->");
            Console.WriteLine(func);

            Vector initialVector = func.InitialVector;

            var watch = System.Diagnostics.Stopwatch.StartNew();
            Vector Xk;
            double fx = SolveBFGSImpl(func, initialVector, out Xk);
            watch.Stop();

            Console.WriteLine($"x = {Xk}");
            Console.WriteLine($"min f(x) = {fx}");
            Console.WriteLine($"Execution time: {watch.ElapsedMilliseconds} ms");
            Console.WriteLine("<--------------TEST FINISHED-------------->");
        }
    }
}
