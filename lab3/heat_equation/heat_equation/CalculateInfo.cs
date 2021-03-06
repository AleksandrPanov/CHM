﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace heat_equation
{
    public class CalculateInfo
    {
        public double T = 0.25;
        public double L = 1.0;
        public double dt = 0.001;
        public double dx = 0.1;
        public double bCoeff = 0.0;

        public string b;
        public double[] alpha;
        public List<double[]> resDirect = new List<double[]>();
        public List<double[]> resImplicit = new List<double[]>();
        public double[] x = new double[0];

        public void setGrid()
        {
            resDirect.Clear();
            resImplicit.Clear();
            int ndt = Convert.ToInt32(T / dt) + 1;
            int ndx = Convert.ToInt32(L / dx) + 1;
            for (int i = 0; i < ndt; i++)
            {
                resDirect.Add(new double[ndx]);
                resImplicit.Add(new double[ndx]);
            }

            x = new double[ndx];
            for (int i = 1; i < ndx; i++)
                x[i] = i * dx;
        }
        private double calcB(double x)
        {
            switch (b)
            {
                case "coeff":
                    return bCoeff;

                case "coeff*x":
                    return bCoeff*x;

                case "coeff*cos(x)":
                    return bCoeff*Math.Cos(x);

                case "coeff*sin(x)":
                    return bCoeff * Math.Sin(x);

                case "coeff*sin(x)*cos(x)":
                    return bCoeff * Math.Cos(x) * Math.Sin(x);
            }
            return 0.0;
        }
        private void init()
        {
            double lambda = Math.PI / L;

            for (int i = 0; i < resDirect[0].Length; i++)
            {
                resDirect[0][i] = alpha[0];                
                for (int k = 1; k < alpha.Length; k++)
                {
                    double x = i * dx;
                    resDirect[0][i] += Math.Cos(k * lambda * x) * alpha[k];                   
                }
                resImplicit[0][i] = resDirect[0][i];
            }
        }

        public void calculate()
        {
            init();
            calculateImplicit();
            calculateDirect();
        }
        private void calculateDirect()
        {
            //var fs = new System.IO.StreamWriter(@"явный.txt", true);
            int sizeX = resDirect[0].Length;           
            for (int j = 0; j < resDirect.Count - 1; j++)
            {
                for (int i = 1; i < sizeX - 1; i++)
                {
                    double u0 = resDirect[j][i - 1];
                    double u1 = resDirect[j][i];
                    double u2 = resDirect[j][i + 1];
                    resDirect[j + 1][i] = (u2 - 2 * u1 + u0) * dt / (dx * dx) + calcB(i * dx) * u1 * dt + u1;
                    if (resDirect[j + 1][i] > 100000)
                        throw new OverflowException();
                }
                resDirect[j + 1][0] = resDirect[j + 1][1];
                resDirect[j + 1][sizeX - 1] = resDirect[j + 1][sizeX - 2];
                //for (int i = 0; i < sizeX; i++)
                //    fs.Write(resDirect[j][i].ToString()+" ");
                //fs.Write("\n");
            }
            //for (int i = 0; i < sizeX; i++)
            //    fs.Write(resDirect[sizeX - 1][i].ToString() + " ");
        }
        private void calculateImplicit()
        {
            //var fs = new System.IO.StreamWriter(@"неявный.txt", true);
            int sizeX = resImplicit[0].Length;
            double tmp = 1.0 / (dx * dx);
            double a = -tmp;
            double a_n = -1;
            double b = -tmp;
            double b_0 = -1;
            double[] c = new double[sizeX];
            double[] f = new double[sizeX];

            double m;           
            for (int j = 0; j < resImplicit.Count - 1; j++)
            {
                //init
                c[0] = 1;
                f[0] = 0;            
                for (int i = 1; i < sizeX - 1; i++)
                {
                    //c[i] = 1 / dt + 2 * tmp;
                    //f[i] = resImplicit[j][i] / dt + calcB(i*dx)*resImplicit[j][i];
                    c[i] = 1 / dt + 2 * tmp - calcB(i * dx);
                    f[i] = resImplicit[j][i] / dt;
                }
                c[sizeX - 1] = 1;
                f[sizeX - 1] = 0;

                //calculate
                m = a / c[0];
                c[1] -= m * b_0;
                f[1] -= m * f[0];
                for (int i = 2; i < sizeX-1; i++)
                {                
                    m = a / c[i - 1];
                    c[i] -= m * b;
                    f[i] -= m * f[i - 1];
                }
                m = a_n / c[sizeX-2];
                c[sizeX - 1] -= m * b;
                f[sizeX - 1] -= m * f[sizeX - 2];

                resImplicit[j + 1][sizeX - 1] = f[sizeX - 1] / c[sizeX - 1];
                for (int i = sizeX - 2; i >= 1; i--)
                    resImplicit[j+1][i] = (f[i] - b * resImplicit[j + 1][i + 1]) / c[i];
                resImplicit[j + 1][0] = (f[0] - b_0 * resImplicit[j + 1][1]) / c[0];
                //for (int i = 0; i < sizeX; i++)
                //    fs.Write(resImplicit[j][i].ToString() + " ");
                //fs.Write("\n");
            }
            //for (int i = 0; i < sizeX; i++)
            //   fs.Write(resImplicit[sizeX - 1][i].ToString() + " ");
        }
    }
}