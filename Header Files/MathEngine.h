#ifndef MATH_ENGINE
#define MATH_ENGINE

#include <cstring>


#define PI       ((float)3.141592654f)
#define PI2      ((float)6.283185307f)
#define PI_DIV_2 ((float)1.570796327f)
#define PI_DIV_4 ((float)0.785398163f)
#define PI_INV   ((float)0.318309886f)

#define EPSILON_E4 (float)(1E-4)
#define EPSILON_E5 (float)(1E-5)
#define EPSILON_E6 (float)(1E-6)


#define MIN(a, b) (((a) < b) ? (a): (b))
#define MAX(a, b) (((a) > b) ? (a): (b))
#define DEG_TO_RAD(ang) ((ang) * PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / PI)
#define SWAP(a, b, t) {t = a; a = b; b = t;}


float cos_look[361];
float sin_look[361];

///////Point and Vector///////////////////////////////////////////////
typedef struct VECTOR2D_TYP
{
	union
	{
		float M[2];
		struct 
		{
			float x, y;
		};
	};
}VECTOR2D, POINT2D, *LPVECTOR2D, *LPPOINT2D;

typedef struct VECTOR3D_TYP
{
	union
	{
		float M[3];
		struct
		{
			float x, y, z;
		};
	};
}VECTOR3D, POINT3D, *LPVECTOR3D, *LPPOINT3D;

typedef struct VECTOR4D_TYP
{
	union
	{
		float M[4];
		struct 
		{
			float x, y, z, w;
		};
	};
}VECTOR4D, POINT4D, *LPVECTOR4D, *LPPOINT4D;




///////Line////////////////////////////////////////////////////////
typedef struct PARAM_LINE2D_TYP
{
	POINT2D  p0;
	POINT2D  p1;
	VECTOR2D v;
}PARAM_LINE2D, *LPPARAM_LINE2D;

typedef struct PARAM_LINE3D_TYP
{
	POINT3D  p0;
	POINT3D  p1;
	VECTOR3D v;
}PARAM_LINE3D, *LPPARAM_LINE3D;




////////Plane//////////////////////////////////////////////////////////////////
typedef struct PLANE_TYP
{
	POINT3D p0;
	VECTOR3D v;
}PLANE3D, *LPPLANE3D;




/////////Matrix//////////////////////////////////////////////////
typedef struct MATRIX_4X4_TYP
{
	union
	{
		float M[4][4];
		struct
		{
			float M00, M01, M02, M03;
			float M10, M11, M12, M13;
			float M20, M21, M22, M23;
			float M30, M31, M32, M33;
		};
	};
}MATRIX_4X4, *LPMATRIX_4X4;

typedef struct MATRIX_4X3_TYP
{
	union
	{
		float M[4][3];
		struct
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
			float M30, M31, M32;
		};
	};
}MATRIX_4X3, *LPMATRIX_4X3;

typedef struct MATRIX_3X3_TYP
{
	union
	{
		float M[3][3];
		struct
		{
			float M00, M01, M02;
			float M10, M11, M12;
			float M20, M21, M22;
		};
	};
}MATRIX_3X3, *LPMATRIX_3X3;

typedef struct MATRIX_3X2_TYP
{
	union
	{
		float M[3][2];
		struct
		{
			float M00, M01;
			float M10, M11;
			float M20, M21;
		};
	};
}MATRIX_3X2, *LPMATRIX_3X2;

typedef struct MATRIX_2X2_TYP
{
	union
	{
		float M[2][2];
		struct
		{
			float M00, M01;
			float M10, M11;
		};
	};
}MATRIX_2X2, *LPMATRIX_2X2;

typedef struct MATRIX_1X2_TYP
{
	union
	{
		float M[2];
		struct
		{
			float M00, M01;
		};
	};
}MATRIX_1X2, *LPMATRIX_1X2;

typedef struct MATRIX_1X3_TYP
{
	union
	{
		float M[3];
		struct
		{
			float M00, M01, M02;
		};
	};
}MATRIX_1X3, *LPMATRIX_1X3;

