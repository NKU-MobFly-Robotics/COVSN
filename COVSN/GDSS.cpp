#include "Mymath.h"
#include "Camera.h"
#include "time.h"

//extern "C" void conv(CPLYLoader plyLoader,float x,float y,float z,
//						   float sina,float cosa,float sinb,float cosb,vertex* xyz);
//extern "C" void shelter(float*f1,float*f2,float*f3,float*f, int num);
extern "C" void occlusion(float *f, int num, vector<int>& result);
void SGDSS(vector<Vector3f> &vertex, vector<Triangle> &triangle, vector<Camera> &camera, vector<vector<int>> &CF,
	float A, float f, float Ca, float ZS, float su, float sv, float Ra, float u0, float v0, float w, float h, float Zn, float Zf)
{
	vector<Triangle> transtri;
	float  a1, a2, a3, b1, b2, b3, c1, c2, c3;
	Vector3f v1, v2, v3, n, E1, E2, T, P1, P2, P3, Q;
	//float biaozhi = 0;
	//camnum = triangle.size();
	//for(int i=0;i<380;i++)
	//{
	//	k2[i]=-1;
	//}
	vector<int> cf; 
	for (int i = 0; i < triangle.size(); i++)
	{
		cf.push_back(0);
	}
	for (int i = 0; i < triangle.size(); i++)//initialize the CF matrix
	{
		CF.push_back(cf);
	}

	for (int i = 0; i<triangle.size(); i++)
	{
		transtri.clear();
		/*if(i==4)
		{
		int yourmother;
		yourmother=0;
		}*/
		for (int j = 0; j<triangle.size(); j++)//转换为摄像机下的坐标
		{
			transtri.push_back(transform(triangle[j], camera[i]));
			//计算第j个三角形在第i个摄像机下的坐标
		}
		for (int j = 0; j<triangle.size(); j++)
		{
			v1 = transtri[j].v1;
			v2 = transtri[j].v2;
			v3 = transtri[j].v3;
			n = normal(v1, v2, v3);
			if (v1.x / v1.z>(su*u0 / f) || v1.x / v1.z<-(su*(w - u0) / f) ||
				v1.y / v1.z>(sv*v0 / f) || v1.y / v1.z<-(sv*(h - v0) / f) ||
				v2.x / v2.z>(su*u0 / f) || v2.x / v2.z<-(su*(w - u0) / f) ||
				v2.y / v2.z>(sv*v0 / f) || v2.y / v2.z<-(sv*(h - v0) / f) ||
				v3.x / v3.z>(su*u0 / f) || v3.x / v3.z<-(su*(w - u0) / f) ||
				v3.y / v3.z>(sv*v0 / f) || v3.y / v3.z<-(sv*(h - v0) / f))//视野约束
			{
				CF[i][j] = 0;
			}
			else
			{
				if (f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v1.z*max(su, sv))<Ra ||
					f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v2.z*max(su, sv))<Ra ||
					f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v3.z*max(su, sv))<Ra)//分辨率约束
				{
					CF[i][j] = 0;
				}
				else
				{
					if (v1.z<Zn || v1.z>Zf ||
						v2.z<Zn || v2.z>Zf ||
						v3.z<Zn || v3.z>Zf)//模糊程度约束
					{
						CF[i][j] = 0;
					}
					else
					{
						CF[i][j] = 1;
					}
				}
			}
		}

		for (int j = 0; j<triangle.size(); j++)//转换为摄像机下的坐标
		{
			v1 = transtri[j].v1;
			v2 = transtri[j].v2;
			v3 = transtri[j].v3;
			/*	v2=v[plyLoader.t[j].vertex2];
			v3=v[plyLoader.t[j].vertex3];*/
			for (int k = 0; k<triangle.size(); k++)
			{
				if (!((transtri[k].v1.x == v1.x&&transtri[k].v1.y == v1.y&&transtri[k].v1.z == v1.z) ||
					(transtri[k].v2.x == v1.x&&transtri[k].v2.y == v1.y&&transtri[k].v2.z == v1.z) ||
					(transtri[k].v3.x == v1.x&&transtri[k].v3.y == v1.y&&transtri[k].v3.z == v1.z) ||
					(transtri[k].v1.x == v2.x&&transtri[k].v1.y == v2.y&&transtri[k].v1.z == v2.z) ||
					(transtri[k].v2.x == v2.x&&transtri[k].v2.y == v2.y&&transtri[k].v2.z == v2.z) ||
					(transtri[k].v3.x == v2.x&&transtri[k].v3.y == v2.y&&transtri[k].v3.z == v2.z) ||
					(transtri[k].v1.x == v3.x&&transtri[k].v1.y == v3.y&&transtri[k].v1.z == v3.z) ||
					(transtri[k].v2.x == v3.x&&transtri[k].v2.y == v3.y&&transtri[k].v2.z == v3.z) ||
					(transtri[k].v3.x == v3.x&&transtri[k].v3.y == v3.y&&transtri[k].v3.z == v3.z)))
				{
					E1.x = transtri[k].v2.x - transtri[k].v1.x;
					E1.y = transtri[k].v2.y - transtri[k].v1.y;
					E1.z = transtri[k].v2.z - transtri[k].v1.z;

					E2.x = transtri[k].v3.x - transtri[k].v1.x;
					E2.y = transtri[k].v3.y - transtri[k].v1.y;
					E2.z = transtri[k].v3.z - transtri[k].v1.z;

					/*G1[k * 6] = E1.x;
					G1[k * 6 + 1] = E1.y;
					G1[k * 6 + 2] = E1.z;
					G1[k * 6 + 3] = E2.x;
					G1[k * 6 + 4] = E2.y;
					G1[k * 6 + 5] = E2.z;*/

					T.x = -transtri[k].v1.x;
					T.y = -transtri[k].v1.y;
					T.z = -transtri[k].v1.z;

					Q = cross(T, E1);

					P1 = cross(v1, E2);
					a1 = dot(Q, E2) / dot(P1, E1);
					b1 = dot(P1, T) / dot(P1, E1);
					c1 = dot(Q, v1) / dot(P1, E1);

					P2 = cross(v2, E2);
					a2 = dot(Q, E2) / dot(P2, E1);
					b2 = dot(P2, T) / dot(P2, E1);
					c2 = dot(Q, v2) / dot(P2, E1);

					P3 = cross(v3, E2);
					a3 = dot(Q, E2) / dot(P3, E1);
					b3 = dot(P3, T) / dot(P3, E1);
					c3 = dot(Q, v3) / dot(P3, E1);
					if (((a1>0) && (a1<1) && (b1>0) && (c1>0) && ((b1 + c1)<1)) ||
						((a2>0) && (a2<1) && (b2>0) && (c2>0) && ((b2 + c2)<1)) ||
						((a3>0) && (a3<1) && (b3>0) && (c3>0) && ((b3 + c3)<1)))//被遮挡
					{
						if (CF[i][j] != 0)
						{
							CF[i][j] = 0;
						}
					}
				}
			}
		}

	}

	//t = 0;
	/*for(int s=0;s<plyLoader.m_totalFaces;s++)
	{
	if(CF[s][s])
	t++;
	}

	for(int s=0;s<plyLoader.m_totalFaces;s++)
	{
	KF[s]=0;
	for(int t=0;t<plyLoader.m_totalFaces;t++)
	{
	KF[s]+=CF[s][t];
	}
	}*/
}



