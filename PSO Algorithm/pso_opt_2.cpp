#include <iostream>
#include <stdio.h>
#include <ctime>
#include <fstream>
#include <stdlib.h>
#include <cfloat>
#include <cmath>

#define Dim 1         //Dimension
#define NumParticle 5 //birds
#define ITERATION 500
#define EXPERIMENT 50
// #define C1 0.3
// #define C2 1.8
double C1;
double C2;
double W;

#define delta 0
/* W can be edited in function init() */

using namespace std;

// Variable Declaration
double lower[Dim], upper[Dim];    // Lower bound and upper bound for the dimension
double x[NumParticle][Dim];       //current position of particle
double v[NumParticle][Dim];       //current velocity of particle
double pbest_fit[NumParticle];    //individual optimal fitness
double pbest_x[NumParticle][Dim]; // individual optimal position
double gbest_fit;                 // social optimal fitness
double gbest_x[Dim];              // social optimal position
int FindMin;

double Random(int min, int max)
{
    return min + (double)rand() * (double)(max - min) / (double)RAND_MAX;
}

void init() //initialization
{
    //W = 0.1;
    lower[0] = -100;
    upper[0] = 100;
    FindMin = 0;

    //printf("{     x    ,      v    }\n");
    for (int i = 0; i < NumParticle; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            x[i][j] = pbest_x[i][j] = Random(-100, 100);
            v[i][j] = Random(-200, 200);
            //printf("{%lf , %lf}\n", x[i][j], v[i][j]);
        }
    }

    for (int i = 0; i < NumParticle; i++)
    {
        pbest_fit[i] = DBL_MAX;
    }
    gbest_fit = DBL_MAX;
    gbest_x[0] = DBL_MAX;
}

void fitness_renew(int gen)
{
    double fitness;
    for (int i = 0; i < NumParticle; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            //fitness calculate
            fitness = abs(x[i][j]);

            //renew pbest
            if (fitness < pbest_fit[i])
            {
                pbest_fit[i] = fitness;
                pbest_x[i][j] = x[i][j];
            }
        }
    }

    //renew gbest
    for (int i = 0; i < NumParticle; i++)
    {

        if (pbest_fit[i] < gbest_fit)
        {
            gbest_fit = pbest_fit[i];

            for (int j = 0; j < Dim; j++)
            {
                gbest_x[j] = pbest_x[i][j];
                FindMin = gen;
            }
        }
    }
}
void fitness_renew()
{
    double fitness;
    for (int i = 0; i < NumParticle; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            //fitness calculate
            fitness = abs(x[i][j]);

            //renew pbest
            if (fitness < pbest_fit[i])
            {
                pbest_fit[i] = fitness;
                pbest_x[i][j] = x[i][j];
            }
        }
    }

    //renew gbest
    for (int i = 0; i < NumParticle; i++)
    {

        if (pbest_fit[i] < gbest_fit)
        {
            gbest_fit = pbest_fit[i];

            for (int j = 0; j < Dim; j++)
            {
                gbest_x[j] = pbest_x[i][j];
            }
        }
    }
}

void renew_x_v()
{
    for (int i = 0; i < NumParticle; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            double r1 = Random(0, 1);
            double r2 = Random(0, 1);

            double tmp = v[i][j];

            fitness_renew();
            v[i][j] = (W * v[i][j]) + (C1 * r1 * (pbest_x[i][j] - x[i][j])) + (C2 * r2 * (gbest_x[j] - x[i][j]));

            //cout << v[i][j] << "=(" << W << "*" << tmp << ")+(" << C1 << "*" << r1 << "(" << pbest_x[i][j] << "-" << x[i][j] << "))+(" << C2 << "*" << r2 << "*(" << gbest_x[j] << "-" << x[i][j] << "))" << endl;

            x[i][j] += v[i][j];
            fitness_renew();
            if (x[i][j] < lower[j])
            {
                //cout << "lower!!" << x[i][j] << endl;
                x[i][j] = Random(-100, 100);
            }

            if (x[i][j] > upper[j])
            {
                //cout << "upper!!" << x[i][j] << endl;
                x[i][j] = Random(-100, 100);
            }
        }
    }
}
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

int main()
{

    //time(NULL)
    ofstream myfile;
    string filename = "pso_opt w c2固定.csv";
    myfile.open(filename);
    myfile << "W"
           << ","
           << "C1"
           << ","
           << "C2"
           << ","
           << "Average" << endl;

    int gen_sum;
    double opt_value_sum;

    //for (double wi = 1.0; wi >= 0.0; wi -= 0.01)
    //{

    for (double c1i = 2.0; c1i >= 0.0; c1i -= 0.001)
    {
        //for (double c2i = 2.0; c2i >= 0.0; c2i -= 0.1)
        //{

        /* correction */
        W = rounding(0.065, 5);
        C1 = rounding(c1i, 5);
        C2 = rounding(1.863, 5);
        /* correction */

        srand(114);
        for (int e = 0; e < EXPERIMENT; e++) //store the exactly generation that find the optimal value
        {
            //cout << "+++++++++++++(EXPERIMENT)+++++++++++++++" << endl;

            init();
            for (int i = 0; i < ITERATION; i++)
            {

                //fitness_renew(i);
                renew_x_v();
            }

            // gen_sum += FindMin;
            opt_value_sum += gbest_fit;
        }

        myfile << W << "," << C1 << "," << C2 << "," << (double)opt_value_sum / (double)EXPERIMENT << endl;

        //gen_sum = 0;
        opt_value_sum = 0;
        /* ⇡⇡⇡⇡⇡PSO algo⇡⇡⇡⇡⇡ */
        //}
    }
    // }
}