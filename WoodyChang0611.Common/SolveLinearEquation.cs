using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WoodyChang0611
{
    public partial class MathFunctions
    {
        public class SolveLinearEquationArugmentWrongException : Exception { };
        public class SolveLinearEquationNoSolutionException : Exception { };


        public static void SolveLinearEquation(Double[,] input, ref Double[] result)
        {
            //Check Input 必須是 [degree,degree+1]
            Int32 degree = input.GetLength(0);
            if(degree <1)
                throw new SolveLinearEquationArugmentWrongException();
            if (input.GetLength(1) != degree + 1)
                throw new SolveLinearEquationArugmentWrongException();

            //高斯消去法
            for (Int32 i = 0; i < degree - 1; i++)
            {
                Double maxAbs = input[i,i];
                Int32 pivotRow = i;
                for (Int32 j = i; j < degree; j++)
                {
                    if (maxAbs < Math.Abs(input[j, i]))
                    {
                        maxAbs = Math.Abs(input[j, i]);
                        pivotRow = j;
                    }
                }
                //Swap Row if Pivot if different
                if (pivotRow != i)
                {
                    for (Int32 j = 0; j < degree + 1; j++)
                    {
                        Double temp = input[i, j];
                        input[i, j] = input[pivotRow, j];
                        input[pivotRow, j] = temp;
                    };
                }
                //Delta 為0 為解
                if (input[i, i] == 0)
                {
                    throw new SolveLinearEquationNoSolutionException();
                }
                
                for (Int32 j = i+1; j < degree; j++)
                {
                    Double ratio = input[j,i] / input[i,i];
                    for (Int32 k = 0; k < degree + 1; k++)
                    {
                        input[ j,k] -= ratio *input[i,k];
                    }
                }
            }
            for (Int32 i = degree - 1; i > 0; i--)
            {
                for (Int32 j = 0; j < i; j++)
                {
                    if (input[i,i] == 0)
                        throw new SolveLinearEquationNoSolutionException();
                    Double ratio = input[j,i] / input[i,i];
                    for (Int32 k = 0; k < degree + 1; k++)
                    {
                        input[j,k] -= ratio * input[i,k];
                    }
                }
            }
            result = new Double[degree];
            for (Int32 i = 0; i < degree; i++)
            {
                result[i] = input[i,degree] / input[i,i];
            }
        }

        static void Output(Double[,] a)
        {
            for (Int32 i = 0; i < a.GetLength(0); i++)
            {
                for (Int32 j = 0; j < a.GetLength(1); j++)
                {
                    Console.Write(a[i, j].ToString() + ",");
                }
                Console.WriteLine();
            }
        }

        public static void Test(string[] args)
        {
            Random rand = new Random((Int32)DateTime.Now.Ticks);
            int dim = 300;
            Double[,] a = new Double[dim,dim+1];
            Double[,] aTemp = new Double[dim, dim+1];
            Double[] b = new Double[dim];
            DateTime start = DateTime.Now;
            for (Int32 i = 0; i < a.GetLength(0); i++)
            {
                for (Int32 j = 0; j < a.GetLength(1); j++)
                {
                    a[i, j] = (Int32)(rand.NextDouble() * 10 -5);
                    aTemp[i, j] = a[i, j];
                }
            }
            Console.WriteLine("Input Array");
            try
            {
                WoodyChang0611.MathFunctions.SolveLinearEquation(a, ref b);
            }
            catch(Exception e)
            {
                Console.Write(e.Message.ToString());
            }
            Console.WriteLine("Result Array");
            Console.WriteLine("Answer");
            foreach (var i in b)
                Console.Write(i.ToString()+",");
            Console.WriteLine();
            Console.WriteLine("Test Answer");
            for (Int32 i = 0; i < a.GetLength(0); i++)
            {
                Double ans = 0;
                for (Int32 j = 0; j < a.GetLength(0); j++)
                {
                    ans += (b[j] * aTemp[i, j]);
                }
                Console.WriteLine(ans.ToString()+","+aTemp[i,dim]);
            }
            Console.WriteLine(start.ToString());
            Console.WriteLine(DateTime.Now.ToString());
            TimeSpan t = DateTime.Now - start;
            Console.WriteLine(t.Milliseconds);
        }
    }
}
