#include <iostream>
#include <cstring>
#include <cmath>
#include <Windows.h>
#include <ddraw.h>


#include "BasicDrawEngine.h"
#include "MathEngine.h"
#include "3DPipeLine.h"


/*
char* Get_Line_PLG(char* buffer, int maxLength, FILE* fp)
{

}*/
/*
int Load_OBJECT4DV1_PLG(LPOBJECT4DV1 obj, 
						char*        filename, 
						LPVECTOR4D   scale, 
						LPVECTOR4D   pos,
						LPVECTOR4D   rot)
{

}*/

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

void Build_XYZ_Rotation_Matrix4X4(float        xTheta, 
								  float        yTheta, 
								  float        zTheta, 
								  LPMATRIX_4X4 pmRotation)
{
	MATRIX_4X4 mx;
	MATRIX_4X4 my;
	MATRIX_4X4 mz;
	MATRIX_4X4 mtemp;

	float sinTheta = 0;
	float cosTheta = 0;
	int   rotSeq   = 0;

	MATRIX_IDENTITY_4X4(pmRotation);

	if (fabs(xTheta) > EPSILON_E5)
		rotSeq = rotSeq | 1;
	if (fabs(yTheta) > EPSILON_E5)
		rotSeq = rotSeq | 2;
	if (fabs(zTheta) > EPSILON_E5)
		rotSeq = rotSeq | 4;

	switch (rotSeq)
	{
	case 0:
		break;

	case 1:
		cosTheta = Fast_Cos(xTheta);
		sinTheta = Fast_Sin(xTheta);

		MATRIX_INIT_4X4(&mx, 1,  0,        0,        0, 
			                 0,  cosTheta, sinTheta, 0,
							 0, -sinTheta, cosTheta, 0,
							 0,  0,        0,        1);

		MATRIX_COPY_4X4(pmRotation, &mx);
		break;

	case 2:
		cosTheta = Fast_Cos(yTheta);
		sinTheta = Fast_Sin(yTheta);

		MATRIX_INIT_4X4(&my, cosTheta, 0, -sinTheta, 0, 
			                 0,        1, 0,         0,
							 sinTheta, 0, cosTheta,  0,
							 0,        0, 0,         1);

		MATRIX_COPY_4X4(pmRotation, &my);
		break;

	case 3:
		cosTheta = Fast_Cos(xTheta);
		sinTheta = Fast_Sin(xTheta);

		MATRIX_INIT_4X4(&mx, 1,  0,        0,        0, 
			                 0,  cosTheta, sinTheta, 0,
							 0, -sinTheta, cosTheta, 0,
							 0,  0,        0,        1);

		cosTheta = Fast_Cos(yTheta);
		sinTheta = Fast_Sin(yTheta);

		MATRIX_INIT_4X4(&my, cosTheta, 0, -sinTheta, 0, 
			                 0,        1, 0,         0,
							 sinTheta, 0, cosTheta,  0,
							 0,        0, 0,         1);

		MATRIX_MUL_4X4(&mx, &my, pmRotation);
		break;

	case 4:
		cosTheta = Fast_Cos(zTheta);
		sinTheta = Fast_Sin(zTheta);

		MATRIX_INIT_4X4(&mz,  cosTheta, sinTheta, 0, 0, 
			                 -sinTheta, cosTheta, 0, 0,
							  0,        0,        1, 0,
							  0,        0,        0, 1);

		MATRIX_COPY_4X4(&pmRotation, &mz);
		break;

	case 5:
		cosTheta = Fast_Cos(xTheta);
		sinTheta = Fast_Sin(xTheta);

		MATRIX_INIT_4X4(&mx, 1,  0,        0,        0, 
			                 0,  cosTheta, sinTheta, 0,
							 0, -sinTheta, cosTheta, 0,
							 0,  0,        0,        1);

		cosTheta = Fast_Cos(zTheta);
		sinTheta = Fast_Sin(zTheta);

		MATRIX_INIT_4X4(&mz,  cosTheta, sinTheta, 0, 0, 
			                 -sinTheta, cosTheta, 0, 0,
							  0,        0,        1, 0,
							  0,        0,        0, 1);

		MATRIX_MUL_4X4(&mx, &mz, pmRotation);
		break;

	case 6:
		cosTheta = Fast_Cos(yTheta);
		sinTheta = Fast_Sin(yTheta);

		MATRIX_INIT_4X4(&my, cosTheta, 0, -sinTheta, 0, 
			                 0,        1, 0,         0,
							 sinTheta, 0, cosTheta,  0,
							 0,        0, 0,         1);

		cosTheta = Fast_Cos(zTheta);
		sinTheta = Fast_Sin(zTheta);

		MATRIX_INIT_4X4(&mz,  cosTheta, sinTheta, 0, 0, 
			                 -sinTheta, cosTheta, 0, 0,
							  0,        0,        1, 0,
							  0,        0,        0, 1);

		MATRIX_MUL_4X4(&my, &mz, pmRotation);
		break;

	case 7:
		cosTheta = Fast_Cos(xTheta);
		sinTheta = Fast_Sin(xTheta);

		MATRIX_INIT_4X4(&mx, 1,  0,        0,        0, 
			                 0,  cosTheta, sinTheta, 0,
							 0, -sinTheta, cosTheta, 0,
							 0,  0,        0,        1);

		cosTheta = Fast_Cos(yTheta);
		sinTheta = Fast_Sin(yTheta);

		MATRIX_INIT_4X4(&my, cosTheta, 0, -sinTheta, 0, 
			                 0,        1, 0,         0,
							 sinTheta, 0, cosTheta,  0,
							 0,        0, 0,         1);

		cosTheta = Fast_Cos(zTheta);
		sinTheta = Fast_Sin(zTheta);

		MATRIX_INIT_4X4(&mz,  cosTheta, sinTheta, 0, 0, 
			                 -sinTheta, cosTheta, 0, 0,
							  0,        0,        1, 0,
							  0,        0,        0, 1);

		MATRIX_MUL_4X4(&mx, &my, &mtemp);
		MATRIX_MUL_4X4(&mtemp, &mz, pmRotation);
		break;

	default:
		break;
	}
}

