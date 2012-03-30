// ecalc.cpp
// Электротехнический калькулятор
// Сборка и запуск: g++ ecalc.cpp -o ecalc && ./ecalc

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;

/* Возвращает левый операнд выражения exp, если оператора в позиции op_pos */
string lop(string exp, int op_pos)
{
  exp.assign(exp, 0, op_pos);
  return exp;
}

/* Возвращает правый операнд выражения exp, если оператора в позиции op_pos */
string rop(string exp, int op_pos)
{
  exp.assign(exp, op_pos + 1, exp.size() - op_pos + 1);
  return exp;
}

/* Возвращает результат математического выражения exp.
В выражении допускаются вещественные числа и операторы: '+' '-' '*' '/' '^'
а также скобки. Расчет выражения по математическим правилам. */
double solve(string exp)
{
  // Очистка выражения от пробелов и табуляций
  for(int i = exp.size()-1; i >= 0; i--)
  {
    if (exp[i] == ' ' || exp[i] == '\t') exp.replace(i,1,"");
  }
  
  int br = 0;
  
  for(int i = 0; i < exp.size(); i++)
  {
    if ( exp[i] == '(' ) br++;
    if ( exp[i] == ')' ) br--;
    if (br < 0) { cout << "   Ошибка в скобках" << endl; return 0; }
  }
  if (br) { cout << "   Ошибка в скобках" << endl; return 0; }
  
  
  // Сложение и вычитание
  for(int i = exp.size()-1; i >= 0; i--)
  {
    if ( exp[i] == ')' ) br++;
    if ( exp[i] == '(' ) br--;
    if (br) continue;
    
    if ( exp[i] == '+' ) return solve(lop(exp,i)) + solve(rop(exp,i));    
    if ( exp[i] == '-' ) return solve(lop(exp,i)) - solve(rop(exp,i));
  }
  
  // Умножение и деление
  for(int i = exp.size()-1; i >= 0; i--)
  {
    if ( exp[i] == ')' ) br++;
    if ( exp[i] == '(' ) br--;
    if (br) continue;
    
    if ( exp[i] == '*' ) return solve(lop(exp,i)) * solve(rop(exp,i));
    if ( exp[i] == '/' ) return solve(lop(exp,i)) / solve(rop(exp,i));
  }
  
  // Возведение в степень
  for(int i = 0; i < exp.size(); i++)
  {
    if ( exp[i] == '(' ) br++;
    if ( exp[i] == ')' ) br--;
    if (br) continue;
    
    if ( exp[i] == '^' ) return pow( solve(lop(exp,i)), solve(rop(exp,i)) );
  }
  
  // Обработка выражения в скобках
  if ( exp[0] == '(' && exp[exp.size() - 1] == ')' )
  {
    exp.assign(exp, 1, exp.size() - 2);
    return solve(exp);
  }
  
  int order = 0; // степень десяти
  // Определить приставку СИ
  switch(exp[exp.size() - 1])
  {
     case 'p': order = -12; break;
     case 'n': order = -9; break;
     case 'u': order = -6; break;
     case 'm': order = -3; break;
     case 'k': order = 3; break;
     case 'M': order = 6; break;
     case 'G': order = 9; break;
  }
  if (order) exp.resize(exp.size() - 1);
  double num = 0.0;
  stringstream(exp) >> num;
  return num * pow(10, order);
}


// Возвращает строку формата: <число><пробел>[приставка СИ]
string output(double value, bool SI_prefix = true)
{
  int order = 0;
  
  while (value >= 1000)
  {
    if (order == 9) break;
    value /= 1000;
    order += 3;
  }
  
  while (value < 1)
  {
    if (order == -12) break;
    value *= 1000;
    order -= 3;
  }
  
  stringstream ss;
  ss << value << " ";
  switch(order)
  {
    case -12: ss << "п";  break;
    case  -9: ss << "н";  break;
    case  -6: ss << "мк"; break;
    case  -3: ss << "м";  break;
    case   3: ss << "к";  break;
    case   6: ss << "М";  break;
    case   9: ss << "Г";  break;
  }
  
  return ss.str();
}

string base_input(string promt)
{
  cout << promt;
  char in[256];
  cin.getline(in, 256);
  return in;
}

void input(string promt, string &result)
{
  result = base_input(promt);
}

double input(string promt)
{
  return solve( base_input(promt) );
}

