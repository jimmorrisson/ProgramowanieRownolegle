using System;
using System.Diagnostics;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;

namespace AK7_CSHARP.Algebra
{
    public class Matrix
    {
        public static int ThreadNumber { get; set; }

        public int Rows { get; }

        public int Columns { get; }

        public double[][] Elems { get; set; }

        public double this[int row, int col]
        {
            get => Elems[row][col];
            set => Elems[row][col] = value;
        }

        public double[] this[int row]
        {
            get => Elems[row];
            set => Elems[row] = value;
        }

        public Matrix(int rows, int columns)
        {
            Rows = rows;
            Columns = columns;
            Elems = new double[Rows][];

            for (int i = 0; i < Rows; i++)
            {
                Elems[i] = new double[Columns];
            }
        }

        public Matrix(Matrix other)
        {
            Rows = other.Rows;
            Columns = other.Columns;
            Elems = new double[Rows][];

            for (int i = 0; i < Rows; i++)
            {
                Elems[i] = new double[Columns];
                Array.Copy(other[i], Elems[i], Columns);
            }
        }

        public Matrix(Vector vector) : this(vector.Size, 1)
        {
            for (int i = 0; i < vector.Size; i++)
            {
                Elems[i][0] = vector[i];
            }
        }

        public static Matrix operator *(double value, Matrix other)
        {
            Matrix ret = new Matrix(other);

            for (int row = 0; row < ret.Rows; row++)
            {
                for (int col = 0; col < ret.Columns; col++)
                {
                    ret[row, col] *= value;
                }
            }

            return ret;
        }

        public static Matrix operator *(Matrix other, double value)
        {
            return value * other;
        }

        public static Vector operator *(Matrix matrix, Vector vector)
        {
            Debug.Assert(matrix.Columns == vector.Size);

            Vector ret = new Vector(matrix.Rows);

            for (int row = 0; row < matrix.Rows; row++)
            {
                for (int col = 0; col < matrix.Columns; col++)
                {
                    ret[row] += vector[col] * matrix[row, col];
                }
            }

            return ret;
        }

        public static Matrix operator +(Matrix lhs, Matrix rhs)
        {
            Debug.Assert(lhs.Rows == rhs.Rows && lhs.Columns == rhs.Columns);

            Matrix ret = new Matrix(lhs);

            for (int row = 0; row < lhs.Rows; row++)
            {
                for (int col = 0; col < lhs.Columns; col++)
                {
                    ret[row, col] += rhs[row, col];
                }
            }

            return ret;
        }

        public static Matrix operator -(Matrix lhs, Matrix rhs)
        {
            Debug.Assert(lhs.Rows == rhs.Rows && lhs.Columns == rhs.Columns);

            Matrix ret = new Matrix(lhs);

            for (int row = 0; row < lhs.Rows; row++)
            {
                for (int col = 0; col < lhs.Columns; col++)
                {
                    ret[row, col] -= rhs[row, col];
                }
            }

            return ret;
        }
        
        public static Matrix operator *(Matrix lhs, Matrix rhs)
        {
            Debug.Assert(lhs.Columns == rhs.Rows);

            Matrix ret = new Matrix(lhs.Rows, rhs.Columns);

            double getMultipliedValue(int row, int col)
            {
                double val = 0.0;
                for (int i = 0; i < lhs.Columns; i++)
                {
                    val += lhs[row, i] * rhs[i, col];
                }
                return val;
            }

#if USE_PARALLEL
            Parallel.For(0, lhs.Rows, new ParallelOptions { MaxDegreeOfParallelism = ThreadNumber }, row =>
            {
                for (int col = 0; col < rhs.Columns; col++)
                {
                    ret[row, col] = getMultipliedValue(row, col);
                }
            });
#else
            for (int row = 0; row < lhs.Rows; row++)
            {
                for (int col = 0; col < rhs.Columns; col++)
                {
                    ret[row, col] = getMultipliedValue(row, col);
                }
            }
#endif

            return ret;
        }

        public static Matrix Identity(int size)
        {
            Matrix ret = new Matrix(size, size);

            for (int row = 0; row < size; row++)
            {
                ret[row, row] = 1.0;
            }

            return ret;
        }

        public static Matrix Transpose(Vector vector)
        {
            Matrix ret = new Matrix(1, vector.Size);

            Array.Copy(vector.Elems, ret[0], vector.Size);
            
            return ret;
        }

        public static Matrix Negate(Matrix matrix)
        {
            Matrix ret = new Matrix(matrix);
            
            for (int row = 0; row < ret.Rows; row++)
            {
                for (int col = 0; col < ret.Columns; col++)
                {
                    ret[row, col] = -ret[row, col];
                }
            }

            return ret;
        }

        public double Scalar()
        {
            Debug.Assert(Rows == 1 && Columns == 1);

            return Elems[0][0];
        }

        public override string ToString()
        {
            return string.Join("\n", Elems.Select(x => "[ " + string.Join(", ", x) + " ]"));
        }
    }
}
