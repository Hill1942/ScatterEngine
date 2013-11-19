#include "MathEngine.h"

#include <cmath>

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