#ifndef TRIANGLE_H_
#define TRIANGLE_H_
#include "Vector3f.h"

struct Triangle{
	Vector3f v1;
	Vector3f v2;
	Vector3f v3;
	Triangle(){}
	Triangle(Vector3f a, Vector3f b, Vector3f c)
	{
		v1=a;
		v2=b;
		v3=c;
	}
};
#endif