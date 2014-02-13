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

int Load_OBJECT4DV1_PLG(LPOBJECT4DV1 obj, 
						char*        filename, 
						LPVECTOR4D   scale, 
						LPVECTOR4D   pos,
						LPVECTOR4D   rot)
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