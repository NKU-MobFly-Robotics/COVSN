#include <time.h>
#include "GASolver.h"
#include <stdlib.h>
#include <fstream>
using std::ofstream;
#define RowVector(a,b)  (&a[b*nDim])
#define CopyVector(a,b) memcpy((a),(b),nDim*sizeof(int))
CGASolver::CGASolver(int D, int pop, int Num, vector<vector<int>>&CF)
{
	nDim = D;
	nPop = pop;
	Min = 0;
	Max = Num - 1;
	pm = 0.01;
	pc = 0.4;
	/*F=0.5;*/
	c = new int[nDim];
	trialSolution2 = new int[nDim];
	trialSolution = new int[nDim];
	worstSolution = new int[nDim];
	bestSolution = new int[nDim];
	popEnergy = new int[nPop];
	population = new int[nPop * nDim];
	populationnext = new int[nPop * nDim];
	cf = new int*[Num];
	for (int i = 0; i<Num; i++)
	{
		cf[i] = new int[Num];
	}
	for (int i = 0; i<Num; i++)
	{
		for (int j = 0; j<Num; j++)
		{
			cf[i][j] = CF[i][j];
		}
	}
}

int CGASolver::solve(int maxGenerations, int *cam)
{
	zuida = new int[maxGenerations];
	b = 0;
	srand((unsigned)time(NULL));
	for (int i = 0; i < nPop; i++)
	{
		for (int j = 0; j < nDim; j++)
		{
			population[i*nDim + j] = rand() % (Max + 1);
		}
		popEnergy[i] = 0;
	}
	for (int i = 0; i < nDim; i++)
	{
		if (EnergyFunction(population + i*nDim)>b)//记录上代最优解
		{
			b = EnergyFunction(population + i*nDim);

			CopyVector(c, population + i*nDim);
		}
		bestSolution[i] = 0;
		worstSolution[i] = 0;
	}//初始�?

	int generation;
	int candidate;
	//int best = 0;//计数�?代以内无优化则终止进�?
	bestEnergy = 0;
	/*bool bAtSolution;*/

	int sum;
	/*int trial=0;*/

	/*bAtSolution = false;*/

	/*for (generation=0;(generation < maxGenerations)&&(best<50);generation++,best++)*/
	for (generation = 0; generation < maxGenerations; generation++)
		/*for (generation=0;(generation < maxGenerations)&&bestEnergy<=323;generation++,best++)*/
	{
		sum = 0;
		for (int i = 0; i<nPop; i++)
		{
			popEnergy[i] = EnergyFunction(population + i*nDim);
			sum += popEnergy[i];
		}
		trialEnergy = 0;
		trialEnergy2 = 10000;
		for (candidate = 0; candidate < nPop; candidate += 2)
		{
			/*trial=0;*/
			int r1, r2;                 //选择
			r1 = select(sum);
			do
			{
				r2 = select(sum);
			} while (r2 == r1);

			for (int i = 0; i<nDim; i++)
			{
				if ((rand() % 1000) / 1000.0<pc)//crossover
				{
					populationnext[candidate*nDim + i] = population[r2*nDim + i];
					populationnext[(candidate + 1)*nDim + i] = population[r1*nDim + i];
				}
				else
				{
					populationnext[candidate*nDim + i] = population[r1*nDim + i];
					populationnext[(candidate + 1)*nDim + i] = population[r2*nDim + i];
				}
				/*populationnext[candidate*nDim + i]=rand()%(Max+1);
				populationnext[(candidate+1)*nDim + i]=rand()%(Max+1);*/
			}


			//if((rand()%1000)/1000.0<pc)//交叉
			//{
			//	int r=rand()%(nDim-1);
			//	for(int i=0;i<=r;i++)
			//	{
			//		populationnext[candidate*nDim + i]=population[r1*nDim+i];
			//		populationnext[(candidate+1)*nDim + i]=population[r2*nDim+i];
			//	}
			//	for(int i=r+1;i<nDim;i++)
			//	{
			//		populationnext[candidate*nDim + i]=population[r2*nDim+i];
			//		populationnext[(candidate+1)*nDim + i]=population[r1*nDim+i];
			//	}
			//}
			/*else
			{
			for(int i=0;i<nDim;i++)
			{
			populationnext[candidate*nDim+i]=population[r1*nDim+i];
			populationnext[(candidate+1)*nDim+i]=population[r2*nDim+i];
			}
			}*/

			for (int i = 0; i<nDim; i++)//mutation
			{
				if ((rand() % 10000) / 10000.0<pm)
					populationnext[candidate*nDim + i] = rand() % (Max + 1);
				if ((rand() % 10000) / 10000.0<pm)
					populationnext[(candidate + 1)*nDim + i] = rand() % (Max + 1);
			}

			//if(EnergyFunction(populationnext+candidate*nDim)<EnergyFunction(population+r1*nDim))
			//	CopyVector(populationnext+candidate*nDim, population+r1*nDim);
			//if(EnergyFunction(populationnext+(candidate+1)*nDim)<EnergyFunction(population+r2*nDim))
			//	CopyVector(populationnext+(candidate+1)*nDim, population+r2*nDim);                 //最后再选择一次，将优秀个体保留，劣等个体淘�?


			if (EnergyFunction(populationnext + candidate*nDim)>trialEnergy)//record the best individual of the present generation
			{
				trialEnergy = EnergyFunction(populationnext + candidate*nDim);

				CopyVector(trialSolution, populationnext + candidate*nDim);

			}
			if (EnergyFunction(populationnext + (candidate + 1)*nDim)>trialEnergy)
			{
				trialEnergy = EnergyFunction(populationnext + (candidate + 1)*nDim);

				CopyVector(trialSolution, populationnext + (candidate + 1)*nDim);
			}
			if (EnergyFunction(populationnext + candidate*nDim)<trialEnergy2)//record the worst individual of the present generation
			{
				trialEnergy2 = EnergyFunction(populationnext + candidate*nDim);
				d = candidate;
			}
			if (EnergyFunction(populationnext + (candidate + 1)*nDim)<trialEnergy2)
			{
				trialEnergy2 = EnergyFunction(populationnext + (candidate + 1)*nDim);
				d = candidate + 1;
			}

			/*trial=trial+EnergyFunction(populationnext+candidate*nDim)+EnergyFunction(populationnext+(candidate+1)*nDim);*/
		}
		if (trialEnergy>b)
		{
			//best = 0;
			b = trialEnergy;//record the number of triangles covered by the best individual
			CopyVector(c, trialSolution);
		}
		else
		{
			CopyVector(populationnext + d*nDim, c);
		}
		zuida[generation] = b;


		for (int i = 0; i < nPop; i++)
		{
			CopyVector(population + i*nDim, populationnext + i*nDim);
		}
		/*if(trial>sum)
		{
		best =0;
		}
		sum=trial;*/
		/*if(bestEnergy>360)
		break;*/
	}
	//generations = generation;//返回结果
	CopyVector(cam, c);
	//if(trialSolution)  //delete防止内存泄露
	//{
	//	delete trialSolution;
	//	trialSolution=NULL;
	//}
	if (trialSolution)
	{
		delete trialSolution;
		trialSolution = NULL;
	}
	if (bestSolution)
	{
		delete bestSolution;
		bestSolution = NULL;
	}
	if (popEnergy)
	{
		delete popEnergy;
		popEnergy = NULL;
	}
	if (population)
	{
		delete population;
		population = NULL;
	}
	if (populationnext)
	{
		delete populationnext;
		populationnext = NULL;
	}
	ofstream fout("GA600.txt");  //output the number of triangles covered by the best solution of every generation
	for (int i = 0; i<maxGenerations; i++)
	{
		fout << zuida[i] << " ";
	}
	fout.close();
	return b;
}

int CGASolver::select(int sum)//Roulette method
{
	CR = rand() % sum;
	int edge = 0;
	for (int i = 0; i<nPop; i++)
	{
		edge += popEnergy[i];
		if (CR<edge)
		{
			return i;
		}
	}
}

int CGASolver::EnergyFunction(int testSolution[])//number of triangles covered by the nDim cameras
{
	int energy = 0;
	bool *list;
	list = new bool[Max + 1];


	for (int j = 0; j <= Max; j++)
	{
		list[j] = true;
	}
	for (int i = 0; i<nDim; i++)
	{
		if (i == 1)
		{
			int k = 0;
		}
		for (int j = 0; j <= Max; j++)
		{
			if (j == 378)
			{
				int s = 0;
			}
			if (cf[testSolution[i]][j] == 1 && list[j] == true)
			{
				energy++;
				list[j] = false;
			}
		}
	}
	if (list)
	{
		delete list;
		list = NULL;
	}
	return energy;
}
