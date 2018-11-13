#include "Mymath.h"

//void Vectorcopy(Vector3f& t, Vector3f& s)
//{
//	t.x = s.x;
//	t.y = s.y;
//	t.z = s.z;
//}

Vector3f normal(Vector3f v1, Vector3f v2, Vector3f v3)
{
	float x12, x23, y12, y23, z12, z23;
	Vector3f v;
	x12 = v2.x - v1.x;								 //AB.x
	x23 = v3.x - v2.x;								 //BC.x
	y12 = v2.y - v1.y;								 //AB.y
	y23 = v3.y - v2.y;								 //BC.y
	z12 = v2.z - v1.z;								 //AB.z
	z23 = v3.z - v2.z;								 //BC.z

	v.x = y12*z23 - z12*y23;					 //norm.x
	v.y = z12*x23 - x12*z23;					 //norm.y
	v.z = x12*y23 - y12*x23;					 //norm.z
	return v;
}

float min(float a, float b)
{
	if (a >= b)
		return b;
	else
		return a;
}

float max(float a, float b)
{
	if (a <= b)
		return b;
	else
		return a;
}

Vector3f cross(Vector3f v1, Vector3f v2)
{
	Vector3f v;
	v.x = v1.y*v2.z - v1.z*v2.y;
	v.y = v1.z*v2.x - v1.x*v2.z;
	v.z = v1.x*v2.y - v1.y*v2.x;
	return v;
}

float dot(Vector3f v1, Vector3f v2)
{
	float x;
	x = v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
	return x;
}

Triangle transform(Triangle triangle, Camera camera)
{
	Triangle tri;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	x1 = triangle.v1.x - camera.x;//Æ½ÒÆ
	y1 = triangle.v1.y - camera.y;
	z1 = triangle.v1.z - camera.z;
	x2 = triangle.v2.x - camera.x;
	y2 = triangle.v2.y - camera.y;
	z2 = triangle.v2.z - camera.z;
	x3 = triangle.v3.x - camera.x;
	y3 = triangle.v3.y - camera.y;
	z3 = triangle.v3.z - camera.z;
	tri.v1.x = -camera.sina* x1 + camera.cosa* y1;//Ğı×ª
	tri.v1.y = camera.cosa* camera.cosb* x1 + camera.sina* camera.cosb* y1 - camera.sinb* z1;
	tri.v1.z = -camera.cosa* camera.sinb* x1 - camera.sina* camera.sinb* y1 - camera.cosb* z1;
	tri.v2.x = -camera.sina* x2 + camera.cosa* y2;
	tri.v2.y = camera.cosa* camera.cosb* x2 + camera.sina* camera.cosb* y2 - camera.sinb* z2;
	tri.v2.z = -camera.cosa* camera.sinb* x2 - camera.sina* camera.sinb* y2 - camera.cosb* z2;
	tri.v3.x = -camera.sina* x3 + camera.cosa* y3;
	tri.v3.y = camera.cosa* camera.cosb* x3 + camera.sina* camera.cosb* y3 - camera.sinb* z3;
	tri.v3.z = -camera.cosa* camera.sinb* x3 - camera.sina* camera.sinb* y3 - camera.cosb* z3;
	return tri;
}

