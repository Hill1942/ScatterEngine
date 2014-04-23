#include "MathEngine.h"




#include <cmath>


float cos_look[361];
float sin_look[361];


void Build_Sin_Cos_Tables()
{
	for (int ang = 0; ang < 360; ang++)
	{
		float theta = (float)ang * PI / (float)180;
		cos_look[ang] = cos(theta);
		sin_look[ang] = sin(theta);
	}
}

float Fast_Sin(float theta)
{
	theta = fmodf(theta, 360);
	if (theta < 0)
		theta += 360.0;

	int theta_int  = (int)theta;
	int theta_frac = theta - theta_int;

	return (sin_look[theta_int] + theta_frac * (sin_look[theta_int + 1] - sin_look[theta_int]));
}

float Fast_Cos(float theta)
{
	theta = fmodf(theta, 360);
	if (theta < 0)
		theta += 360.0;

	int theta_int  = (int)theta;
	int theta_frac = theta - theta_int;

	return (cos_look[theta_int] + theta_frac * (cos_look[theta_int + 1] - cos_look[theta_int]));
}

int Fast_Distance_2D(int x, int y)
{
	x = abs(x);
	y = abs(y);
	int min = MIN(x, y);
	return x + y - min>>1 - min>>2 + min>>4;
}

float Fast_Distance_3D(float fx, float fy, float fz)
{
	int temp;  // used for swaping
    int x,y,z; // used for algorithm

// make sure values are all positive
    x = fabs(fx) * 1024;
    y = fabs(fy) * 1024;
    z = fabs(fz) * 1024;

// sort values
    if (y < x) SWAP(x,y,temp)

    if (z < y) SWAP(y,z,temp)

    if (y < x) SWAP(x,y,temp)

    int dist = (z + 11 * (y >> 5) + (x >> 2) );

// compute distance with 8% error
    return((float)(dist >> 10));

}


//////////////////////////////////////////////////////////////////////////////
                 
/*                     QUAD Functions                             */

//////////////////////////////////////////////////////////////////////////////
void QUAT_ADD(LPQUAT qa, LPQUAT qb, LPQUAT qsum)
{
	qsum->w = qa->w + qb->w;
	qsum->x = qa->x + qb->x;
	qsum->y = qa->y + qb->y;
	qsum->z = qa->z + qb->z;
}

void QUAT_SUB(LPQUAT qa, LPQUAT qb, LPQUAT qdiff)
{
	qdiff->w = qa->w - qb->w;
	qdiff->x = qa->x - qb->x;
	qdiff->y = qa->y - qb->y;
	qdiff->z = qa->z - qb->z;
}

void QUAT_CONJUGATE(LPQUAT q, LPQUAT qconj)
{
	qconj->w =   q->w;
	qconj->x = - q->x;
	qconj->y = - q->y;
	qconj->z = - q->z;
}

void QUAT_SCALE(LPQUAT q, float k, LPQUAT qScaled)
{
	qScaled->w = k * q->w;
	qScaled->x = k * q->x;
	qScaled->y = k * q->y;
	qScaled->z = k * q->z;
}

void QUAT_SCALE(LPQUAT q, float k)
{
	q->w *= k;
	q->x *= k;
	q->y *= k;
	q->z *= k;
}

float QUAT_NORM(LPQUAT q)
{
	return (sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z));
}

float QUAT_NORM_SQUARED(LPQUAT q)
{
	return q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z;
}

void QUAT_NORMALISE(LPQUAT q, LPQUAT qn)
{
	float qlength_inv = 1.0 / sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
	qn->w = q->w * qlength_inv;
	qn->x = q->x * qlength_inv;
	qn->y = q->y * qlength_inv;
	qn->z = q->z * qlength_inv;
}

void QUAT_NORMALISE(LPQUAT q)
{
	float qlength_inv = 1.0 / sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
	q->w = q->w * qlength_inv;
	q->x = q->x * qlength_inv;
	q->y = q->y * qlength_inv;
	q->z = q->z * qlength_inv;
}

void QUAT_UNIT_INVERSE(LPQUAT q, LPQUAT qi)
{
	qi->w =   q->w;
	qi->x = - q->x;
	qi->y = - q->y;
	qi->z = - q->z;
}

void QUAT_UNIT_INVERSE(LPQUAT q)
{
	q->x = - q->x;
	q->y = - q->y;
	q->z = - q->z;
}

void  QUAT_INVERSE(LPQUAT q, LPQUAT qi)
{

}
void  QUAT_INVERSE(LPQUAT q)
{

}



