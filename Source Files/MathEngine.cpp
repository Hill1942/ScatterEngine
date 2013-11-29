#include "MathEngine.h"
#include "IOHandling.h"

#include <cmath>

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

float Fast_Distance_3D(float x, float y, float z)
{

}


////////////   QUAD Functions  ///////////////
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

void VECTOR2D_PRINT(LPVECTOR2D va, char* name = "v")
{
	Write_Error("\n%s=[", name);
	for (int index = 0; index < 2; index++)
		Write_Error("%f, ", va->M[index]);
	Write_Error("]");
}

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

void  VECTOR3D_PRINT(LPVECTOR3D va, char* name = "v")
{
	Write_Error("\n%s=[", name);
	for (int index = 0; index < 3; index++)
		Write_Error("%f, ",va->M[index]);
	Write_Error("]");
}

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
	result->x = term->x - term->x;
	result->y = term->y - term->y;
	result->z = term->z - term->z;
	result->w = 1;
}

float VECTOR4D_COSTH(LPVECTOR4D va, LPVECTOR4D vb)
{
	return VECTOR4D_DOT(va, vb) / (VECTOR4D_LENGTH(va) * VECTOR4D_LENGTH(vb));
}

void VECTOR4D_PRINT(LPVECTOR4D va, char* name = "v")
{
	Write_Error("\n%s[", name);
	for (int index = 0; index < 4; index++)
		 Write_Error("%f, ", va->M[index]);
	Write_Error("]");
}







///////////////////////  Matrix Function   ////////////////////////////////
void MATRIX_INIT_2X2(LPMATRIX_2X2 m, float m00, float m01, float m10, float m11)
{
	m->M00 = m00; m->M01 = m01;
	m->M10 = m10; m->M11 = m11;
}

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
}

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


void SOLVE_SYSTEM_2X2(LPMATRIX_2X2 ma, LPMATRIX_2X2 mx, LPMATRIX_2X2 mb)
{


void MATRIX_INIT_3X3(LPMATRIX_3X3 m,
					  float m00, float m01, float m02,
					  float m10, float m11, float m12,
					  float m20, float m21, float m22);
void MATRIX_PRINT_3X3(LPMATRIX_3X3 m, char* name);
float MATRIX_DET_3X3(LPMATRIX_3X3 m);
void MATRIX_ADD_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mb, LPMATRIX_3X3 msum);
void MATRIX_MUL_1X2_3X2(LPMATRIX_1X2 ma, LPMATRIX_3X2 mb, LPMATRIX_1X2 mprod);
void MATRIX_MUL_1X3_3X3(LPMATRIX_1X3 ma, LPMATRIX_3X3 mb, LPMATRIX_1X3 mprod);
void MATRIX_MUL_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mb, LPMATRIX_3X3 mprod);
void MATRIX_INVERSE_3X3(LPMATRIX_3X3 m, LPMATRIX_3X3 mi);
void SOLVE_SYSTEM_3X3(LPMATRIX_3X3 ma, LPMATRIX_3X3 mx, LPMATRIX_3X3 mb);

void MATRIX_INIT_4X4(LPMATRIX_4X4 m, float m00, float m01, float m02, float m03,
					                  float m10, float m11, float m12, float m13,
									  float m20, float m21, float m22, float m23,
									  float m30, float m31, float m32, float m33);
void MATRIX_PRINT_4X4(LPMATRIX_4X4 m, char* name);
float MATRIX_DET_4X4(LPMATRIX_4X4 m);
void MATRIX_ADD_4X4(LPMATRIX_4X4 va, LPMATRIX_4X4 vb, LPMATRIX_4X4 vsum);
void MATRIX_MUL_4X4(LPMATRIX_4X4 va, LPMATRIX_4X4 vb, LPMATRIX_4X4 vprod);
void MATRIX_MUL_1X4_4X4(LPMATRIX_1X4 va, LPMATRIX_4X4, LPMATRIX_4X4 vprod);
void MATRIX_MUL_VECTOR3D_4X4(LPVECTOR3D va);
void MATRIX_INVERSE_4X4(LPMATRIX_4X4 m, LPMATRIX_4X4 mi);
void SOLVE_SYSTEM_4X4(LPMATRIX_4X4 a, LPMATRIX_4X4 x, LPMATRIX_4X4 b);

































































































