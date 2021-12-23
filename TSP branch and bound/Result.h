#include <iostream>
#include <limits.h>
#include <vector>
using namespace std;
double intInf = 1E300; 
void Result_Run(double** input, int n) {
	double** A = input;
	int* a = new int[n - 1];
	vector<int*> xopts;
	double fopt = DBL_MAX;
	double intInf = 1E300;
	for (int i = 0; i < n - 1; i++) {
		a[i] = i + 1;
	}
	while (true) {
		//Initialize situation
		int index = n - 3;
		int tmp;
		while (index > -1 && a[index] >= a[index + 1]) {
			index--;
		}
		if (index == -1) break;
		tmp = a[index];
		for (int i = n - 2; i > index; i--) {
			if (a[i] > tmp) {
				a[index] = a[i];
				a[i] = tmp;
				break;
			}
		}
		for (int i = (index - 1 + n)/2 + 1; i < n - 1; i++) {
			tmp = a[i];
			a[i] = a[index + n - 1 - i];
			a[index + n - 1 - i] = tmp;
		}
		//Calculating
		double cost = 0;
		for (int i = 0; i < n - 2; i++) {
			cost += A[a[i]][a[i + 1]];
		}
		cost += A[0][a[0]];
		cost += A[a[n-2]][0];
		if (cost < fopt) {
			int* xopt = new int[n - 1];
			fopt = cost;
			for (int i = 0; i < n - 1; i++) {
				xopt[i] = a[i];
			}
			for (int i = 0; i < xopts.size(); i++) {
				delete(xopts[i]);
			}
			xopts.clear();
			xopts.push_back(xopt);
		}
		else if (cost == fopt) {
			int* xopt = new int[n - 1];
			for (int i = 0; i < n - 1; i++) {
				xopt[i] = a[i];
			}
			xopts.push_back(xopt);
		}
		for (int i = 0; i < n - 1; i++) {
			cout << a[i] + 1 << " ";
		}
		cout << endl;
	}
	cout << "---------------------- BEGIN REAL Result ----------------------------" << endl;
	cout << "Cost: " << fopt << endl;
	for (int ix = 0; ix < xopts.size(); ix++) {
		cout << "1 -> ";
		for (int j = 0; j < n - 1; j++) {
			cout << xopts[ix][j] + 1 << " -> ";
		}
		cout << "1" << endl;
	}
	cout << "---------------------- END REAL Result ----------------------------" << endl;
	return;
}