////////////////   POINT TO POLAR, CYLINDRICAL
void POLAR2D_TO_POINT2D(LPPOLAR2D polar, LPPOINT2D point)
{
	point->x = polar->r * cosf(polar->theta);
	point->y = polar->r * sinf(polar->theta);
}

void POLAR2D_TO_XY(LPPOLAR2D polar, float* x, float* y)
{
	*x = polar->r * cosf(polar->theta);
	*y = polar->r * sinf(polar->theta);
}

void POINT2D_TO_POLAR2D(LPPOINT2D point, LPPOLAR2D polar)
{
	polar->r     = sqrtf(point->x * point->x + point->y * point->y);
	polar->theta = atanf(point->y / point->x);
}

void POINT2D_TO_POLAR_RTH(LPPOINT2D point, float* r, float* theta)
{
	*r     = sqrtf(point->x * point->x + point->y * point->y);
	*theta = atanf(point->y / point->x);
}

void CYLINDRICAL3D_TO_POINT3D(LPCYLINDRICAL3D cyl, LPPOINT3D point)
{
	point->x = cyl->r * cosf(cyl->theta);
	point->y = cyl->r * sinf(cyl->theta);
	point->z = cyl->z;
}

void CYLINDRICAL3D_TO_XYZ(LPCYLINDRICAL3D cyl, float* x, float* y, float* z)
{
	*x = cyl->r * cosf(cyl->theta);
	*y = cyl->r * sinf(cyl->theta);
	*z = cyl->z;
}

void POINT3D_TO_CYLINDRICAL3D(LPPOINT3D point, LPCYLINDRICAL3D cyl)
{
	cyl->r     = sqrtf(point->x * point->x + point->y * point->y);
	cyl->theta = atanf(point->y / point->x);
	cyl->z     = point->z;
}

void POINT3D_TO_CYLINDRICAL3D_RTHZ(LPPOINT3D point, float* r, float* theta, float* z)
{
	*r     = sqrtf(point->x * point->x + point->y * point->y);
	*theta = atanf(point->y / point->x);
	*z     = point->z;
}

void SPHERICAL3D_TO_POINT3D(LPSPHERICAL3D sph, LPPOINT3D point)
{
	float r;

	r        = sph->p * sinf(sph->phi);
	point->z = sph->p * cosf(sph->phi);

	point->x = r * cosf(sph->theta);
	point->y = r * sinf(sph->theta);
}

void SPHERICAL3D_TO_XYZ(LPSPHERICAL3D sph, float* x, float* y, float* z)
{
	float r;

	r  = sph->p * sinf(sph->phi);
	*z = sph->p * cosf(sph->phi);

	*x = r * cosf(sph->theta);
	*y = r * sinf(sph->theta);
}

void POINT3D_TO_SPHERICAL3D(LPPOINT3D point, LPSPHERICAL3D sph)
{
	sph->p     = sqrtf(point->x * point->x + point->y * point->y + point->z * point->z);
	sph->theta = atanf(point->y / point->x);

	float r    = sqrtf(point->x * point->x + point->y * point->y);
	sph->phi   = asinf(r / sph->p);
}

void POINT3D_TO_SPHERICAL3D_PTHPI(LPPOINT3D point, float* p, float* theta, float* phi)
{
	*p      = sqrtf(point->x * point->x + point->y * point->y + point->z * point->z);
	*theta  = atanf(point->y / point->x);

	float r = sqrtf(point->x * point->x + point->y * point->y);
	*phi    = asinf(r / (*p));
}


//////////////////////   Vector2D Functions //////////////
void VECTOR2D_ADD(LPVECTOR2D va, LPVECTOR2D vb, LPVECTOR2D vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
}

VECTOR2D VECTOR2D_ADD(LPVECTOR2D va, LPVECTOR2D vb)
{
	VECTOR2D vsum;
	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	return vsum;
}

void VECTOR2D_SUB(LPVECTOR2D va, LPVECTOR2D vb, LPVECTOR2D vdiff)
{
	vdiff->x = va->x - vb->y;
	vdiff->y = va->y - vb->y;
}

VECTOR2D VECTOR2D_SUB(LPVECTOR2D va, LPVECTOR2D vb)
{
	VECTOR2D vdiff;
	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	return vdiff;
}

void VECTOR2D_SCALE(float k, LPVECTOR2D vector)
{
	vector->x *= k;
	vector->y *= k;
}

void VECTOR2D_SCALE(float k, LPVECTOR2D vector, LPVECTOR2D scaled)
{
	scaled->x = vector->x * k;
	scaled->y = vector->y * k;
}

