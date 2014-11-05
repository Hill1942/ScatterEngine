#ifndef SEPERSPECTIVE_H
#define SEPERSPECTIVE_H



#define POLY4D_ATTR_2SIDED      0x0001
#define POLY4D_ATTR_TRANSPARENT 0x0002
#define POLY4D_ATTR_8BITCOLOR   0x0004
#define POLY4D_ATTRRGB16       0x0008
#define POLY4D_ATTRRGB24       0x0010


#define POLY4D_ATTR_SHADE_MODE_PURE      0x0020
#define POLY4D_ATTR_SHADE_MODE_CONSTANT  0x0020
#define POLY4D_ATTR_SHADE_MODE_FLAT      0x0040
#define POLY4D_ATTR_SHADE_MODE_GOURAUD   0x0080
#define POLY4D_ATTR_SHADE_MODE_PHONG     0x0100
#define POLY4D_ATTR_SHADE_MODE_FASTPHONG 0x0100
#define POLY4D_ATTR_SHADE_MODE_TEXTURE   0x0200

#define POLY4D_STATE_ACTIVE    0x0001
#define POLY4D_STATE_CLIPPED   0x0002
#define POLY4D_STATE_BACKFACE  0x0004

#define OBJECT4D_MAX_VERTICES  1024
#define OBJECT4D_MAX_POLYS     1024

#define OBJECT4D_STATE_ACTIVE  0x0001
#define OBJECT4D_STATE_VISIBLE 0x0002
#define OBJECT4D_STATE_CULLED  0x0004


#define RENDERLIST4D_MAX_POLYS 32768

#define TRANSFORM_LOCAL_ONLY     0
#define TRANSFORM_TRANS_ONLY     1
#define TRANSFORM_LOCAL_TO_TRANS 2

#define CULL_OBJECT_X_PLANE    0x0001
#define CULL_OBJECT_Y_PLANE    0x0002
#define CULL_OBJECT_Z_PLANE    0x0004
#define CULL_OBJECT_XYZ_PLANES 0x0111

#define CAM_ROT_SEQ_XYZ 0
#define CAM_ROT_SEQ_YXZ 1
#define CAM_ROT_SEQ_XZY 2
#define CAM_ROT_SEQ_YZX 3
#define CAM_ROT_SEQ_ZYX 4
#define CAM_ROT_SEQ_ZXY 5

#define CAM_PROJ_NORMALIZED 0x0001
#define CAM_PROJ_SCREEN     0x0002
#define CAM_PROJ_FOV90      0x0004
#define CAM_MODEL_EULER     0x0008
#define CAM_MODEL_UVN       0x0010

#define UVN_MODE_SIMPLE     0
#define UVN_MODE_SPHERICAL  1

typedef struct POLY4D_TYP
{
	int state;
	int attr;
	int color;
	int lightColor;

	LPPOINT4D vList;
	int       vert[3];

}POLY4D, *LPPOLY4D;

typedef struct POLYF4D_TYP
{
	int state;
	int attr;
	int color;
	int lightColor;

	POINT4D vList[3];
	POINT4D vTranList[3];

	POLYF4D_TYP* next;
	POLYF4D_TYP* prev;

}POLYF4D, *LPPOLYF4D;

typedef struct OBJECT4D_TYP
{
	int   id;
	char  name[64];
	int   state;
	int   attr;
	float avgRadius;
	float maxRadius;

	POINT4D  worldPos;
	VECTOR4D dir;
	VECTOR4D ux;
	VECTOR4D uy;
	VECTOR4D uz;

	int     numVertics;
	POINT4D vLocalList[OBJECT4D_MAX_VERTICES];
	POINT4D vTransList[OBJECT4D_MAX_VERTICES];

	int      numPolys;
	POLY4D polyList[OBJECT4D_MAX_POLYS];

}OBJECT4D, *LPOBJECT4D;

typedef struct CAM4D_TYP
{
	int state;
	int attr;

	POINT4D  pos;
	VECTOR4D dir;

	VECTOR4D u;
	VECTOR4D v;
	VECTOR4D n;

	VECTOR4D target;

	float viewDistance;
	float fov;

	float nearClipZ;
	float farClipZ;

	PLANE3D rightClipPlane;
	PLANE3D leftClipPlane;
	PLANE3D topClipPlane;
	PLANE3D bottomClipPlane;

	float viewPlaneWidth;
	float viewPlaneHeight;

	float viewPortWidth;
	float viewPortHeight;
	float viewPortCenterX;
	float viewPortCenterY;

	float aspectRatio;
	
	MATRIX_4X4 mWordToCam;
	MATRIX_4X4 mCamToPer;
	MATRIX_4X4 mPerToScreen;

}CAM4D, *LPCAM4D;

typedef struct RENDERLIST4D_TYP
{
	int state;
	int attr;

	LPPOLYF4D polyPointer[RENDERLIST4D_MAX_POLYS];
	POLYF4D   polyData[RENDERLIST4D_MAX_POLYS];

	int numPolys;
}RENDERLIST4D, *LPRENDERLIST4D;


char* Get_Line_PLG(char* buffer, int maxLength, FILE* fp);

float Compute_OBJECT4D_Radius(LPOBJECT4D obj);