void input(string promt, int & result)
{
  stringstream( base_input(promt) ) >> result;
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
          i = input(" Ток, А: ");
          if (i)    if (++n == 2) break;
        }
      
      if (u == 0)
        {
          u = input(" Напряжение, В: ");
          if (u)    if (++n == 2) break;
        }
      
      if (r == 0)
        {
          r = input(" Сопротивление, Ом: ");
          if (r) if (++n == 2) break;
        }
      
      if (p == 0)
        {
          p = input(" Мощность, Вт: ");
          if (p)      if (++n == 2) break;
        }
    }
  
  if (p && i) { u = p/i;       r = p / (i*i); }
  if (p && r) { i = sqrt(p/r); u = sqrt(p*r); }
  if (p && u) { i = p/u;       r = u*u/p;     }
  if (i && u) { p = i*u;       r = u/i;       }
  if (i && r) { u = i*r;       p = i*i*r;     }
  if (u && r) { i = u/r;       p = u*u/r;     }
  
  cout << "  Ток = "            << output(i) << "А"  << endl;
  cout << "  Напряжение = "     << output(u) << "В"  << endl;
  cout << "  Сопротивление = "  << output(r) << "Ом" << endl;
  cout << "  Мощность = "       << output(p) << "Вт" << endl;
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
          i = input(" Ток, А: ");
          if (i)    if (++n == 3) break;
        }
      
      if (ud == 0)
        {
          ud = input(" Диодное напряжение, В: ");
          if (ud)    if (++n == 3) break;
        }
      
      if (e == 0)
        {
          e = input(" Напряжение питания, В: ");
          if (e)      if (++n == 3) break;
        }
    }
    
    ur = e - ud;
    r = ur / i;
    p = i*ur;
    eff = ur / e;
    
    cout << "  Ток = "                   << output(i)  << "А"  << endl;
    cout << "  Диодное напряжение = "    << output(ud) << "В"  << endl;
    cout << "  Напряжение питания = "    << output(e)  << "В"  << endl;
    cout << "  Характеристики резистора:"                       << endl;
    cout << "    Сопротивление = "       << output(r)  << "Ом" << endl;
    cout << "    Мощность = "            << output(p)  << "Вт" << endl;
    cout << "    Напряжение = "          << output(ur) << "В"  << endl;
    cout << "    КПД = "                 << eff*100    << " %" << endl;
}

// Ряды номиналов рядиодеталей 
const int E6[]   = { 10, 15, 22, 33, 47, 68, 100 };
const int E12[]  = { 10, 12, 15, 18, 22, 27, 33, 39, 47, 56, 68, 82, 100 };
const int E24[]  = { 10, 11, 12, 13, 15, 16, 18, 20, 22, 24, 27, 30, 33, 36, 39, 43, 47, 51, 56, 62, 68, 75, 82, 91, 100 };
const int E48[]  = {100, 105, 110, 115, 121, 127, 133, 140, 147, 154, 162, 169, 178, 187, 196, 205, 215, 226, 237, 249, 261, 274, 287, 301, 316, 332, 348, 365, 383, 402, 422, 442, 464, 487, 511, 536, 562, 590, 619, 649, 681, 715, 750, 787, 825, 866, 909, 953, 1000 };
const int E96[]  = {100, 102, 105, 107, 110, 113, 115, 118, 121, 124, 127, 130, 133, 137, 140, 143, 147, 150, 154, 158, 162, 165, 169, 174, 178, 182, 187, 191, 196, 200, 205, 210, 215, 221, 226, 232, 237, 243, 249, 255, 261, 267, 274, 280, 287, 294, 301, 309, 316, 324, 332, 340, 348, 357, 365, 374, 383, 392, 402, 412, 422, 432, 442, 453, 464, 475, 487, 499, 511, 523, 536, 549, 562, 576, 590, 604, 619, 634, 649, 665, 681, 698, 715, 732, 750, 768, 787, 806, 825, 845, 866, 887, 866, 887, 909, 931, 953, 976, 1000 };
const int E192[] = {100, 101, 102, 104, 105, 106, 107, 109, 110, 111, 113, 114, 115, 117, 118, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135, 137, 138, 140, 142, 143, 145, 147, 149, 150, 152, 154, 156, 158, 160, 162, 164, 165, 167, 169, 172, 174, 176, 178, 180, 182, 184, 187, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213, 215, 218, 221, 223, 226, 229, 232, 234, 237, 240, 243, 246, 249, 252, 255, 258, 261, 264, 267, 271, 274, 277, 280, 284, 287, 291, 294, 298, 301, 305, 309, 312, 316, 320, 324, 328, 332, 336, 340, 344, 348, 352, 357, 361, 365, 370, 374, 379, 383, 388, 392, 397, 402, 407, 412, 417, 422, 427, 432, 437, 442, 448, 453, 459, 464, 470, 475, 481, 487, 493, 499, 505, 511, 517, 523, 530, 536, 542, 549, 556, 562, 569, 576, 583, 590, 597, 604, 612, 619, 626, 634, 642, 649, 657, 665, 673, 681, 690, 698, 706, 715, 723, 732, 741, 750, 759, 768, 777, 787, 796, 806, 816, 825, 835, 845, 856, 866, 876, 887, 898, 909, 919, 931, 942, 953, 965, 976, 988, 1000 };