float VECTOR2D_DOT(LPVECTOR2D va, LPVECTOR2D vb)
{
	return va->x * vb->x + va->y * vb->y;
}

float VECTOR2D_LENGTH(LPVECTOR2D vector)
{
	return sqrtf(vector->x * vector->x + vector->y * vector->y);
}

float VECTOR2D_LENGTH_FAST(LPVECTOR2D vector)
{
	return Fast_Distance_2D(vector->x, vector->y);
}

void VECTOR2D_NORMALIZE(LPVECTOR2D vector)
{
	float temp = 1 / sqrtf(vector->x * vector->x + vector->y * vector->y);
	vector->x *= temp;
	vector->y *= temp;
}
void VECTOR2D_NORMALIZE(LPVECTOR2D va, LPVECTOR2D vn)
{
	float temp = 1 / sqrtf(va->x * va->x + va->y * va->y);
	vn->x = va->x * temp;
	vn->y = va->y * temp;
}

void VECTOR2D_BUILD(LPVECTOR2D init, LPVECTOR2D term, LPVECTOR2D result)
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
}

float VECTOR2D_COSTH(LPVECTOR2D va, LPVECTOR2D vb)
{
	return VECTOR2D_DOT(va, vb) / (VECTOR2D_LENGTH(va) * VECTOR2D_LENGTH(vb));
}
/*
void VECTOR2D_PRINT(LPVECTOR2D va, char* name = "v")
{
	Write_Error("\n%s=[", name);
	for (int index = 0; index < 2; index++)
		Write_Error("%f, ", va->M[index]);
	Write_Error("]");
}*/

//////////////////  Vector3D Functions  ///////////////////
void VECTOR3D_ADD(LPVECTOR3D va, LPVECTOR3D vb, LPVECTOR3D vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
}

VECTOR3D VECTOR3D_ADD(LPVECTOR3D va, LPVECTOR3D vb)
{
	VECTOR3D vsum;
	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	vsum.z = va->z + vb->z;
	return vsum;
}

void VECTOR3D_SUB(LPVECTOR3D va, LPVECTOR3D vb, LPVECTOR3D vdiff)
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
}

VECTOR3D VECTOR3D_SUB(LPVECTOR3D va, LPVECTOR3D vb)
{
	VECTOR3D vdiff;
	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	vdiff.z = va->z - vb->z;
	return vdiff;
}

void VECTOR3D_SCALE(float k, LPVECTOR3D vector)
{
	vector->x *= k;
	vector->y *= k;
	vector->z *= k;
}

void VECTOR3D_SCALE(float k, LPVECTOR3D vector, LPVECTOR3D scaled)
{
	scaled->x = vector->x * k;
	scaled->y = vector->y * k;
	scaled->z = vector->z * k;
}

float VECTOR3D_DOT(LPVECTOR3D va, LPVECTOR3D vb)
{
	return va->x * vb->x + va->y * vb->y + va->z * vb->z;
}

void VECTOR3D_CROSS(LPVECTOR3D va, LPVECTOR3D vb, LPVECTOR3D vn)
{
	vn->x = va->y * vb->z - va->z * vb->y;
	vn->y = va->z * vb->x - va->x * vb->z;
	vn->z = va->x * vb->y - va->y * vb->x;
}

VECTOR3D VECTOR3D_CROSS(LPVECTOR3D va, LPVECTOR3D vb)
{
	VECTOR3D vn;
	vn.x = va->y * vb->z - va->z * vb->y;
	vn.y = va->z * vb->x - va->x * vb->z;
	vn.z = va->x * vb->y - va->y * vb->x;
	return vn;
}

float VECTOR3D_LENGTH(LPVECTOR3D vector)
{
	return sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

float VECTOR3D_LENGTH_FAST(LPVECTOR3D vector)
{
	return Fast_Distance_3D(vector->x, vector->y, vector->z);
}
void VECTOR3D_NORMALIZE(LPVECTOR3D vector)
{
	float temp = 1 / sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
	vector->x *= temp;
	vector->y *= temp;
	vector->z *= temp;
}

void VECTOR3D_NORMALIZE(LPVECTOR3D va, LPVECTOR3D vn)
{
	float temp = 1 / sqrtf(va->x * va->x + va->y * va->y + va->z * va->z);
	vn->x = va->x * temp;
	vn->y = va->y * temp;
	vn->z = va->z * temp;
}

void VECTOR3D_BUILD(LPVECTOR3D init, LPVECTOR3D term, LPVECTOR3D result)
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
}

