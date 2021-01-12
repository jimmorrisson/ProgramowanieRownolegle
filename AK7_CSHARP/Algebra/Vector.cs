using System;
using System.Diagnostics;

namespace AK7_CSHARP.Algebra
{
    public class Vector
    {
        public int Size { get; }

        public double[] Elems { get; set; }

        public double this[int i]
        {
            get => Elems[i];
            set => Elems[i] = value;
        }

        public Vector()
        {
            Size = 0;
            Elems = null;
        }

        public Vector(int size)
        {
            Size = size;
            Elems = new double[size];
        }

        public Vector(int size, double value) : this(size)
        {
            Array.Fill(Elems, value);
        }

        public Vector(Vector other)
        {
            Size = other.Size;

            Elems = new double[Size];
            Array.Copy(other.Elems, Elems, Size);
        }

        public static Vector operator *(double lhs, Vector rhs)
        {
            Vector ret = new Vector(rhs);

            for (int i = 0; i < ret.Size; i++)
            {
                ret[i] *= lhs;
            }

            return ret;
        }

        public static Vector operator -( Vector lhs, Vector rhs)
        {
            Debug.Assert(lhs.Size == rhs.Size);

            Vector ret = new Vector(lhs);

            for (int i = 0; i < ret.Size; i++)
            {
                ret[i] -= rhs[i];
            }

            return ret;
        }

        public static Vector operator +(Vector lhs, Vector rhs)
        {
            Debug.Assert(lhs.Size == rhs.Size);

            Vector ret = new Vector(lhs);

            for (int i = 0; i < ret.Size; i++)
            {
                ret[i] += rhs[i];
            }

            return ret;
        }

        public static Vector operator -(Vector other)
        {
            Vector ret = new Vector(other.Size);

            for (int i = 0; i < other.Size; i++)
            {
                ret[i] = -other[i];
            }

            return ret;
        }

        public static double Dot(Vector lhs, Vector rhs)
        {
            Debug.Assert(lhs.Size == rhs.Size);

            double ret = 0.0;

            for (int i = 0; i < lhs.Size; i++)
            {
                ret += lhs[i] * rhs[i];
            }

            return ret;
        }

        public double Norm()
        {
            double ret = 0.0;

            foreach (double elem in Elems)
            {
                ret += DMath.Pow2(elem);
            }

            return Math.Sqrt(ret);
        }

        public double Scalar()
        {
            Debug.Assert(Size == 1);

            return Elems[0];
        }

        public override string ToString()
        {
            return "[ " + String.Join("; ", Elems) + " ]";
        }

    }
}
