#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;
ifstream input_file;

typedef struct Data
{
    string date;
    double price;
} Data;

Data data[42];
double value[30][21];

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
    for (int i = 1; i <= 20; i++)
    {
        output_file << ","
                    << "SMA" << i;
    }
    output_file << endl;

    //填入詳細內容
    for (int i = 1; i <= 30; i++)
    {
        if (i % 7 != 5 && i % 7 != 6 && i != 14) //trading day (0614端午節)
        {
            //30天 日期
            output_file << "6/" << i
                        << "/2021";

            //SMA1 ~ SMA20
            for (int j = 1; j <= 20; j++)
            {
                output_file << ","
                            << fixed << setprecision(10)
                            << value[i][j];
            }
            output_file << endl;
        }
    }

    output_file.close();
}

double GetPrice(int index)
{
    return data[index].price;
}

int FindIndex(string target)
{
    int index;
    for (int i = 0; i < 42; i++)
    {
        if (target.compare(data[i].date) == 0)
        {
            index = i;
            break;
        }
    }
    return index;
}

void Calc()
{
    // SMA1 ~ SMA20
    for (int i = 1; i <= 20; i++)
    {
        // 0601 ~ 0630
        for (int j = 1; j <= 30; j++)
        {
            if (j % 7 != 5 && j % 7 != 6 && j != 14) //trading day (0614 holiday)
            {
                string target = "6/" + to_string(j) + "/2021";
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

                value[j][i] = numerator / denominator;
            }
        }
    }
}

int main()
{
    //長榮 2603
    Read("2603TW.csv");
    Calc();
    Output("2603 SMA1-SMA20.csv");

    //陽明 2609
    Read("2609TW.csv");
    Calc();
    Output("2609 SMA1-SMA20.csv");

    //萬海 2615
    Read("2615TW.csv");
    Calc();
    Output("2615 SMA1-SMA20.csv");

    return 0;
}