float VECTOR3D_COSTH(LPVECTOR3D va, LPVECTOR3D vb)
{
	return VECTOR3D_DOT(va, vb) / (VECTOR3D_LENGTH(va) * VECTOR3D_LENGTH(vb));
}
/*
void  VECTOR3D_PRINT(LPVECTOR3D va, char* name = "v")
{
	Write_Error("\n%s=[", name);
	for (int index = 0; index < 3; index++)
		Write_Error("%f, ",va->M[index]);
	Write_Error("]");
}*/

////////////////// Vector4D Functions  /////////////////
void VECTOR4D_ADD(LPVECTOR4D va, LPVECTOR4D vb, LPVECTOR4D vsum)
{
	vsum->x = va->x + vb->x;
	vsum->y = va->y + vb->y;
	vsum->z = va->z + vb->z;
	vsum->w = 1;
}

VECTOR4D VECTOR4D_ADD(LPVECTOR4D va, LPVECTOR4D vb)
{
	VECTOR4D vsum;
	vsum.x = va->x + vb->x;
	vsum.y = va->y + vb->y;
	vsum.z = va->z + vb->z;
	vsum.w = 1;
	return vsum;
}

void VECTOR4D_SUB(LPVECTOR4D va, LPVECTOR4D vb, LPVECTOR4D vdiff)
{
	vdiff->x = va->x - vb->x;
	vdiff->y = va->y - vb->y;
	vdiff->z = va->z - vb->z;
	vdiff->w = 1;
}

VECTOR4D VECTOR4D_SUB(LPVECTOR4D va, LPVECTOR4D vb)
{
	VECTOR4D vdiff;
	vdiff.x = va->x - vb->x;
	vdiff.y = va->y - vb->y;
	vdiff.z = va->z - vb->z;
	vdiff.w = 1;
	return vdiff;
}

void VECTOR4D_SCALE(float k, LPVECTOR4D vector)
{
	vector->x *= k;
	vector->y *= k;
	vector->z *= k;
	vector->w = 1;
}

void VECTOR4D_SCALE(float k, LPVECTOR4D vector, LPVECTOR4D scaled)
{
	scaled->x = vector->x * k;
	scaled->y = vector->y * k;
	scaled->z = vector->z * k;
	scaled->w = 1;
}

float VECTOR4D_DOT(LPVECTOR4D va, LPVECTOR4D vb)
{
	return va->x * vb->y + va->y * vb->y + va->z * vb->z;
}

void VECTOR4D_CROSS(LPVECTOR4D va, LPVECTOR4D vb, LPVECTOR4D vn)
{
	vn->x = va->y * vb->z - va->z * vb->y;
	vn->y = va->z * va->x - va->x * vb->z;
	vn->z = va->x * vb->y - va->y * vb->x;
	vn->w = 1;
}

VECTOR4D VECTOR4D_CROSS(LPVECTOR4D va, LPVECTOR4D vb)
{
	VECTOR4D vn;
	vn.x = va->y * vb->z - va->z * vb->y;
	vn.y = va->z * va->x - va->x * vb->z;
	vn.z = va->x * vb->y - va->y * vb->x;
	vn.w = 1;
	return vn;
}

float VECTOR4D_LENGTH(LPVECTOR4D vector)
{
	return sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
}

float VECTOR4D_LENGTH_FAST(LPVECTOR4D vector)
{
	return Fast_Distance_3D(vector->x , vector->y, vector->y);
}

void VECTOR4D_NORMALIZE(LPVECTOR4D vector)
{
	float temp = 1 / sqrtf(vector->x * vector->x + vector->y * vector->y + vector->z * vector->z);
	vector->x *= temp;
	vector->y *= temp;
	vector->z *= temp;
	vector->w = 1;
}

void VECTOR4D_NORMALIZE(LPVECTOR4D va, LPVECTOR4D vn)
{
	float temp = 1 / sqrtf(vn->x * vn->x + vn->y * vn->y + vn->z * vn->z);
	vn->x = va->x * temp;
	vn->y = va->y * temp;
	vn->z = va->z * temp;
}

void VECTOR4D_BUILD(LPVECTOR4D init, LPVECTOR4D term, LPVECTOR4D result)
{
	result->x = term->x - init->x;
	result->y = term->y - init->y;
	result->z = term->z - init->z;
	result->w = 1;
}