typedef struct MATRIX_1X4_TYP
{
	union
	{
		float M[4];
		struct
		{
			float M00, M01, M02, M03;
		};
	};
}MATRIX_1X4, *LPMATRIX_1X4;



///////4d quaternion////////////////////////////////////////
typedef struct QUAT_TYP
{
	union
	{
		float M[4];
		struct
		{
			float real;
			VECTOR3D imag;
		};
		struct 
		{
			float w, x, y, z;
		};
	};
}QUAT, *LPQUAT;

/////2D polar coordinates//////////////////
typedef struct POLAR2D_TYP
{
	float r;
	float theta;
}POLAR2D, *LPPOLAR2D;

//3D cylindrical coordinates//////////////////////////
typedef struct CYLINDRICAL3D_TYP
{
	float r;
	float theta;
	float z;
}CYLINDRICAL3D, *LPCYLINDRICAL3D;

typedef struct SPHERICAL3D_TYP
{
	float p;
	float theta;      //the 
	float phi;
}SPHERICAL3D, *LPSPHERICAL3D;







////inline function about Vector//////////////////////////////////////
inline void VECTOR2D_ZERO(LPVECTOR2D pV) { pV->x = pV->y = 0.0; }
inline void VECTOR3D_ZERO(LPVECTOR3D pV) { pV->x = pV->y = pV->z = 0.0; }
inline void VECTOR4D_ZERO(LPVECTOR4D pV) { pV->x = pV->y = pV->z = 0.0; pV->w = 1.0; }
inline void VECTOR2D_INITXY(LPVECTOR2D pV, float x, float y) { pV->x = x; pV->y = y; }
inline void VECTOR3D_INITXYZ(LPVECTOR3D pV, float x, float y, float z) { pV->x = x; pV->y = y; pV->z = z; }
inline void VECTOR4D_INITXYZ(LPVECTOR4D pV, float x, float y, float z) { pV->x = x; pV->y = y; pV->z = z; pV->w = 1.0; }
inline void VECTOR2D_INIT(LPVECTOR2D pDest, LPVECTOR2D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; }
inline void VECTOR3D_INIT(LPVECTOR3D pDest, LPVECTOR3D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; }
inline void VECTOR4D_INIT(LPVECTOR4D pDest, LPVECTOR4D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }
inline void VECTOR2D_COPY(LPVECTOR2D pDest, LPVECTOR2D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; }
inline void VECTOR3D_COPY(LPVECTOR3D pDest, LPVECTOR3D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; }
inline void VECTOR4D_COPY(LPVECTOR4D pDest, LPVECTOR4D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }



////inline function about Point
inline void POINT2D_INIT(LPPOINT2D pDest, LPPOINT2D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; }
inline void POINT3D_INIT(LPPOINT3D pDest, LPPOINT3D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; }
inline void POINT4D_INIT(LPPOINT4D pDest, LPPOINT4D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }
inline void POINT2D_COPY(LPPOINT2D pDest, LPPOINT2D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; }
inline void POINT3D_COPY(LPPOINT3D pDest, LPPOINT3D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; }
inline void POINT4D_COPY(LPPOINT4D pDest, LPPOINT4D pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }



//Const about Matrix
const MATRIX_4X4 IMAT_4X4 = {1, 0, 0, 0,
	                         0, 1, 0, 0,
					         0, 0, 1, 0,
					         0, 0, 0, 1};
const MATRIX_4X3 IMAT_4X3 = {1, 0, 0,
	                         0, 1, 0,
					         0, 0, 1,
					         0, 0, 0,};
const MATRIX_3X3 IMAT_3X3 = {1, 0, 0,
	                         0, 1, 0,
					         0, 0, 1};
const MATRIX_2X2 IMAT_2X2 = {1, 0,
	                         0, 1};
///////////////Macro and Function about Matrix//////////////////////////////////////////
#define MATRIX_ZERO_2X2(m) { memset((void*)(m), 0, sizeof(MATRIX_2X2)); }
#define MATRIX_ZERO_3X3(m) { memset((void*)(m), 0, sizeof(MATRIX_3X3)); }
#define MATRIX_ZERO_4X4(m) { memset((void*)(m), 0, sizeof(MATRIX_4X4)); }
#define MATRIX_ZERO_4X3(m) { memset((void*)(m), 0, sizeof(MATRIX_4X3)); }