void nom()
{
  int e = 0; // Кол-во элементво в ряду (6, 12, 24, 48, 96 или 192)
  double v = 0;
  
  int n = 0;
  // цикл прервется, когда будет введено достаточное количесво параметров
  while(1)
    {
      if (e == 0)
        {
          input(" Ряд: E", e);
          if (e != 6 && e != 12 && e != 24 && e != 48 && e != 96 && e != 192)
            { e = 0; continue; }
        }
      
      if (v == 0)
        {
          v = input(" Значение: ");
          if (v) break;
        }
    }
  
  // Выделение мантисы
  int exp = 0;
  while(v < 1) { v *= 10; exp--; }
  while(v > 10) { v /= 10; exp++; }

  // Приведение мантисы к порядку, соответсвующему значениям ряда
  if (e > 24) { v *= 100; exp -= 2;}
  else { v *= 10; exp -= 1;}
 
  const int * r;
  switch (e)
  {
    case 6   : r = E6;   break;
    case 12  : r = E12;  break;
    case 24  : r = E24;  break;
    case 48  : r = E48;  break;
    case 96  : r = E96;  break;
    case 192 : r = E192; break;
  }
    
  for(int i = 0; i <= e; i++)
  {
    if (r[i] < v) continue;
    
    double nom = r[i] * pow(10, exp);
    cout << "  Номинал = "<< output(nom) << "Ед." << endl;
    
    if (r[i] == v) break;
    
    nom = r[i-1] * pow(10, exp);
    cout << "  Номинал = "<< output(nom) << "Ед." << endl;
    
    break;
  }
}

void div()
{
  double i = 0;   // ток в делителе
  double ru = 0;  // верхнее сопротивление
  double rd = 0;  // нижнее сопротивление
  double uo = 0;  // входное напряжение
  double ui = 0;  // выходное напряжение
  
  int n = 0;
  // цикл прервется, когда будет введено достаточное количесво параметров
  while(1)
    {
      if (ui == 0)
        {
          ui = input(" Входное напряжение, В: ");
          if (ui)    if (++n == 3) break;
        }
      
      if (uo == 0)
        {
          uo = input(" Выходное напряжение, В: ");
          if (uo)    if (++n == 3) break;
        }
      
      if (ru == 0)
        {
          ru = input(" Cопротивление у входа, Ом: ");
          if (ru)      if (++n == 3) break;
        }
        
      if (rd == 0)
        {
          rd = input(" Сопротивление у земли , Ом: ");
          if (rd)      if (++n == 3) break;
        }
    }
    
  if (ui && rd && ru) uo = ui*rd/(rd+ru);
  if (uo && rd && ru) ui = uo * (rd + ru) / rd;
  if (uo && ui && rd) ru = rd * (ui / uo - 1);
  if (uo && ui && ru) rd = ru / (ui / uo - 1);
      
  i = uo/rd;
  
  cout << "  Входное напряжение = "    << output(ui) << "В"  << endl;
  cout << "  Выходное напряжение = "   << output(uo) << "В"  << endl;
  cout << "  Cопротивление у входа = " << output(ru) << "Ом" << endl;
  cout << "  Сопротивление у земли = "  << output(rd) << "Ом" << endl;
  cout << "  Ток в делителе = "        << output(i)  << "А"  << endl;
}