float VECTOR4D_COSTH(LPVECTOR4D va, LPVECTOR4D vb)
{
	return VECTOR4D_DOT(va, vb) / (VECTOR4D_LENGTH(va) * VECTOR4D_LENGTH(vb));
}
/*
void VECTOR4D_PRINT(LPVECTOR4D va, char* name = "v")
{
	Write_Error("\n%s[", name);
	for (int index = 0; index < 4; index++)
		 Write_Error("%f, ", va->M[index]);
	Write_Error("]");
}*/



//////////////////////////////////////////////////////////////////////////////
                 
/*                     All 2D Matrix Functions                             */

//////////////////////////////////////////////////////////////////////////////
void MATRIX_INIT_2X2(LPMATRIX_2X2 m, float m00, float m01, float m10, float m11)
{
	m->M00 = m00; m->M01 = m01;
	m->M10 = m10; m->M11 = m11;
}
/*
void MATRIX_PRINT_2X2(LPMATRIX_2X2 m, char* name = "M")
{
	Write_Error("\n%s=\n", name);
	for (int r = 0; r < 2; r++, Write_Error("\n"))
	{
		for (int c = 0; c < 2; c++)
		{
			Write_Error("%f ", m->M[r][c]);
		}
	}
}*/

float MATRIX_DET_2X2(LPMATRIX_2X2 m)
{
	return m->M00 * m->M11 - m->M01 * m->M10;
}

void MATRIX_ADD_2X2(LPMATRIX_2X2 ma, LPMATRIX_2X2 mb, LPMATRIX_2X2 msum)
{
	msum->M00 = ma->M00 + mb->M00;
	msum->M01 = ma->M01 + mb->M01;
	msum->M10 = ma->M10 + mb->M10;
	msum->M11 = ma->M11 + mb->M11;
}

void MATRIX_MUL_2X2(LPMATRIX_2X2 ma, LPMATRIX_2X2 mb, LPMATRIX_2X2 mprod)
{
	mprod->M00 = ma->M00 * mb->M00 + ma->M01 * mb->M10;
	mprod->M01 = ma->M00 * mb->M01 + ma->M01 * mb->M11;
	mprod->M10 = ma->M10 * mb->M00 + ma->M11 * mb->M10;
	mprod->M11 = ma->M10 * mb->M01 + ma->M11 * mb->M11;
}

void MATRIX_INVERSE_2X2(LPMATRIX_2X2 m, LPMATRIX_2X2 mi)
{
	float det = m->M00 * m->M11 - m->M01 * m->M10;
	float det_inv = 1.0 / det;
	mi->M00 =  m->M11 * det_inv;
	mi->M01 = -m->M01 * det_inv;
	mi->M10 = -m->M10 * det_inv;
	mi->M11 =  m->M00 * det_inv;
}

void  SOLVE_SYSTEM_2X2(LPMATRIX_1X2 mx, LPMATRIX_2X2 ma, LPMATRIX_1X2 mb)
{
	float det_a = MATRIX_DET_2X2(ma);
	MATRIX_2X2 temp_matrix;

	MATRIX_COPY_2X2(&temp_matrix, ma);
	MATRIX_ROW_SWAP_2X2(&temp_matrix, 0, mb);
	mx->M00 = MATRIX_DET_2X2(&temp_matrix) / det_a;

	MATRIX_COPY_2X2(&temp_matrix, ma);
	MATRIX_ROW_SWAP_2X2(&temp_matrix, 1, mb);
	mx->M01 = MATRIX_DET_2X2(&temp_matrix) / det_a;
}


//////////////////////////////////////////////////////////////////////////////
               
/*                     All 3D Matrix Functions                             */

//////////////////////////////////////////////////////////////////////////////
void MATRIX_INIT_3X3(LPMATRIX_3X3 m,
					  float m00, float m01, float m02,
					  float m10, float m11, float m12,
					  float m20, float m21, float m22)
{
	m->M00 = m00; m->M01 = m01; m->M02 = m02;
	m->M10 = m10; m->M11 = m11; m->M12 = m12;
	m->M20 = m20; m->M21 = m21; m->M22 = m22;
}
/*
void MATRIX_PRINT_3X3(LPMATRIX_3X3 m, char* name)
{
	Write_Error("\n%s=\n", name);
	for (int row = 0; row < 3; row++, Write_Error("\n"))
	{
		for (int col = 0; col < 3; col)
		{
			Write_Error("%f ", m->M[row][col]);
		}
	}
}*/

float MATRIX_DET_3X3(LPMATRIX_3X3 m)
{
	return m->M00 * (m->M11 * m->M22 - m->M12 * m->M21) - 
		   m->M01 * (m->M10 * m->M22 - m->M12 * m->M20) + 
		   m->M02 * (m->M10 * m->M21 - m->M11 * m->M20);
} 

