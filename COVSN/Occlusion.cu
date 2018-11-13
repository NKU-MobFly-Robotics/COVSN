#include <stdio.h>
#include <stdlib.h>
#include <cuda_runtime.h> 
#include <time.h>
#include"PlyLoader.h"
#include <Windows.h>
#define DATA_SIZE 1024
#define NUM 512
#define checkCudaErrors(err)  __checkCudaErrors (err, __FILE__, __LINE__)
#define getLastCudaError(msg)  __getLastCudaError (msg, __FILE__, __LINE__)

int data[DATA_SIZE];

////////////////////////////////////////////////////////////////////////////////
// These are CUDA Helper functions

// This will output the proper CUDA error strings in the event that a CUDA host call returns an error


//inline void __checkCudaErrors(cudaError err, const char *file, const int line )
//{
//    if(cudaSuccess != err)
//    {
//        fprintf(stderr, "%s(%i) : CUDA Runtime API error %d: %s.\n",file, line, (int)err, cudaGetErrorString( err ) );
//        OutputDebugString("");
//	 return ;        
//    }
//}
//
//// This will output the proper error string when calling cudaGetLastError
//
//
//inline void __getLastCudaError(const char *errorMessage, const char *file, const int line )
//{
//    cudaError_t err = cudaGetLastError();
//    if (cudaSuccess != err)
//    {
//        fprintf(stderr, "%s(%i) : getLastCudaError() CUDA error : %s : (%d) %s.\n",
//        file, line, errorMessage, (int)err, cudaGetErrorString( err ) );
//        return ;
//    }
//}

// end of CUDA Helper Functions

