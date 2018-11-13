#include "PlyLoader.h"
#include "Camera.h"
void SGDSS(vector<Vector3f> &vertex, vector<Triangle> &triangle, vector<Camera> &camera, vector<vector<int>> &CF,     //serial approach of Generating discrete solution space
	float A, float f, float Ca, float ZS, float su, float sv, float Ra, float u0, float v0, float w, float h, float Zn, float Zf);

void PGDSS(vector<Vector3f> &vertex, vector<Triangle> &triangle, vector<Camera> &camera, vector<vector<int>> &CF,//parallel approach of Generating discrete solution space
	float A, float f, float Ca, float ZS, float su, float sv, float Ra, float u0, float v0, float w, float h, float Zn, float Zf);