void Build_Model_To_World_Matrix4X4(LPVECTOR4D pvPostion, LPMATRIX_4X4 pm)
{
	MATRIX_INIT_4X4(pm, 1,            0,            0,            0,
		                0,            1,            0,            0,
					    0,            0,            1,            0,
					    pvPostion->x, pvPostion->y, pvPostion->z, 1);
}

void Build_Camera_To_Perspective_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm)
{
	MATRIX_INIT_4X4(pm, cam->viewDistance, 0,                                    0, 0,
		                0,                 cam->viewDistance / cam->aspectRatio, 0, 0,
						0,                 0,                                    1, 1,
						0,                 0,                                    0, 0);
}

void Build_Perspective_To_Screen_4D_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm)
{
	float alpha = (0.5 * cam->viewPortWidth  - 0.5);
	float beta  = (0.5 * cam->viewPortHeight - 0.5);

	MATRIX_INIT_4X4(pm, alpha,  0,    0, 0,
		                0,     -beta, 0, 0, 
						alpha,  beta, 0, 0,
						0,      0,    0, 1);
}

void Build_Perspective_To_Screen_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm)
{
	float alpha = (0.5 * cam->viewPortWidth  - 0.5);
	float beta  = (0.5 * cam->viewPortHeight - 0.5);

	MATRIX_INIT_4X4(pm, alpha,  0,    0, 0,
		                0,     -beta, 0, 0, 
						alpha,  beta, 0, 0,
						0,      0,    0, 1);
}

void Build_Camera_To_Screen_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm)
{
	float alpha = (0.5 * cam->viewPortWidth  - 0.5);
	float beta  = (0.5 * cam->viewPortHeight - 0.5);

	MATRIX_INIT_4X4(pm, cam->viewDistance,  0,                 0, 0,
		                0,                 -cam->viewDistance, 0, 0,
		                alpha,              beta,              1, 1,
		                0,                  0,                 0, 0);
}

void Transform_OBJECT4DV1(LPOBJECT4DV1 obj, 
						  LPMATRIX_4X4 pmTransform,
						  int          coordinate, 
						  int          transformBasis)
{
	switch (coordinate)
	{
	case TRANSFORM_LOCAL_ONLY:
		for (int vertex = 0; vertex < obj->numVertics; vertex++)
		{
			POINT4D pResult;
			MATRIX_MUL_1X4_4X4(&obj->vLocalList[vertex], pmTransform, &pResult);
			VECTOR4D_COPY(&obj->vLocalList[vertex], &pResult);
		}
		break;

	case TRANSFORM_TRANS_ONLY:
		for (int vertex = 0; vertex < obj->numVertics; vertex++)
		{
			POINT4D pResult;
			MATRIX_MUL_1X4_4X4(&obj->vTransList[vertex], pmTransform, &pResult);
			VECTOR4D_COPY(&obj->vTransList[vertex], &pResult);
		}
		break;

	case TRANSFORM_LOCAL_TO_TRANS:
		for (int vertex = 0; vertex < obj->numVertics; vertex++)
		{
			MATRIX_MUL_1X4_4X4(&obj->vLocalList[vertex], pmTransform, &obj->vTransList[vertex]);
		}
		break;

	default:
		break;
	}

	if (transformBasis)
	{
		VECTOR4D vResult;

		MATRIX_MUL_1X4_4X4(&obj->ux, pmTransform, &vResult);
		VECTOR4D_COPY(&obj->ux, &vResult);

		MATRIX_MUL_1X4_4X4(&obj->uy, pmTransform, &vResult);
		VECTOR4D_COPY(&obj->uy, &vResult);

		MATRIX_MUL_1X4_4X4(&obj->uz, pmTransform, &vResult);
		VECTOR4D_COPY(&obj->uz, &vResult);
	}
}

