#include <stdio.h>

#include <ctime>
#include <iostream>

#define ITEM_NUM 100
#define EXPERIMENT 50
#define POPULATION 10
#define ITERATION 1000
#define DELTA 0.02
#define WEIGHT_LIMIT 275

using namespace std;

typedef struct Chromo {
    int gene[ITEM_NUM];  //items for choose
    int weight;  //weight of knapsack
    int value;  //value of knapsack
} Chromo;

Chromo chrom[POPULATION];  //population
Chromo opt_sol;
int opt_index;
int found;  //the generation which find the optimal solution

float Q[ITEM_NUM] = {0.0};  //propobility array

void init() {
    for (int i = 0; i < ITEM_NUM; i++) {
        Q[i] = 0.5;
    }
}

void measure() {
    for (int i = 0; i < POPULATION; i++) {
        for (int j = 0; j < ITEM_NUM; j++) {
            double randNum = (double)rand() / RAND_MAX;  //0~1

            if (randNum < Q[j]) {
                chrom[i].gene[j] = 1;
            }
            else {
                chrom[i].gene[j] = 0;
            }
        }
    }
}

void fitness() {
    for (int i = 0; i < POPULATION; i++) {
        int weight_ind = 0, value_ind = 0;
        chrom[i].weight = 0;
        chrom[i].value = 0;
        for (int j = 0; j < ITEM_NUM; j++) {
            // weight fitness
            weight_ind = j / 10 + 1;
            chrom[i].weight += chrom[i].gene[j] * weight_ind;

            //value fitness
            value_ind = weight_ind + 5;
            chrom[i].value += chrom[i].gene[j] * value_ind;
        }
    }
}

void renew(int gen)  //run every generation
{
    int best = INT_MIN;  //同一世代的整個族群中最佳
    int best_index = 0;

    int worst = INT_MAX;  //同一世代的整個族群中最差
    int worst_index = 0;

    for (int i = 0; i < POPULATION; i++) {
        //find the best value in POPULATION
        if (chrom[i].value >= best && chrom[i].weight <= WEIGHT_LIMIT) {
            best = chrom[i].value;
            best_index = i;
        }

        //find the worst value in POPULATION
        int dev = abs(chrom[i].weight - WEIGHT_LIMIT);
        if (dev > worst || (dev == worst && chrom[i].value < chrom[worst_index].value))  //choose the max of the deviation
        {
            worst = chrom[i].value;
            worst_index = i;
        }

        /*
        
        find the worst value in POPULATION
        if (chrom[i].value < worst && chrom[i].weight <= WEIGHT_LIMIT)
        {
            worst = chrom[i].value;
            worst_index = i;
        }
        
        */
    }

    //renew the propability in Q[]
    for (int i = 0; i < ITEM_NUM; i++) {
        if (chrom[best_index].gene[i] == 1 && chrom[worst_index].gene[i] == 0) {
            Q[i] += DELTA;
        }
        else if (chrom[best_index].gene[i] == 0 && chrom[worst_index].gene[i] == 1) {
            Q[i] -= DELTA;
        }
        else  //chrom[best_index].gene[i] == chrom[worst_index].gene[i]
        {
            //probability will not renew
        }
    }

    //renew the optimal solution (global best)

    if (chrom[best_index].value > opt_sol.value) {
        opt_sol = chrom[best_index];
        opt_index = best_index;
        found = gen;  //the generation which find the optimal solution
    }
}

int main() {
    srand(time(NULL));
    //srand(1);
    int gen_sum = 0;
    int opt_value_sum = 0;
    for (int i = 0; i < EXPERIMENT; i++)  //store the exactly generation that find the optimal value
    {
        init();
        //found = 0;
        //opt_sol.value = 0;

        for (int j = 0; j < ITERATION; j++) {
            measure();
            fitness();
            renew(j);
        }

        // cout << "---" << i << "---"
        //      << "" << endl
        //      << "opt value: " << opt_sol.value << endl
        //      << "opt index: " << opt_index << endl
        //      << "opt weight: " << opt_sol.weight << endl
        //      << endl;

        // cout << "found: " << found << endl;
        //cout << "opt_sol.value: " << opt_sol.value << endl
        //      << endl;

        gen_sum += found;
        opt_value_sum += opt_sol.value;
    }
    cout << "Average Found Generation: " << gen_sum / EXPERIMENT << endl;
    cout << "Average Value: " << opt_value_sum / EXPERIMENT << endl;
}