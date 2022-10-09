#include <iostream>
#include <stdio.h>
#include <ctime>

#define weight_limit 300 //weight limit
#define item_num 8
using namespace std;

//item      weight     value
//gene[0]   50          120
//gene[1]   30          50
//gene[2]   78          100
//gene[3]   20          70
//gene[4]   40          10
//gene[5]   10          30
//gene[6]   90          66
//gene[7]   80          40

typedef struct Chromo
{
    int gene[item_num]; //items for choose
    int weight;         //weight of knapsack
    int value;          //value of knapsack
} Chromo;

Chromo chrom[4]; // 4 chromosome
Chromo tmp[4];   //4 chromosome of max_value
Chromo optimal_sol;

void init()
{
    srand(time(NULL));
    cout << "---Initial--- " << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "chromosome " << i << " : ";
        for (int j = 0; j < item_num; j++)
        {
            chrom[i].gene[j] = rand() % 2;
            cout << chrom[i].gene[j];
        }
        cout << endl;
    }
    cout << endl;
}

void fitness()
{
    for (int i = 0; i < 4; i++)
    {
        // weight fitness

        chrom[i].weight = chrom[i].gene[0] * 50 + chrom[i].gene[1] * 30 +
                          chrom[i].gene[2] * 78 + chrom[i].gene[3] * 20 +
                          chrom[i].gene[4] * 40 + chrom[i].gene[5] * 10 +
                          chrom[i].gene[6] * 90 + chrom[i].gene[7] * 80;
        cout << "weight fit " << i << " : " << chrom[i].weight;

        //value fitness
        chrom[i].value = chrom[i].gene[0] * 120 + chrom[i].gene[1] * 50 +
                         chrom[i].gene[2] * 100 + chrom[i].gene[3] * 70 +
                         chrom[i].gene[4] * 10 + chrom[i].gene[5] * 30 +
                         chrom[i].gene[6] * 66 + chrom[i].gene[7] * 40;
        cout << "      value  fit " << i << " : " << chrom[i].value << endl;
    }
    cout << endl;
}

void selection()
{
    //find best value
    int max_value = 0, max_index = -1;
    for (int i = 0; i < 4; i++)
    {
        if (chrom[i].value >= max_value && chrom[i].weight <= weight_limit)
        {
            max_value = chrom[i].value;
            max_index = i;
        }
    }
    cout << "max_value: " << max_value;
    cout << "   max_index: " << max_index << endl;
    if (max_value > optimal_sol.value ||
        (max_value == optimal_sol.value && chrom[max_index].weight <= optimal_sol.weight))
    {
        optimal_sol.value = max_value;
        optimal_sol.weight = chrom[max_index].weight;
        for (int i = 0; i < item_num; i++)
        {
            optimal_sol.gene[i] = chrom[max_index].gene[i];
        }
    }

    //copy best to tmp
    for (int i = 0; i < item_num; i++)
    {
        tmp[0].gene[i] = chrom[max_index].gene[i];
    }
    tmp[0].value = chrom[max_index].value;
    tmp[0].weight = chrom[max_index].weight;

    //find second best value
    int max2_value = 0, max2_index = -1;
    for (int i = 0; i < 4; i++)
    {
        if (chrom[i].value >= max2_value && chrom[i].weight <= weight_limit && i != max_index)
        {
            max2_value = chrom[i].value;
            max2_index = i;
        }
    }
    cout << "max2_value: " << max2_value;
    cout << "   max2_index: " << max2_index << endl
         << endl;

    //copy second best to tmp
    for (int i = 0; i < item_num; i++)
    {
        tmp[1].gene[i] = chrom[max2_index].gene[i];
    }
    tmp[1].value = chrom[max2_index].value;
    tmp[1].weight = chrom[max2_index].weight;
}

void crossover()
{
    for (int i = 0; i < 3; i++)
    {
        tmp[2].gene[i] = tmp[0].gene[i];
        tmp[3].gene[i] = tmp[1].gene[i];
    }
    for (int i = 3; i < item_num; i++)
    {
        tmp[2].gene[i] = tmp[1].gene[i];
        tmp[3].gene[i] = tmp[0].gene[i];
    }

    //print
    cout << "..After Cossover.." << endl;
    for (int i = 0; i < 4; i++)
    {
        cout << "chromosome " << i << " : ";
        for (int j = 0; j < item_num; j++)
        {
            chrom[i].gene[j] = tmp[i].gene[j];
            cout << chrom[i].gene[j];
        }
        cout << endl;
    }
    cout << endl;
}

void mutation()
{
    cout << "..Mutation.." << endl;
    for (int i = 2; i < 4; i++)
    {

        int change = rand() % item_num;
        cout << change << endl;

        chrom[i].gene[change] = !chrom[i].gene[change];
    }

    for (int i = 0; i < 4; i++)
    {
        cout << "chromosome " << i << " : ";
        for (int j = 0; j < item_num; j++)
        {
            cout << chrom[i].gene[j];
        }
        cout << endl;
    }
    cout << endl;
}

int main()
{
    srand(time(NULL));
    init();

    for (int i = 0; i < 15; i++)
    {

        cout << "---[" << i << "]--- " << endl;

        cout << "optimal solution: ";
        for (int j = 0; j < item_num; j++)
        {
            cout << optimal_sol.gene[j];
        }
        cout << endl;
        cout << "optimal value: " << optimal_sol.value << endl;
        cout << "optimal weight: " << optimal_sol.weight << endl
             << endl;

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