#ifndef GREEDYSOLVER_H_
#define GREEDYSOLVER_H_
#include <vector>
using namespace std;
class CGreedSolver//greedy approach of selecting cameras
{
public:
	CGreedSolver(int D, int Num, vector<vector<int>>& CF);
	int solve(int* cam);
	int **cf;       //a copy of CF matrix
	int *triofevecam;//number of triangles covered by a camera
	int nDim;       //dimension of the solution
	int num;		//number of total candidate cameras
	int n;			//number of triangles covered by the D cameras
protected:
	int biggest(int* s);
	void clear(int s);
	void count();
};
#endif