using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WoodyChang0611
{
    public sealed partial class MathFunctions
    {
        static public IEnumerable<T[]> Combination<T>(T[] input, int count)
        {
            if (input.Count() < count)
            {
                String message = String.Format("Length of input: {0} is less than count: {1}",input.Count(),count);
                throw new ArgumentException(message);
            }

            List<T[]> finalResult = new List<T[]>();
            if (count == 1)
            {
                foreach (var element in input)
                {
                    T[] combination = new T[count];
                    combination[0] = element;
                    finalResult.Add(combination);
                }
                return finalResult;
            }

            for (int i = 0; i < input.Count() - count + 1; i++)
            {
                var result = Combination(input.Skip(i + 1).ToArray(), count - 1);
                foreach (var element in result)
                {
                    T[] combination = new T[count];
                    combination[0] = input[i];
                    for (int j = 1; j < count; j++)
                    {
                        combination[j] = element[j - 1];
                    }
                    finalResult.Add(combination);
                }
            }
            return finalResult;
        }
    }
}
