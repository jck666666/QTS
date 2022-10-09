#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include <stdlib.h>

#define INITIAL 1000000

#define ITEM_NUM 32 //0~7:B1 , 8~15:B2 , 16~23:S1 , 24~31:S2
#define POPULATION 10
#define ITERATION 1000
#define DELTA 0.0004
#define EXPERIMENT 1

using namespace std;
ifstream input_file;

typedef struct Data
{
    string date;
    double price;
} Data;
Data data[360];

typedef struct Chromo
{
    int gene[ITEM_NUM]; //items for choose
    int SMA_B1;
    int SMA_B2;
    int SMA_S1;
    int SMA_S2;
    double ROI; //fitness
} Chromo;
Chromo chrom[POPULATION]; //population
Chromo opt_sol;

int opt_index;
int BestGen;
int BestAnsTimes = 0;
double value[150][256];    //先假設150天
float Q[ITEM_NUM] = {0.0}; //propobility array of buy
int totalTday;             //0201~0630的交易天數

/* ⇣⇣⇣ calculate SMA1 ~ SMA256 ⇣⇣⇣ */
bool isTradingDay(string target)
{
    for (int i = 0; i < 360; i++)
    {
        if (target.compare(data[i].date) == 0) //找到了
        {
            return true;
        }
    }
    return false;
}

double GetPrice(int index)
{
    return data[index].price;
}

int FindIndex(string target)
{
    int index;
    for (int i = 0; i < 360; i++)
    {
        if (target.compare(data[i].date) == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

void Read(string filename)
{
    input_file.open(filename, ios::in); //讀入的檔名
    string line;
    int index = 0;
    getline(input_file, line); //讀掉第一行項目標題

    while (getline(input_file, line))
    {
        string tmp;
        stringstream ss(line); //字串處理

        //日期
        getline(ss, tmp, ',');
        data[index].date = tmp;

        //股價
        getline(ss, tmp, ',');
        data[index].price = atof(tmp.c_str());

        index++;
    }

    input_file.close(); //開檔要記得關檔
}

void Output(string filename)
{
    ofstream output_file;

    output_file.open(filename);

    output_file << "Date";

    //第一行項目
    for (int i = 1; i <= 256; i++)
    {
        output_file << ","
                    << "SMA" << i;
    }
    output_file << endl;

    //填入詳細內容,月初填到月底
    int tmp = 1;
    for (int m = 2; m <= 6; m++)
    {
        for (int j = 1; j <= 31; j++)
        {
            string target = to_string(m) + "/" + to_string(j) + "/2021";
            if (isTradingDay(target))
            {
                //日期
                output_file << target;

                //SMA1 ~ SMA256
                for (int k = 1; k <= 256; k++)
                {
                    output_file << ","
                                << fixed << setprecision(10)
                                << value[tmp][k];
                }
                output_file << endl;
                tmp++;
            }
        }
    }
    totalTday = tmp;

    output_file.close();
}

void CalcSMA()
{
    // SMA1 ~ SMA256
    for (int i = 1; i <= 256; i++)
    {
        int tmp = 1;

        // 0201 ~ 0630
        for (int m = 2; m <= 6; m++)
        {
            for (int j = 1; j <= 31; j++)
            {
                string target = to_string(m) + "/" + to_string(j) + "/2021";
                if (isTradingDay(target))
                {
                    int index = FindIndex(target);

                    int T = index;
                    int cnt = i;
                    double numerator = 0, denominator = i;

                    // from Today to Furthest_Day
                    while (cnt != 0)
                    {
                        numerator += GetPrice(T--);
                        cnt--;
                    }

                    value[tmp++][i] = numerator / denominator;
                }
            }
        }
    }
}

/* ⇡⇡⇡ calculate SMA1 ~ SMA256 ⇡⇡⇡ */

/* ⇣⇣⇣ QTS ⇣⇣⇣ */
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
            double randNum = (double)rand() / RAND_MAX; //0~1
            //Q[]
            if (randNum <= Q[j]) //????
            {
                chrom[i].gene[j] = 1;
            }
            else
            {
                chrom[i].gene[j] = 0;
            }
        }

        //B1 : 0~7
        chrom[i].SMA_B1 = 0;
        for (int j = 0; j <= 7; j++)
        {
            chrom[i].SMA_B1 += chrom[i].gene[j] * pow(2, 7 - j);
        }
        chrom[i].SMA_B1 += 1;

        //B2 : 8~15
        chrom[i].SMA_B2 = 0;
        for (int j = 8; j <= 15; j++)
        {
            chrom[i].SMA_B2 += chrom[i].gene[j] * pow(2, 15 - j);
        }
        chrom[i].SMA_B2 += 1;

        //S1 : 16~23
        chrom[i].SMA_S1 = 0;
        for (int j = 16; j <= 23; j++)
        {
            chrom[i].SMA_S1 += chrom[i].gene[j] * pow(2, 23 - j);
        }
        chrom[i].SMA_S1 += 1;

        //S2 : 24~31
        chrom[i].SMA_S2 = 0;
        for (int j = 24; j <= 31; j++)
        {
            chrom[i].SMA_S2 += chrom[i].gene[j] * pow(2, 31 - j);
        }
        chrom[i].SMA_S2 += 1;
    }
}

