#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#define stock 29
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
	input_file.open("20210501-20210531.csv", ios::in); //讀入的檔名
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
					first_row = false;
				}
				col++;
			}
			else
			{
				data[col].price[row] = stod(tmp);
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

void PrintAns()
{
	cout << endl;
	for (int i = 0; i < T; i++)
	{
		cout << "T " << i + 1 << ": "
			 << fixed << setprecision(10)
			 << total[i] << endl;
	}
}

void hw1()
{
	// capital level of AAPL

	string target = "AAPL";
	cout << "-----------------⇣ hw1 ⇣-----------------" << endl;

	CalcLevel(FindIndex(target), target, initial);
	PrintAns();

	cout << "-----------------⇡ hw1 ⇡-----------------" << endl
		 << endl;
}

void hw2()
{
	string target0 = "AAPL";
	string target1 = "MCD";
	string target2 = "MSFT";
	double tmp[T];
	tmp[0] = initial;

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

	PrintAns();

	cout << "-----------------⇡ hw2 ⇡-----------------";
}

int main()
{
	read();

	hw1();
	hw2();

	return 0;
}
