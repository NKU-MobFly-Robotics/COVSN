#include "PlyLoader.h"
#include "Mymath.h"
CPLYLoader::CPLYLoader()
{
	//this->m_totalConnectedQuads = 0;
	this->m_totalConnectedPoints = 0;
	m_ModelData.iTotalConnectedTriangles = 0;
	m_ModelData.iTotalConnectededges = 0;
}

int CPLYLoader::LoadModel(char *filename, vector<Vector3f> &vertex, vector<Triangle> &triangle)
{
	printf("Loading %s...\n", filename);
	char* pch = strstr(filename, ".ply");

	if (pch != NULL)
	{
		errno_t err;
		FILE* file;
		err = fopen_s(&file, filename, "r");
		if (err)
		{
			printf("load PLY file %s failed\n", filename);
			return false;
		}
		fseek(file, 0, SEEK_END);
		long fileSize = ftell(file);

		try
		{
			mp_vertexXYZ = (float*)malloc(ftell(file));
			//mp_vertexNorm = (float*)malloc(ftell(file));
			//mp_vertexRGB = (float*)malloc(ftell(file));
		}
		catch (char*)
		{
			return -1;
		}
		if (mp_vertexXYZ == NULL) return -1;
		//if (mp_vertexNorm == NULL) return -2;
		//if (mp_vertexRGB == NULL) return -3;
		fseek(file, 0, SEEK_SET);

		if (file)
		{
			int i = 0;
			int temp = 0;
			//int quads_index = 0;
			//int triangle_index = 0;
			//int normal_index = 0;
			//int colorIndex = 0;
			char buffer[1000];


			fgets(buffer, 300, file);         // ply


			// READ HEADER
			// -----------------

			// Find number of vertexes
			while (strncmp("element vertex", buffer, strlen("element vertex")) != 0)
			{
				fgets(buffer, 300, file);         // format
			}
			strcpy_s(buffer, buffer + strlen("element vertex"));
			sscanf_s(buffer, "%i", &this->m_totalConnectedPoints);

			// Find number of faces
			fseek(file, 0, SEEK_SET);
			while (strncmp("element face", buffer, strlen("element face")) != 0)
			{
				fgets(buffer, 300, file);         // format
			}
			strcpy_s(buffer, buffer + strlen("element face"));
			sscanf_s(buffer, "%i", &this->m_totalFaces);


			// go to end_header
			while (strncmp("end_header", buffer, strlen("end_header")) != 0)
			{
				fgets(buffer, 300, file);         // format
			}

			//----------------------


			// read vertices
			i = 0;
			for (int iterator = 0; iterator < this->m_totalConnectedPoints; iterator++)//3488
			{
				char tmp[1];
				fgets(buffer, 300, file);

//				sscanf_s(buffer, "%f %f %f %f %f %f %c %f %f %f", &mp_vertexXYZ[i], &mp_vertexXYZ[i + 1],
//					&mp_vertexXYZ[i + 2], &mp_vertexNorm[i], &mp_vertexNorm[i + 1], &mp_vertexNorm[i + 2],
//					tmp, sizeof(char), &mp_vertexRGB[i], &mp_vertexRGB[i + 1], &mp_vertexRGB[i + 2]);
				sscanf_s(buffer,"%f %f %f", &mp_vertexXYZ[i], &mp_vertexXYZ[i+1],
				&mp_vertexXYZ[i+2]);
				/*mp_vertexRGB[i]=100;
				mp_vertexRGB[i+1]=100;
				mp_vertexRGB[i+2]=100;*/
				vertex.push_back(Vector3f(mp_vertexXYZ[i], mp_vertexXYZ[i + 1], mp_vertexXYZ[i + 2]));
				i += 3;
			}


			//Ccreatstruct cre;
			//for(int j=0;j<m_totalConnectedPoints;j++)
			//{
			//	/*v[i]=new vertex;*/
			//	cre.v[j].x=mp_vertexXYZ[3*j];
			//	cre.v[j].y=mp_vertexXYZ[3*j+1];
			//	cre.v[j].z=mp_vertexXYZ[3*j+2];
			//}
			//



			// read faces
			i = 0;
			for (int iterator = 0; iterator < this->m_totalFaces; iterator++)//6920
			{
				fgets(buffer, 300, file);

				if (buffer[0] == '3')
				{
					int vertex1 = 0, vertex2 = 0, vertex3 = 0;
					buffer[0] = ' ';
					sscanf_s(buffer, "%i%i%i", &vertex1, &vertex2, &vertex3);//number of vertex eg:5,7,6


					/*v[i]=new vertex;*/
					triangle.push_back(Triangle(vertex[vertex1],vertex[vertex2],vertex[vertex3]));
		

				/*	//三角面 点的坐标
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex1]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex1 + 1]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex1 + 2]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex2]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex2 + 1]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex2 + 2]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex3]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex3 + 1]);
					m_ModelData.vecFaceTriangles.push_back(mp_vertexXYZ[3 * vertex3 + 2]);

					//边 点的坐标（每个三角形有三条边，所以每个点要记录两遍12，23，31）
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex1 + 1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex1 + 2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex2 + 1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex2 + 2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex2 + 1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex2 + 2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex3]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex3 + 1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex3 + 2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex3]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex3 + 1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex3 + 2]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex1 + 1]);
					m_ModelData.vecEdges.push_back(mp_vertexXYZ[3 * vertex1 + 2]);

					//三角面 点的颜色
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex1] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex1 + 1] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex1 + 2] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex2] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex2 + 1] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex2 + 2] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex3] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex3 + 1] / 255.0f);
					m_ModelData.vecFaceTriangleColors.push_back(mp_vertexRGB[3 * vertex3 + 2] / 255.0f);

					//边 点的颜色
					for (int j = 0; j<18; j++)
					{
						m_ModelData.vecEdgeColors.push_back(0);//全是黑色
					}

					//三角面 点的法线
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex1]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex1 + 1]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex1 + 2]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex2]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex2 + 1]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex2 + 2]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex3]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex3 + 1]);
					m_ModelData.vecNormals.push_back(mp_vertexNorm[3 * vertex3 + 2]);

					//边 点的法线
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex1 + 1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex1 + 2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex2 + 1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex2 + 2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex2 + 1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex2 + 2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex3]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex3 + 1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex3 + 2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex3]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex3 + 1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex3 + 2]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex1 + 1]);
					m_ModelData.vecEdgeNormals.push_back(mp_vertexNorm[3 * vertex1 + 2]);
					*/

					//triangle_index += 9;
					//m_ModelData.iTotalConnectedTriangles += 3;
					//m_ModelData.iTotalConnectededges += 6;
				}

				i += 3;
			}

			fclose(file);
			printf("%s Loaded!\n", filename);

		}

		else
		{
			printf("File can't be opened\n");
		}
	}
	else
	{
		printf("File does not have a .PLY extension. ");
	}

	return 0;
}