#define MATRIX_IDENTITY_2X2(m) { memcpy((void*)(m), (void*)&IMAT_2X2, sizeof(MATRIX_2X2)); }
#define MATRIX_IDENTITY_3X3(m) { memcpy((void*)(m), (void*)&IMAT_3X3, sizeof(MATRIX_3X3)); }
#define MATRIX_IDENTITY_4X4(m) { memcpy((void*)(m), (void*)&IMAT_4X4, sizeof(MATRIX_4X4)); }
#define MATRIX_IDENTITY_4X3(m) { memcpy((void*)(m), (void*)&IMAT_4X3, sizeof(MATRIX_4X3)); }

#define MATRIX_COPY_2X2(mDest, mSour) { memcpy((void*)(mDest), (void*)(mSour), sizeof(MATRIX_2X2)); }
#define MATRIX_COPY_3X3(mDest, mSour) { memcpy((void*)(mDest), (void*)(mSour), sizeof(MATRIX_3X3)); }
#define MATRIX_COPY_4X4(mDest, mSour) { memcpy((void*)(mDest), (void*)(mSour), sizeof(MATRIX_4X4)); }
#define MATRIX_COPY_4X3(mDest, mSour) { memcpy((void*)(mDest), (void*)(mSour), sizeof(MATRIX_4X3)); }

inline void MATRIX_TRANSPOSE_3X3(LPMATRIX_3X3 pM)
{
	MATRIX_3X3 m;
	m.M00 = pM->M00; m.M01 = pM->M10; m.M02 = pM->M20;
	m.M10 = pM->M01; m.M11 = pM->M11; m.M12 = pM->M21;
	m.M20 = pM->M02; m.M21 = pM->M12; m.M22 = pM->M22;
	memcpy((void*)pM, (void*)&m, sizeof(MATRIX_3X3));
}
inline void MATRIX_TRANSPOSE_3X3(LPMATRIX_3X3 pDest, LPMATRIX_3X3 pSour)
{
	pDest->M00 = pSour->M00; pDest->M01 = pSour->M10; pDest->M02 = pSour->M20;
	pDest->M10 = pSour->M01; pDest->M11 = pSour->M11; pDest->M12 = pSour->M21;
	pDest->M20 = pSour->M02; pDest->M21 = pSour->M12; pDest->M22 = pSour->M22;
}

inline void MATRIX_TRANSPOSE_4X4(LPMATRIX_4X4 pM)
{
	MATRIX_4X4 m;
	m.M00 = pM->M00; m.M01 = pM->M10; m.M02 = pM->M20; m.M03 = pM->M30;
	m.M10 = pM->M01; m.M11 = pM->M11; m.M12 = pM->M21; m.M13 = pM->M31;
	m.M20 = pM->M02; m.M21 = pM->M12; m.M22 = pM->M22; m.M23 = pM->M32;
	m.M30 = pM->M03; m.M31 = pM->M13; m.M32 = pM->M23; m.M33 = pM->M33;
	memcpy((void*)pM, (void*)&m, sizeof(MATRIX_4X4));
}
inline void MATRIX_TRANSPOSE_4X4(LPMATRIX_4X4 pDest, LPMATRIX_4X4 pSour)
{
	pDest->M00 = pSour->M00; pDest->M01 = pSour->M10; pDest->M02 = pSour->M20; pDest->M03 = pSour->M30;
	pDest->M10 = pSour->M01; pDest->M11 = pSour->M11; pDest->M12 = pSour->M21; pDest->M13 = pSour->M31;
	pDest->M20 = pSour->M02; pDest->M21 = pSour->M12; pDest->M22 = pSour->M22; pDest->M23 = pSour->M32;
	pDest->M30 = pSour->M03; pDest->M31 = pSour->M13; pDest->M32 = pSour->M23; pDest->M33 = pSour->M33;
}

