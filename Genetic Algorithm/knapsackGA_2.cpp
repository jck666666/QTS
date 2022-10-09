#include <iostream>
#include <stdio.h>

#define weight_limit 275 //weight limit
#define POPULATION 2000
#define item_num 100
#define tmp_num 4
#define ELITE 30
#define ITERATION 250
#define MUTATION_RATE 10
using namespace std;

typedef struct Chromo
{
    int gene[item_num]; //items for choose
    int weight;         //weight of knapsack
    int value;          //value of knapsack
} Chromo;

Chromo chrom[POPULATION];
Chromo tmp[tmp_num]; //4 chromosome of max_value
Chromo elite[ELITE];
Chromo optimal_sol;
int max_value, max_index;
int max2_value, max2_index;

void init()
{
    //Initial Chromosome
    srand(time(NULL));
    cout << "---Initial--- " << endl;
    for (int i = 0; i < POPULATION; i++)
    {
       
        for (int j = 0; j < item_num; j++)
        {
            chrom[i].gene[j] = rand() % 2;
           
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
        for (int j = 0; j < 100; j++)
        {
            // weight fitness
            weight_ind = j / 10 + 1;
            chrom[i].weight += chrom[i].gene[j] * weight_ind;

            //value fitness
            value_ind = weight_ind + 5;
            chrom[i].value += chrom[i].gene[j] * value_ind;
        }
    }
    cout << endl;
}

void selection()
{

    //find elite
    bool selected[POPULATION] = {false};
    int max = 0, now = 0, i, j;
    int mm = 0; //max index

    for (j = 0; j < ELITE; j++)
    {
        max = 0;
        for (i = 0; i < POPULATION; i++)
        {
            if (chrom[i].value > max && chrom[i].weight <= weight_limit && selected[i] == false)
            {
                max = chrom[i].value;
                mm = i;
                selected[mm] = true;
            }
        }
        elite[j] = chrom[mm];
    }

    //copy
    tmp[0] = elite[0];
    tmp[1] = elite[1];
    for (int i = 0; i < ELITE; i++)
    {
        chrom[i] = elite[i];
    }
    for (int i = ELITE; i < POPULATION; i++)
    {
        chrom[i] = chrom[rand() % POPULATION];
    }

    //print max max2
    cout << "max_value: " << elite[0].value << endl;
    cout << "max2_value: " << elite[1].value << endl<<endl;

    //renew optimal
    if (elite[0].value > optimal_sol.value)
    {
        optimal_sol = elite[0];
    }
}

void crossover()
{
    int r=rand()%item_num ;
    for (int i = 0; i < r; i++)
    {
        tmp[2].gene[i] = tmp[0].gene[i];
        tmp[3].gene[i] = tmp[1].gene[i];
    }
    for (int i = r+1; i < rand()%item_num; i++)
    {
        tmp[2].gene[i] = tmp[1].gene[i];
        tmp[3].gene[i] = tmp[0].gene[i];
    }

    //substitute
    chrom[POPULATION - 1] = tmp[2];
    chrom[POPULATION - 2] = tmp[3];
}

void mutation()
{
    // cout << "..Mutation.." << endl;
    for (int i = 1; i < POPULATION; i++)
    {
        if (rand() % 100 < MUTATION_RATE)
        {
            continue;
        }
        int change = rand() % item_num;
        chrom[i].gene[change] = !chrom[i].gene[change];
    }
}

int main()
{

    srand(time(NULL));
    init();

    for (int i = 0; i < ITERATION; i++)
    {
        cout << "---[" << i << "]--- ";
        fitness();
        selection();
        crossover();
        mutation();
    }
    cout << "---[optimal]---" << endl;
    cout << "optimal solution: ";
    for (int j = 0; j < item_num; j++)
    {
        cout << optimal_sol.gene[j];
    }
    cout << endl;
    cout << "optimal value: " << optimal_sol.value << endl;
    cout << "optimal weight: " << optimal_sol.weight << endl
         << endl;

    return 0;
}