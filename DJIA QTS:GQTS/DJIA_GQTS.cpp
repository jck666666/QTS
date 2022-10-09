#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <stdlib.h>

#define T 20
#define initial 10000000

#define ITEM_NUM 30
#define POPULATION 10
#define ITERATION 10000
#define DELTA 0.0004
#define EXPERIMENT 1

using namespace std;
ifstream input_file;

int BestGen = 0;
int BestAnsTimes = 0;
int StockNum = 0;

typedef struct Data
{
    string name;
    double price[T];
} Data;
Data data[ITEM_NUM];

typedef struct Chromo
{
    int gene[ITEM_NUM]; //items for choose
    double trend;       //trend value of knapsack
    double slope;
    double level; //capital level
    double risk;
} Chromo;
Chromo chrom[POPULATION]; //population
Chromo opt_sol;

typedef struct Ans
{
    string StockName;
    int Number;
    int DistribueFunds;
    double RemainFunds;
    double fs[T];
} Ans;
Ans ans[ITEM_NUM]; //detail of answer

int opt_index;
double total[T];            //capital level
double Q[ITEM_NUM] = {0.0}; //propobility array

//

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

                if (col == ITEM_NUM - 1)
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

void Output(char* filename)
{
    ofstream output_file;

    output_file.open(filename);

    output_file << "Iteration"
                << ","
                << ITERATION
                << endl;

    output_file << "Element number"
                << ","
                << POPULATION
                << endl;

    output_file << "Delta"
                << ","
                << DELTA
                << endl;

    output_file << "Exp times"
                << ","
                << EXPERIMENT
                << endl
                << endl;

    output_file << "Init funds"
                << ","
                << initial
                << endl;

    output_file << "Final funds"
                << ","
                << fixed << setprecision(10) << opt_sol.level
                << endl;

    output_file << "Real award"
                << ","
                << opt_sol.level - initial
                << endl
                << endl;

    output_file << "m"
                << ","
                << opt_sol.slope
                << endl;

    output_file << "Daily_risk"
                << ","
                << opt_sol.risk
                << endl;

    output_file << "Trend"
                << ","
                << opt_sol.trend
                << endl
                << endl;

    output_file << "Best generation"
                << ","
                << BestGen
                << endl;

    output_file << "Best experiment"
                << ","
                << 1
                << endl;

    output_file << "Best answer times"
                << ","
                << BestAnsTimes
                << endl
                << endl;

    output_file << "Stock number"
                << ","
                << StockNum
                << endl;

    output_file << "Stock#"
                << ",";
    for (int i = 0; i < StockNum; i++)
    {
        output_file << ans[i].StockName << ",";
    }
    output_file << endl;

    output_file << "Number"
                << ",";
    for (int i = 0; i < StockNum; i++)
    {
        output_file << ans[i].Number << ",";
    }
    output_file << endl;

    output_file << "Distribue funds"
                << ",";
    for (int i = 0; i < StockNum; i++)
    {
        output_file << ans[i].DistribueFunds << ",";
    }
    output_file << endl;

    output_file << "Remain funds"
                << ",";
    for (int i = 0; i < StockNum; i++)
    {
        output_file << ans[i].RemainFunds << ",";
    }
    output_file << endl;

    for (int i = 0; i < T; i++)
    {
        double acc = 0;
        output_file << "fs(" << i + 1 << ")"
                    << ",";
        for (int j = 0; j < StockNum; j++)
        {
            output_file << ans[j].fs[i] << ",";
            acc += ans[j].fs[i];
        }

        output_file << acc << endl;
    }

    output_file.close();
}
//