/*void CPLYLoader::Draw() //implemented in GLPainter, not called again
{
if (m_ModelData.vecFaceTriangleColors.empty())
{
cout << "model data is null"<<endl;
exit(-1);
}

glEnableClientState(GL_VERTEX_ARRAY);
glEnableClientState(GL_NORMAL_ARRAY);
glEnableClientState(GL_COLOR_ARRAY);

glEnable (GL_POLYGON_OFFSET_FILL);//开启深度偏移
glPolygonOffset (0.5, 1.0);//一个像素点上画两个点的话，显示深度小的
glVertexPointer(3,GL_FLOAT, 0,m_ModelData.vecFaceTriangles.data());
glColorPointer(3,GL_FLOAT,0,m_ModelData.vecFaceTriangleColors.data());
glNormalPointer(GL_FLOAT, 0, m_ModelData.vecNormals.data());
glDrawArrays(GL_TRIANGLES, 0, m_ModelData.iTotalConnectedTriangles);//画三角面 灰色
glDisable( GL_POLYGON_OFFSET_FILL );

glVertexPointer(3,GL_FLOAT, 0,m_ModelData.vecEdges.data());
glColorPointer(3,GL_FLOAT,0,m_ModelData.vecEdgeColors.data());
glNormalPointer(GL_FLOAT, 0, m_ModelData.vecEdgeNormals.data());
glDrawArrays(GL_LINES,0,m_ModelData.iTotalConnectededges);//画边 黑色

glDisableClientState(GL_VERTEX_ARRAY);
glDisableClientState(GL_NORMAL_ARRAY);
glDisableClientState(GL_COLOR_ARRAY);
}*/