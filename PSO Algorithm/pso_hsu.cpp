#include <iostream>
#include <typeinfo>
#include <iomanip>
#include <cstdlib>
#include <cfloat>
#include <cmath>

#define n 5
#define Dim 1
#define c1 0.3 // c1調這邊
#define c2 1.8 // c2調這邊
#define min -100
#define max 100
#define loop 500
#define test 50
#define rand_seed 114
double w_init = 0.1;  // w開始的值調這邊
double w_final = 0.1; // w結束的值調這邊

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
    double w = w_init - ((w_init - w_final) / (double)(loop - 1) * (double)now);

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
                cout << "out of bound!\n";
            }
            else if (x[i][j] < min)
            {
                x[i][j] = min + (double)rand() * (max - min) / (double)RAND_MAX;
                cout << "out of bound!\n";
            }
        }
    }
}

int main()
{
    srand(rand_seed);
    double avg_ans = 0.0;
    for (int i = 0; i < test; i++)
    {
        cout << "******* Experiment *******\n";
        Gbest = DBL_MAX;
        init();

        for (now = 0; now < loop; now++)
        {
            printf("%d\n{    x    ,      v    }\n",now+1);
            for (int i = 0; i < n; i++)
            {
                for (int j = 0; j < Dim; j++)
                {
                    cout << "{" << x[i][j] << ", " << v[i][j] << "}" << endl;
                }
            }
            fitness();
            update();
            move();
        }
        cout << "Gbest = " << Gbest << endl;
        avg_ans += Gbest;
    }

    cout << "avg_ans = " << avg_ans / (double)50 << endl;

    return 0;
}