void Rotate_XYZ_OBJECT4DV1(LPOBJECT4DV1 obj,
						   float      xTheta, 
						   float      yTheta, 
						   float      zTheta)
{
	MATRIX_4X4 mrot;

	Build_XYZ_Rotation_Matrix4X4(xTheta, yTheta, zTheta, &mrot);
	
	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		POINT4D pResult;
		MATRIX_MUL_1X4_4X4(&obj->vLocalList[vertex], &mrot, &pResult);
		VECTOR4D_COPY(&obj->vTransList[vertex], &pResult);
	}

	VECTOR4D vResult;

	MATRIX_MUL_1X4_4X4(&obj->ux, &mrot, &vResult);
	VECTOR4D_COPY(&obj->ux, &vResult);

	MATRIX_MUL_1X4_4X4(&obj->uy, &mrot, &vResult);
	VECTOR4D_COPY(&obj->uy, &vResult);

	MATRIX_MUL_1X4_4X4(&obj->uz, &mrot, &vResult);
	VECTOR4D_COPY(&obj->uz, &vResult);
}

void Model_To_Word_OBJECT4DV1(LPOBJECT4DV1 obj, int coordinate)
{
	if (coordinate = TRANSFORM_LOCAL_TO_TRANS)
	{
		for (int vertex = 0; vertex < obj->numVertics; vertex++)
		{
			VECTOR4D_ADD(&obj->vLocalList[vertex], &obj->worldPos, &obj->vTransList[vertex]);
		}
	}
	else
	{
		for (int vertex = 0; vertex < obj->numVertics; vertex++)
		{
			VECTOR4D_ADD(&obj->vTransList[vertex], &obj->worldPos, &obj->vTransList[vertex]);
		}
	}
}

int Cull_OBJEC4DV1(LPOBJECT4DV1 obj, 
				   LPCAM4DV1    cam,
				   int          cullFlags)
{
	POINT4D boundingCenter;

	MATRIX_MUL_1X4_4X4(&obj->worldPos, &cam->mWordToCam, &boundingCenter);

	if (cullFlags & CULL_OBJECT_Z_PLANE)
	{
		if ( (boundingCenter.z - obj->maxRadius) > cam->farClipZ ||
			(boundingCenter.z + obj->maxRadius) < cam->nearClipZ )
		{
			SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
			return 1;
		}
	}

	if (cullFlags & CULL_OBJECT_X_PLANE)
	{
		float left_right_half = 0.5 * cam->viewPlaneWidth * boundingCenter.z / cam->viewDistance;
		if ( (boundingCenter.x - obj->maxRadius) > left_right_half ||
			 (boundingCenter.x + obj->maxRadius) < -left_right_half )
		{
			SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
			return 1;
		}
	}

	if (cullFlags & CULL_OBJECT_Y_PLANE)
	{
		float top_bottom_half = 0.5 * cam->viewPlaneHeight * boundingCenter.z / cam->viewDistance;
		if ( (boundingCenter.y - obj->maxRadius) > top_bottom_half || 
			 (boundingCenter.y + obj->maxRadius) < -top_bottom_half )
		{
			SET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);
			return 1;
		}
	}
	return 0;
}

void Remove_Backfaces_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam)
{
	if (obj->state & OBJECT4DV1_STATE_CULLED)
		return;

	for (int poly = 0; obj->numPolys; poly++)
	{
		LPPOLY4DV1 currentPoly = &obj->polyList[poly];

		if ( !(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
			  (currentPoly->state & POLY4DV1_STATE_CLIPPED) || 
			  (currentPoly->attr  & POLY4DV1_ATTR_2SIDED)   ||
			  (currentPoly->state & POLY4DV1_STATE_BACKFACE))
	        continue;

		int v0 = currentPoly->vert[0];
		int v1 = currentPoly->vert[1];
		int v2 = currentPoly->vert[2];

		VECTOR4D u;
		VECTOR4D v;
		VECTOR4D n;

		VECTOR4D_BUILD(&obj->vTransList[v0], &obj->vTransList[v1], &u);
		VECTOR4D_BUILD(&obj->vTransList[v0], &obj->vTransList[v2], &v);

		VECTOR4D_CROSS(&u, &v, &n);

		VECTOR4D view;
		VECTOR4D_BUILD(&obj->vTransList[v0], &cam->pos, &n);

		if (VECTOR4D_DOT(&n, &view) <= 0.0)
			SET_BIT(currentPoly->state, POLY4DV1_STATE_BACKFACE);
	}
}

void Remove_Backfaces_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

		if ( (currentPoly == NULL)                        ||
			!(currentPoly->state & POLY4DV1_STATE_ACTIVE) ||
			 (currentPoly->attr  & POLY4DV1_ATTR_2SIDED)  ||
			 (currentPoly->state & POLY4DV1_STATE_BACKFACE))
		    continue;

		VECTOR4D u;
		VECTOR4D v;
		VECTOR4D n;

		VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[1], &u);
		VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[2], &v);

		VECTOR4D_CROSS(&u, &v, &n);

		VECTOR4D view;
		VECTOR4D_BUILD(&currentPoly->vTranList[0], &cam->pos, &view);

		if (VECTOR4D_DOT(&n, &view))
			SET_BIT(currentPoly->state, POLY4DV1_STATE_BACKFACE);
	}
}

void World_To_Camera_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam)
{
	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		POINT4D pResult;
		MATRIX_MUL_1X4_4X4(&obj->vTransList[vertex], &cam->mWordToCam, &pResult);
		VECTOR4D_COPY(&obj->vTransList[vertex], &pResult);
	}
}