void fitness()
{
    for (int i = 0; i < POPULATION; i++)
    {
        int shareholding = 0;
        double leftFunds = INITIAL;
        for (int t = 2; t < totalTday; t++) //0202~0630
        {
            //Golden Cross
            if (value[t - 1][chrom[i].SMA_B1] < value[t - 1][chrom[i].SMA_B2] && value[t][chrom[i].SMA_B1] > value[t][chrom[i].SMA_B2])
            {
                shareholding += leftFunds / value[t][1]; //持有股數+=買進股數
                leftFunds -= shareholding * value[t][1];
            }

            //Death Cross
            if (value[t - 1][chrom[i].SMA_S1] > value[t - 1][chrom[i].SMA_S2] && value[t][chrom[i].SMA_S1] < value[t][chrom[i].SMA_S2])
            {
                leftFunds += shareholding * value[t][1];
                shareholding -= shareholding; //遇死亡交叉全部賣掉
            }
            //如果遇到黃金、死亡、黃金、死亡 這樣的順序
        }

        //如果沒有黃金交叉->維持原資金->獲利0%
        //如果沒有死亡交叉->最後一天6/30 ( price : value[totalTday][1] )賣掉
        if (shareholding != 0) //如果到最後還有股票沒賣（沒遇到死亡交叉）
        {
            leftFunds += shareholding * value[totalTday][1];
        }

        chrom[i].ROI = (leftFunds - INITIAL) / INITIAL * 100;
    }
}
/* ⇡⇡⇡ QTS ⇡⇡⇡ */

void renew(int gen) //run every generation
{
    double best = INT_MIN; //同一世代的整個族群中最佳
    int best_index = 0;

    double worst = INT_MAX; //同一世代的整個族群中最差
    int worst_index = 0;

    for (int i = 0; i < POPULATION; i++)
    {

        //find local best
        if (chrom[i].ROI > best)
        {
            best = chrom[i].ROI;
            best_index = i;
        }

        //find local worst
        if (chrom[i].ROI < worst)
        {
            worst = chrom[i].ROI;
            worst_index = i;
        }
    }

    //renew the propability in Q[]
    for (int i = 0; i < ITEM_NUM; i++)
    {
        if (chrom[best_index].gene[i] == 1 && chrom[worst_index].gene[i] == 0)
        {
            Q[i] += DELTA;
        }
        else if (chrom[best_index].gene[i] == 0 && chrom[worst_index].gene[i] == 1)
        {
            Q[i] -= DELTA;
        }
        else //chrom[best_index].gene[i] == chrom[worst_index].gene[i]
        {
            //probability will not renew
        }
    }

    //renew the optimal solution (global best) in this generation
    if (chrom[best_index].ROI > opt_sol.ROI)
    {
        opt_sol = chrom[best_index];
        opt_index = best_index;
        BestGen = gen;
        BestAnsTimes = 1;
    }
    else if (chrom[best_index].ROI == opt_sol.ROI)
    {
        BestAnsTimes++;
    }
}

int main()
{
    //srand(time(NULL));
    srand(114);

    /* ⇣⇣⇣ calculate SMA1 ~ SMA256 ⇣⇣⇣ */
    Read("2603.csv");
    CalcSMA();
    Output("2603 SMA1-SMA256.csv");
    /* ⇡⇡⇡ calculate SMA1 ~ SMA256 ⇡⇡⇡ */

    /* ⇣⇣⇣ QTS ⇣⇣⇣ */
    for (int i = 0; i < EXPERIMENT; i++) //store the exactly generation that find the optimal value
    {
        init();

        for (int j = 0; j < ITERATION; j++)
        {
            measure();
            fitness();
            renew(j);
        }
    }
    /* ⇡⇡⇡ QTS ⇡⇡⇡ */

    cout << "------ANSWER------" << endl
         << "SMA BUY1: " << opt_sol.SMA_B1 << endl
         << "SMA BUY2: " << opt_sol.SMA_B2 << endl
         << endl
         << "SMA SELL1: " << opt_sol.SMA_S1 << endl
         << "SMA SELL12: " << opt_sol.SMA_S2 << endl
         << endl
         << "ROI: " << opt_sol.ROI << " %" << endl
         << endl
         << "Best Generation: " << BestGen << endl
         << "Best Answer Times: " << BestAnsTimes << endl;

    return 0;
}