inline void MATRIX_COLUMN_SWAP_2X2(LPMATRIX_2X2 m, int c, LPMATRIX_1X2 v) { m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; }
inline void MATRIX_COLUMN_SWAP_3X3(LPMATRIX_3X3 m, int c, LPMATRIX_1X3 v) { m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; }
inline void MATRIX_COLUMN_SWAP_4X4(LPMATRIX_4X4 m, int c, LPMATRIX_1X4 v) { m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3]; }
inline void MATRIX_COLUMN_SWAP_4X3(LPMATRIX_4X3 m, int c, LPMATRIX_1X4 v) { m->M[0][c] = v->M[0]; m->M[1][c] = v->M[1]; m->M[2][c] = v->M[2]; m->M[3][c] = v->M[3]; }

inline void MATRIX_ROW_SWAP_2X2(LPMATRIX_2X2 m, int c, LPMATRIX_1X2 v) { m->M[c][0] = v->M[0]; m->M[c][1] = v->M[1]; }
inline void MATRIX_ROW_SWAP_3X3(LPMATRIX_3X3 m, int c, LPMATRIX_1X3 v) { m->M[c][0] = v->M[0]; m->M[c][1] = v->M[1]; m->M[c][2] = v->M[2]; }
inline void MATRIX_ROW_SWAP_4X4(LPMATRIX_4X4 m, int c, LPMATRIX_1X4 v) { m->M[c][0] = v->M[0]; m->M[c][1] = v->M[1]; m->M[c][2] = v->M[2]; m->M[c][3] = v->M[3]; }
inline void MATRIX_ROW_SWAP_4X3(LPMATRIX_4X3 m, int c, LPMATRIX_1X3 v) { m->M[c][0] = v->M[0]; m->M[c][1] = v->M[1]; m->M[c][2] = v->M[2]; }

/////  Function about Quaternion
inline void QUAT_ZERO(LPQUAT pQ) { pQ->x = pQ->y = pQ->z = pQ->w = 0.0; }
inline void QUAT_INITWXYZ(LPQUAT pQ, float x, float y, float, float z, float w) { pQ->x = x; pQ->y = y; pQ->z = z; pQ->w = w; }
inline void QUAT_INIT_VECTOR3D(LPQUAT pQ, LPVECTOR3D pV) { pQ->w = 0; pQ->x = pV->x; pQ->y = pV->y; pQ->z = pV->z; }
inline void QUAT_INIT(LPQUAT pDest, LPQUAT pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }
inline void QUAT_COPY(LPQUAT pDest, LPQUAT pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }



///////////////////Function Prototypes///////////////////////


float Fast_Sin(float theta);
float Fast_Cos(float theta);

int   Fast_Distance_2D(int x, int y);
float Fast_Distance_3D(float x, float y, float z);



//////////////////////////////////////////////////////////////////////////////
                 
/*               Convertion between different coordinates                   */

//////////////////////////////////////////////////////////////////////////////
void POLAR2D_TO_POINT2D(LPPOLAR2D polar, LPPOINT2D point);
void POLAR2D_TO_XY(LPPOLAR2D, float* x, float* y);
void POINT2D_TO_POLAR2D(LPPOINT2D point, LPPOLAR2D polar);
void POINT2D_TO_POLAR_RTH(LPPOINT2D point, float* r, float* theta);
void CYLINDRICAL3D_TO_POINT3D(LPCYLINDRICAL3D cyl, LPPOINT3D);
void CYLINDRICAL3D_TO_XYZ(LPCYLINDRICAL3D cyl, float* x, float* y, float* z);
void POINT3D_TO_CYLINDRICAL3D(LPPOINT3D point, LPCYLINDRICAL3D cyl);
void POINT3D_TO_CYLINDRICAL3D_RTHZ(LPPOINT3D, float* r, float* theta, float* z);
void SPHERICAL3D_TO_POINT3D(LPSPHERICAL3D sph, LPPOINT3D point);
void SPHERICAL3D_TO_XYZ(LPSPHERICAL3D sph, float* x, float* y, float* z);
void POINT3D_TO_SPHERICAL3D(LPPOINT3D point, LPSPHERICAL3D sph);
void POINT3D_TO_SPHERICAL3D_PTHPI(float* p, float* theta, float* phi);



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 2D Vector Functions                             */

