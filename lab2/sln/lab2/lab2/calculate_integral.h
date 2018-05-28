#pragma once
#include <string>
#include "math_function.h"

int rounding(double x)
{
    return x + 0.5;
}

class CalculateIntegral
{
protected:
    std::string name;


public:
    virtual double calculate(MathFuncton *f, double a, double b, int numPoints) = 0;
    virtual double calculate(MathFuncton *f, double a, double b, double dx) = 0;
    string getName()
    {
        return name;
    }
};

class RightRectangleMethod : CalculateIntegral
{
public:
    RightRectangleMethod()
    {
        name = "right rectangle method";
    }
    double calculate(MathFuncton *f, double a, double b, int numPoints) override
    {
        double dx = (b - a) / (numPoints - 1);
        return calculate(f, a, b, dx);
    }

    double calculate(MathFuncton *f, double a, double b, double dx) override
    {
        double res = 0;
        int numPoints = rounding((b - a) / dx); //+0.5 серьёзно? - это для правильного округления
        double x = a + dx;
        for (int i = 0; i < numPoints; i++)
        {
            res += (*f)(x)*dx;
            x += dx;
        }
        return res;
    }
};


class LeftRectangleMethod : CalculateIntegral
{
public:
    LeftRectangleMethod()
    {
        name = "left rectangle method"; 
    }
    double calculate(MathFuncton *f, double a, double b, int numPoints) override
    {
        double dx = (b - a) / (numPoints - 1);
        return calculate(f, a, b, dx);
    }

    double calculate(MathFuncton *f, double a, double b, double dx) override
    {
        double res = 0;
        int numPoints = rounding((b - a) / dx);
        double x = a;
        for (int i = 0; i < numPoints; i++)
        {
            res += (*f)(x)*dx;
            x += dx;
        }
        return res;
    }
};

class TrapezeMethod : CalculateIntegral
{
public:
	TrapezeMethod()
	{
		name = "trapeze method";
	}
	double calculate(MathFuncton *f, double a, double b, int numPoints) override
	{
		double dx = (b - a) / (numPoints - 1);
		return calculate(f, a , b, dx);
	}

	double calculate(MathFuncton *f, double a, double b, double dx) override
	{
		double res = 0;
		int numPoints = rounding((b - a) / dx);
		double x = a;
		for (int i = 0; i < numPoints; i++)
		{
			res += ((*f)(x + dx) + (*f)(x)) / 2 * dx; //f(x.i+1)+f(x.i)/2 * (x.i+1 - x.i) Формула метода трапеций С Википедии
			x += dx;
		}
		return res;
	}
};

class Monte_KarloMethod : CalculateIntegral
{
public:
	Monte_KarloMethod()
	{
		name = "monte karlo method";
	}
	double calculate(MathFuncton *f, double a, double b, int numPoints) override
	{
		double dx = (b - a) / (numPoints - 1);
		return calculate(f, a, b, dx);
	}

	double calculate(MathFuncton *f, double a, double b, double dx) override
	{
		double res = 0;
		int numPoints = rounding((b - a) / dx);
		double x = a;
		for (int i = 0; i < numPoints; i++)
		{
			res += (*f)(x);
			x += dx;
		}
		res *=(b - a) / numPoints;
		return res;
	}
};

class Gauss_Method : CalculateIntegral
{
public:
	Gauss_Method()
	{
		name = "gauss method";
	}
	double calculate(MathFuncton *f, double a, double b, int numPoints) override
	{
		double dx = (b - a) / (numPoints - 1);
		return calculate(f, a, b, dx);
	}

	double calculate(MathFuncton *f, double a, double b, double dx) override
	{
		double res = 0;
		int numPoints = rounding((b - a) / dx);
		double x = a;

		for (int i = 0; i < numPoints; i++)
		{
			res += dx / 2 * ((*f)(((x + x + dx) / 2) - dx / 2 * sqrt(3)) + (*f)(((x + x + dx) / 2) + dx / 2 * sqrt(3)));
			x += dx;
		}
		return res;
	}
};

