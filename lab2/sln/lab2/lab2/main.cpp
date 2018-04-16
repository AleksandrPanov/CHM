#include "menu.h"
#include "math_function.h"
#include "calculate_integral.h"
using namespace std;

struct CalculateInfo
{
	vector<MathFuncton*> functions;
	vector<CalculateIntegral*> calculateSchemes;

	int function;
	int calculateScheme;
	double a, b;
	double dx;
	int N;

	double calculate()
	{
		return calculateSchemes[calculateScheme]->calculate(functions[function], a, b, N);
	}
    double calculateExactly()
    {
        if (function == 0)
        {
            return cos(a) - cos(b);
        }
        else if (function == 1)
        {
            return (sin(b) - sin(a));
        }
        else if (function == 2)
        {
            return (b*b/2 - a*a/2);
        }
        return 0;
    }
    bool isReady()
    {
        return (isFunction & isCalculateScheme & isA & isB & isDx & isN);
    }
    void printStatus()
    {
        if (!isFunction)
            cout << "set the function \n";
        else
            cout << "function is " << functions[function]->getName()<<"\n";
        if (!isCalculateScheme)
            cout << "set the calculate scheme \n";
        else
            cout << "calculate scheme is " << calculateSchemes[calculateScheme]->getName() << "\n";
        if (!isA || !isB)
            cout << "set the limits \n";
        else
            cout << "left limit is " <<a<<"right limit is "<<b<<"\n";
        if (!isDx || ! isN)
            cout << "set the number of points \n";
        else
            cout << "number of points is " << N << "dx is" << dx << "\n";
        cout << "\n";
    }

	bool isFunction = false;
	bool isCalculateScheme = false;
	bool isA = false;
	bool isB = false;
	bool isDx = false;
	bool isN = false;
};

void printFunctions(const CalculateInfo &calculateInfo)
{
    for (int i = 0; i < calculateInfo.functions.size(); i++)
    {
        cout << i << ' ' << calculateInfo.functions[i]->getName()<<'\n';
    }
}
void printCalculateSchemes(const CalculateInfo &calculateInfo)
{
    for (int i = 0; i < calculateInfo.calculateSchemes.size(); i++)
    {
        cout << i << ' ' << calculateInfo.calculateSchemes[i]->getName() << '\n';
    }
}
void readFunction(CalculateInfo &calculateInfo)
{
    cout << "Choice the function:\n";
    printFunctions(calculateInfo);
    cin >> calculateInfo.function;
    calculateInfo.isFunction = true;
}
void readScheme(CalculateInfo &calculateInfo)
{
    cout << "Choice the calculate scheme:\n";
    printCalculateSchemes(calculateInfo);
    cin >> calculateInfo.calculateScheme;
    calculateInfo.isCalculateScheme = true;
}
void readLimits(CalculateInfo &calculateInfo)
{
    cout << "print left and right borders:\n";
    cin >> calculateInfo.a >> calculateInfo.b;
    calculateInfo.isA = true;
    calculateInfo.isB = true;
}
void readSize(CalculateInfo &calculateInfo)
{
    cout << "print numbers of points\n";
    cin >> calculateInfo.N;
    calculateInfo.isN = true;
    calculateInfo.isDx = true;
}

bool isReady(CalculateInfo &calculateInfo)
{
    return calculateInfo.isReady();
}

void init(CalculateInfo &calculateInfo)
{
    Sinus *msin = new Sinus();
    Cosinus* mcos = new Cosinus();
	LinearFuncton* mlf = new LinearFuncton();
	calculateInfo.functions = { (MathFuncton*)msin , (MathFuncton*)mcos, (MathFuncton*)mlf };

    RightRectangleMethod *m1 = new RightRectangleMethod();
    LeftRectangleMethod *m2 = new LeftRectangleMethod();
    TrapezeMethod *m3 = new TrapezeMethod();
    Monte_KarloMethod *m4 = new Monte_KarloMethod();
	Gauss_Method *m5 = new Gauss_Method();
    Simpson_Method *m6 = new Simpson_Method();
    Boole_Method5 *m7 = new Boole_Method5();
    Newton_Cotesa_Method4 *m8 = new Newton_Cotesa_Method4();
    Simpson_Method4 *m9 = new Simpson_Method4();

	calculateInfo.calculateSchemes = { (CalculateIntegral*)m1, (CalculateIntegral*)m2,
		(CalculateIntegral*)m3, (CalculateIntegral*)m4,
        (CalculateIntegral*)m5, (CalculateIntegral*)m6,
        (CalculateIntegral*)m7,  (CalculateIntegral*)m8,
        (CalculateIntegral*)m9
    };
}

int main()
{
    Menu menu;
	CalculateInfo calculateInfo;

    init(calculateInfo);

    menu.push_back("calculate integral"); //Лучше чтобы меню было на русском, а то Эгаму будет не понятно!!!
	menu.push_back("set integral (set all)");
	menu.push_back("set function");
	menu.push_back("set limit of integration");
	menu.push_back("set scheme");
	menu.push_back("set num point");
    menu.push_back("print status");
    while (1)
    {
        menu.print();
        cin >> menu.choice;
        if (menu.choice == menu.exit)
        {
            break;
        }
        else if (menu.choice == "1")
        {
			if (isReady(calculateInfo))
			{
				menu.clear();
                double res = calculateInfo.calculate();
                cout << res << "\n";
                double exactlyRes = calculateInfo.calculateExactly();
                cout << "the exact value of the integral is " << exactlyRes << "\n";
                cout << "error is " << exactlyRes - res << "\n\n";             
			}
            else
            {
                menu.clear();
                calculateInfo.printStatus();
            }
        }
        else if (menu.choice == "2")
        {
			readFunction(calculateInfo);
			readLimits(calculateInfo);
			readScheme(calculateInfo);
			readSize(calculateInfo);
            menu.clear();
        }
		else if (menu.choice == "3")
		{
			readFunction(calculateInfo);
            menu.clear();
		}
		else if (menu.choice == "4")
		{
			readLimits(calculateInfo);
            menu.clear();
		}
		else if (menu.choice == "5")
		{
			readScheme(calculateInfo);
            menu.clear();
		}
		else if (menu.choice == "6")
		{
			readSize(calculateInfo);
            menu.clear();
		}
        else if (menu.choice == "7")
        {
            menu.clear();
            calculateInfo.printStatus();
        }
        else
        {
            menu.clear();
        }
    }
    return 0;
}