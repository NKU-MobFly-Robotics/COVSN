#ifndef MYMATH_H_
#define MYMATH_H_

#include <vector>
#include "Camera.h"
#include "Vector3f.h"
#include "Triangle.h"
using namespace std;
//void Vectorcopy(Vector3f& t, Vector3f& s);

Vector3f normal(Vector3f v1, Vector3f v2, Vector3f v3);

float min(float a, float b);

float max(float a, float b);

Vector3f cross(Vector3f v1, Vector3f v2);

float dot(Vector3f v1, Vector3f v2);

Triangle transform(Triangle triangle, Camera camera);




#endif