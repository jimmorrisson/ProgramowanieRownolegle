using AK7_CSHARP.Algebra;

namespace AK7_CSHARP.Functions
{
    public abstract class IFunction
    {
        public int Size { get; }

        public string Name { get; }

        private Vector initialVector;

        public Vector InitialVector
        {
            get { return new Vector(initialVector); }
        }

        public IFunction(int size, string name, Vector initialVector)
        {
            Size = size;
            Name = name;
            this.initialVector = initialVector;
        }

        public abstract double Func(Vector x, ref Vector grad);

        public override string ToString()
        {
            return $"{Name}\nSize: {Size}\nInitial vector: {InitialVector}";
        }
    }
}