//////////////////////////////////////////////////////////////////////////////
void     VECTOR2D_ADD(LPVECTOR2D va, LPVECTOR2D vb, LPVECTOR2D vsum);
VECTOR2D VECTOR2D_ADD(LPVECTOR2D va, LPVECTOR2D vb);
void     VECTOR2D_SUB(LPVECTOR2D va, LPVECTOR2D vb, LPVECTOR2D vdiff);
VECTOR2D VECTOR2D_SUB(LPVECTOR2D va, LPVECTOR2D vb);
void     VECTOR2D_SCALE(float k, LPVECTOR2D vector);
void     VECTOR2D_SCALE(float k, LPVECTOR2D vector, LPVECTOR2D scaled);
float    VECTOR2D_DOT(LPVECTOR2D va, LPVECTOR2D vb);
float    VECTOR2D_LENGTH(LPVECTOR2D vector);
float    VECTOR2D_LENGTH_FAST(LPVECTOR2D vector);
void     VECTOR2D_NORMALIZE(LPVECTOR2D vector);
void     VECTOR2D_NORMALIZE(LPVECTOR2D va, LPVECTOR2D vn);
void     VECTOR2D_BUILD(LPVECTOR2D init, LPVECTOR2D term, LPVECTOR2D result);
float    VECTOR2D_COSTH(LPVECTOR2D va, LPVECTOR2D vb);
void     VECTOR2D_PRINT(LPVECTOR2D va, char* name);



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 3D Vector Functions                             */

//////////////////////////////////////////////////////////////////////////////
void     VECTOR3D_ADD(LPVECTOR3D va, LPVECTOR3D vb, LPVECTOR3D vsum);
VECTOR3D VECTOR3D_ADD(LPVECTOR3D va, LPVECTOR3D vb);
void     VECTOR3D_SUB(LPVECTOR3D va, LPVECTOR3D vb, LPVECTOR3D vdiff);
VECTOR3D VECTOR3D_SUB(LPVECTOR3D va, LPVECTOR3D vb);
void     VECTOR3D_SCALE(float k, LPVECTOR3D vector);
void     VECTOR3D_SCALE(float k, LPVECTOR3D vector, LPVECTOR3D scaled);
float    VECTOR3D_DOT(LPVECTOR3D va, LPVECTOR3D vb);
void     VECTOR3D_CROSS(LPVECTOR3D va, LPVECTOR3D vb, LPVECTOR3D vn);
VECTOR3D VECTOR3D_CROSS(LPVECTOR3D va, LPVECTOR3D vb);
float    VECTOR3D_LENGTH(LPVECTOR3D vector);
float    VECTOR3D_LENGTH_FAST(LPVECTOR3D vector);
void     VECTOR3D_NORMALIZE(LPVECTOR3D vector);
void     VECTOR3D_NORMALIZE(LPVECTOR3D va, LPVECTOR3D vn);
void     VECTOR3D_BUILD(LPVECTOR3D init, LPVECTOR3D term, LPVECTOR3D result);
float    VECTOR3D_COSTH(LPVECTOR3D va, LPVECTOR3D vb);
void     VECTOR3D_PRINT(LPVECTOR3D va, char* name);



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 4D Vector Functions                             */

