#ifndef PSOSOLVER_H_
#define PSOSOLVER_H_
#include <vector>
#include<time.h>
using namespace std;
struct particle
{
	int *X;
	int *P;
	int *V;
	int Fitness;
};

class CPSOSolver{

	int nDim;
	int nPop;
	int Max;
	int **cf;
	int *zuida;
	struct particle* Particle;
	int GBestIndex;
	int *GBest;
	double W;
	int C1;
	int C2;
	int Xup;
	int Xdown;
	int Vmax;

public:
	CPSOSolver(int D, int pop, int num, vector<vector<int>>&CF);
	int solve(int maxGenerations, int *cam);
	int FitFunction(int testSolution[]);
	void RandInitofSwarm(void);
	void ComputFitofSwarm(void);
	void FirstComputPandGbest(void);
	void UpdateofVandX(void);
	void UpdatePandGbest(void);
	/*double InertWeight(void);
	int CriteriaofStop(void);  */
};
#endif