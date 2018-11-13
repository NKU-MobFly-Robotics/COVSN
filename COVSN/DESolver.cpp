#include <time.h>
#include "DESolver.h"
#include <stdlib.h>
#include <fstream>

//using std::ofstream;
#define RowVector(a,b)  (&a[b*nDim])
#define CopyVector(a,b) memcpy((a),(b),nDim*sizeof(int))
CDESolver::CDESolver(int D, int pop, int num, vector<vector<int>>&CF)
{
	nDim = D;
	nPop = pop;
	Min = 0;
	Max = num - 1;
	F = 0.5;
	CR = 0.3;
	trialSolution = new int[nDim];
	bestSolution = new int[nDim];
	popEnergy = new int[nPop];
	population = new int[nPop * nDim];
	populationnext = new int[nPop * nDim];
	cf = new int*[num];
	for (int i = 0; i<num; i++)
	{
		cf[i] = new int[num];
	}
	for (int i = 0; i<num; i++)
	{
		for (int j = 0; j<num; j++)
		{
			cf[i][j] = CF[i][j];
		}
	}
}

int CDESolver::solve(int maxGenerations, int *cam)
{
	zuida = new int[maxGenerations];
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
		bestSolution[i] = 0;
	}//initialize

	int generation;
	int candidate;
	//int best = 0;//计数�?0代以内无变化则终止进�?
	/*bool bAtSolution;*/

	bestEnergy = 0;
	/*bAtSolution = false;*/

	//for (generation=0;(generation < maxGenerations)&&(best<20);generation++,best++)
	for (generation = 0; generation < maxGenerations; generation++)
		/*for (generation=0;(generation < maxGenerations)&&bestEnergy<=323;generation++,best++)*/
	{
		for (candidate = 0; candidate < nPop; candidate++)
		{
			strategy(candidate);
			trialEnergy = EnergyFunction(trialSolution);

			if (trialEnergy > popEnergy[candidate])
			{
				// New low for this candidate
				popEnergy[candidate] = trialEnergy;
				CopyVector(RowVector(populationnext, candidate), trialSolution);
				//best = 0;
				// Check if all-time low
				if (trialEnergy > bestEnergy)
				{
					bestEnergy = trialEnergy;
					CopyVector(bestSolution, trialSolution);
				}

			}
			else
			{
				CopyVector(RowVector(populationnext, candidate), RowVector(population, candidate));
			}
		}
		zuida[generation] = bestEnergy;
		for (int i = 0; i < nPop; i++)
		{
			for (int j = 0; j < nDim; j++)
			{
				population[i*nDim + j] = populationnext[i*nDim + j];
			}
		}
	}
//	generations = generation;//返回结果
	CopyVector(cam, bestSolution);
	if (trialSolution)  //delete防止内存泄露
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
	ofstream fout("DE600.txt");
	for (int i = 0; i<maxGenerations; i++)
	{
		fout << zuida[i] << " ";
	}
	fout << endl;
	fout.close();
	return bestEnergy;
}

void CDESolver::strategy(int candidate)//mutation and crossover
{
	int r1, r2, r3;
	int n;

	SelectSamples(candidate, &r1, &r2, &r3);
	n = rand() % nDim;

	CopyVector(trialSolution, RowVector(population, candidate));
	for (int i = 0; i < nDim - 1; i++) //只循环nDim-1�?
	{                                   //确保变异后个体至少一个基因进入trialSolution
		n = (n + 1) % nDim;
		if ((float(rand() % 100)) / 100.0 < CR)
		{
			trialSolution[n] = bestSolution[n]
				+ int(F * (population[r1*nDim + n] - population[r2*nDim + n]));
			if (trialSolution[n]<0 || trialSolution[n]>Max)
			{
				trialSolution[n] = rand() % (Max + 1);
			}
		}
	}
}

int CDESolver::EnergyFunction(int testSolution[])//number of triangles covered by the D cameras
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
		/*if(i==1)
		{
		int k=0;
		}*/
		for (int j = 0; j <= Max; j++)
		{
			/*if(j==378)
			{
			int s=0;
			}*/
			if (cf[testSolution[i]][j] == 1 && list[j] == true)
			{
				energy++;
				list[j] = false;
			}
		}
	}
	if (list)
	{
		delete[]list;
		list = NULL;
	}
	return energy;
}

void CDESolver::SelectSamples(int candidate, int *r1, int *r2, int *r3)
{                                               //generate random integer number

	do
	{
		/**r1 = (int)RandomUniform(0,nPop-1);*/
		*r1 = rand() % (nPop);
	} while (*r1 == candidate);

	do
	{
		*r2 = rand() % (nPop);
	} while ((*r2 == candidate) || (*r2 == *r1));

	do
	{
		*r3 = rand() % (nPop);
	} while ((*r3 == candidate) || (*r3 == *r2) || (*r3 == *r1));
}
