#include <iostream>
#include <vector>
#include <limits.h>

using namespace std;
int* a, * xopt;
int N;
double fopt;
//double intInf = 1E300;
vector<int> cols;
vector<int> rows;
typedef struct xyBound {
    int x;
    int y;
    double bound;
};
void printBinArr(double* x, int n) {
    for (int i = 0; i < n; i++) {
        if (x[i] >= intInf) {
            cout << "∞ ";
        }
        else {
            cout << x[i] << " ";
        }
    }
    cout << endl;
}
double reduce(double** A, int k) {
    double min, sum = 0;
    for (int i = 0; i < k; i++) {
        min = A[i][0];
        for (int j = 1; j < k; j++) {
            if (min > A[i][j]) min = A[i][j];
        }
        sum += min;
        for (int j = 0; j < k; j++) {
            A[i][j] -= min;
        }
    }
    for (int i = 0; i < k; i++) {
        min = A[0][i];
        for (int j = 1; j < k; j++) {
            if (min > A[j][i]) min = A[j][i];
        }
        sum += min;
        for (int j = 0; j < k; j++) {
            A[j][i] -= min;
        }
    }
    return sum;
}
xyBound bestEdge(double** A, int k) {
    double minOfRows[100];
    int hasZero;
    double min, maxC = -1, c;
    xyBound x;
    for (int i = 0; i < k; i++) {
        minOfRows[i] = DBL_MAX;
        hasZero = -1;
        for (int j = 0; j < k; j++) {
            if (A[i][j] == 0) {
                if (hasZero != -1) {
                    minOfRows[i] = 0;
                    break;
                }
                else hasZero = j;
            }
            else if (minOfRows[i] > A[i][j]) {
                minOfRows[i] = A[i][j];
            }
        }
    }
    for (int i = 0; i < k; i++) {
        min = DBL_MAX;
        hasZero = -1;
        for (int j = 0; j < k; j++) {
            if (A[j][i] == 0) {
                if (hasZero != -1) {
                    min = 0;
                    break;
                }
                else hasZero = j;
            }
            else if (min > A[j][i]) {
                min = A[j][i];
            }
        }
        if (min) {
            if (hasZero != -1) {
                c = min + minOfRows[hasZero];
                if (c > maxC) {
                    maxC = c;
                    x.x = hasZero;
                    x.y = i;
                    x.bound = c;
                }
            }
        }
        else {
            for (int j = 0; j < k; j++) {
                if (A[j][i] == 0) {
                    c = minOfRows[j];
                    if (c > maxC) {
                        maxC = c;
                        x.x = j;
                        x.y = i;
                        x.bound = c;
                    }
                }
            }
        }
    }
    return x;
}
double** NewArray(double** A, int k, int row, int col) {
    double** outp = new double* [k - 1];
    for (int i = 0; i < row; i++) {
        outp[i] = new double[k - 1];
        for (int j = 0; j < col; j++) {
            outp[i][j] = A[i][j];
        }
        for (int j = col + 1; j < k; j++) {
            outp[i][j - 1] = A[i][j];
        }
    }
    for (int i = row + 1; i < k; i++) {
        outp[i - 1] = new double[k - 1];
        for (int j = 0; j < col; j++) {
            outp[i - 1][j] = A[i][j];
        }
        for (int j = col + 1; j < k; j++) {
            outp[i - 1][j - 1] = A[i][j];
        }
    }
    return outp;
}
void TSP(int n, double** A, double cost) {
    cost += reduce(A, n);
    if (fopt <= cost) return;
    if (n == 2) {
        if (A[0][0] + A[1][1] >= intInf) {
            if (A[0][1] + A[1][0] < intInf) {
                a[rows[0]] = cols[1];
                a[rows[1]] = cols[0];
                ////////////////////////-- Not important code
                /*cout << "Last choose: (" << rows[0] + 1 << "; " << cols[1] + 1 << "); ";
                cout << "(" << rows[1] + 1 << "; " << cols[0] + 1 << ")" << endl;
                cout << "---------------------------------------" << endl;*/
                /// //////////////////////
            }
            else return;
        }
        else if (A[0][1] + A[1][0] >= intInf) {
            a[rows[0]] = cols[0];
            a[rows[1]] = cols[1];
        }
        else if(A[0][0] + A[1][1] >= A[0][1] + A[1][0])
        {
            a[rows[0]] = cols[1];
            a[rows[1]] = cols[0];
        }
        else {
            a[rows[0]] = cols[0];
            a[rows[1]] = cols[1];
            ////////////////////////-- Not important code
            /*cout << "Last choose: (" << rows[0] + 1 << "; " << cols[0] + 1 << "); ";
            cout << "(" << rows[1] + 1 << "; " << cols[1] + 1 << ")" << endl;
            cout << "---------------------------------------" << endl;*/
            /// //////////////////////
        }
        for (int i = 0; i < N; i++) {
            xopt[i] = a[i];
        }
        fopt = cost;
        return;
    }
    xyBound x = bestEdge(A, n);
    int ri, ci, rx, cx;
    //Left branch k = n - 1
    double** NewA = NewArray(A, n, x.x, x.y);
    ri = rows[x.x];
    ci = cols[x.y];
    a[ri] = ci;
    ////////////////////////-- Not important code
    /*cout << "Rows: ";
    for (int j = 0; j < n; j++) {
        cout << rows[j] + 1 << " ";
    }
    cout << endl << "Cols: ";
    for (int j = 0; j < n; j++) {
        cout << cols[j] + 1 << " ";
    }
    cout << endl;
    cout << endl;*/
    /// //////////////////////
    rows.erase(rows.begin() + x.x);
    cols.erase(cols.begin() + x.y);
    rx = cx = -1;
    for (int j = 0; j < n - 1; j++) {
        if (cols[j] == ri) {
            cx = j;
            break;
        }
    }
    for (int j = 0; j < n - 1; j++) {
        if (rows[j] == ci) {
            rx = j;
            break;
        }
    }
    if (rx > -1 && cx > -1) {
        NewA[rx][cx] = DBL_MAX;
    }
    ////////////////////////-- Not important code
    /*for (int i = 0; i < n; i++) {
        printBinArr(A[i], n);
    }
    cout << "Best edge: " << ri + 1 << " - " << ci + 1 << "; ";
    cout << "Low Bound: " << cost << endl;
    cout << "---------------------------------------" << endl;*/
    /// //////////////////////
    TSP(n - 1, NewA, cost);
    //Right branch k = n
    rows.insert(rows.begin() + x.x, ri);
    cols.insert(cols.begin() + x.y, ci);
    if (fopt <= cost + x.bound) return;
    A[x.x][x.y] = DBL_MAX;
    TSP(n, A, cost);
}
void TSP_Run(double** input, int n) {
    double** A;
    A = new double* [n];
    a = new int[n];
    xopt = new int[n];
    fopt = DBL_MAX;
    for (int i = 0; i < n; i++) {
        cols.push_back(i);
        rows.push_back(i);
        A[i] = new double[n];
        for (int j = 0; j < n; j++) {
            A[i][j] = input[i][j];
            if (i == j) A[i][j] = DBL_MAX;
        }
    }
    N = n;

    
    TSP(n, A, 0);
    cout << "---------------------- BEGIN TSP Result ----------------------------" << endl;
    cout << "Cost: " << fopt << endl;
    int ix = 0;
    cout << 1 << " -> ";
    while (xopt[ix]) {
        cout << xopt[ix] + 1 << " -> ";
        ix = xopt[ix];
    }
    cout << 1 << endl;
    cout << "---------------------- END TSP Result ----------------------------" << endl;
}
