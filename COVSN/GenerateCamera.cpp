#include "Mymath.h"
#include"Camera.h"
#include<stdio.h>
#include <vector>
void GenerateCamera(vector<Vector3f> &vertex, vector<Triangle> &triangle, vector<Camera> &camera,
	float A, float f, float Ca, float ZS, float su, float sv, float Ra, float u0, float v0, float w, float h, float Zn, float Zf)//获得平移矩阵和旋转矩阵
{

/*	A = m_A;
	f = m_f;
	Ca = m_Ca;
	ZS = m_ZS;
	su = m_su;
	sv = m_sv;
	Ra = m_Ra;
	u0 = m_uo;
	v0 = m_vo;
	w = m_w;
	h = m_h;*/
	/*Zn=A*f*ZS/(A*f+Ca*(ZS-f));
	Zr=f/(Ra* max(su,sv));
	Zf=A*f*ZS/(A*f-Ca*(ZS-f));*/
	float fn;
	Vector3f n;
	float x, y, z, sina, cosa, sinb, cosb;
	/*if(Zf<0)
	{
	Zf=Zr;
	}
	else
	{
	Zf=min(Zr,Zf);
	}*/
	//求三角面的法向量时，先得到两个向量AB,BC；两者的向量积 AB X BC 即为法向量
	Vector3f v1, v2, v3;
	for (int i = 0; i<triangle.size(); i++)
	{
		v1 = triangle[i].v1;//A.x
		v2 = triangle[i].v2;//B.x
		v3 = triangle[i].v3;//C.x

		n=normal(v1, v2, v3);

		fn=(sqrt(n.x*n.x + n.y*n.y + n.z*n.z));               //法向量的模
	
		x=(v1.x + v2.x + v3.x) / 3 + 0.5*(Zn + Zf) * n.x / fn;			//Ti=oi+kni
		y=(v1.y + v2.y + v3.y) / 3 + 0.5*(Zn + Zf) * n.y / fn;
		z=(v1.z + v2.z + v3.z) / 3 + 0.5*(Zn + Zf) * n.z / fn;
		sinb=sqrt((n.x*n.x + n.y*n.y) / (fn * fn));				  //Ri由sina,cosa,sinb,cosb表示
		cosb=n.z / fn;
		if (sinb != 0)
		{
			sina=(n.y / fn) / sinb;
			cosa=(n.x / fn) / sinb;
		}
		else
		{
			sina=0.0;
			cosa=1.0;
		}
		camera.push_back(Camera(x, y, z, sina, cosa, sinb, cosb));
	}
}