void Camera_To_Perspective_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam)
{
	for(int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		float z = obj->vTransList[vertex].z;

		obj->vTransList[vertex].x = cam->viewDistance * obj->vTransList[vertex].x / z;
		obj->vTransList[vertex].y = cam->viewDistance * obj->vTransList[vertex].y / (z * cam->aspectRatio);
	}
}

void Camera_To_Perspective_Screen_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam)
{
	float alpha = 0.5 * cam->viewPlaneWidth  - 0.5;
	float beta  = 0.5 * cam->viewPlaneHeight - 0.5;

	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		float z = obj->vTransList[vertex].z;

		obj->vTransList[vertex].x = cam->viewDistance * obj->vTransList[vertex].x / z;
		obj->vTransList[vertex].y = cam->viewDistance * obj->vTransList[vertex].y / z;

		obj->vTransList[vertex].x =  obj->vTransList[vertex].x + alpha;
		obj->vTransList[vertex].y = -obj->vTransList[vertex].y + beta;
	}
}

void Perspective_To_Screen_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam)
{
	float alpha = 0.5 * cam->viewPortWidth  - 0.5;
	float beta  = 0.5 * cam->viewPortHeight - 0.5;

	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		obj->vTransList[vertex].x = alpha + alpha * obj->vTransList[vertex].x;
		obj->vTransList[vertex].y = beta  - beta  * obj->vTransList[vertex].y;
	}
}

void Convert_From_Homogeneous4D_OBJECT4DV1(LPOBJECT4DV1 obj)
{
	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		VECTOR4D_DIV_BY_W(&obj->vTransList[vertex]);
	}
}

void Transform_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, 
							  LPMATRIX_4X4     pmTransform,
							  int              coordinate)
{
	switch (coordinate)
	{
	case TRANSFORM_LOCAL_ONLY:
		for (int poly = 0; poly < renderList->numPolys; poly++)
		{
			LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

			if ( (currentPoly == NULL)                         ||
				!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
				 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
				 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
			    continue;

			for (int vertex = 0; vertex < 3; vertex++)
			{
				POINT4D pResult;
				MATRIX_MUL_1X4_4X4(&currentPoly->vList[vertex], pmTransform, &pResult);
				VECTOR4D_COPY(&currentPoly->vList[vertex], &pResult);
			}
		}
		break;

	case TRANSFORM_TRANS_ONLY:
		for (int poly = 0; poly < renderList->numPolys; poly++)
		{
			LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

			if ( (currentPoly == NULL)                         ||
				!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
				 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
				 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
			    continue;

			for (int vertex = 0; vertex < 3; vertex++)
			{
				POINT4D pResult;
				MATRIX_MUL_1X4_4X4(&currentPoly->vTranList[vertex], pmTransform, &pResult);
				VECTOR4D_COPY(&currentPoly->vTranList[vertex], &pResult);
			}
		}
		break;

	case TRANSFORM_LOCAL_TO_TRANS:
		for (int poly = 0; poly < renderList->numPolys; poly++)
		{
			LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

			if ( (currentPoly == NULL)                         ||
				!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
				 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
				 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
			    continue;

			for (int vertex = 0; vertex < 3; vertex++)
			{
				POINT4D pResult;
				MATRIX_MUL_1X4_4X4(&currentPoly->vList[vertex], pmTransform, &currentPoly->vTranList[vertex]);
			}
		}
		break;

	default:
		break;
	}
}

void Model_To_World_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, 
								   LPPOINT4D        worldPos,
								   int              coordinate)
{
	if (coordinate == TRANSFORM_LOCAL_TO_TRANS)
	{
		for (int poly = 0; poly < renderList->numPolys; poly++)
		{
			LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

			if ( (currentPoly == NULL)                         ||
				!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
				 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
				 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
			    continue;

			for (int vertex = 0; vertex < 3; vertex++)
			{
				VECTOR4D_ADD(&currentPoly->vList[vertex], worldPos, &currentPoly->vTranList[vertex]);
			}
		}
	}
	else
	{
		for (int poly = 0; poly < renderList->numPolys; poly++)
		{
			LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

			if ( (currentPoly == NULL)                         ||
				!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
				 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
				 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
			    continue;

			for (int vertex = 0; vertex < 3; vertex++)
			{
				VECTOR4D_ADD(&currentPoly->vTranList[vertex], worldPos, &currentPoly->vTranList[vertex]);
			}
		}
	}
}

void Convert_From_Homogeneous4D_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

		if ( (currentPoly == NULL)                         ||
			!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
			 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
		    continue;

		for (int vertex = 0; vertex < 3; vertex++)
		{
			VECTOR4D_DIV_BY_W(&currentPoly->vTranList[vertex]);
		}
	}
}

void World_To_Camera_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

		if ( (currentPoly == NULL) ||
			!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
			 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
		    continue;

		for (int vertex = 0; vertex < 3; vertex++)
		{
			POINT4D pResult;
			MATRIX_MUL_1X4_4X4(&currentPoly->vTranList[vertex], 
				               &cam->mWordToCam, 
							   &pResult);
			VECTOR4D_COPY(&currentPoly->vTranList[vertex], &pResult);
		}
	}
}

void Camera_To_Perspective_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

		if ( (currentPoly == NULL) ||
			!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
			 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
		    continue;

		for (int vertex = 0; vertex < 3; vertex++)
		{
			float z = currentPoly->vTranList[vertex].z;
			currentPoly->vTranList[vertex].x = 
				cam->viewDistance * currentPoly->vTranList[vertex].x / z;
			currentPoly->vTranList[vertex].y = 
				cam->viewDistance * currentPoly->vTranList[vertex].y / (z * cam->aspectRatio);
		}
	}
}

void Camera_To_Perspective_Screen_OBJECT4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

		if ( (currentPoly == NULL) ||
			!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
			 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
		    continue;

		float alpha = 0.5 * cam->viewPortWidth  - 0.5;
		float beta  = 0.5 * cam->viewPortHeight - 0.5;

		for (int vertex = 0; vertex < 3; vertex++)
		{
			float z = currentPoly->vTranList[vertex].z;
			currentPoly->vTranList[vertex].x = cam->viewDistance * currentPoly->vTranList[vertex].x / z;
			currentPoly->vTranList[vertex].y = cam->viewDistance * currentPoly->vTranList[vertex].y / z;

			currentPoly->vTranList[vertex].x = currentPoly->vTranList[vertex].x + alpha;
			currentPoly->vTranList[vertex].y = currentPoly->vTranList[vertex].y + beta;
		}
	}
}