//////////////////////////////////////////////////////////////////////////////
void     VECTOR4D_ADD(LPVECTOR4D va, LPVECTOR4D vb, LPVECTOR4D vsum);
VECTOR4D VECTOR4D_ADD(LPVECTOR4D va, LPVECTOR4D vb);
void     VECTOR4D_SUB(LPVECTOR4D va, LPVECTOR4D vb, LPVECTOR4D vdiff);
VECTOR4D VECTOR4D_SUB(LPVECTOR4D va, LPVECTOR4D vb);
void     VECTOR4D_SCALE(float k, LPVECTOR4D vector);
void     VECTOR4D_SCALE(float k, LPVECTOR4D vector, LPVECTOR4D scaled);
float    VECTOR4D_DOT(LPVECTOR4D va, LPVECTOR4D vb);
void     VECTOR4D_CROSS(LPVECTOR4D va, LPVECTOR4D vb, LPVECTOR4D vn);
VECTOR4D VECTOR4D_CROSS(LPVECTOR4D va, LPVECTOR4D vb);
float    VECTOR4D_LENGTH(LPVECTOR4D vector);
float    VECTOR4D_LENGTH_FAST(LPVECTOR4D vector);
void     VECTOR4D_NORMALIZE(LPVECTOR4D vector);
void     VECTOR4D_NORMALIZE(LPVECTOR4D va, LPVECTOR4D vn);
void     VECTOR4D_BUILD(LPVECTOR4D init, LPVECTOR4D term, LPVECTOR4D result);
float    VECTOR4D_COSTH(LPVECTOR4D va, LPVECTOR4D vb);
void     VECTOR4D_PRINT(LPVECTOR4D va, char* name);



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 2D Matrix Functions                             */

//////////////////////////////////////////////////////////////////////////////
void  MATRIX_INIT_2X2(LPMATRIX_2X2 m, float m00, float m01, float m10, float m11);
void  MATRIX_PRINT_2X2(LPMATRIX_2X2 m, char* name);
float MATRIX_DET_2X2(LPMATRIX_2X2 m);
void  MATRIX_ADD_2X2(LPMATRIX_2X2 ma, LPMATRIX_2X2 mb, LPMATRIX_2X2 msum);
void  MATRIX_MUL_2X2(LPMATRIX_2X2 ma, LPMATRIX_2X2 mb, LPMATRIX_2X2 mprod);
void  MATRIX_MUL_2X2_2X1(LPMATRIX_2X2 ma, LPMATRIX_1X2 mb, LPMATRIX_2X2 mprod);
void  MATRIX_INVERSE_2X2(LPMATRIX_2X2 m, LPMATRIX_2X2 mi);
void  SOLVE_SYSTEM_2X2(LPMATRIX_1X2 mx, LPMATRIX_2X2 ma, LPMATRIX_1X2 mb);



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 3D Matrix Functions                             */

//////////////////////////////////////////////////////////////////////////////
void  MATRIX_INIT_3X3(LPMATRIX_3X3 m,
					  float m00, float m01, float m02,
					  float m10, float m11, float m12,
					  float m20, float m21, float m22);
void  MATRIX_PRINT_3X3(LPMATRIX_3X3 m, char* name);
float MATRIX_DET_3X3(LPMATRIX_3X3 m);
void  MATRIX_ADD_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mb, LPMATRIX_3X3 msum);
void  MATRIX_MUL_1X2_3X2(LPMATRIX_1X2 ma, LPMATRIX_3X2 mb, LPMATRIX_1X2 mprod);
void  MATRIX_MUL_1X3_3X3(LPMATRIX_1X3 ma, LPMATRIX_3X3 mb, LPMATRIX_1X3 mprod);
void  MATRIX_MUL_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mb, LPMATRIX_3X3 mprod);
void  MATRIX_INVERSE_3X3(LPMATRIX_3X3 m, LPMATRIX_3X3 mi);
void  SOLVE_SYSTEM_3X3(LPMATRIX_1X3 mx, LPMATRIX_3X3 ma, LPMATRIX_1X3 mb);



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 4D Matrix Functions                             */

//////////////////////////////////////////////////////////////////////////////
void  MATRIX_INIT_4X4(LPMATRIX_4X4 m, float m00, float m01, float m02, float m03,
					                  float m10, float m11, float m12, float m13,
									  float m20, float m21, float m22, float m23,
									  float m30, float m31, float m32, float m33);
