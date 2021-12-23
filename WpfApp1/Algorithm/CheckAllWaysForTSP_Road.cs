using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfApp1.Algorithm
{
    internal class CheckAllWaysForTSP_Road
    {
		public List<int[]> Result_Run(double[][] input, int n)
		{
			double[][] A = input;
			int[] a = new int[n];
			List<int[]> xopts = new List<int[]>();
			double fopt = double.MaxValue;
			//double intInf = 1E300;
			for (int i = 0; i < n; i++)
			{
				a[i] = i;
			}
			while (true)
			{
				//Calculating
				double cost = 0;
				for (int i = 0; i < n - 1; i++)
				{
					cost += A[a[i]][a[i + 1]];
				}
				if (cost < fopt)
				{
					int[] xopt = new int[n];
					fopt = cost;
					for (int i = 0; i < n; i++)
					{
						xopt[i] = a[i];
					}
					xopts.Clear();
					xopts.Add(xopt);
					GC.Collect();
				}
				else if (cost == fopt)
				{
					int[] xopt = new int[n];
					for (int i = 0; i < n; i++)
					{
						xopt[i] = a[i];
					}
					xopts.Add(xopt);
				}
				//Initialize situation
				int index = n - 2;
				int tmp;
				while (index > -1 && a[index] >= a[index + 1])
				{
					index--;
				}
				if (index == -1) break;
				tmp = a[index];
				for (int i = n - 1; i > index; i--)
				{
					if (a[i] > tmp)
					{
						a[index] = a[i];
						a[i] = tmp;
						break;
					}
				}
				for (int i = (index + n) / 2 + 1; i < n; i++)
				{
					tmp = a[i];
					a[i] = a[index + n - i];
					a[index + n - i] = tmp;
				}
			}
			Console.WriteLine("---------------------- BEGIN REAL Result ----------------------------");
			Console.WriteLine("Cost: {0}", fopt);
			for (int ix = 0; ix < xopts.Count(); ix++)
			{
				Console.Write("{0}", xopts[ix][0]);
				for (int j = 1; j < n; j++)
				{
					Console.Write(" -> " + (xopts[ix][j] + 1));
				}
				Console.WriteLine();
			}
			Console.WriteLine("---------------------- END REAL Result ----------------------------");
			return xopts;
		}
	}
}