void PGDSS(vector<Vector3f> &vertex, vector<Triangle> &triangle, vector<Camera> &camera, vector<vector<int>> &CF,
	float A, float f, float Ca, float ZS, float su, float sv, float Ra, float u0, float v0, float w, float h, float Zn, float Zf)
{
	vector<int> CF2;
	vector<Triangle> transtri;
	vector<int> record;
	float  a1, a2, a3, b1, b2, b3, c1, c2, c3;
	Vector3f v1, v2, v3, n, E1, E2, T, P1, P2, P3, Q;
	//float biaozhi = 0;
	//camnum = triangle.size();
	//for(int i=0;i<380;i++)
	//{
	//	k2[i]=-1;
	//}
	vector<int> cf;
	for (int i = 0; i < triangle.size(); i++)
	{
		cf.push_back(0);
	}
	for (int i = 0; i < triangle.size(); i++)//initialize the CF matrix
	{
		CF.push_back(cf);
	}

	for (int i = 0; i < triangle.size(); i++)
	{
		record.clear();
		transtri.clear();
		/*if(i==4)
		{
		int yourmother;
		yourmother=0;
		}*/
		for (int j = 0; j < triangle.size(); j++)//转换为摄像机下的坐标
		{
			transtri.push_back(transform(triangle[j], camera[i]));
			//计算第j个三角形在第i个摄像机下的坐标
		}
		for (int j = 0; j<triangle.size(); j++)
		{
			v1 = transtri[j].v1;
			v2 = transtri[j].v2;
			v3 = transtri[j].v3;
			n = normal(v1, v2, v3);
			if (v1.x / v1.z>(su*u0 / f) || v1.x / v1.z<-(su*(w - u0) / f) ||
				v1.y / v1.z>(sv*v0 / f) || v1.y / v1.z<-(sv*(h - v0) / f) ||
				v2.x / v2.z>(su*u0 / f) || v2.x / v2.z<-(su*(w - u0) / f) ||
				v2.y / v2.z>(sv*v0 / f) || v2.y / v2.z<-(sv*(h - v0) / f) ||
				v3.x / v3.z>(su*u0 / f) || v3.x / v3.z<-(su*(w - u0) / f) ||
				v3.y / v3.z>(sv*v0 / f) || v3.y / v3.z < -(sv*(h - v0) / f))//视野约束
			{
				CF[i][j] = 0;
			}
			else
			{
				record.push_back(j);
				if (f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v1.z*max(su, sv)) < Ra ||
					f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v2.z*max(su, sv)) < Ra ||
					f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v3.z*max(su, sv)) < Ra)//分辨率约束
				{
					CF[i][j] = 0;
				}
				else
				{
					if (v1.z<Zn || v1.z>Zf ||
						v2.z<Zn || v2.z>Zf ||
						v3.z<Zn || v3.z>Zf)//模糊程度约束
					{
						CF[i][j] = 0;
					}
					else
					{
						CF[i][j] = 1;
					}
				}
			}
		}
		float* flo;
		flo = new float[5000];
		
		for (int j = 0, k = 0; j<record.size();)
		{
			flo[k] = transtri[j].v1.x;
						flo[k + 1] = transtri[j].v1.y;
						flo[k + 2] = transtri[j].v1.z;
						flo[k + 3] = transtri[j].v2.x;
						flo[k + 4] = transtri[j].v2.y;
						flo[k + 5] = transtri[j].v2.z;
						flo[k + 6] = transtri[j].v3.x;
						flo[k + 7] = transtri[j].v3.y;
						flo[k + 8] = transtri[j].v3.z;
			j += 1;
			k += 9;
		}
		for (int k = 0; k<record.size(); k++)
		{
			CF2.push_back(1);
		}
		
		occlusion(flo, record.size(), CF2);
		
		for (int k = 0; k<record.size(); k++)
		{
			CF[i][record[k]] = CF[i][record[k]] && CF2[k];
		}
		CF2.clear();
	}
}



