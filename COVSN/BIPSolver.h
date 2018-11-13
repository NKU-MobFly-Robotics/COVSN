#ifndef BIPSOLVER_H_
#define BIPSOLVER_H_
#include <windows.h>
#include "gurobi_c++.h"
using namespace std;
#include <vector>
class CBIPSolver   //Binary Integer Programming approach of selecting cameras
{
public:
	int max;
	int n_dim;
	int **cf;
	CBIPSolver(int num, vector<vector<int>>&CF, int N_DIM);
	int solve(int * cam);
};
#endif