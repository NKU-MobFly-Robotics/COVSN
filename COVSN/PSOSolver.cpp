#include "PSOSolver.h"
//#include <stdlib.h>
#include <fstream>

using namespace std;
#define CopyVector(a,b) memcpy((a),(b),nDim*sizeof(int))

CPSOSolver::CPSOSolver(int D, int pop, int num, vector<vector<int>>&CF)
{
	nDim = D;
	nPop = pop;
	Max = num - 1;
	GBest = new int[nDim];
	Particle = new particle[nPop];
	for (int i = 0; i<nPop; i++)
	{
		Particle[i].X = new int[nDim];
		Particle[i].P = new int[nDim];
		Particle[i].V = new int[nDim];
	}
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

int CPSOSolver::solve(int maxGenerations, int *cam)
{
	zuida = new int[maxGenerations];
	/*int n=0;  */
	int i = 1;
	//printf("Random Initialization of the swarm:\n\n");  
	RandInitofSwarm();
	//printf("Computation of the fitness of each particle:\n");  
	ComputFitofSwarm();
	//printf("FirstComputPandGbest:\n");  
	FirstComputPandGbest();

	zuida[0] = FitFunction(GBest);

	for (i = 1; i <= maxGenerations; i++)
	{
		/*printf("The %dth time to calculate .\n",n );  */
		//printf("Updated of the swarm:\n\n");  
		UpdateofVandX();
		//printf("Updated of the swarm's Fitness:\n");  
		ComputFitofSwarm();
		//printf("Replaced of P and Gbest:\n\n");  
		UpdatePandGbest();

		zuida[i] = FitFunction(GBest);
	}

	/*ofstream fout("DE200pc0.3.txt");*/
	ofstream fout("PSO600.txt");
	for (int i = 0; i<maxGenerations; i++)
	{
		fout << zuida[i] << " ";
	}
	fout << endl;
	fout.close();

	CopyVector(cam, GBest);
	return FitFunction(GBest);
	/*getchar();
	return 0;*/

}

//Random Initialization of the swarm  
void CPSOSolver::RandInitofSwarm(void)
{
	W = 1;
	C1 = 2;
	C2 = 2;
	Xdown = 0;
	Xup = Max;
	Vmax = 20;

	srand((unsigned)time(NULL));
	for (int i = 0; i<nPop; i++)
	{
		//printf(" The %dth of X is: ",i);  
		for (int j = 0; j<nDim; j++)
		{
			//Particle[i].X[j] = rand()/(double)RAND_MAX*(Xup-Xdown)+Xdown;      //-100~100  
			//Particle[i].V[j] = rand()/(double)RAND_MAX*Vmax*2-Vmax;                 //-2~2  
			Particle[i].X[j] = rand() % (Max + 1);
			Particle[i].V[j] = rand() % (Vmax * 2 + 1) - Vmax;
			//printf(" %.2f \n ",s->Particle[i].X[j]);  
		}
	}
}

//Computation of the fitness of each particle  
void CPSOSolver::ComputFitofSwarm(void)
{
	int i;
	/*srand((unsigned)time(NULL));  */
	for (i = 0; i<nPop; i++)
	{
		//printf(" The Fitness of %dth Particle: ",i);  
		Particle[i].Fitness = FitFunction(Particle[i].X);
		//printf(" %.2f\n",s->Particle[i].Fitness);  
	}
}

int CPSOSolver::FitFunction(int testSolution[])//8台摄像机能看到的三角面总个�?
{
	int fit = 0;
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
				fit++;
				list[j] = false;
			}
		}
	}
	if (list)
	{
		delete[]list;
		list = NULL;
	}
	return fit;
}

void CPSOSolver::FirstComputPandGbest(void)
{
	int i, j;
	//P=X;  
	for (i = 0; i<nPop; i++)
	{
		for (j = 0; j<nDim; j++)
		{
			Particle[i].P[j] = Particle[i].X[j];
		}
	}
	//Computation of GBest  
	GBestIndex = 0;
	for (i = 0; i<nPop; i++)
	if (Particle[i].Fitness >= Particle[GBestIndex].Fitness)
		GBestIndex = i;
	for (j = 0; j<nDim; j++)
	{
		GBest[j] = Particle[GBestIndex].P[j];
	}
	/*printf("GBestIndex , GBest , Fitness of GBest:%d ,%.2f ,%.2f ,%.2f \n",
	GBestIndex ,GBest[0],GBest[1],
	Particle[GBestIndex].Fitness);  */
}
//update  V and X   
void CPSOSolver::UpdateofVandX(void)
{
	int i, j;
	/*srand((unsigned)time(NULL));  */
	for (i = 0; i<nPop; i++)
	{
		//printf(" The %dth of X is: ",i);  
		for (j = 0; j<nDim; j++)
			Particle[i].V[j] = int(W*Particle[i].V[j] +
			rand() / (double)RAND_MAX*C1*(Particle[i].P[j] - Particle[i].X[j]) +
			rand() / (double)RAND_MAX*C2*(GBest[j] - Particle[i].X[j]));
		for (j = 0; j<nDim; j++)
		{
			if (Particle[i].V[j]>Vmax)
				Particle[i].V[j] = Vmax;
			if (Particle[i].V[j]<-Vmax)
				Particle[i].V[j] = -Vmax;
		}

		for (j = 0; j<nDim; j++)
		{
			Particle[i].X[j] += Particle[i].V[j];
			if ((Particle[i].X[j])>Xup)
				Particle[i].X[j] = Xup;
			if ((Particle[i].X[j])<Xdown)
				Particle[i].X[j] = Xdown;
		}
		//printf(" %.2f %.2f \n",s->Particle[i].X[0],s->Particle[i].X[1]);  
	}
}

/*static double ComputAFitness(double X[])
{
return X[0]*X[0]+X[1]*X[1]+3;
}*/
void CPSOSolver::UpdatePandGbest(void)
{
	int i, j;
	//update of P if the X is bigger than current P  
	for (i = 0; i < nPop; i++)
	{
		//printf(" The %dth of P is: ",i);  
		if (Particle[i].Fitness > FitFunction(Particle[i].P))
		{
			for (j = 0; j<nDim; j++)
			{
				Particle[i].P[j] = Particle[i].X[j];
			}
		}
		//printf(" %.2f %.2f \n",s->Particle[i].P[0],s->Particle[i].P[1]);  
	}
	//for (i = 0; i < nPop; i++)  
	//{  
	//    //printf("The %dth of P's Fitness is : %.2f  \n",i,ComputAFitness(s->Particle[i].P));  
	//}  
	//update of GBest  
	for (i = 0; i<nPop; i++)
	{
		if (FitFunction(Particle[i].P) >= FitFunction(Particle[GBestIndex].P))
		{
			GBestIndex = i;
		}
	}
	for (j = 0; j<nDim; j++)
	{
		GBest[j] = Particle[GBestIndex].P[j];
	}
	/* printf("GBestIndex , GBest , Fitness of GBest:%d ,%.2f ,%.2f ,%.2f \n",
	GBestIndex ,GBest[0],GBest[1],
	ComputAFitness(GBest));  */
}

/*double CPSOSolver::InertWeight(void)
{
return 1.0;
}
int CPSOSolver::CriteriaofStop()
{
int n=N;
return (n--==0);
} */