//void Ccheck::GetCF3(CPLYLoader* plyLoader, Ccamera* camera, int CF[][2000])
//{
//	float x1, y1, z1, x2, y2, z2, x3, y3, z3, a1, a2, a3, b1, b2, b3, c1, c2, c3;
//	camnum = triangle.size();
//	vertex v1, v2, v3, n, E1, E2, T, P, Q;
//
//	for (int i = 0; i<triangle.size(); i++)
//	{
//		for (int k = 0; k<triangle.size(); k++)
//		{
//			record[k] = -1;
//		}
//		int s = 0;
//		for (int j = 0; j<triangle.size(); j++)//转换为摄像机下的坐标
//		{
//			//计算第j个三角形在第i个摄像机下的坐标
//			x1 = plyLoader->v[plyLoader->t[j].vertex1].x - camera->x[i];//平移
//			y1 = plyLoader->v[plyLoader->t[j].vertex1].y - camera->y[i];
//			z1 = plyLoader->v[plyLoader->t[j].vertex1].z - camera->z[i];
//			x2 = plyLoader->v[plyLoader->t[j].vertex2].x - camera->x[i];
//			y2 = plyLoader->v[plyLoader->t[j].vertex2].y - camera->y[i];
//			z2 = plyLoader->v[plyLoader->t[j].vertex2].z - camera->z[i];
//			x3 = plyLoader->v[plyLoader->t[j].vertex3].x - camera->x[i];
//			y3 = plyLoader->v[plyLoader->t[j].vertex3].y - camera->y[i];
//			z3 = plyLoader->v[plyLoader->t[j].vertex3].z - camera->z[i];
//			v[plyLoader->t[j].vertex1].x = -camera[i].sina* x1 + camera[i].cosa* y1;//旋转
//			v[plyLoader->t[j].vertex1].y = camera[i].cosa* camera[i].cosb* x1 + camera[i].sina* camera[i].cosb* y1 - camera[i].sinb* z1;
//			v[plyLoader->t[j].vertex1].z = -camera[i].cosa* camera[i].sinb* x1 - camera[i].sina* camera[i].sinb* y1 - camera[i].cosb* z1;
//			v[plyLoader->t[j].vertex2].x = -camera[i].sina* x2 + camera[i].cosa* y2;
//			v[plyLoader->t[j].vertex2].y = camera[i].cosa* camera[i].cosb* x2 + camera[i].sina* camera[i].cosb* y2 - camera[i].sinb* z2;
//			v[plyLoader->t[j].vertex2].z = -camera[i].cosa* camera[i].sinb* x2 - camera[i].sina* camera[i].sinb* y2 - camera[i].cosb* z2;
//			v[plyLoader->t[j].vertex3].x = -camera[i].sina* x3 + camera[i].cosa* y3;
//			v[plyLoader->t[j].vertex3].y = camera[i].cosa* camera[i].cosb* x3 + camera[i].sina* camera[i].cosb* y3 - camera[i].sinb* z3;
//			v[plyLoader->t[j].vertex3].z = -camera[i].cosa* camera[i].sinb* x3 - camera[i].sina* camera[i].sinb* y3 - camera[i].cosb* z3;
//		}
//		for (int j = 0; j<triangle.size(); j++)
//		{
//			if (i == 1 && j == 1)
//			{
//				int msda = 0;
//			}
//			v1 = v[plyLoader->t[j].vertex1];
//			v2 = v[plyLoader->t[j].vertex2];
//			v3 = v[plyLoader->t[j].vertex3];
//			n = normal(v1, v2, v3);
//			if (v1.x / v1.z>(su*camera->u0 / f) || v1.x / v1.z<-(su*(camera->w - camera->u0) / f) ||
//				v1.y / v1.z>(sv*camera->v0 / f) || v1.y / v1.z<-(sv*(camera->h - camera->v0) / f) ||
//				v2.x / v2.z>(su*camera->u0 / f) || v2.x / v2.z<-(su*(camera->w - camera->u0) / f) ||
//				v2.y / v2.z>(sv*camera->v0 / f) || v2.y / v2.z<-(sv*(camera->h - camera->v0) / f) ||
//				v3.x / v3.z>(su*camera->u0 / f) || v3.x / v3.z<-(su*(camera->w - camera->u0) / f) ||
//				v3.y / v3.z>(sv*camera->v0 / f) || v3.y / v3.z<-(sv*(camera->h - camera->v0) / f))//视野约束
//			{
//				CF[i][j] = 0;
//			}
//			else
//			{
//				record[s++] = j;//记录满足视野约束的三角面序号
//				if (f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v1.z*max(su, sv))<Ra ||
//					f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v2.z*max(su, sv))<Ra ||
//					f*(-n.z / sqrt(n.x*n.x + n.y*n.y + n.z*n.z)) / (v3.z*max(su, sv))<Ra)//分辨率约束
//				{
//					CF[i][j] = 0;
//				}
//				else
//				{
//					if (v1.z<camera->Zn || v1.z>camera->Zf ||
//						v2.z<camera->Zn || v2.z>camera->Zf ||
//						v3.z<camera->Zn || v3.z>camera->Zf)//模糊程度约束
//					{
//						CF[i][j] = 0;
//					}
//					else//遮挡约束
//					{
//						CF[i][j] = 1;
//					}
//
//				}
//			}
//		}
//		float f[18000];
//		for (int j = 0, k = 0; j<s;)
//		{
//			f[k] = transtri[j].v1.x;
//			f[k + 1] = transtri[j].v1.y;
//			f[k + 2] = transtri[j].v1.z;
//			f[k + 3] = transtri[j].v2.x;
//			f[k + 4] = transtri[j].v2.y;
//			f[k + 5] = transtri[j].v2.z;
//			f[k + 6] = transtri[j].v3.x;
//			f[k + 7] = transtri[j].v3.y;
//			f[k + 8] = transtri[j].v3.z;
//			j += 1;
//			k += 9;
//		}
//		for (int k = 0; k<s; k++)
//		{
//			CF2[k] = 1;
//		}
//		shelter(f, s, CF2);
//		for (int k = 0; k<s; k++)
//		{
//			CF[i][record[k]] = CF[i][record[k]] && CF2[k];
//		}
//	}
//
//
//	t = 0;
//	for (int s = 0; s<triangle.size(); s++)
//	{
//		if (CF[s][s])
//			t++;
//	}
//
//	for (int s = 0; s<triangle.size(); s++)
//	{
//		KF[s] = 0;
//		for (int t = 0; t<triangle.size(); t++)
//		{
//			KF[s] += CF[s][t];
//		}
//	}
//}