void MATRIX_ADD_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mb, LPMATRIX_3X3 msum)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			msum->M[row][col] = ma->M[row][col] + ma->M[row][col];
		}
	}
}

void MATRIX_MUL_1X2_3X2(LPMATRIX_1X2 ma, LPMATRIX_3X2 mb, LPMATRIX_1X2 mprod)
{
	mprod->M00 = ma->M00 * mb->M00 + ma->M01 * mb->M10 + mb->M20;
	mprod->M01 = ma->M00 * mb->M01 + ma->M01 * mb->M11 + mb->M21;
}

void MATRIX_MUL_1X3_3X3(LPMATRIX_1X3 ma, LPMATRIX_3X3 mb, LPMATRIX_1X3 mprod)
{
	for (int col = 0; col < 3; col++)
	{
		float temp_sum = 0.0;
		for (int row = 0; row < 3; row++)
		{
			temp_sum += ma->M[row] * mb->M[row][col];
		}
		mprod->M[col] = temp_sum;
	}
}

void MATRIX_MUL_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mb, LPMATRIX_3X3 mprod)
{
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			mprod->M[row][col] = ma->M[row][0] * mb->M[0][col] +  ma->M[row][1] * mb->M[1][col] +  ma->M[row][2] * mb->M[2][col];
		}
	}
}

void MATRIX_INVERSE_3X3(LPMATRIX_3X3 m, LPMATRIX_3X3 mi)
{
	float det = m->M00 * (m->M11 * m->M22 - m->M12 * m->M21) - 
		        m->M01 * (m->M10 * m->M22 - m->M12 * m->M20) + 
		        m->M02 * (m->M10 * m->M21 - m->M11 * m->M20);
	float det_inv = 1.0 / det;

	mi->M00 =  det_inv * (m->M11 * m->M22 - m->M12 * m->M21);
	mi->M10 = -det_inv * (m->M10 * m->M22 - m->M12 * m->M20);
	mi->M20 =  det_inv * (m->M10 * m->M21 - m->M11 * m->M20);

	mi->M01 = -det_inv * (m->M01 * m->M22 - m->M02 * m->M21);
	mi->M11 =  det_inv * (m->M00 * m->M22 - m->M02 * m->M20);
	mi->M21 = -det_inv * (m->M00 * m->M21 - m->M01 * m->M20);

	mi->M02 =  det_inv * (m->M01 * m->M12 - m->M02 * m->M11);
	mi->M12 = -det_inv * (m->M00 * m->M12 - m->M02 * m->M10);
	mi->M22 =  det_inv * (m->M00 * m->M11 - m->M01 * m->M10);
}

void SOLVE_SYSTEM_3X3(LPMATRIX_1X3 mx, LPMATRIX_3X3 ma, LPMATRIX_1X3 mb)
{
	float det = ma->M00 * (ma->M11 * ma->M22 - ma->M12 * ma->M21) - 
		        ma->M01 * (ma->M10 * ma->M22 - ma->M12 * ma->M20) + 
		        ma->M02 * (ma->M10 * ma->M21 - ma->M11 * ma->M20);
	MATRIX_3X3 temp_matrix;

	MATRIX_COPY_3X3(&temp_matrix, ma);
	MATRIX_ROW_SWAP_3X3(&temp_matrix, 0, mb);
	mx->M00 = MATRIX_DET_3X3(&temp_matrix) / det;

	MATRIX_COPY_3X3(&temp_matrix, ma);
	MATRIX_ROW_SWAP_3X3(&temp_matrix, 1, mb);
	mx->M00 = MATRIX_DET_3X3(&temp_matrix) / det;

	MATRIX_COPY_3X3(&temp_matrix, ma);
	MATRIX_ROW_SWAP_3X3(&temp_matrix, 2, mb);
	mx->M00 = MATRIX_DET_3X3(&temp_matrix) / det;
}



//////////////////////////////////////////////////////////////////////////////
               
/*                     All 4D Matrix Functions                             */

