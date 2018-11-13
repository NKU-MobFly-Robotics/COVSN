
#include "BIPSolver.h"
#include <iostream>
using namespace std;
CBIPSolver::CBIPSolver(int num, vector<vector<int>>&CF, int N_DIM)
{
	n_dim = N_DIM;
	max = num;
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

int CBIPSolver::solve(int * cam)
{
	try {
		GRBEnv env = GRBEnv();

		GRBModel model = GRBModel(env);

		// Create variables

		GRBVar *x = new GRBVar[max];
		for (int i = 0; i<max; i++)
		{
			x[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
		}
		GRBVar *y = new GRBVar[max];
		for (int i = 0; i<max; i++)
		{
			y[i] = model.addVar(0.0, 1.0, 0.0, GRB_BINARY);
		}

		// Integrate new variables

		model.update();

		// Set objective: maximize x + y + 2 z

		GRBLinExpr obj1 = 0.0;
		for (int i = 0; i<max; i++)
		{
			obj1 += y[i];
		}
		model.setObjective(obj1, GRB_MINIMIZE);

		// Add constraint: x + 2 y + 3 z <= 4

		GRBLinExpr obj2 = 0.0;
		for (int i = 0; i<max; i++)
		{
			for (int j = 0; j<max; j++)
			{
				obj2 += x[j] * cf[j][i];
			}
			obj2 += y[i];
			model.addConstr(obj2, GRB_GREATER_EQUAL, 1);
			obj2 = 0.0;
		}

		GRBLinExpr obj3 = 0.0;
		for (int i = 0; i<max; i++)
		{
			obj3 += x[i];
		}
		model.addConstr(obj3, GRB_EQUAL, n_dim);

		// Optimize model

		model.optimize();

		int j = 0;
		for (int i = 0; i<max; i++)
		{
			if (x[i].get(GRB_DoubleAttr_X) == 1)
			{
				cam[j] = i;
				j++;
			}
		}
		/*int a[380];
		for(int i=0;i<max;i++)
		{
		a[i]=y[i].get(GRB_DoubleAttr_X);
		}*/
		return	(max - model.get(GRB_DoubleAttr_ObjVal));

	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
	catch (...) {
		cout << "Exception during optimization" << endl;
	}

}