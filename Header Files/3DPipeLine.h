#ifndef THREEDPIPELINE
#define THREEDPIPELINE



#define POLY4DV1_ATTR_2SIDED      0x0001
#define POLY4DV1_ATTR_TRANSPARENT 0x0002
#define POLY4DV1_ATTR_8BITCOLOR   0x0004
#define POLY4DV1_ATTR_RGB16       0x0008
#define POLY4DV1_ATTR_RGB24       0x0010

#define POLY4DV1_ATTR_SHADE_MODE_PURE      0x0020
#define POLY4DV1_ATTR_SHADE_MODE_CONSTANT  0x0020
#define POLY4DV1_ATTR_SHADE_MODE_FLAT      0x0040
#define POLY4DV1_ATTR_SHADE_MODE_GOURAUD   0x0080
#define POLY4DV1_ATTR_SHADE_MODE_PHONG     0x0100
#define POLY4DV1_ATTR_SHADE_MODE_FASTPHONG 0x0100
#define POLY4DV1_ATTR_SHADE_MODE_TEXTURE   0x0200

#define POLY4DV1_STATE_ACTIVE    0x0001
#define POLY4DV1_STATE_CLIPPED   0x0002
#define POLY4DV1_STATE_BACKFACE  0x0004

#define OBJECT4DV1_MAX_VERTICES  1024
#define OBJECT4DV1_MAX_POLYS     1024

#define OBJECT4DV1_STATE_ACTIVE  0x0001
#define OBJECT4DV1_STATE_VISIBLE 0x0002
#define OBJECT4DV1_STATE_CULLED  0x0004

#define RENDERLIST4DV1_MAX_POLYS 32768

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

typedef struct POLY4DV1_TYP
{
	int state;
	int attr;
	int color;

	LPPOINT4D vList;
	int       vert[3];

}POLY4DV1, *LPPOLY4DV1;

typedef struct POLYF4DV1_TYP
{
	int state;
	int attr;
	int color;

	POINT4D vList[3];
	POINT4D vTranList[3];

	POLYF4DV1_TYP* next;
	POLYF4DV1_TYP* prev;

}POLYF4DV1, *LPPOLYF4DV1;

typedef struct OBJECT4DV1_TYP
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
	POINT4D vLocalList[OBJECT4DV1_MAX_VERTICES];
	POINT4D vTransList[OBJECT4DV1_MAX_VERTICES];

	int      numPolys;
	POLY4DV1 polyList[OBJECT4DV1_MAX_POLYS];

}OBJECT4DV1, *LPOBJECT4DV1;

typedef struct CAM4DV1_TYP
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

}CAM4DV1, *LPCAM4DV1;

typedef struct RENDERLIST4DV1_TYP
{
	int state;
	int attr;

	LPPOLYF4DV1 polyPointer[RENDERLIST4DV1_MAX_POLYS];
	POLYF4DV1   polyData[RENDERLIST4DV1_MAX_POLYS];

	int numPolys;
}RENDERLIST4DV1, *LPRENDERLIST4DV1;


char* Get_Line_PLG(char* buffer, int maxLength, FILE* fp);

float Compute_OBJECT4DV1_Radius(LPOBJECT4DV1 obj);

int   Load_OBJECT4DV1_PLG(LPOBJECT4DV1 obj, char* filename, LPVECTOR4D scale, 
						  LPVECTOR4D pos, LPVECTOR4D rot);

void  Translate_OBJECT4DV1(LPOBJECT4DV1 obj, LPVECTOR4D pvTranslate);

void  Scale_OBJECT4DV1(LPOBJECT4DV1 obj, LPVECTOR4D pvScale);

void  Build_XYZ_Rotation_Matrix4X4(float xTheta, float yTheta, float zTheta,
								   LPMATRIX_4X4 pmRotation);

void  Transform_OBJECT4DV1(LPOBJECT4DV1 obj, LPMATRIX_4X4 pmTransform, 
						   int coordinate, int transformBasis);

void  Rotate_XYZ_OBJECT4DV1(LPOBJECT4DV1 obj, float xTheta, float yTheta, float zTheta);

void  Model_To_Word_OBJECT4DV1(LPOBJECT4DV1 obj, int coordinate = TRANSFORM_LOCAL_TO_TRANS);

int   Cull_OBJEC4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam, int cullFlags);

void  Remove_Backfaces_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam);

void  Remove_Backfaces_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam);

void  World_To_Camera_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam);

void  Camera_To_Perspective_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam);

void  Camera_To_Perspective_Screen_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam);

void  Perspective_To_Screen_OBJECT4DV1(LPOBJECT4DV1 obj, LPCAM4DV1 cam);

void  Transform_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPMATRIX_4X4 pmTransform,
							   int coordinate);

void  Model_To_World_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPPOINT4D worldPos,
								   int coordinate = TRANSFORM_LOCAL_TO_TRANS);

void  World_To_Camera_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam);

void  Camera_To_Perspective_RENDERLIST4DV1(LPRENDERLIST4DV1 rendrerList, LPCAM4DV1 cam);

void  Camera_To_Perspective_Screen_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam);

void  Perspective_To_Screen_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPCAM4DV1 cam);




void  Reset_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList);

void  Reset_OBJECT4DV1(LPOBJECT4DV1 obj);

int   Insert_POLY4DV1_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPPOLY4DV1 poly);

int   Insert_POLYF4DV1_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPPOLYF4DV1 poly);

int   Insert_OBJECT4DV1_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList, LPOBJECT4DV1 obj, 
									   int insertLocation = 0);

void  Draw_OBJECT4DV1_Wire(LPOBJECT4DV1 obj, UCHAR* videoBuffer, int lPitch);

void  Draw_RENDERLIST4DV1_Wire(LPRENDERLIST4DV1 renderList, UCHAR* videoBuffer, 
							   int lPitch);

void  Draw_OBJECT4DV1_Wire16(LPOBJECT4DV1 obj, UCHAR* videoBuffer, int lPitch);

void  Draw_RENDERLIST4DV1_Wire16(LPRENDERLIST4DV1 renderList, UCHAR* videoBuffer, 
								 int lPitch);

void  Draw_RENDERLIST4DV1_Wire32(LPRENDERLIST4DV1 renderList, UCHAR* videoBuffer, 
								 int lPitch);

void  Build_Model_To_World_Matrix4X4(LPVECTOR4D pvPos, LPMATRIX_4X4 pm);

void  Build_Camera_To_Perspective_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm);

void  Build_Perspective_To_Screen_4D_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm);

void  Build_Perspective_To_Screen_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4 pm);

void  Build_Camera_To_Screen_Matrix4X4(LPCAM4DV1 cam, LPMATRIX_4X4);

void  Convert_From_Homogeneous4D_OBJECT4DV1(LPOBJECT4DV1 obj);

void  Convert_From_Homogeneous4D_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList);

void  Build_CAM4DV1_Matrix_Euler(LPCAM4DV1 cam, int camRotSeq);

void  Build_CAM4DV1_Matrix_UVM(LPCAM4DV1 cam, int mode);

void  Init_CAM4DV1(LPCAM4DV1 cam, int attr, LPPOINT4D position, LPVECTOR4D direction,
				   LPVECTOR4D target, float nearClipZ, float farClipZ, float fov, 
				   float viewPortWidth, float viewPortHeight);


#endif THREEDPIPELINE