//////////////////////////////////////////////////////////////////////////////
void MATRIX_INIT_4X4(LPMATRIX_4X4 m,  float m00, float m01, float m02, float m03,
					                  float m10, float m11, float m12, float m13,
									  float m20, float m21, float m22, float m23,
									  float m30, float m31, float m32, float m33)
{
	m->M00 = m00; m->M01 = m01; m->M02 = m02; m->M03 = m03;
	m->M10 = m10; m->M11 = m11; m->M12 = m12; m->M13 = m13;
	m->M20 = m20; m->M21 = m21; m->M22 = m22; m->M23 = m23;
	m->M30 = m30; m->M31 = m31; m->M32 = m32; m->M33 = m33;
}
/*
void MATRIX_PRINT_4X4(LPMATRIX_4X4 m, char* name)
{
	Write_Error("\n%s=\n", name);
	for (int row = 0; row < 4; row++, Write_Error("\n"))
	{
		for (int col = 0; col < 4; col)
		{
			Write_Error("%f ", m->M[row][col]);
		}
	}
}*/
/*
float MATRIX_DET_4X4(LPMATRIX_4X4 m)
{

}*/
void MATRIX_ADD_4X4(LPMATRIX_4X4 va, LPMATRIX_4X4 vb, LPMATRIX_4X4 vsum)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			vsum->M[row][col] = va->M[row][col] + vb->M[row][col];
		}
	}
}

void MATRIX_MUL_4X4(LPMATRIX_4X4 ma, LPMATRIX_4X4 mb, LPMATRIX_4X4 mprod)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			mprod->M[row][col] = ma->M[row][0] * mb->M[0][col] +  ma->M[row][1] * mb->M[1][col] +  
				                 ma->M[row][2] * mb->M[2][col] + ma->M[row][3] * mb->M[3][col];
		}
	}
}

void MATRIX_MUL_1X4_4X4(LPMATRIX_1X4 ma, LPMATRIX_4X4 mb, LPMATRIX_1X4 mprod)
{
	for (int col = 0; col < 4; col++)
	{
		float temp_sum = 0.0;
		for (int row = 0; row < 4; row++)
		{
			temp_sum += ma->M[row] * mb->M[row][col];
		}
		mprod->M[col] = temp_sum;
	}
}

void MATRIX_MUL_1X4_4X4(LPVECTOR4D va, LPMATRIX_4X4 m, LPVECTOR4D vres)
{
	for (int col = 0; col < 4; col++)
	{
		float temp_sum = 0.0;
		for (int row = 0; row < 4; row++)
		{
			temp_sum += va->M[row] * m->M[row][col];
		}
		vres->M[col] = temp_sum;
	}
}

void MATRIX_MUL_1X3_4X4(LPMATRIX_1X3 ma, LPMATRIX_4X4 mb, LPMATRIX_1X3 mprod)
{
	for (int col = 0; col < 3; col++)
	{
		float temp_sum = 0.0;
		int row;
		for (row = 0; row < 3; row++)
		{
			temp_sum += ma->M[row] * mb->M[row][col];
		}
		temp_sum += mb->M[row][col];
		mprod->M[col] = temp_sum;
	}
}

void MATRIX_MUL_1X3_4X4(LPVECTOR3D va, LPMATRIX_4X4 m, LPVECTOR3D vres)
{
	for (int col = 0; col < 3; col++)
	{
		float temp_sum = 0.0;
		int row;
		for (row = 0; row < 3; row++)
		{
			temp_sum += va->M[row] * m->M[row][col];
		}
		temp_sum += m->M[row][col];
		vres->M[col] = temp_sum;
	}
}

void MATRIX_INVERSE_4X4(LPMATRIX_4X4 m, LPMATRIX_4X4 mi)
{

}
void SOLVE_SYSTEM_4X4(LPMATRIX_1X4 mx, LPMATRIX_4X4 ma, LPMATRIX_1X4 mb)
{

}





void  Init_line2D(LPPOINT2D pInit, 
				  LPPOINT2D pTerm, 
				  LPLINE2D  pLine)
{
	VECTOR2D_INIT(&(pLine->p0), pInit);
	VECTOR2D_INIT(&(pLine->p1), pTerm);
	VECTOR2D_BUILD(pInit, pTerm, &(pLine->v));
}
void  Compute_Line2D(LPLINE2D  pLine, 
					 float     t, 
					 LPPOINT2D pPoint)
{
	pPoint->x = pLine->p0.x + pLine->v.x * t;
	pPoint->y = pLine->p0.y + pLine->v.y * t;
}
int   Intersect_Lines2D(LPLINE2D pLine1,
						LPLINE2D pLine2, 
						float*   t1, 
						float*   t2)
{
	float det = pLine1->v.x * pLine2->v.y - pLine1->v.y * pLine2->v.x;

	if (det <= EPSILON_E5)
		return LINE_NO_INTERSECT;

	*t1 = ((pLine1->p0.y - pLine2->p0.y) * pLine2->v.x - (pLine1->p0.x - pLine2->p0.x) * pLine2->v.y) / det;
	*t2 = (pLine1->v.x * (pLine1->p0.y - pLine2->p0.y) - (pLine1->p0.x - pLine2->p0.x) * pLine1->v.y) / det;

	if ((*t1 >= 0) && (*t1 <= 1) && (*t2 >= 0) && (*t2 <= 1))
		return LINE_INTERSECT_IN_SEGMENT;
	else
		return LINE_INTERSECT_OUT_SEGMENT;
}