class Simpson_Method : CalculateIntegral
{
public:
    Simpson_Method()
    {
        name = "simpson method";
    }
    double calculate(MathFuncton *f, double a, double b, int numPoints) override
    {
        double res = 0.0;
        double h = (b - a) / numPoints;
        double sum4 = 0, sum2 = 0, sum = 0;
        for (int i = 1; i <= numPoints; i += 2)
        {
            sum4 += (*f)(a + h*i);//Значения с нечётными индексами, которые нужно умножить на 4.
            sum2 += (*f)(a + h*(i + 1));//Значения с чётными индексами, которые нужно умножить на 2.
        }
        sum = (*f)(a) + 4 * sum4 + 2 * sum2 - (*f)(b);//Отнимаем значение f(b) так как ранее прибавили его дважды. 
        return (h / 3)*sum;
    }
    double calculate(MathFuncton *f, double a, double b, double dx) override
    {
        return 0.0;
    }
};

class Simpson_Method4 : CalculateIntegral
{
public:
    Simpson_Method4()
    {
        name = "four-point simpson method";
    }
    double calculate(MathFuncton *f, double a, double b, int numPoints) override
    {
        double res = 0.0;
        double h = (b - a) / numPoints;
        double x1 = a, x2 = a + h, x3 = a + 2 * h, x4 = a + 3*h;
        for (int i = 3; i < numPoints; i += 3)
        {
            res += (*f)(x1);
            res += 3.0*(*f)(x2);
            res += 3.0*(*f)(x3);
            res += (*f)(x4);

            x1 += 3 * h;
            x2 += 3 * h;
            x3 += 3 * h;
            x4 += 3 * h;
        }
        return res * h / 8.0;
    }
    double calculate(MathFuncton *f, double a, double b, double dx) override
    {
        return 0.0;
    }
};

class Boole_Method5 : CalculateIntegral
{
public:
    Boole_Method5()
    {
        name = "five-point method Boole";
    }
    double calculate(MathFuncton *f, double a, double b, int numPoints) override
    {
        double res = 0.0;
        double h = (b - a) / numPoints;
        double x1 = a, x2 = a + h, x3 = a + 2 * h,  x4 = a + 3 * h, x5 = a + 4 * h;
        for (int i = 4; i < numPoints; i += 4)
        {
            res += (2.0*h / 45.0)*(7.0*(*f)(x1) + 32.0*(*f)(x2) + 12.0*(*f)(x3) + 32.0*(*f)(x4) + 7.0*(*f)(x5));

            x1 += 4 * h;
            x2 += 4 * h;
            x3 += 4 * h;
            x4 += 4 * h;
            x5 += 4 * h;
        }
        return res;
    }
    double calculate(MathFuncton *f, double a, double b, double dx) override
    {
        return 0.0;
    }
};

class Newton_Cotesa_Method4 : CalculateIntegral
{
public:
    Newton_Cotesa_Method4()
    {
        name = "four-point method Newton Cotesa";
    }
    double calculate(MathFuncton *f, double a, double b, int numPoints) override
    {
        double res = 0.0;
        double h = (b - a) / numPoints;
        double x1 = a, x2 = a + h, x3 = a + 2 * h,  x4 = a + 3 * h;
        for (int i = 3; i < numPoints; i += 3)
        {
            res += (*f)(x1);
            res += 3.0*(*f)(x2);
            res += 3.0*(*f)(x3);
            res += (*f)(x4);
           
            x1 += 3 * h;
            x2 += 3 * h;
            x3 += 3 * h;
            x4 += 3 * h;           
        }
        return (res * 3.0*h) / 8.0;
    }
    double calculate(MathFuncton *f, double a, double b, double dx) override
    {
        return 0.0;
    }
};