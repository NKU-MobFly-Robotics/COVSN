#ifndef PLYREADER_H_
#define PLYREADER_H_
//#include <GL/glut.h>
//#include <GL/glu.h>
//#include <GL/gl.h>
#include <stdio.h>
#include "Vector3f.h"
#include "Triangle.h"
#include <vector>
//#include "Creatstruct.h"
using namespace std;

struct SModelData
{
	vector <float> vecFaceTriangles; // = face * 9
	vector <float> vecFaceTriangleColors; // = face * 9
	vector <float> vecNormals; // = face * 9
	vector <float> vecEdges;
	vector <float> vecEdgeColors;
	vector <float> vecEdgeNormals;
	int iTotalConnectedTriangles;
	int iTotalConnectededges;
};

class CPLYLoader
{
public:
	CPLYLoader();
	int LoadModel(char *filename, vector<Vector3f> &vertex, vector<Triangle> &triangle);
	//void Draw();
	//private:
	float* mp_vertexXYZ;
	//float* mp_vertexNorm;
	//float* mp_vertexRGB;
	//int m_totalConnectedQuads;
	int m_totalConnectedPoints;		//number of vertexes
	int m_totalFaces;				//number of faces
	SModelData m_ModelData;
};
#endif