int   Intersect_Lines2D(LPLINE2D pLine1, 
						LPLINE2D pLine2,
						LPPOINT2D pPoint)
{
	float det = pLine1->v.x * pLine2->v.y - pLine1->v.y * pLine2->v.x;

	if (det <= EPSILON_E5)
		return LINE_NO_INTERSECT;

	float t1 = ((pLine1->p0.y - pLine2->p0.y) * pLine2->v.x - (pLine1->p0.x - pLine2->p0.x) * pLine2->v.y) / det;
	float t2 = (pLine1->v.x * (pLine1->p0.y - pLine2->p0.y) - (pLine1->p0.x - pLine2->p0.x) * pLine1->v.y) / det;

	pPoint->x = pLine1->p0.x + t1 * pLine1->v.x;
	pPoint->y = pLine1->p0.y + t1 * pLine1->v.y;

	if ((t1 >= 0) && (t1 <= 1) && (t2 >= 0) && (t2 <= 1))
		return LINE_INTERSECT_IN_SEGMENT;
	else
		return LINE_INTERSECT_OUT_SEGMENT;
}

void  Init_Line3D(LPPOINT3D pInit,
				  LPPOINT3D pTerm, 
				  LPLINE3D pLine)
{
	VECTOR3D_INIT(&(pLine->p0), pInit);
	VECTOR3D_INIT(&(pLine->p1), pTerm);
	VECTOR3D_BUILD(pInit, pTerm, &(pLine->v));
}
void  Compute_Line3D(LPLINE3D pLine, 
					 float t,
					 LPPOINT3D pPoint)
{
	pPoint->x = pLine->p0.x + pLine->v.x * t;
	pPoint->y = pLine->p0.y + pLine->v.y * t;
	pPoint->z = pLine->p0.z + pLine->v.z * t;
}


// if normalize is 1, then pNormal is not a normal vector, we should normalize it 
void  Plane3D_Init(LPPLANE3D pPlane,
				   LPPOINT3D pPoint, 
				   LPVECTOR3D pNormal, 
				   int normalize)
{
	POINT3D_COPY(&pPlane->p0, pPoint);

	if (normalize)
		VECTOR3D_NORMALIZE(pNormal, &pPlane->v);
	else
		VECTOR3D_COPY(&pPlane->v, pNormal);
}

float Compute_Point_In_Plane3D(LPPOINT3D pPoint,
							  LPPLANE3D pPlane)
{
	float test = 
		pPlane->v.x * (pPoint->x - pPlane->p0.x) + 
		pPlane->v.x * (pPoint->x - pPlane->p0.x) + 
		pPlane->v.x * (pPoint->x - pPlane->p0.x);

	return test;
}
int   Intersect_Line3D_Plane3D(LPLINE3D pLine,
							   LPPLANE3D pPlane, 
							   float* t, 
							   LPPOINT3D pPoint)
{
	float plane_dot_line = VECTOR3D_DOT(&pLine->v, &pPlane->v);

	if (fabs(plane_dot_line) <= EPSILON_E5)
	{
		if (Compute_Point_In_Plane3D(&pLine->p0, pPlane) <= EPSILON_E5)
			return LINE_INTERSECT_EVERYWHERE;
		else
			return LINE_NO_INTERSECT;
	}

	*t =  -(pLine->p0.x  * pPlane->v.x + 
	        pLine->p0.y  * pPlane->v.y + 
	        pLine->p0.z  * pPlane->v.z -
	        pPlane->p0.x * pPlane->v.x -
	        pPlane->p0.y * pPlane->v.y -
	        pPlane->p0.z * pPlane->v.z) / plane_dot_line;

	pPoint->x = pLine->p0.x + pLine->v.x * (*t);
	pPoint->y = pLine->p0.y + pLine->v.y * (*t);
	pPoint->z = pLine->p0.z + pLine->v.z * (*t);

	if ((*t >= 0) && (*t <= 1))
		return LINE_INTERSECT_IN_SEGMENT;
	else
		return LINE_INTERSECT_OUT_SEGMENT;

}


























































































