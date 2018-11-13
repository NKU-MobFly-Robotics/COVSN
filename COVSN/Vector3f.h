#ifndef VECTOR3F_H_
#define VECTOR3F_H_

struct Vector3f{
	float x;
	float y;
	float z;
	Vector3f(){}
	Vector3f(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
};
#endif