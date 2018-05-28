using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Forms.DataVisualization.Charting;

namespace heat_equation
{    
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }
        private CalculateInfo calc = new CalculateInfo();
        private List<TextBlock> alphaTextBlock = new List<TextBlock>();
        private List<TextBox> alphaTextBox = new List<TextBox>();
        private void Grid_Loaded(object sender, RoutedEventArgs e)
        {
            // Все графики находятся в пределах области построения ChartArea, создадим ее
            chart.ChartAreas.Add(new ChartArea("Default"));
            chart.ChartAreas[0].AxisX.Interval = 1;

            // Добавим линию, и назначим ее в ранее созданную область "Default"
            chart.Series.Add(new Series("Series1"));
            chart.Series["Series1"].ChartArea = "Default";
            chart.Series["Series1"].ChartType = SeriesChartType.Line;

            chart.Series.Add(new Series("Series2"));
            chart.Series["Series2"].ChartArea = "Default";
            chart.Series["Series2"].ChartType = SeriesChartType.Line;

            chart.Series.Add(new Series("Series3"));
            chart.Series["Series3"].ChartArea = "Default";
            chart.Series["Series3"].ChartType = SeriesChartType.Line;

            // добавим данные линии
            string[] axisXData = new string[] { "a", "b", "c" };
            double[] axisYData = new double[] { 0.1, 1.5, 1.9};
            chart.Series["Series1"].Points.DataBindXY(axisXData, axisYData);

            axisYData = new double[] { -0.1, 0.5, 0.9 };
            chart.Series["Series2"].Points.DataBindXY(axisXData, axisYData);

            axisYData = new double[] { -0.5, 2, -0.1 };
            chart.Series["Series3"].Points.DataBindXY(axisXData, axisYData);
        }

        private bool validation()
        {
            string error = "";
            int n = getFormN();

            try { setT(getFormT()); }   
            catch
            {
                error+="T должно быть положительным вещественным числом\n";
            }
            try { setL(getFormL());}
            catch
            {
                error += ("L должно быть положительным вещественным числом\n");
            }
            try { set_t(getForm_tBox()); }
            catch
            {
                error += ("t должно быть неотрицательным вещественным числом и быть меньше T\n");
            }
            try { set_dt(getFormDt()); }
            catch
            {
                error += ("dt должно быть положительным вещественным числом\n");
            }
            try { set_dx(getFormDx()); }
            catch
            {
                error += ("dx должно быть положительным вещественным числом\n");
            }
            try { calc.alpha = getFormAlpha(); }
            catch
            {
                error += ("α должно быть вещественным числом");
            }
            if (error != "")
            {
                MessageBox.Show(error);
                return false;
            }
            return true;
        }
        private void setT(double T)
        {
            TBox.Text = T.ToString();
            tSlider.Maximum = T;
            calc.T = T;
        }
        private double getFormT()
        {
            double T = Convert.ToDouble(TBox.Text);
            if (T <= 0)
                throw new Exception();
            return T;
        }

        private void setL(double L)
        {
            LBox.Text = L.ToString();
            calc.L = L;
        }
        private double getFormL()
        {
            double L = Convert.ToDouble(LBox.Text);
            if (L <= 0)
                throw new Exception();
            return L;
        }

        private void set_t(double t)
        {
            t_Box.Text = t.ToString();
            tSlider.Value = t;
        }
        private double getForm_t()
        {
            return tSlider.Value;
        }
        private double getForm_tBox()
        {
            double t = Convert.ToDouble(t_Box.Text);
            if (t < 0 || t > tSlider.Maximum)
                throw new Exception();
            return t;
        }

        private void set_dt(double dt)
        {
            dtBox.Text = dt.ToString();
            calc.dt = dt;
        }
        private double getFormDt()
        {
            double dt = Convert.ToDouble(dtBox.Text);
            if (dt <= 0)
                throw new Exception();
            return dt;
        }

        private void set_dx(double dx)
        {
            calc.dx = dx;
        }
        private double getFormDx()
        {
            double dx = Convert.ToDouble(dxBox.Text);
            if (dx <= 0)
                throw new Exception();
            return dx;
        }

        private int getFormN()
        {
            ComboBoxItem item = (ComboBoxItem)nBox.SelectedItem;
            return Convert.ToInt32(item.Content);
        }
        private double[] getFormAlpha()
        {
            int n = getFormN();          
            double []alpha = new double[n];
            if (n > 0)
            {
                alpha[0] = Convert.ToDouble(aBox.Text);
                for (int i = 1; i < n; i++)
                    alpha[i] = Convert.ToDouble(alphaTextBox[i - 1].Text);
            }
            return alpha;
        }

        private void tSlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            double t = tSlider.Value;
            string time = t.ToString();
            if (time.Length > 5)
                time = time.Substring(0, 5);
            t_Box.Text = time;
            int indexT = (int)(t / calc.dt);
            if (calc.resDirect.Count > indexT)
            {
                chart.Series["Series2"].Points.DataBindXY(calc.x, calc.resDirect[indexT]);
                chart.Series["Series3"].Points.DataBindXY(calc.x, calc.resImplicit[indexT]);
            }
        }
        private void nBox_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            for (int i = 0; i < alphaTextBlock.Count; i++)
                stack1.Children.Remove(alphaTextBlock[i]);
            for (int i = 0; i < alphaTextBox.Count; i++)
                stack2.Children.Remove(alphaTextBox[i]);
            alphaTextBlock.Clear();
            alphaTextBox.Clear();
            int n = getFormN();
            for (int i = 1; i < n; i++)
            {
                alphaTextBlock.Add(new TextBlock
                {
                    Text = ("α " + i.ToString()),
                    FontSize = 20,
                    Height = 40,
                    Margin =  new System.Windows.Thickness(5)
                });
                TextBox box = (new TextBox
                {
                    FontSize = 20,
                    Text = "1,0",
                    Height = 40,
                    Margin = new System.Windows.Thickness(5)
                });
                alphaTextBox.Add(box);

                stack1.Children.Add(alphaTextBlock[i - 1]);
                stack2.Children.Add(alphaTextBox[i - 1]);
            }
            calc.alpha = getFormAlpha();
        }

        private void Window_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Space)
            {
                set_t(0.0);
                if (validation())
                {
                    try
                    {
                        calc.setGrid();
                        calc.calculate();
                        MessageBox.Show("Посчитано");
                        int indexT = (int)(tSlider.Value / calc.dt);
                        if (calc.L / calc.dx >= 50)
                        {
                            chart.ChartAreas[0].AxisX.Interval = calc.L/50;
                        }
                        else
                            chart.ChartAreas[0].AxisX.Interval = calc.dx;
                        chart.Series["Series1"].Points.DataBindXY(calc.x, calc.resDirect[0]);
                        chart.Series["Series2"].Points.DataBindXY(calc.x, calc.resDirect[indexT]);
                        chart.Series["Series3"].Points.DataBindXY(calc.x, calc.resImplicit[indexT]);
                    }
                    catch(OverflowException ex)
                    {
                        MessageBox.Show("Увеличьте dx или уменьшите dt, явный метод работает некорректно");
                    }
                    catch
                    {
                        MessageBox.Show("Увеличьте dt или dx");
                    }               
                }
            }
        }
    }
}