void CalcLevel(int index, double money)
{
    total[0] = money;
    //cout << "⇣ capital level ⇣" << endl;
    //cout << target << " T1"<< ": " << fixed << setprecision(10) << total[0] << endl;
    int stockNum = money / data[index].price[0];

    for (int i = 1; i < T; i++)
    {
        double left = total[i - 1] - (stockNum * data[index].price[i - 1]);

        total[i] = (stockNum * data[index].price[i]) + left;

        //cout << target << " T" << i + 1 << ": " << total[i] << endl;
    }

    //cout << "⇡ capital level ⇡" << endl<< endl;
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

double CalcRisk()
{
    double numerator = 0, denominator = 0;
    for (int i = 0; i < T; i++)
    {
        double yi = total[i];                          //yi：第i天真實資金水位
        double Yi = (CalcSlpoe() * (i + 1) + initial); //Yi：第i天預期資金水位

        numerator += pow((yi - Yi), 2);
    }
    denominator = T;

    return sqrt(numerator / denominator);
}

double CalcTrend()
{
    return CalcSlpoe() / CalcRisk();
}

//

void init()
{
    for (int i = 0; i < ITEM_NUM; i++)
    {
        Q[i] = 0.5;
    }
}

void measure()
{
    for (int i = 0; i < POPULATION; i++)
    {
        for (int j = 0; j < ITEM_NUM; j++)
        {
            //float randNum = rand() % 10000 / (float)10000; //0~1
            double randNum = (double)rand() / RAND_MAX;

            // cout<<"randNum: "<<randNum<<endl;

            if (randNum > Q[j])
            {
                chrom[i].gene[j] = 0;
            }
            else
            {
                chrom[i].gene[j] = 1;
            }
        }
    }
}

void fitness()
{
    for (int i = 0; i < POPULATION; i++)
    {
        int value_ind = 0;
        chrom[i].trend = 0;
        int cnt = 0;

        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (chrom[i].gene[j] == 1)
            {
                cnt++; //資金分cnt等分
            }
        }

        double money = initial / cnt;
        double remain = initial - (initial / cnt) * cnt;
        double tmp[T] = {0};

        for (int j = 0; j < ITEM_NUM; j++)
        {
            if (chrom[i].gene[j] == 1)
            {
                CalcLevel(j, money);
                for (int i = 1; i < T; i++)
                {
                    tmp[i] += total[i];
                }
            }
        }

        total[0] = initial;
        for (int i = 1; i < T; i++)
        {
            total[i] = tmp[i] + remain;
        }

        chrom[i].trend = CalcTrend();
        chrom[i].slope = CalcSlpoe();
        chrom[i].level = total[T - 1];
        chrom[i].risk = CalcRisk();
    }
}

void renew(int gen) //run every generation
{
    double worst = INT_MAX; //同一世代的整個族群中最差
    int worst_index = 0;
    bool renew = false;

    for (int i = 0; i < POPULATION; i++)
    {
        //find global best
        if (chrom[i].trend > opt_sol.trend)
        {

            opt_sol = chrom[i];
            opt_index = i;

            BestGen = gen;
            BestAnsTimes = 1;
            renew = true;
        }

        //find local worst
        if (chrom[i].trend < worst)
        {
            worst = chrom[i].trend;
            worst_index = i;
        }

        //count  BestAnsTimes
        if (renew == true)
        {
            BestAnsTimes++;
        }
    }

    //renew the propability in Q[]
    for (int i = 0; i < ITEM_NUM; i++)
    {
        if (opt_sol.gene[i] == 1 && chrom[worst_index].gene[i] == 0)
        {
            Q[i] += DELTA;
        }
        else if (opt_sol.gene[i] == 0 && chrom[worst_index].gene[i] == 1)
        {
            Q[i] -= DELTA;
        }
        else //opt_sol.gene[i] == chrom[worst_index].gene[i]
        {
            //probability will not renew
        }
    }
}

//

int main()
{
    srand(114);
    // srand(time(NULL));

    read();

    //QTS
    init();

    for (int i = 0; i < ITERATION; i++)
    {
        measure();
        fitness();
        renew(i);
    }
    cout << "opt: " << opt_sol.trend << endl
         << endl;

    //

    for (int j = 0; j < ITEM_NUM; j++)
    {
        if (opt_sol.gene[j] == 1)
        {
            StockNum++; //計算最佳解中有幾檔股票
        }
    }

    int k = 0; //0~StockNum
    for (int j = 0; j < ITEM_NUM; j++)
    {
        if (opt_sol.gene[j] == 1 && k < StockNum)
        {
            //Stock#
            ans[k].StockName = data[j].name;

            //Number
            ans[k].Number = (initial / StockNum) / data[j].price[0];

            //Distribue funds
            ans[k].DistribueFunds = initial / StockNum;

            //Remain funds
            ans[k].RemainFunds = (initial / StockNum) - (ans[k].Number * data[j].price[0]);

            //fs
            double money = initial / StockNum;
            double remain = initial - (money)*StockNum;

            ans[k].fs[0] = initial / StockNum; //day1
            for (int t = 1; t < T; t++)        //day2~day20
            {
                CalcLevel(j, money);
                ans[k].fs[t] = total[t];
            }

            k++;
        }
    }

    Output("DJIA GQTS.csv");

    return 0;
}
