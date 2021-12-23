#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>
#include "Result.h"
#include "TSP.h"
using namespace std;
double cost_cal(double** A, int* x, int n) {
    double cost = 0;
    for (int i = 0; i < n; i++) {
        cost += A[x[i]][x[i + 1]];
    }
    return cost;
}
int main()
{

    return;
    int n = 6;
    std::cout << "Initializing\n";
    fstream f("Text.txt", ios::in);
    if (!f.eof())
    {
        f >> n;
    }
    double** input = new double* [n];
    for (int i = 0; i < n; i++) {
        input[i] = new double[n];
        for (int j = 0; j < n; j++) {
            f >> input[i][j];
        }
    }
    f.close();
    for (int i = 0; i < n; i++) {
        input[i][i] = DBL_MAX;
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (input[i][j] >= intInf) {
                cout << setw(10);
                cout << "∞ ";
            }
            else {
                cout << setw(10);
                cout << input[i][j] << " ";
            }
        }
        cout << endl;
    }
    cout << "------------------End Initializing--------------------" << endl;
    TSP_Run(input, n);
    //Result_Run(input, n);

    //calculating
    /*cout << cost_cal(input, new int[7]{ 0,3,5,2,1,4,0 }, 6) << endl;
    cout << cost_cal(input, new int[7]{ 0,3,5,2,4,1,0 }, 6) << endl;*/
    return 0;
}

