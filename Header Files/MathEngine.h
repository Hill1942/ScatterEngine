#ifndef MATH_ENGINE
#define MATH_ENGINE


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
			x, y, z, w;
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
typedef struct PLANE_TYP;
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
}MATRIX_4X4, *PMATRIX_4X4;

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
}MATRIX_4X3, *PMATRIX_4X3;

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
}MATRIX_3X3, *PMATRIX_3X3;

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
}MATRIX_3X2, *PMATRIX_3X2;

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
}MATRIX_2X2, *PMATRIX_2X2;

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
}MATRIX_1X2, *PMATRIX_1X2;

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
}MATRIX_1X3, *PMATRIX_1X3;

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
}MATRIX_1X4, *PMATRIX_1X4;



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
}QUAT, *PQUAT;










#endif