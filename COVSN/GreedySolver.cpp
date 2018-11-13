#include "GreedySolver.h"
CGreedSolver::CGreedSolver(int D, int Num, vector<vector<int>>& CF)
{
	n = 0;
	nDim = D;
	num = Num;
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
	triofevecam = new int[num];
	for (int i = 0; i<num; i++)
	{
		triofevecam[i] = 0;
	}
}


int CGreedSolver::solve(int* cam)
{
	for (int i = 0; i<nDim; i++)
	{
		count();
		cam[i] = biggest(triofevecam);
		clear(cam[i]);
	}
	return n;
}

int CGreedSolver::biggest(int* s) // select the camera which covers maximum triangles
{
	int r = 0;
	int big = 0;
	for (int i = 0; i<num; i++)
	{
		if (s[i]>big)
		{
			r = i;
			big = s[i];
		}
	}
	n += big;
	return r;
}

void CGreedSolver::clear(int s)  //triangles covered by the selected camera will be deleted from other cameras' FOV 
{
	for (int j = 0; j<num; j++)
	{
		if (cf[s][j] == 1)
		{
			for (int i = 0; i<num; i++)
			{
				cf[i][j] = 0;
			}
		}
	}
}

void CGreedSolver::count()//count the number of triangles covered by each camera
{
	for (int i = 0; i<num; i++)
	{
		triofevecam[i] = 0;
	}
	for (int i = 0; i<num; i++)
	{
		for (int j = 0; j<num; j++)
		{
			triofevecam[i] += cf[i][j];
		}
	}
}