const int NNC_NN_to_Mantis[96] = {100, 102, 105, 107, 110, 113, 115, 118, 121, 124, 127, 130, 133, 137, 140, 143, 147, 150, 154, 158, 162, 165, 169, 174, 178, 182, 187, 191, 196, 200, 205, 210, 215, 221, 226, 232, 237, 243, 249, 255, 261, 267, 274, 280, 287, 294, 301, 309, 316, 324, 332, 340, 348, 357, 365, 374, 383, 392, 402, 412, 422, 432, 442, 453, 464, 475, 487, 499, 511, 523, 536, 549, 562, 576, 590, 604, 619, 634, 649, 665, 681, 698, 715, 732, 750, 768, 787, 806, 825, 845, 866, 887, 909, 931, 953, 976};
     
const int CNN_NN_to_Mantis[60] = {100, 110, 120, 130, 150, 160, 180, 200, 220, 240, 270, 300, 330, 360, 390, 430, 470, 510, 560, 620, 680, 750, 820, 910, 100, 110, 120, 130, 150, 160, 180, 200, 220, 240, 270, 300, 330, 360, 390, 430, 470, 510, 560, 620, 680, 750, 820, 910, 100, 120, 150, 180, 220, 270, 330, 390, 470, 560, 680, 820};
     
void chip()
{
  double r = 0.0;
  
  while(1)
  {
    string mark;
    input(" Маркировка: ", mark);

    int num = 0;
    
    if (mark == "cancel") return;
    
    if ( mark.length() == 4 ) // 4х символьная маркировка
    {
      stringstream ss_mark(mark);
      ss_mark >> num;
      if ( !ss_mark.fail() ) 
      {
        for(int i = 0; i < 192; i++)
          if ( num/10 == E192[i] )
          {
            r = (num/10) * pow(10, (num%10));
            break;
          }
        if (r == 0) cout << "  Несуществующий номинал: " << num/10 << endl;
      }
    }
    else if ( mark.length() == 3 ) // 3х символьная маркировка
    {
      int c_pos = 0;
      
      while(1)
      {
        switch ( mark[c_pos] )
        {
          case 'S': r = 0.01;   break;
          case 'X': r = 0.1;    break;
          case 'R': r = 0.1;    break;
          case 'A': r = 1;      break;
          case 'B': r = 10;     break;
          case 'C': r = 100;    break;
          case 'D': r = 1000;   break;
          case 'E': r = 10000;  break;
          case 'F': r = 100000; break;
        }
        if (r != 0 || c_pos == 2) break;
        c_pos = 2;
      }
      
      if (r != 0)
      {
        mark.erase(c_pos, 1);
        int nom_cod;
        stringstream ss_mark(mark);
        ss_mark >> nom_cod;
        if ( !ss_mark.fail() )
        {
          int nom = 0;
          if (c_pos == 0 && nom_cod >= 1 && nom_cod <= 60)
            nom = CNN_NN_to_Mantis[nom_cod-1];
          if (c_pos == 2 && nom_cod >= 1 && nom_cod <= 96)
            nom = NNC_NN_to_Mantis[nom_cod-1];
          r *= nom;
        }
      }
    }
    
    if (r != 0) break;
    
    cout << "  Маркировка не распознана" << endl;
  }
  
  cout << "  Сопротивление = " << output(r) << "Ом" << endl;
}

void nomlst()
{
  int e = 0; // Кол-во элементво в ряду (6, 12, 24, 48, 96 или 192)
  
  // цикл прервется, когда будет введено достаточное количесво параметров
  while(1)
  {
    input(" Ряд: E", e);
    if (e == 6 || e == 12 || e == 24 || e == 48 || e == 96 || e == 192) break;
  }
   
  const int * r;
  
  switch (e)
  {
    case 6   : r = E6;   break;
    case 12  : r = E12;  break;
    case 24  : r = E24;  break;
    case 48  : r = E48;  break;
    case 96  : r = E96;  break;
    case 192 : r = E192; break;
  }
  
  int col_count = sqrt(e);
  int row_count = e/col_count;
  cout << " =========" << endl;
  for (int row = 0; row < row_count; row++)
  {
    cout << "  ";
    for (int col = 0; col < col_count; col++)
    {
      cout << r[row + col*row_count] << "  ";
    }
    cout << endl;
  }
  
  cout << endl;
}