void Perspective_To_Screen_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		LPPOLYF4DV1 currentPoly = renderList->polyPointer[poly];

		if ( (currentPoly == NULL) ||
			!(currentPoly->state & POLY4DV1_STATE_ACTIVE)  ||
			 (currentPoly->state & POLY4DV1_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4DV1_STATE_BACKFACE) )
		    continue;

		float alpha = 0.5 * cam->viewPortWidth  - 0.5;
		float beta  = 0.5 * cam->viewPortHeight - 0.5;

		for (int vertex = 0; vertex < 3; vertex++)
		{
			currentPoly->vTranList[vertex].x = alpha + alpha * currentPoly->vTranList[vertex].x;
			currentPoly->vTranList[vertex].y = beta  - beta  * currentPoly->vTranList[vertex].y;
		}
	}
}

void Reset_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList)
{
	renderList->numPolys = 0;
}

void Reset_OBJECT4DV1(LPOBJECT4DV1 obj)
{
	RESET_BIT(obj->state, OBJECT4DV1_STATE_CULLED);

	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		LPPOLY4DV1 currentPoly = &obj->polyList[poly];
		if (!currentPoly->state & POLY4DV1_STATE_ACTIVE)
			continue;
		
		RESET_BIT(currentPoly->state, POLY4DV1_STATE_CLIPPED);
		RESET_BIT(currentPoly->state, POLY4DV1_STATE_BACKFACE);
	}
}

int Insert_POLY4DV1_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPPOLY4DV1 poly)
{
	if (renderList->numPolys >= RENDERLIST4DV1_MAX_POLYS)
		return 0;

	renderList->polyPointer[renderList->numPolys] = &renderList->polyData[renderList->numPolys];

	renderList->polyData[renderList->numPolys].state = poly->state;
	renderList->polyData[renderList->numPolys].attr  = poly->attr;
	renderList->polyData[renderList->numPolys].color = poly->color;

	VECTOR4D_COPY(&renderList->polyData[renderList->numPolys].vTranList[0], &poly->vList[poly->vert[0]]);
	VECTOR4D_COPY(&renderList->polyData[renderList->numPolys].vTranList[1], &poly->vList[poly->vert[1]]);
	VECTOR4D_COPY(&renderList->polyData[renderList->numPolys].vTranList[2], &poly->vList[poly->vert[2]]);
	
	VECTOR4D_COPY(&renderList->polyData[renderList->numPolys].vList[0], &poly->vList[poly->vert[0]]);
	VECTOR4D_COPY(&renderList->polyData[renderList->numPolys].vList[1], &poly->vList[poly->vert[1]]);
	VECTOR4D_COPY(&renderList->polyData[renderList->numPolys].vList[2], &poly->vList[poly->vert[2]]);

	if (renderList->numPolys == 0)
	{
		renderList->polyData[0].next = NULL;
		renderList->polyData[0].prev = NULL;
	}
	else
	{
		renderList->polyData[renderList->numPolys].next = NULL;
		renderList->polyData[renderList->numPolys].prev = &renderList->polyData[renderList->numPolys - 1];

		renderList->polyData[renderList->numPolys - 1].next = &renderList->polyData[renderList->numPolys];
	}

	renderList->numPolys++;

	return 1;
}

int Insert_POLYF4DV1_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPPOLYF4DV1 poly)
{
	if (renderList->numPolys >= RENDERLIST4DV1_MAX_POLYS)
		return 0;

	renderList->polyPointer[renderList->numPolys] = &renderList->polyData[renderList->numPolys];

	memcpy((void*)&renderList->polyData[renderList->numPolys], (void*)poly, sizeof(POLYF4DV1));

	if (renderList->numPolys == 0)
	{
		renderList->polyData[0].next = NULL;
		renderList->polyData[0].prev = NULL;
	}
	else
	{
		renderList->polyData[renderList->numPolys].next = NULL;
		renderList->polyData[renderList->numPolys].prev = &renderList->polyData[renderList->numPolys - 1];
	    renderList->polyData[renderList->numPolys - 1].next = &renderList->polyData[renderList->numPolys];
	}
	renderList->numPolys++;

	return 1;
}

