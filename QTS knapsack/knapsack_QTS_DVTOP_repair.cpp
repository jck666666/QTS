#include <iostream>
#include <stdio.h>
#include <ctime>
#include <fstream>

#define ITEM_NUM 100
#define POPULATION 10
#define ITERATION 1000
#define DELTA 0.05
#define WEIGHT_LIMIT 275

using namespace std;

typedef struct Chromo
{
    int gene[ITEM_NUM]; //items for choose
    int weight;         //weight of knapsack
    int value;          //value of knapsack
} Chromo;

Chromo chrom[POPULATION]; //population
Chromo opt_sol;
int opt_index;

float Q[ITEM_NUM] = {0.0}; //propobility array

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
            float randNum = rand() % 101 / (float)100; //0~1
            // cout<<"randNum: "<<randNum<<endl;

            if (randNum < Q[j])
            {
                chrom[i].gene[j] = 1;
            }
            else
            {
                chrom[i].gene[j] = 0;
            }
        }
    }
}

void fitness()
{
    for (int i = 0; i < POPULATION; i++)
    {
        int weight_ind = 0, value_ind = 0;
        chrom[i].weight = 0;
        chrom[i].value = 0;
        for (int j = 0; j < ITEM_NUM; j++)
        {
            // weight fitness
            weight_ind = j / 10 + 1;
            chrom[i].weight += chrom[i].gene[j] * weight_ind;

            //value fitness
            value_ind = weight_ind + 5;
            chrom[i].value += chrom[i].gene[j] * value_ind;
        }

        //repaired
        int index = ITEM_NUM;
        while (chrom[i].weight > WEIGHT_LIMIT)
        {
            if (chrom[i].gene[index] == 1)
            {
                //  1→0
                chrom[i].gene[index] = 0;

                //weight⇣   value⇣
                chrom[i].weight -=  (index / 10 + 1); //weight_ind = index / 10 + 1;
                chrom[i].value -=  (index / 10 + 6);  //value_ind = weight_ind + 5;
            }
            index--;
        }
    }
}

void renew() //run every generation
{
    int best = INT_MIN; //同一世代的整個族群中最佳
    int best_index = 0;

    int worst = INT_MIN; //同一世代的整個族群中與 WEIGHT_LIMIT 差最多的個體
    int worst_index = 0;

    for (int i = 0; i < POPULATION; i++)
    {
        

        //find the best value in POPULATION
        if (chrom[i].value > best && chrom[i].weight <= WEIGHT_LIMIT)
        {
            best = chrom[i].value;
            best_index = i;
        }

        //find the worst value in POPULATION
        int dev = abs(chrom[i].weight - WEIGHT_LIMIT);
        if (dev > worst || (dev == worst && chrom[i].value < chrom[worst_index].value)) //choose the max of the deviation
        {
            worst = chrom[i].value;
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
    if (chrom[best_index].value > opt_sol.value)
    {
        opt_sol = chrom[best_index];
        opt_index = best_index;
    }

    //Correction the value
    for (int i = 0; i < POPULATION; i++)
    {
        if (chrom[i].weight > WEIGHT_LIMIT)
        {
            chrom[i].value -= opt_sol.value;
        }
    }
}

int main()
{
    ofstream file("QTS_repaired.txt");
    file << "Particle:" << endl;

    srand(time(NULL));
    //srand(1);
  
    init();

    for (int i = 0; i < ITERATION; i++)
    {
        measure();
        fitness();
        renew();

        file << "*" << i + 1 << " " << opt_sol.value << ": ";

        for (int j = 0; j < POPULATION; j++)
        {
            file << chrom[j].value << " ";
            //file << "weight:" << chrom[j].weight;
            file << "bag" << j + 1;

            for (int k = 0; k < ITEM_NUM; k++)
            {
                file << " " << chrom[j].gene[k] << ",";
                file << Q[k] << ",";
                file << "bit" << k + 1;
            }
            file << "/";
        }
        file << endl;
    }
    cout << "opt_sol.value: " << opt_sol.value << endl;
    cout << "output file done";
    return 0;
}