using AK7_CSHARP.Functions;
using AK7_CSHARP.Algebra;
using System;

namespace AK7_CSHARP
{
    class Program
    {
        static void Main(string[] args)
        {
            int size;
            int threads = Environment.ProcessorCount;

#if DEBUG
            size = 60;
#else
            if (args.Length == 0)
            {
                Console.WriteLine("Invalid number of arguments.");
                Console.WriteLine("To run the program pass vector size (divisible by 12) and number of threads.");
                return;
            }

            if (!int.TryParse(args[0], out size))
            {
                Console.WriteLine("Invalid size. Needs to be a number.");
                return;
            }

            if (size % 12 != 0)
            {
                Console.WriteLine("Invalid size. Needs to be divisible by 12.");
                return;
            }

            if (args.Length == 2 && !int.TryParse(args[1], out threads))
            {
                Console.WriteLine("Invalid number of threads. Needs to be a number.");
                return;
            }
#endif

#if USE_PARALLEL
            Matrix.ThreadNumber = threads;
            Console.WriteLine($"Parallel. Number of threads: {Matrix.ThreadNumber}");
#else
            Matrix.Threads = 1;
            Console.WriteLine("Sequential.");
#endif

            TestUtil testUtil = new TestUtil(0.01, 100000);

            IFunction extendedRosenbrock = new ExtendedRosenbrockFunction(size);
            testUtil.RunTest(extendedRosenbrock);

            IFunction powellSingular = new PowellSingularFunction(size);
            testUtil.RunTest(powellSingular);

            IFunction quadratic = new QuadraticFunction(size);
            testUtil.RunTest(quadratic);
        }
    }
}
