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
// #define C1 0.8
// #define C2 0.8
double C1;
double C2;

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
double W;

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

// void fitness_renew(int gen)
// {
//     double fitness;
//     for (int i = 0; i < NumParticle; i++)
//     {
//         for (int j = 0; j < Dim; j++)
//         {
//             //fitness calculate
//             fitness = abs(x[i][j]);

//             //renew pbest
//             if (fitness < pbest_fit[i])
//             {
//                 pbest_fit[i] = fitness;
//                 pbest_x[i][j] = x[i][j];
//             }
//         }
//     }

//     //renew gbest
//     for (int i = 0; i < NumParticle; i++)
//     {

//         if (pbest_fit[i] < gbest_fit)
//         {
//             gbest_fit = pbest_fit[i];

//             for (int j = 0; j < Dim; j++)
//             {
//                 gbest_x[j] = pbest_x[i][j];
//                 FindMin = gen;
//             }
//         }
//     }
// }
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

void renew_x_v()
{
    for (int i = 0; i < NumParticle; i++)
    {
        for (int j = 0; j < Dim; j++)
        {
            
            v[i][j] = (W * v[i][j]) + (C1 * Random(0, 1) * (pbest_x[i][j] - x[i][j])) + (C2 * Random(0, 1) * (gbest_x[j] - x[i][j]));

            x[i][j] += v[i][j];

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

int main()
{

    //time(NULL)
    ofstream myfile;
    string filename = "pso.csv";
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

    for (double wi = 1.0; wi >= 0.0; wi -= 0.01)
    {
        W = round(wi * 100) / 100;

        for (double c1i = 1.9; c1i >= 0.1; c1i -= 0.1)
        {
            C1 = round(c1i * 10) / 10;
            for (double c2i = 2.2; c2i >= 0.5; c2i -= 0.1)
            {
                C2 = round(c2i * 10) / 10;

                /*   ⇣⇣⇣⇣⇣PSO algo⇣⇣⇣⇣⇣ */
                for (int e = 0; e < EXPERIMENT; e++) //store the exactly generation that find the optimal value
                {
                    //cout << "+++++++++++++(EXPERIMENT)+++++++++++++++" << endl;
                    srand(114);
                    init();
                    for (int i = 0; i < ITERATION; i++)
                    {

                        fitness_renew(i);
                        renew_x_v();
                    }

                    // gen_sum += FindMin;
                    opt_value_sum += gbest_fit;
                }

                myfile << W << "," << C1 << "," << C2 << "," << (double)opt_value_sum / (double)EXPERIMENT << endl;

                //gen_sum = 0;
                opt_value_sum = 0;
                /* ⇡⇡⇡⇡⇡PSO algo⇡⇡⇡⇡⇡ */
            }
        }
    }
}