//__global__ static void sumOfSquares(float * gpu_yourmother,float * gpuf,int *gpuresult,float * gpuG2)
/*__global__ static void sumOfSquares(float * gpuf,int *gpuresult,float * gpuG2)*/
__global__ static void sumOfSquares(float * gpuf, int *gpuresult)
{
	int idx = threadIdx.x;
	int tid = blockIdx.x;
	float x1 = gpuf[idx * 9];
	float y1 = gpuf[idx * 9 + 1];
	float z1 = gpuf[idx * 9 + 2];
	float x2 = gpuf[idx * 9 + 3];
	float y2 = gpuf[idx * 9 + 4];
	float z2 = gpuf[idx * 9 + 5];
	float x3 = gpuf[idx * 9 + 6];
	float y3 = gpuf[idx * 9 + 7];
	float z3 = gpuf[idx * 9 + 8];//遮挡三角形三个点坐标

	//vertex v0,v1,v2;
	//v0.x=gpuf[idx*9];
	//v0.y=gpuf[idx*9+1];
	//v0.z=gpuf[idx*9+2];
	//v1.x=gpuf[idx*9+3];
	//v1.y=gpuf[idx*9+4];
	//v1.z=gpuf[idx*9+5];
	//v2.x=gpuf[idx*9+6];
	//v2.y=gpuf[idx*9+7];
	//v2.z=gpuf[idx*9+8];//遮挡三角形三个点坐标

	//vertex E1,E2;
	//E1.x=v1.x-v0.x;
	//E1.y=v1.y-v0.y;
	//E1.z=v1.z-v0.z;
	//E2.x=v2.x-v0.x;
	//E2.y=v2.y-v0.y;
	//E2.z=v2.z-v0.z;
	/*vertex v;*/
	float x12 = x2 - x1;
	float y12 = y2 - y1;
	float z12 = z2 - z1;
	float x13 = x3 - x1;
	float y13 = y3 - y1;
	float z13 = z3 - z1;

	/*gpuG2[idx*6]=x12;
	gpuG2[idx*6+1]=y12;
	gpuG2[idx*6+2]=z12;
	gpuG2[idx*6+3]=x13;
	gpuG2[idx*6+4]=y13;
	gpuG2[idx*6+5]=z13;*/

	float x01 = gpuf[tid * 9];
	float y01 = gpuf[tid * 9 + 1];
	float z01 = gpuf[tid * 9 + 2];
	float x02 = gpuf[tid * 9 + 3];
	float y02 = gpuf[tid * 9 + 4];
	float z02 = gpuf[tid * 9 + 5];
	float x03 = gpuf[tid * 9 + 6];
	float y03 = gpuf[tid * 9 + 7];
	float z03 = gpuf[tid * 9 + 8];//被遮挡三角形三个点坐标

	//vertex P1,P2,P3;
	//P1.x=y01*E2.z-z01*E2.y;
	//P1.y=z01*E2.x-x01*E2.z;
	//P1.z=x01*E2.y-y01*E2.x;

	//P2.x=y02*E2.z-z02*E2.y;
	//P2.y=z02*E2.x-x02*E2.z;
	//P2.z=x02*E2.y-y02*E2.x;

	//P3.x=y03*E2.z-z03*E2.y;
	//P3.y=z03*E2.x-x03*E2.z;
	//P3.z=x03*E2.y-y03*E2.x;
	float P1x = y01*z13 - z01*y13;
	float P1y = z01*x13 - x01*z13;
	float P1z = x01*y13 - y01*x13;

	float P2x = y02*z13 - z02*y13;
	float P2y = z02*x13 - x02*z13;
	float P2z = x02*y13 - y02*x13;

	float P3x = y03*z13 - z03*y13;
	float P3y = z03*x13 - x03*z13;
	float P3z = x03*y13 - y03*x13;


	float Qx = z1*y12 - y1*z12;
	float Qy = x1*z12 - z1*x12;
	float Qz = y1*x12 - x1*y12;

	float t1 = (Qx*x13 + Qy*y13 + Qz*z13) / (P1x*x12 + P1y*y12 + P1z*z12);
	float u1 = (-P1x*x1 - P1y*y1 - P1z*z1) / (P1x*x12 + P1y*y12 + P1z*z12);
	float v1 = (Qx*x01 + Qy*y01 + Qz*z01) / (P1x*x12 + P1y*y12 + P1z*z12);

	float t2 = (Qx*x13 + Qy*y13 + Qz*z13) / (P2x*x12 + P2y*y12 + P2z*z12);
	float u2 = (-P2x*x1 - P2y*y1 - P2z*z1) / (P2x*x12 + P2y*y12 + P2z*z12);
	float v2 = (Qx*x02 + Qy*y02 + Qz*z02) / (P2x*x12 + P2y*y12 + P2z*z12);

	float t3 = (Qx*x13 + Qy*y13 + Qz*z13) / (P3x*x12 + P3y*y12 + P3z*z12);
	float u3 = (-P3x*x1 - P3y*y1 - P3z*z1) / (P3x*x12 + P3y*y12 + P3z*z12);
	float v3 = (Qx*x03 + Qy*y03 + Qz*z03) / (P3x*x12 + P3y*y12 + P3z*z12);
	
	if ((x1 == x01&&y1 == y01&&z1 == z01) || (x1 == x02&&y1 == y02&&z1 == z02) || (x1 == x03&&y1 == y03&&z1 == z03) ||
		(x2 == x01&&y2 == y01&&z2 == z01) || (x2 == x02&&y2 == y02&&z2 == z02) || (x2 == x03&&y2 == y03&&z2 == z03) ||
		(x3 == x01&&y3 == y01&&z3 == z01) || (x3 == x02&&y3 == y02&&z3 == z02) || (x3 == x03&&y3 == y03&&z3 == z03))
	{//如果有公共点的话，则没被遮挡
		gpuresult[tid*blockDim.x + idx] = 1;
	}
	else
	{
		if (((t1>0) && (t1<1) && (u1>0) && (v1>0) && (u1 + v1)<1) ||
			((t2>0) && (t2<1) && (u2>0) && (v2>0) && (u2 + v2)<1) ||
			((t3>0) && (t3<1) && (u3>0) && (v3>0) && (u3 + v3)<1))//被遮挡
		{
			gpuresult[tid*blockDim.x + idx] = 0;
		}
		else
		{
			gpuresult[tid*blockDim.x + idx] = 1;
		}
	}

	//gpuresult[j*blockDim.x+i]=(a1<0||a1>1||b1<0||b1>1||c1<0||c1>1)&&(a2<0||a2>1||b2<0||b2>1||c2<0||c2>1)&&(a3<0||a3>1||b3<0||b3>1||c3<0||c3>1);//被遮挡
}

/*extern "C" void shelter(int camera_index,float*f, int num,int *result,float *G2)*/
extern "C" void occlusion(float *f, int num, vector<int> &result)
{
	cudaSetDevice(0);
	float * gpuf;
	int * gpuresult;
	int * FF;
	
	FF = (int*)malloc(sizeof(int)*num*num);

	clock_t c1 = clock();

	cudaMalloc((void**)&gpuf, sizeof(float)* 9 * num);

	clock_t c2 = clock();
	int a = c2 - c1;


	cudaMalloc((void**)&gpuresult, sizeof(int)*num*num);


	/*checkCudaErrors(cudaMemcpy(gpuf, f, sizeof(float)*9*num,cudaMemcpyHostToDevice));*/
	cudaMemcpy(gpuf, f, sizeof(float)* 9 * num, cudaMemcpyHostToDevice);

	sumOfSquares << <num, num >> >(gpuf, gpuresult);

	cudaMemcpy(FF, gpuresult, sizeof(int)*num*num, cudaMemcpyDeviceToHost);

	cudaFree(gpuf);
	cudaFree(gpuresult);

	for (int s = 0; s<num; s++)
	{
		for (int t = 0; t<num; t++)
		{
			FF[s*num] = FF[s*num + t] && FF[s*num];
		}
		result[s] = result[s] && FF[s*num];
	}
	if (FF)
	{
		delete FF;
		FF = NULL;
	}
	
}


