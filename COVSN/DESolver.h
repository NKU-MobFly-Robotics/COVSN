#ifndef DESOLVER_H_
#define DESOLVER_H_
//#define CAMERA 380
using namespace std;
#include <vector>
class CDESolver   //Differential Evolution Algorithm of selecting cameras
{
public:
	CDESolver(int D, int pop, int num, vector<vector<int>>&CF);
	int solve(int maxGenerations, int *cam);
	int **cf;
	int *zuida;
	int nDim;//解空间维数（8）
	int nPop;//种群中个体数
	int generations;//进化代数
	int Min;//摄像机编号最小值（0）
	int Max;//摄像机编号最大值（379）
	int *trialSolution;
	int *bestSolution;
	int *popEnergy;
	int *population;
	int *populationnext;
	int bestEnergy;//最优情况下能看到的三角面数
	int trialEnergy;//选择依据
	//int CF3[CAMERA][CAMERA];
	float F;//缩放因子
	float CR;//交叉算子
protected:
	void strategy(int candidate);
	int EnergyFunction(int testSolution[]);
	void SelectSamples(int candidate, int *r1, int *r2, int *r3);
};
#endif