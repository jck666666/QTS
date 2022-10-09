#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#define stock 30
#define T 20
#define initial 10000000

using namespace std;
ifstream input_file;

typedef struct Data
{
	string name;
	double price[T];
} Data;

Data data[stock];
double total[T];

void read()
{
	input_file.open("train_2019_06(2019 Q1).csv", ios::in); //讀入的檔名
	string line;

	int row = -1;
	bool first_row = true;
	while (getline(input_file, line)) //從input_file讀入，暫存在line
	{
		string tmp;
		stringstream ss(line); //字串處理

		int col = 0;
		while (getline(ss, tmp, ',')) //csv檔是逗號分隔檔，所以透過逗號來讀取檔案裡面的資料
		{

			if (first_row)
			{
				data[col].name = tmp;
				//cout << data[col].name[col] ;

				if (col == stock - 1)
				{
					first_row = false; //已經讀到最後一個股票名稱
				}
				col++;
			}
			else
			{
				data[col].price[row] = atof(tmp.c_str());
				//cout <<data[col].price[row] ;

				col++;
			}

			//cout<<",";
		}
		//cout<< endl;
		row++;
	}
	input_file.close(); //開檔要記得關檔
}

int FindIndex(string target) //find index of the target
{
	int index;
	for (int i = 0; i < stock; i++)
	{
		if (target.compare(data[i].name) == 0)
		{
			index = i;
			break;
		}
	}
	return index;
}

void CalcLevel(int index, string target, double money)
{
	total[0] = money;
	cout << "⇣ capital level ⇣" << endl;
	cout << target << " T1"
		 << ": " << fixed << setprecision(10) << total[0] << endl;
	int stockNum = money / data[index].price[0];

	for (int i = 1; i < T; i++)
	{
		double left = total[i - 1] - (stockNum * data[index].price[i - 1]);

		total[i] = (stockNum * data[index].price[i]) + left;

		cout << target << " T" << i + 1 << ": " << total[i] << endl;
	}

	cout << "⇡ capital level ⇡" << endl
		 << endl;
}

double CalcSlpoe()
{
	double numerator = 0, denominator = 0;

	for (int i = 1; i <= T; i++)
	{
		numerator += (i * (total[i - 1] - initial)); //total of T1 : total[0]
		denominator += pow(i, 2);
	}
	return numerator / denominator;
}

double expected()
{
	double Yd = CalcSlpoe() * T + initial;
	double Y1 = CalcSlpoe() * 1 + initial;
	return (Yd - Y1) / (T-1);
}

double CalcRisk()
{
	double numerator = 0, denominator = 0;
	for (int i = 0; i < T; i++)
	{
		double yi = total[i];						   //yi：第i天真實資金水位
		double Yi = (CalcSlpoe() * (i + 1) + initial); //Yi：第i天預期資金水位

		numerator += pow((yi - Yi), 2);
	}
	denominator = T;

	return sqrt(numerator / denominator);
}

void PrintAns(string title)
{

	cout << endl
		 << title << " expected : " << expected() << endl;

	cout << title << " Slpoe: " << CalcSlpoe() << endl;

	cout << title << " Risk: " << CalcRisk() << endl;

	cout << title << " trend_value"
		 << " : " << CalcSlpoe() / CalcRisk() << endl
		 << endl;
}

void hw1()
{
	// capital level of MCD

	string target = "MCD";
	cout << "-----------------⇣ hw1 ⇣-----------------" << endl;

	CalcLevel(FindIndex(target), target, initial);
	PrintAns("hw1");

	cout << "-----------------⇡ hw1 ⇡-----------------" << endl
		 << endl;
}

void hw2()
{
	string target0 = "INTC";
	string target1 = "MCD";
	string target2 = "NKE";
	double ary0[T], ary1[T], ary2[T];
	double tmp[T];
	tmp[0]=initial;

	cout << endl
		 << "-----------------⇣ hw2 ⇣-----------------" << endl;

	//target 0
	CalcLevel(FindIndex(target0), target0, initial / 3);
	for (int i = 1; i < T; i++)
	{
		tmp[i] += total[i];
	}

	//target 1
	CalcLevel(FindIndex(target1), target1, initial / 3);
	for (int i = 1; i < T; i++)
	{
		tmp[i] += total[i];
	}

	//target 2
	CalcLevel(FindIndex(target2), target2, initial / 3);
	for (int i = 1; i < T; i++)
	{
		tmp[i] += total[i];
	}

	//count total capital level
	double remain = initial - (initial / 3) * 3;
	cout << "total 1:" << initial << endl;
	total[0] = initial;
	for (int i = 1; i < T; i++)
	{
		total[i] = tmp[i] + remain;

		cout << "total " << i + 1 << ": " << total[i] << endl;
	}

	PrintAns("hw2");

	cout << "-----------------⇡ hw2 ⇡-----------------";
}

int main()
{
	read();

	hw1();
	hw2();

	return 0;
}
