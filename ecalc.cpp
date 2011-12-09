// ecalc.cpp
// Электротехнический калькулятор
// Компиляция: g++ ecalc.cpp

#include <iostream>
#include <cmath>

using namespace std;

double input(string promt)
{
  cout << promt << ": ";
  double result;
  cin >> result;
  if (cin.fail()) { cin.clear(); string str; cin >> str; }
  return result;
}

void input(string promt, string &result)
{
  cout << promt << ": ";
  cin >> result;
}

void ohm()
{
  double i = 0;
  double p = 0;
  double u = 0;
  double r = 0;
  
  int n = 0;
  // цикл прервется, когда будет введено достаточное количесво параметров
  while(1)
    {
      if (i == 0)
        {
          i = input(" Ток");
          if (i)    if (++n == 2) break;
        }
      
      if (u == 0)
        {
          u = input(" Напряжение");
          if (u)    if (++n == 2) break;
        }
      
      if (p == 0)
        {
          p = input(" Мощность");
          if (p)      if (++n == 2) break;
        }
      
      if (r == 0)
        {
          r = input(" Сопротивление");
          if (r) if (++n == 2) break;
        }
    }

  if (p && i)
    {
      u = p/i; r = p / (i*i);
      cout << "  Напряжение = " << u << " В" << endl;
      cout << "  Сопротивление = "<< r << " Ом" << endl;
      return;
    }
    
  if (p && r)
    {
      i = sqrt(p/r); u = sqrt(p*r);
      cout << "  Ток = " << i << " А" << endl;
      cout << "  Напряжение = "<< u << " В" << endl;
      return;
    }
    
  if (p && u)
    {
      i = p/u; r = u*u/p;
      cout << "  Ток = " << i << " А" << endl;  
      cout << "  Сопротивление = "<< r << " Ом" << endl;
      return;
    }
    
  if (i && u)
    {
      p = i*u; r = u/i;
      cout << "  Сопротивление = " << r << " Ом" << endl;
      cout << "  Мощность = "<< p << " Вт" << endl;
      return;
    }
    
  if (i && r)
    {
      u = i*r; p = i*i*r;
      cout << "  Напряжение = " << u << " В" << endl;
      cout << "  Мощность = "<< p << " Вт" << endl;
      return;
    }
  
  if (u && r)
    {
      i = u/r; p = u*u/r;
      cout << "  Ток = " << i << " А" << endl;
      cout << "  Мощность = "<< p << " Вт" << endl;
      return;
    }
}

void diode()
{
  double i = 0;   // ток в цепи
  double p = 0;   // мощность на резисторе
  double ud = 0;  // диодное напряжение
  double ur = 0;  // напряжение резистора
  double e = 0;   // напряжение питания
  double r = 0;   // сопротивление
  double eff = 0; // КПД
  
  int n = 0;
  // цикл прервется, когда будет введено достаточное количесво параметров
  while(1)
    {
      if (i == 0)
        {
          i = input(" Ток");
          if (i)    if (++n == 3) break;
        }
      
      if (ud == 0)
        {
          ud = input(" Диодное напряжение");
          if (ud)    if (++n == 3) break;
        }
      
      if (e == 0)
        {
          e = input(" Напряжение питания");
          if (e)      if (++n == 3) break;
        }
    }
    
    ur = e - ud;
    r = ur / i;
    p = i*ur;
    eff = ur / e;
    
    cout << "  Характеристики резистора" << endl;
    cout << "    Сопротивление = " << r << " Ом" << endl;
    cout << "    Мощность = "<< p << " Вт" << endl;
    cout << "    Напряжение = "<< ur << " В" << endl;
    cout << "    КПД = "<< eff*100 << " %" << endl;
}

void
nom()
  {
    
  }

void big_help()
{
  cout << "Описание функций:" << endl << endl;
  
  cout << "  ohm - Закон Ома. Расчет двух из четырех параметров" << endl;
  cout << "        (напряжение, ток, мощность, сопротивление)," << endl; 
  cout << "        когда заданы два другие." << endl << endl;
  
  cout << "  nom - Расчет левого и правого номинала для заданного значения\n\n";
  
  cout << "  diode - Расчет резистора в цепи диода." << endl << endl;
  
  cout << "  quit или exit - Выход из программы." << endl << endl; 
  
  cout << "  help - Справка. Описание функций." << endl << endl;
  
  cout << "  h - Справка. Список функций без описания." << endl << endl; 
}

void mini_help()
{
  cout << "Список функций:" << endl;
  cout << "ohm, diode, quit, exit, help, h" << endl;
}

int main(int argv, char **argc)
{
  while(1)
    {
      string func;
      input("Функция", func);
      if (func == "ohm") { ohm(); continue; }
      if (func == "diode") { diode(); continue; }
      if (func == "help") { big_help(); continue; }
      if (func == "h") { mini_help(); continue; }
      if (func == "quit") break;
      if (func == "exit") break;
    }
  return 0;
}

