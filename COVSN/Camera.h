#ifndef CAMERA_H_
#define CAMERA_H_
#include <vector>
#include "Vector3f.h"
#include "Triangle.h"
using namespace std;

struct Camera  
{
	float x;
	float y;
	float z;
	float sina;
	float cosa;
	float sinb;
	float cosb;
	Camera(){}
	Camera(float a, float b, float c, float d, float e, float f, float g)
	{
		x = a;
		y = b;
		z = c;
		sina = d;
		cosa = e;
		sinb = f;
		cosb = g;
	}
};

void  GenerateCamera(vector<Vector3f> &vertex, vector<Triangle> &triangle, vector<Camera>& camera,
	float A, float f, float Ca, float ZS, float su, float sv, float Ra, float u0, float v0, float w, float h, float Zn, float Zf);//获得平移矩阵和旋转矩阵

#endif