int Insert_OBJECT4DV1_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList,
									 LPOBJECT4DV1     obj,
									 int              insertLocale)
{
	if (!(obj->state & OBJECT4DV1_STATE_ACTIVE) ||
		 (obj->state & OBJECT4DV1_STATE_CULLED) ||
		!(obj->state & OBJECT4DV1_STATE_VISIBLE))
	    return 0;

	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		LPPOLY4DV1 currentPoly = &obj->polyList[poly];

		if (!(currentPoly->state == POLY4DV1_STATE_ACTIVE)  ||
			 (currentPoly->state == POLY4DV1_STATE_CLIPPED) ||
			 (currentPoly->state == POLY4DV1_STATE_BACKFACE))
		    continue;

		LPPOINT4D vOldList = currentPoly->vList;

		if (insertLocale)
			currentPoly->vList = obj->vLocalList;
		else
			currentPoly->vList = obj->vTransList;

		if (!Insert_POLY4DV1_RENDERLIST4DV1(renderList, currentPoly))
		{
			currentPoly->vList = vOldList;
			return 0;
		}
		currentPoly->vList = vOldList;
	}

	return 1;
}


void Draw_OBJECT4DV1_Wire(LPOBJECT4DV1 obj, 
						  UCHAR*       videoBuffer,
						  int          lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4DV1_STATE_ACTIVE)  ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_BACKFACE))
		    continue;

		int v0 = obj->polyList[poly].vert[0];
		int v1 = obj->polyList[poly].vert[1];
		int v2 = obj->polyList[poly].vert[2];

		Draw_Clip_Line(obj->vTransList[v0].x,
			           obj->vTransList[v0].y,
					   obj->vTransList[v1].x,
					   obj->vTransList[v1].y,
					   obj->polyList[poly].color,
					   videoBuffer,
					   lPitch);

		Draw_Clip_Line(obj->vTransList[v1].x,
			           obj->vTransList[v1].y,
					   obj->vTransList[v2].x,
					   obj->vTransList[v2].y,
					   obj->polyList[poly].color,
					   videoBuffer,
					   lPitch);

		Draw_Clip_Line(obj->vTransList[v2].x,
			           obj->vTransList[v2].y,
					   obj->vTransList[v0].x,
					   obj->vTransList[v0].y,
					   obj->polyList[poly].color,
					   videoBuffer,
					   lPitch);
	}
}

void Draw_RENDERLIST4DV1_Wire(LPRENDERLIST4DV1 renderList, 
							  UCHAR*           videoBuffer,
							  int              lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4DV1_STATE_ACTIVE)  ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_BACKFACE))
		    continue;

		
		Draw_Clip_Line(renderList->polyPointer[poly]->vTranList[0].x,
			           renderList->polyPointer[poly]->vTranList[0].y,
					   renderList->polyPointer[poly]->vTranList[1].x,
					   renderList->polyPointer[poly]->vTranList[1].y,
					   renderList->polyPointer[poly]->color,
					   videoBuffer,
					   lPitch);

		Draw_Clip_Line(renderList->polyPointer[poly]->vTranList[1].x,
			           renderList->polyPointer[poly]->vTranList[1].y,
					   renderList->polyPointer[poly]->vTranList[2].x,
					   renderList->polyPointer[poly]->vTranList[2].y,
					   renderList->polyPointer[poly]->color,
					   videoBuffer,
					   lPitch);

		Draw_Clip_Line(renderList->polyPointer[poly]->vTranList[2].x,
			           renderList->polyPointer[poly]->vTranList[2].y,
					   renderList->polyPointer[poly]->vTranList[0].x,
					   renderList->polyPointer[poly]->vTranList[0].y,
					   renderList->polyPointer[poly]->color,
					   videoBuffer,
					   lPitch);		
	}
}

void Draw_OBJECT4DV1_Wire16(LPOBJECT4DV1 obj,
							UCHAR*       videoBuffer,
							int          lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4DV1_STATE_ACTIVE)  ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_BACKFACE))
		    continue;

		int v0 = obj->polyList[poly].vert[0];
		int v1 = obj->polyList[poly].vert[1];
		int v2 = obj->polyList[poly].vert[2];

		Draw_Clip_Line16(obj->vTransList[v0].x,
			             obj->vTransList[v0].y,
					     obj->vTransList[v1].x,
					     obj->vTransList[v1].y,
					     obj->polyList[poly].color,
					     videoBuffer,
					     lPitch);

		Draw_Clip_Line16(obj->vTransList[v1].x,
			             obj->vTransList[v1].y,
					     obj->vTransList[v2].x,
					     obj->vTransList[v2].y,
					     obj->polyList[poly].color,
					     videoBuffer,
					     lPitch);

		Draw_Clip_Line16(obj->vTransList[v2].x,
			             obj->vTransList[v2].y,
					     obj->vTransList[v0].x,
					     obj->vTransList[v0].y,
					     obj->polyList[poly].color,
					     videoBuffer,
					     lPitch);
	}
}