int   Load_OBJECT4D_PLG(LPOBJECT4D obj, char* filename, LPVECTOR4D scale, 
						  LPVECTOR4D pos, LPVECTOR4D rot);

void  Translate_OBJECT4D(LPOBJECT4D obj, LPVECTOR4D pvTranslate);

void  Scale_OBJECT4D(LPOBJECT4D obj, LPVECTOR4D pvScale);

void  Build_XYZ_Rotation_Matrix4X4(float xTheta, float yTheta, float zTheta,
								   LPMATRIX_4X4 pmRotation);

void  Transform_OBJECT4D(LPOBJECT4D obj, LPMATRIX_4X4 pmTransform, 
						   int coordinate, int transformBasis);

void  Rotate_XYZ_OBJECT4D(LPOBJECT4D obj, float xTheta, float yTheta, float zTheta);

void  Model_To_Word_OBJECT4D(LPOBJECT4D obj, int coordinate = TRANSFORM_LOCAL_TO_TRANS);

int   Cull_OBJEC4DV1(LPOBJECT4D obj, LPCAM4D cam, int cullFlags);

void  Remove_Backfaces_OBJECT4D(LPOBJECT4D obj, LPCAM4D cam);

void  Remove_Backfaces_RENDERLIST4D(LPRENDERLIST4D renderList, LPCAM4D cam);

void  World_To_Camera_OBJECT4D(LPOBJECT4D obj, LPCAM4D cam);

void  Camera_To_Perspective_OBJECT4D(LPOBJECT4D obj, LPCAM4D cam);

void  Camera_To_Perspective_Screen_OBJECT4D(LPOBJECT4D obj, LPCAM4D cam);

void  Perspective_To_Screen_OBJECT4D(LPOBJECT4D obj, LPCAM4D cam);

void  Transform_RENDERLIST4D(LPRENDERLIST4D renderList, LPMATRIX_4X4 pmTransform,
							   int coordinate);

void  Model_To_World_RENDERLIST4D(LPRENDERLIST4D renderList, LPPOINT4D worldPos,
								   int coordinate = TRANSFORM_LOCAL_TO_TRANS);

void  World_To_Camera_RENDERLIST4D(LPRENDERLIST4D renderList, LPCAM4D cam);

void  Camera_To_Perspective_RENDERLIST4D(LPRENDERLIST4D rendrerList, LPCAM4D cam);

void  Camera_To_Perspective_Screen_RENDERLIST4D(LPRENDERLIST4D renderList, LPCAM4D cam);

void  Perspective_To_Screen_RENDERLIST4D(LPRENDERLIST4D renderList, LPCAM4D cam);




void  Reset_RENDERLIST4D(LPRENDERLIST4D renderList);

void  Reset_OBJECT4D(LPOBJECT4D obj);

int   Insert_POLY4D_RENDERLIST4D(LPRENDERLIST4D renderList, LPPOLY4D poly);

int   Insert_POLYF4D_RENDERLIST4D(LPRENDERLIST4D renderList, LPPOLYF4D poly);

int   Insert_OBJECT4D_RENDERLIST4D(LPRENDERLIST4D renderList, LPOBJECT4D obj, 
									   int insertLocation = 0);

void  Draw_OBJECT4D_Wire(LPOBJECT4D obj, UCHAR* videoBuffer, int lPitch);

void  Draw_RENDERLIST4D_Wire(LPRENDERLIST4D renderList, UCHAR* videoBuffer, 
							   int lPitch);

void  Draw_OBJECT4D_Wire16(LPOBJECT4D obj, UCHAR* videoBuffer, int lPitch);

void  Draw_RENDERLIST4D_Wire16(LPRENDERLIST4D renderList, UCHAR* videoBuffer, 
								 int lPitch);

void  Draw_RENDERLIST4D_Wire32(LPRENDERLIST4D renderList, UCHAR* videoBuffer, 
								 int lPitch);

void  Build_Model_To_World_Matrix4X4(LPVECTOR4D pvPos, LPMATRIX_4X4 pm);

void  Build_Camera_To_Perspective_Matrix4X4(LPCAM4D cam, LPMATRIX_4X4 pm);

void  Build_Perspective_To_Screen_4D_Matrix4X4(LPCAM4D cam, LPMATRIX_4X4 pm);

void  Build_Perspective_To_Screen_Matrix4X4(LPCAM4D cam, LPMATRIX_4X4 pm);

void  Build_Camera_To_Screen_Matrix4X4(LPCAM4D cam, LPMATRIX_4X4);

void  Convert_From_Homogeneous4D_OBJECT4D(LPOBJECT4D obj);

void  Convert_From_Homogeneous4D_RENDERLIST4D(LPRENDERLIST4D renderList);

void  Build_CAM4D_Matrix_Euler(LPCAM4D cam, int camRotSeq);

void  Build_CAM4D_Matrix_UVM(LPCAM4D cam, int mode);

void  Init_CAM4D(LPCAM4D cam, int attr, LPPOINT4D position, LPVECTOR4D direction,
				   LPVECTOR4D target, float nearClipZ, float farClipZ, float fov, 
				   float viewPortWidth, float viewPortHeight);


#endif THREEDPIPELINE