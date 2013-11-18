#ifndef MATH_ENGINE
#define MATH_ENGINE

#include <cstring>

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
		float M[1][2];
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
		float M[1][3];
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
		float M[1][4];
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
typedef struct CYLINDRICAL_TYP
{
	float r;
	float theta;
	float z;
}CYLINDRICAL, *PCYLINDRICAL;

typedef struct SPHERICAL3D_TYP
{
	float p;
	float theta;      //the 
	float phi;
}SPHERICAL3D, *LPSPHERICAL3D;







////Function about Vector//////////////////////////////////////
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



////Function about Point
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
#define MATRIX_ZERO_2X2(m) { memset((void*)m, 0, sizeof(MATRIX_2X2)); }
#define MATRIX_ZERO_3X3(m) { memset((void*)m, 0, sizeof(MATRIX_3X3)); }
#define MATRIX_ZERO_4X4(m) { memset((void*)m, 0, sizeof(MATRIX_4X4)); }
#define MATRIX_ZERO_4X3(m) { memset((void*)m, 0, sizeof(MATRIX_4X3)); }

#define MATRIX_ZERO_2X2(m) { memcpy((void*)m, (void*)&IMAT_2X2, sizeof(MATRIX_2X2)); }
#define MATRIX_ZERO_3X3(m) { memcpy((void*)m, (void*)&IMAT_3X3, sizeof(MATRIX_3X3)); }
#define MATRIX_ZERO_4X4(m) { memcpy((void*)m, (void*)&IMAT_4X4, sizeof(MATRIX_4X4)); }
#define MATRIX_ZERO_4X3(m) { memcpy((void*)m, (void*)&IMAT_4X3, sizeof(MATRIX_4X3)); }

#define MATRIX_COPY_2X2(mDest, mSour) { memcpy((void*)mDest, (void*)mSour, sizeof(MATRIX_2X2)); }
#define MATRIX_COPY_3X3(mDest, mSour) { memcpy((void*)mDest, (void*)mSour, sizeof(MATRIX_3X3)); }
#define MATRIX_COPY_4X4(mDest, mSour) { memcpy((void*)mDest, (void*)mSour, sizeof(MATRIX_4X4)); }
#define MATRIX_COPY_4X3(mDest, mSour) { memcpy((void*)mDest, (void*)mSour, sizeof(MATRIX_4X3)); }

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






/////  Function about Quaternion
inline void QUAT_ZERO(LPQUAT pQ) { pQ->x = pQ->y = pQ->z = pQ->w = 0.0; }
inline void QUAT_INITWXYZ(LPQUAT pQ, float x, float y, float, float z, float w) { pQ->x = x; pQ->y = y; pQ->z = z; pQ->w = w; }
inline void QUAT_INIT_VECTOR3D(LPQUAT pQ, LPVECTOR3D pV) { pQ->w = 0; pQ->x = pV->x; pQ->y = pV->y; pQ->z = pV->z; }
inline void QUAT_INIT(LPQUAT pDest, LPQUAT pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }
inline void QUAT_COPY(LPQUAT pDest, LPQUAT pSour) { pDest->x = pSour->x; pDest->y = pSour->y; pDest->z = pSour->z; pDest->w = pSour->w; }

#endif