void Draw_RENDERLIST4DV1_Wire16(LPRENDERLIST4DV1 renderList, 
							    UCHAR*           videoBuffer,
							    int              lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4DV1_STATE_ACTIVE)  ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_BACKFACE))
		    continue;

		
		Draw_Clip_Line16(renderList->polyPointer[poly]->vTranList[0].x,
			             renderList->polyPointer[poly]->vTranList[0].y,
					     renderList->polyPointer[poly]->vTranList[1].x,
					     renderList->polyPointer[poly]->vTranList[1].y,
					     renderList->polyPointer[poly]->color,
					     videoBuffer,
					     lPitch);

		Draw_Clip_Line16(renderList->polyPointer[poly]->vTranList[1].x,
			             renderList->polyPointer[poly]->vTranList[1].y,
					     renderList->polyPointer[poly]->vTranList[2].x,
					     renderList->polyPointer[poly]->vTranList[2].y,
					     renderList->polyPointer[poly]->color,
					     videoBuffer,
					     lPitch);

		Draw_Clip_Line16(renderList->polyPointer[poly]->vTranList[2].x,
			             renderList->polyPointer[poly]->vTranList[2].y,
					     renderList->polyPointer[poly]->vTranList[0].x,
					     renderList->polyPointer[poly]->vTranList[0].y,
					     renderList->polyPointer[poly]->color,
					     videoBuffer,
					     lPitch);		
	}
}

void Build_CAM4DV1_Matrix_Euler(LPCAM4DV1 cam, int camRotSeq)
{
	MATRIX_4X4 mt_inv;
	MATRIX_4X4 mx_inv;
	MATRIX_4X4 my_inv;
	MATRIX_4X4 mz_inv;
	MATRIX_4X4 mrot;
	MATRIX_4X4 mtmp;

	MATRIX_INIT_4X4(&mt_inv,  1,           0,           0,          0,
		                      0,           1,           0,          0,
							  0,           0,           1,          0,
							 -cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

	float xTheta = cam->dir.x;
	float yTheta = cam->dir.y;
	float zTheta = cam->dir.z;

	float cosTheta =  Fast_Cos(xTheta);
	float sinTheta = -Fast_Sin(xTheta);

	MATRIX_INIT_4X4(&mx_inv, 1,  0,        0,        0,
		                     0,  cosTheta, sinTheta, 0, 
							 0, -sinTheta, cosTheta, 0, 
							 0,  0,        0,        1);

	cosTheta =  Fast_Cos(yTheta);
	sinTheta = -Fast_Sin(yTheta);

	MATRIX_INIT_4X4(&my_inv, cosTheta, 0, -sinTheta, 0,
		                     0,        1,  0,        0, 
							 sinTheta, 0,  cosTheta, 0, 
							 0,        0,  0,        1);

	cosTheta =  Fast_Cos(zTheta);
	sinTheta = -Fast_Sin(zTheta);

	MATRIX_INIT_4X4(&mz_inv,  cosTheta, sinTheta, 0, 0,
		                     -sinTheta, cosTheta, 0, 0, 
							  0,        0,        1, 0,
							  0,        0,        0, 1);

	switch (camRotSeq)
	{
	case CAM_ROT_SEQ_XYZ:
		MATRIX_MUL_4X4(&mx_inv, &my_inv, &mtmp);
		MATRIX_MUL_4X4(&mtmp,   &mz_inv, &mrot);
		break;

    case CAM_ROT_SEQ_YXZ:
		MATRIX_MUL_4X4(&my_inv, &mx_inv, &mtmp);
		MATRIX_MUL_4X4(&mtmp,   &mz_inv, &mrot);
		break;

	case CAM_ROT_SEQ_XZY:
		MATRIX_MUL_4X4(&mx_inv, &mx_inv, &mtmp);
		MATRIX_MUL_4X4(&mtmp,   &my_inv, &mrot);
		break;

	case CAM_ROT_SEQ_YZX:
		MATRIX_MUL_4X4(&my_inv, &mz_inv, &mtmp);
		MATRIX_MUL_4X4(&mtmp,   &mx_inv, &mrot);
		break;

	case CAM_ROT_SEQ_ZYX:
		MATRIX_MUL_4X4(&mz_inv, &my_inv, &mtmp);
		MATRIX_MUL_4X4(&mtmp,   &mx_inv, &mrot);
		break;

	case CAM_ROT_SEQ_ZXY:
		MATRIX_MUL_4X4(&mz_inv, &mx_inv, &mtmp);
		MATRIX_MUL_4X4(&mtmp,   &my_inv, &mrot);
		break;

	default:
		break;
	}

	MATRIX_MUL_4X4(&mt_inv, &mrot, &cam->mWordToCam);
}

void Build_CAM4DV1_Matrix_UVM(LPCAM4DV1 cam, int mode)
{
	MATRIX_4X4 mt_inv;
	MATRIX_4X4 mt_uvn;
	MATRIX_4X4 mtmp;

	MATRIX_INIT_4X4(&mt_inv,  1,           0,           0,          0,
		                      0,           1,           0,          0,
							  0,           0,           1,          0,
							 -cam->pos.x, -cam->pos.y, -cam->pos.z, 1);

	if (mode == UVN_MODE_SPHERICAL)
	{
		float phi   = cam->dir.x;
		float theta = cam->dir.y;

		float sin_phi   = Fast_Sin(phi);
		float cos_phi   = Fast_Cos(phi);

		float sin_theta = Fast_Sin(theta);
		float cos_theta = Fast_Cos(theta);

		cam->target.x = -1 * sin_phi * sin_theta;
		cam->target.y =  1 * cos_phi;
		cam->target.z =  1 * sin_phi * cos_theta;
	}

	VECTOR4D_BUILD(&cam->pos, &cam->target, &cam->n);

	VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);

	VECTOR4D_CROSS(&cam->v, &cam->n, &cam->u);

	VECTOR4D_CROSS(&cam->n, &cam->u, &cam->v);

	VECTOR4D_NORMALIZE(&cam->u);
	VECTOR4D_NORMALIZE(&cam->v);
	VECTOR4D_NORMALIZE(&cam->n);

	MATRIX_INIT_4X4(&mt_uvn, cam->u.x, cam->v.x, cam->n.x, 0, 
		                     cam->u.y, cam->v.y, cam->n.y, 0, 
							 cam->u.z, cam->v.z, cam->n.z, 0, 
							 0,        0,        0,        1);

	MATRIX_MUL_4X4(&mt_inv, &mt_uvn, &cam->mWordToCam);
}

