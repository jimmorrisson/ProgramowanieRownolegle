namespace AK7_CSHARP.Algebra
{
    public class DMath
    {
        /*
         * Standard Integer number raising.
         * To replace Math.Pow that uses Double power argument causing rounding errors.
         */
        public static double Pow(double val, uint pow)
        {
            if (pow == 0)
            {
                return 1.0;
            }

            double ret = val;

            for (int i = 1; i < pow; i++)
            {
                ret *= val;
            }

            return ret;
        }

        public static double Pow2(double val)
        {
            return val * val;
        }

        public static double Pow3(double val)
        {
            return val * val * val;
        }

        public static double Pow4(double val)
        {
            double val2 = val * val;

            return val2 * val2;
        }
    }
}