void db()
{
  double db_P = 0;
  double db_U_or_I = 0;
  double times_P = 0;
  double times_U_or_I = 0;
  
  int n;
  while(1)
  {
    db_U_or_I = input(" Отношение напряжения или тока, дБ: ");
    if (db_U_or_I)
    {
      times_U_or_I = pow(10, db_U_or_I/20);
      break;
    }

    times_U_or_I = input(" Отношение напряжений или токов, разы: ");
    if (times_U_or_I)
    {
      db_U_or_I = 20 * log10(times_U_or_I);
      break;
    }

    db_P = input(" Отношение мощностей, дБ: ");
    if (db_P)
    {
      times_P = pow(10, db_P/10);
      break;
    }

    times_P = input(" Отношение мощностей, разы: ");
    if (times_P)
    {
      db_P = 10 * log10(times_P);
      break;
    }
    
    break;
  }
  
  if (times_U_or_I)
  {
    cout << "  Отношение напряжений или токов = "<< db_U_or_I << " дБ" << endl;
    cout << "  Отношение напряжений или токов = "<< times_U_or_I << endl;
    return;
  }
  
  if (times_P)
  {
    cout << "  Отношение мощностей = "<< db_P << " дБ" << endl;
    cout << "  Отношение мощностей = "<< times_P << endl;
    return;
  }
}

void big_help()
{
  cout << "Описание команд:" << endl << endl;
  
  cout <<"  ohm - Закон Ома. Расчет двух из четырех параметров" << endl;
  cout <<"        (напряжение, ток, мощность, сопротивление)," << endl;
  cout <<"        когда заданы два другие." << endl << endl;
  
  cout <<"  nom - Расчет ближайщих номиналов для заданного значения.\n\n";
  
  cout <<"  nomlst - Выводит список номиналов для заданого ряда:\n";
  cout <<"           E6, E12, E24, E48, E96, E192\n\n";
  
  cout <<"  div - Расчет параметров делителя.\n\n";
  
  cout <<"  diode - Расчет параметров резистора в цепи диода." << endl << endl;
  
  cout <<"  chip - Расшифровка маркировки чип-резистора" << endl << endl;
  
  cout <<"  db - Перевод значений из децибелов в разы и наоборот";
  cout << endl << endl;

  cout <<"  res - Расчет параллельного соединения резисторов" << endl << endl;
  
  cout <<"  solve - Расчет математического выражения." << endl << endl;
  
  cout <<"  quit или exit - Выход из программы." << endl << endl;
  
  cout <<"  help - Справка. Описание команд." << endl << endl;
  
  cout <<"  h - Справка. Список команд без описания." << endl << endl;
}

void mini_help()
{
  cout << "Список команд:" << endl;
  cout << "ohm, diode, nom, nomlst, div, chip, solve, db, res, quit, exit, help, h";
  cout << endl;
}

void test_solve()
{
  cout << "  Ответ: " << input(" Выражение: ") << endl;
}

void res()
{
  int r_count = 0;  
  input(" Количество параллельных сопротивлений: ", r_count);
  double Y = 0;
  stringstream out_ss;
  for (int r = 0; r < r_count; r++)
  {
    stringstream ss;
    ss << " Сопротивление " << r + 1 << ", Ом: ";
    string str = ss.str();
    double R = input(str);
    out_ss << R;
    if (r < r_count - 1) out_ss << " || ";
    Y += 1/R;
  }
  cout << "  " << out_ss.str() << " = " << output(1/Y) << "Ом" << endl;
}

int main(int argv, char **argc)
{
  while(1)
    {
      string func;
      input("Команда: ", func);
      if (func == "res")   { res();       continue; }
      if (func == "db")    { db();        continue; }
      if (func == "solve") { test_solve();continue; }
      if (func == "ohm")   { ohm();       continue; }
      if (func == "diode") { diode();     continue; }
      if (func == "nom")   { nom();       continue; }
      if (func == "nomlst"){ nomlst();    continue; }
      if (func == "div")   { div();       continue; }
      if (func == "chip")  { chip();      continue; }
      if (func == "help")  { big_help();  continue; }
      if (func == "h")     { mini_help(); continue; }
      if (func == "quit")  break;
      if (func == "exit")  break;
      
      /* Русский вариант команд */
      if (func == "ом")       { ohm();       continue; }
      if (func == "диод")     { diode();     continue; }
      if (func == "номинал")  { nom();       continue; }
      if (func == "делитель") { div();       continue; }
      if (func == "справка")  { big_help();  continue; }
      if (func == "функции")  { mini_help(); continue; }
      if (func == "выход")    break;
    }
  return 0;
}