void Init_CAM4DV1(LPCAM4DV1  cam,
				  int        attr,
				  LPPOINT4D  postion,
				  LPVECTOR4D direction,
				  LPPOINT4D  target,
				  float      nearClipZ,
				  float      farClipZ,
				  float      fov,
				  float      viewPortWidth,
				  float      viewPortHeight)
{
	cam->attr = attr;
	VECTOR4D_COPY(&cam->pos, postion);
	VECTOR4D_COPY(&cam->dir, direction);

	VECTOR4D_INITXYZ(&cam->u, 1, 0, 0);
	VECTOR4D_INITXYZ(&cam->v, 0, 1, 0);
	VECTOR4D_INITXYZ(&cam->n, 0, 0, 1);

	if (target != NULL)
		VECTOR4D_COPY(&cam->target, target);
	else
		VECTOR4D_ZERO(&cam->target);

	cam->nearClipZ       = nearClipZ;
	cam->farClipZ        = farClipZ;
	cam->viewPortWidth   = viewPortWidth;
	cam->viewPortHeight  = viewPortHeight;
	cam->viewPortCenterX = (viewPortWidth - 1)  / 2;
	cam->viewPortCenterY = (viewPortHeight - 1) / 2;
	cam->aspectRatio     = (float)viewPortWidth / (float)viewPortHeight;

	MATRIX_IDENTITY_4X4(&cam->mWordToCam);
	MATRIX_IDENTITY_4X4(&cam->mCamToPer);
	MATRIX_IDENTITY_4X4(&cam->mPerToScreen);

	cam->fov             = fov;
	cam->viewPlaneWidth  = 2.0;
	cam->viewPlaneHeight = 2.0 / cam->aspectRatio;
	cam->viewDistance    = 0.5 * (cam->viewPlaneWidth) / tan(DEG_TO_RAD(fov / 2));

	if (fov == 90)
	{
		VECTOR3D vn;
		POINT3D  ptOrigin;
		VECTOR3D_INITXYZ(&ptOrigin, 0, 0, 0);
		
		VECTOR3D_INITXYZ(&vn, 1, 0, -1);
		Plane3D_Init(&cam->rightClipPlane, &ptOrigin, &vn, 1);  //x=z plane

		VECTOR3D_INITXYZ(&vn, -1, 0, -1);
		Plane3D_Init(&cam->leftClipPlane, &ptOrigin, &vn ,1);  //-x=z plane

		VECTOR3D_INITXYZ(&vn, 0, 1, -1);
		Plane3D_Init(&cam->topClipPlane, &ptOrigin, &vn ,1);  //y=z plane

		VECTOR3D_INITXYZ(&vn, 0, -1, -1);
		Plane3D_Init(&cam->bottomClipPlane, &ptOrigin, &vn ,1);  //-y=z plane
	}
	else
	{
		VECTOR3D vn;
		POINT3D  ptOrigin;
		VECTOR3D_INITXYZ(&ptOrigin, 0, 0, 0);

		VECTOR3D_INITXYZ(&vn, cam->viewDistance, 0, -cam->viewPlaneWidth / 2);
		Plane3D_Init(&cam->rightClipPlane, &ptOrigin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, -cam->viewDistance, 0, -cam->viewPlaneWidth / 2);
		Plane3D_Init(&cam->leftClipPlane, &ptOrigin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, 0, cam->viewDistance, -cam->viewPlaneWidth / 2);
		Plane3D_Init(&cam->topClipPlane, &ptOrigin, &vn, 1);

		VECTOR3D_INITXYZ(&vn, 0, -cam->viewDistance, -cam->viewPlaneWidth / 2);
		Plane3D_Init(&cam->bottomClipPlane, &ptOrigin, &vn, 1);
	}		
}