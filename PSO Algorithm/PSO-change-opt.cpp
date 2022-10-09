#include <iostream>
#include <string.h>
#include <typeinfo>
#include <iomanip>
#include <cstdlib>
#include <cfloat>
#include <cmath>
#include <fstream>

#define n 5
#define Dim 1
double c1 = 0.4; // c1調這邊
double c2 = 1.9; // c2調這邊
#define min -100
#define max 100
#define loop 500
#define test 50
#define rand_seed 114
double w_init = 0.07;  // w開始的值調這邊
double w_final = 0.07; // w結束的值調這邊

using namespace std;

double x[n][Dim] = {0.0};
double v[n][Dim] = {0.0};
double fit[n] = {0.0}; // save fitness

int now = 0; // 目前代數

struct Pbest
{
	double pos[Dim];
	double val;
} Pbest[n];

double Gbest = DBL_MAX, Gbest_pos[Dim];

double rounding(double num, int index)
{
	bool isNegative = false; // whether is negative number or not

	if (num < 0) // if this number is negative, then convert to positive number
	{
		isNegative = true;
		num = -num;
	}

	if (index >= 0)
	{
		int multiplier;
		multiplier = pow(10, index);
		num = (int)(num * multiplier + 0.5) / (multiplier * 1.0);
	}

	if (isNegative) // if this number is negative, then convert to negative number
	{
		num = -num;
	}

	return num;
}

void init()
{

	for (int i = 0; i < n; i++)
	{

		/* initialize xid and vid */
		for (int j = 0; j < Dim; j++)
		{
			x[i][j] = min + (double)rand() * (max - min) / (double)RAND_MAX;
			v[i][j] = -200 + (double)rand() * (400) / (double)RAND_MAX;
		}

		/* initialize Pbest[n] */
		Pbest[i].val = DBL_MAX;
	}
}

void fitness()
{
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			fit[i] = fabs(x[i][j]); // f(x) = |x|
		}
	}
}

void update()
{
	/* update Pbest */
	for (int i = 0; i < n; i++)
	{
		if (fit[i] < Pbest[i].val)
		{
			Pbest[i].val = fit[i]; // update value

			/* update loc */
			for (int j = 0; j < Dim; j++)
			{
				Pbest[i].pos[j] = x[i][j];
			}
		}
	}

	/* update Gbest */
	for (int i = 0; i < n; i++)
	{
		if (Pbest[i].val < Gbest)
		{
			Gbest = Pbest[i].val; // update value

			/* update loc */
			for (int j = 0; j < Dim; j++)
			{
				Gbest_pos[j] = Pbest[i].pos[j];
			}
		}
	}
}

void move()
{
	//	double w = w_init - ((w_init-w_final)/(double)(loop-1)*(double)now);
	double w = w_init;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < Dim; j++)
		{
			v[i][j] = w * v[i][j] + c1 * (double)rand() / RAND_MAX * (Pbest[i].pos[j] - x[i][j]) // update velocity
					  + c2 * (double)rand() / RAND_MAX * (Gbest_pos[j] - x[i][j]);

			x[i][j] += v[i][j]; // update loc

			/* avoid overflow →會超過邊界就移動邊界內 */
			if (x[i][j] > max)
			{
				x[i][j] = min + (double)rand() * (max - min) / (double)RAND_MAX;
			}
			else if (x[i][j] < min)
			{
				x[i][j] = min + (double)rand() * (max - min) / (double)RAND_MAX;
			}
			fitness();
			update();
		}
	}
}

int main()
{
	std::ofstream fp;
	fp.open("數據v(-200~200).csv");
	fp << "w,c1,c2,avg_ans" << endl;
	/* for 校正 */
	char s[256];
	double tmp = 0.0;

	double avg_ans = 0.0;

	//for (w_init = 1.0, w_final = 1.0; w_init >= 0; w_init -= 0.01, w_final -= 0.01) // 調整w
	//{
	for (c1 = 2.0; c1 >= 0; c1 -= 0.001) // 調整c1
	{
		//for (c2 = 2.0; c2 >= 0; c2 -= 0.1) // 調整c2
		//{
		fp << w_init << "," << c1 << "," << c2 << ",";
		avg_ans = 0.0;

		/* ↓校正數據 ↓*/
		w_init = rounding(0.065, 5);
		c1 = rounding(c1, 5);
		c2 = rounding(1.863, 5);
		/* ↑校正數據 ↑*/



		srand(rand_seed);
		for (int i = 0; i < test; i++) // 實驗
		{
			Gbest = DBL_MAX;
			init();
			for (now = 0; now < loop; now++)
			{
				fitness();
				update();
				move();
			}
			avg_ans += Gbest;
		}
		fp << avg_ans / (double)50 << endl;
		//}
	}
	//}

	fp.close();

	return 0;
}
