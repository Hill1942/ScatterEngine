#include <iostream>
#include <cstring>
#include <cmath>
#include <Windows.h>
#include <ddraw.h>

#include "3DPipeLine.h"
#include "MathEngine.h"
#include "BasicDrawEngine.h"


char* Get_Line_PLG(char* buffer, int maxLength, FILE* fp)
{

}

int Load_OBJECT4DV1_PLG(LPOBJECT4DV1 obj, char* filename, 
						LPVECTOR4D scale, LPVECTOR4D pos, LPVECTOR4D rot)
{

}

float Compute_OBJECT4DV1_Radius(LPOBJECT4DV1 obj)
{
	obj->avgRadius = 0;
	obj->maxRadius = 0;

	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		float vertexDistance = sqrt(obj->vLocalList[vertex].x * obj->vLocalList[vertex].x + 
			                        obj->vLocalList[vertex].y * obj->vLocalList[vertex].y + 
									obj->vLocalList[vertex].z * obj->vLocalList[vertex].z);
		obj->avgRadius += vertexDistance;
		if (vertexDistance > obj->maxRadius)
			obj->maxRadius = vertexDistance;
	}
	obj->avgRadius /= obj->numVertics;

	return obj->maxRadius;
}


void Translate_OBJECT4DV1(LPOBJECT4DV1 obj, LPVECTOR4D pvTranslate)
{
	VECTOR4D_ADD(&obj->worldPos, pvTranslate, &obj->worldPos);
}

void Scale_OBJECT4DV1(LPOBJECT4DV1 obj, LPVECTOR4D pvScale)
{
	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		obj->vLocalList[vertex].x *= pvScale->x;
		obj->vLocalList[vertex].y *= pvScale->y;
		obj->vLocalList[vertex].z *= pvScale->z;
	}

	float scale = MAX(pvScale->x, pvScale->y);
	scale = MAX(scale, pvScale->z);

	obj->maxRadius = obj->maxRadius * scale;
	obj->avgRadius = obj->avgRadius * scale;
}

void Build_XYZ_Rotation_Matrix4X4(float xTheta, float yTheta, float zTheta, LPMATRIX_4X4 pmRotate)
{
}


	