void  MATRIX_PRINT_4X4(LPMATRIX_4X4 m, char* name);
float MATRIX_DET_4X4(LPMATRIX_4X4 m);
void  MATRIX_ADD_4X4(LPMATRIX_4X4 ma, LPMATRIX_4X4 mb, LPMATRIX_4X4 msum);
void  MATRIX_MUL_4X4(LPMATRIX_4X4 ma, LPMATRIX_4X4 mb, LPMATRIX_4X4 mprod);
void  MATRIX_MUL_1X4_4X4(LPMATRIX_1X4 ma, LPMATRIX_4X4 mb, LPMATRIX_1X4 mprod);
void  MATRIX_MUL_1X4_4X4(LPVECTOR4D va, LPMATRIX_4X4 m, LPVECTOR4D vres);
void  MATRIX_MUL_1X3_4X4(LPMATRIX_1X3 ma, LPMATRIX_4X4 mb, LPMATRIX_1X3 mprod);
void  MATRIX_MUL_1X3_4X4(LPVECTOR3D va, LPMATRIX_4X4 m, LPVECTOR3D vres);
void  MATRIX_INVERSE_4X4(LPMATRIX_4X4 m, LPMATRIX_4X4 mi);
void  SOLVE_SYSTEM_4X4(LPMATRIX_1X4 mx, LPMATRIX_4X4 ma, LPMATRIX_1X4 mb);

void  QUAT_ADD(LPQUAT qa, LPQUAT qb, LPQUAT qsum);
void  QUAT_SUB(LPQUAT qa, LPQUAT qb, LPQUAT qdiff);
void  QUAT_CONJUGATE(LPQUAT qa, LPQUAT qconj);
void  QUAT_SCALE(LPQUAT qa, float k, LPQUAT qScaled);
void  QUAT_SCALE(LPQUAT qa, float k);
float QUAT_NORM(LPQUAT q);
float QUAT_NORM_SQUARED(LPQUAT q);
void  QUAT_NORMALISE(LPQUAT q, LPQUAT qn);
void  QUAT_NORMALISE(LPQUAT	q);
void  QUAT_UNIT_INVERSE(LPQUAT q, LPQUAT qi);
void  QUAT_UNIT_INVERSE(LPQUAT q);
void  QUAT_INVERSE(LPQUAT q, LPQUAT qi);
void  QUAT_INVERSE(LPQUAT q);
void  QUAT_MUL(LPQUAT qa, LPQUAT qb, LPQUAT qprod);
void  QUAT_TRIPLE_PRODUCT(LPQUAT qa, LPQUAT qb, LPQUAT qc, LPQUAT qprod);
void  VECTOR3D_THETA_TO_QUAT(LPQUAT q, LPVECTOR3D v, float theta);
void  VECTOR4D_THETA_TO_QUAT(LPQUAT q, LPVECTOR4D v, float theta);
void  EULERZYX_TO_QUAT(LPQUAT q, float theta_z, float theta_y, float theta_x);
void  QUAT_TO_VECTOR3D_THETA(LPQUAT q, LPVECTOR3D v, float* theta);
void  QUAT_PRINT(LPQUAT q, char* name);

void  Init_Param_line2D(LPPOINT2D pInit, LPPOINT2D pTerm, LPPARAM_LINE2D pLine);
void  Compute_Param_Line2D(LPPARAM_LINE2D pLine, float t, LPPOINT2D pPoint);
int   Intersect_Param_Lines2D(LPPARAM_LINE2D pLine1, LPPARAM_LINE2D pLine2, float* t1, float* t2);
int   Intersect_Param_Lines2D(LPPARAM_LINE2D pLine1, LPPARAM_LINE2D pLine2, LPPOINT2D pPoint);

void  Init_Param_Line3D(LPPOINT3D pInit, LPPOINT3D pTerm, LPPARAM_LINE3D pLine);
void  Compute_Param_Line3D(LPPARAM_LINE3D pLine, float t, LPPOINT3D pPoint);

void  Plane3D_Init(LPPLANE3D pPlane, LPPOINT3D pPoint, LPVECTOR3D pNormal, int normalize);
float Compute_Point_In_Plan3D(LPPOINT3D pPoint, LPPLANE3D pPlane);
int   Intersect_Param_Line3D_Plane3D(LPPARAM_LINE3D pLine, LPPLANE3D pPlane, float* t, LPPOINT3D pPoint);





#endif