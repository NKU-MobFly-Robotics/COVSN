#ifndef GASOLVER_H_
#define GASOLVER_H_
#include <vector>
using namespace std;
class CGASolver   //Genetic Algorithm of selecting cameras
{
public:
	CGASolver(int D, int pop, int Num, vector<vector<int>>&CF);
	int solve(int maxGenerations, int *cam);
	int b;//最优评价函数值
	int d;//最差在第几代
	int *code;
	int **cf;
	int nDim;//解空间维数（8）
	//int longth;//基因编码长度
	int nPop;//种群中个体数
	//int generations;//进化代数
	int Min;//摄像机编号最小值（0）
	int Max;//摄像机编号最大值（379）
	int *c;//最优解
	int *trialSolution2;
	int *trialSolution;
	int *worstSolution;
	int *bestSolution;
	int *popEnergy;
	int *population;
	int *populationnext;
	int *zuida;
	int *worstEnegy;//最差情况下能看到的三角面数
	int bestEnergy;//最优情况下能看到的三角面数
	int trialEnergy2;
	int trialEnergy;//选择依据
	//int CF3[CAMERA][CAMERA];
	float pc;//crossover probability
	float pm;//mutation probability
	//float F;//交叉算子
	float CR;//选择算子
protected:
	int select(int sum);
	/*void strategy(int candidate);*/
	int EnergyFunction(int testSolution[]);
	/*void SelectSamples(int candidate,int *r1,int *r2,int *r3);*/
};
#endif