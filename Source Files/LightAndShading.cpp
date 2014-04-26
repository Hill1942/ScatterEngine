#include <iostream>
#include <Windows.h>
#include <ddraw.h>


#include "BasicDrawEngine.h"
#include "MathEngine.h"
#include "3DPipeLine.h"
#include "LightAndShading.h"

MATERIAL materials[MAX_MATERIAL];
int        numMaterials;

LIGHT    lights[MAX_LIGHTS];
int        numLights;


void Draw_OBJECT4D_Solid(LPOBJECT4D obj, 
						   UCHAR* buffer, 
						   int lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4D_STATE_ACTIVE) ||
			 (obj->polyList[poly].state & POLY4D_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4D_STATE_BACKFACE))
			 continue;

		int vindex_0 = obj->polyList[poly].vert[0];
		int vindex_1 = obj->polyList[poly].vert[1];
		int vindex_2 = obj->polyList[poly].vert[2];
	
		Draw_2D_Triangle(obj->vTransList[vindex_0].x, obj->vTransList[vindex_0].y,
			obj->vTransList[vindex_1].x, obj->vTransList[vindex_1].y,
			obj->vTransList[vindex_2].y, obj->vTransList[vindex_2].z,
			obj->polyList[poly].color, buffer, lPitch);
	}
}

void Draw_OBJECT4D_Solid16(LPOBJECT4D obj, 
						   UCHAR* buffer, 
						   int lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4D_STATE_ACTIVE) ||
			 (obj->polyList[poly].state & POLY4D_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4D_STATE_BACKFACE))
			 continue;

		int vindex_0 = obj->polyList[poly].vert[0];
		int vindex_1 = obj->polyList[poly].vert[1];
		int vindex_2 = obj->polyList[poly].vert[2];
	
		Draw_2D_Triangle16(obj->vTransList[vindex_0].x, obj->vTransList[vindex_0].y,
			obj->vTransList[vindex_1].x, obj->vTransList[vindex_1].y,
			obj->vTransList[vindex_2].y, obj->vTransList[vindex_2].z,
			obj->polyList[poly].color, buffer, lPitch);
	}
}

void Draw_OBJECT4D_Solid32(LPOBJECT4D obj, 
						   UCHAR* buffer, 
						   int lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4D_STATE_ACTIVE) ||
			 (obj->polyList[poly].state & POLY4D_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4D_STATE_BACKFACE))
			 continue;

		int vindex_0 = obj->polyList[poly].vert[0];
		int vindex_1 = obj->polyList[poly].vert[1];
		int vindex_2 = obj->polyList[poly].vert[2];
	
		Draw_2D_Triangle32(obj->vTransList[vindex_0].x, obj->vTransList[vindex_0].y,
			obj->vTransList[vindex_1].x, obj->vTransList[vindex_1].y,
			obj->vTransList[vindex_2].y, obj->vTransList[vindex_2].z,
			obj->polyList[poly].lightColor, buffer, lPitch);
	}
}

void Draw_RENDERLIST4D_Solid(LPRENDERLIST4D renderList, 
							   UCHAR* buffer,
							   int lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4D_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_BACKFACE))
			 continue;

		Draw_2D_Triangle(renderList->polyPointer[poly]->vTranList[0].x,
			renderList->polyPointer[poly]->vTranList[0].y, 
			renderList->polyPointer[poly]->vTranList[1].x, 
			renderList->polyPointer[poly]->vTranList[1].y, 
			renderList->polyPointer[poly]->vTranList[2].x, 
			renderList->polyPointer[poly]->vTranList[2].y,
			renderList->polyPointer[poly]->color,
			buffer, 
			lPitch);
	}
}

void Draw_RENDERLIST4D_Solid16(LPRENDERLIST4D renderList, 
							   UCHAR* buffer,
							   int lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4D_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_BACKFACE))
			 continue;

		Draw_2D_Triangle16(renderList->polyPointer[poly]->vTranList[0].x,
			renderList->polyPointer[poly]->vTranList[0].y, 
			renderList->polyPointer[poly]->vTranList[1].x, 
			renderList->polyPointer[poly]->vTranList[1].y, 
			renderList->polyPointer[poly]->vTranList[2].x, 
			renderList->polyPointer[poly]->vTranList[2].y,
			renderList->polyPointer[poly]->color,
			buffer, 
			lPitch);
	}
}

void Draw_RENDERLIST4D_Solid32(LPRENDERLIST4D renderList, 
							   UCHAR* buffer,
							   int lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4D_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_BACKFACE))
			 continue;

		Draw_2D_Triangle32(renderList->polyPointer[poly]->vTranList[0].x,
			renderList->polyPointer[poly]->vTranList[0].y, 
			renderList->polyPointer[poly]->vTranList[1].x, 
			renderList->polyPointer[poly]->vTranList[1].y, 
			renderList->polyPointer[poly]->vTranList[2].x, 
			renderList->polyPointer[poly]->vTranList[2].y,
			renderList->polyPointer[poly]->lightColor,
			buffer, 
			lPitch);
	}
}


int Init_Light(int        index,        //index of light
			     int        _state,
			     int        _attr,
			     ARGB     _ambient,
			     ARGB     _diffuse,
			     ARGB     _specular,
			     LPPOINT4D  _pos,
			     LPVECTOR4D _dir,
			     float      _kc,       //constant factor for point lights
			     float      _kl,       //linear factor for point lights
			     float      _kq,       //quadratic factor for point lights
			     float      _spotInnerTheta, 
			     float      _spotOuterTheta,
			     float      _pf)       //power factor spot lights
{
	if (index < 0 || index >= MAX_LIGHTS)
		return 0;

	lights[index].state          = _state;
	lights[index].id             = index;
	lights[index].attr           = _attr;
	lights[index].ambient        = _ambient;
	lights[index].diffuse        = _diffuse;
	lights[index].specular       = _specular;
	lights[index].kc             = _kc;
	lights[index].kl             = _kl;
	lights[index].kq             = _kq;
	lights[index].spotInnerTheta = _spotInnerTheta;
	lights[index].spotOuterTheta = _spotOuterTheta;

	if (_pos)
		VECTOR4D_COPY(&lights[index].pos, (LPVECTOR4D)_pos);
	if(_dir)
	{
		VECTOR4D_COPY(&lights[index].dir, (LPVECTOR4D)_dir);
		VECTOR4D_NORMALIZE(&lights[index].dir);
	}

	return index;
}

int Reset_Lights()
{
	static int resetLightLabel = 1;
	memset(lights, 0, MAX_LIGHTS * sizeof(LIGHT));
	numLights       =0;
	resetLightLabel = 0;

	return 1;
}
/*
int Reset_Materials()
{
	static int resetMaterialLabel = 1;

	if (resetMaterialLabel)
	{
		memset(materials, 0, MAX_MATERIAL * sizeof(MATERIAL));
		resetMaterialLabel = 1;
	}

	for (int i = 0; i < MAX_MATERIAL; i++)
	{
		
	}
}*/
/*
int Insert_OBJECT4D_RENDERLIST4DV2(LPRENDERLIST4D  renderList,
									 LPOBJECT4D      obj,
									 int               isLocal,
									 int               isLightOn)
{
	if (!(obj->state & OBJECT4D_STATE_ACTIVE) || 
		 (obj->state & OBJECT4D_STATE_CULLED) ||
		!(obj->state & OBJECT4D_STATE_VISIBLE))
		return 0;

	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		LPPOLY4D currentPoly = &obj->polyList[poly];

		if (!(currentPoly->state & POLY4D_STATE_ACTIVE)  ||
			 (currentPoly->state & POLY4D_STATE_CLIPPED) ||
			!(currentPoly->state & POLY4D_STATE_BACKFACE))
			continue;

		LPPOINT4D oldList = currentPoly->vList;

		if (isLocal)
			currentPoly->vList = obj->vLocalList;
		else
			currentPoly->vList = obj->vTransList;

		if (isLightOn == 1)
		{
			currentPoly->lightColor;
		}

		if (!Insert_POLY4D_RENDERLIST4D(renderList, currentPoly))
		{
			currentPoly->vList = oldList;
			return 0;
		}
	}
}
*/

int Light_OBJECT4D_32(LPOBJECT4D obj,
						LPCAM4D    cam,
						LPLIGHT    lights,
						int          maxLights)
{
	int a_base = 0;
	int r_base = 0;
	int g_base = 0;
	int b_base = 0;
	int a_sum  = 0;
	int r_sum  = 0;
	int g_sum  = 0;
	int b_sum  = 0;

	if (!(obj->state & OBJECT4D_STATE_ACTIVE) ||
		 (obj->state & OBJECT4D_STATE_CULLED) ||
		!(obj->state & OBJECT4D_STATE_VISIBLE))
		return 0;

	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		LPPOLY4D currentPoly = &obj->polyList[poly];
		if (!(currentPoly->state & POLY4D_STATE_ACTIVE) ||
			 (currentPoly->state & POLY4D_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4D_STATE_BACKFACE))
			continue;

		int vindex_0 = currentPoly->vert[0];
		int vindex_1 = currentPoly->vert[1];
		int vindex_2 = currentPoly->vert[2];

		if (currentPoly->attr & POLY4D_ATTR_SHADE_MODE_FLAT||
			currentPoly->attr & POLY4D_ATTR_SHADE_MODE_GOURAUD)
		{
			BREAKRGB32(currentPoly->color, a_base, r_base, g_base,b_base);
			for (int currentLight = 0; currentLight < maxLights; currentLight++)
			{
				if (lights[currentLight].state == LIGHT_STATE_OFF)
					continue;
				if (lights[currentLight].attr & LIGHT_ATTR_AMBIENT)
				{
					r_sum += ((lights[currentLight].ambient.r * r_base) / 256);
					g_sum += ((lights[currentLight].ambient.g * g_base) / 256);
					b_sum += ((lights[currentLight].ambient.b * b_base) / 256);
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_INFINITE)
				{
					VECTOR4D u, v, n;

					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &obj->vTransList[vindex_1], &u);
					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &obj->vTransList[vindex_2], &v);
					VECTOR4D_CROSS(&u, &v, &n);


					float length = VECTOR4D_LENGTH_FAST(&n);
					float dot    = VECTOR4D_DOT(&n, &lights[currentLight].dir);

					if (dot > 0)
					{
						float i = dot / length;
						r_sum += (lights[currentLight].diffuse.r * r_base * i) / 256;
						g_sum += (lights[currentLight].diffuse.g * g_base * i) / 256;
						b_sum += (lights[currentLight].diffuse.b * b_base * i) / 256;
					}
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_POINT)
				{
					VECTOR4D u, v, n, l;

					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &obj->vTransList[vindex_1], &u);
					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &obj->vTransList[vindex_2], &v);
					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &lights[currentLight].pos, &l);
					VECTOR4D_CROSS(&u, &v, &n);

					float n_length = VECTOR4D_LENGTH_FAST(&n);
					float l_length = VECTOR4D_LENGTH_FAST(&l);

					float cosTheta = VECTOR4D_DOT(&n, &l) / (n_length * l_length);
					if (cosTheta > 0)
					{
						float denominator = (lights[currentLight].kc + 
							lights[currentLight].kl * l_length +
							lights[currentLight].kq * l_length * l_length);
						float i = cosTheta / denominator;
						r_sum += (lights[currentLight].diffuse.r * r_base * i) / 256;
						g_sum += (lights[currentLight].diffuse.g * g_base * i) / 256;
						b_sum += (lights[currentLight].diffuse.b * b_base * i) / 256;
					}
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_SPOTLIGHT1)
				{
					VECTOR4D u, v, n, l;

					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &obj->vTransList[vindex_1], &u);
					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &obj->vTransList[vindex_2], &v);
					VECTOR4D_BUILD(&obj->vTransList[vindex_0], &lights[currentLight].pos, &l);
					VECTOR4D_CROSS(&u, &v, &n);

					float n_length = VECTOR4D_LENGTH_FAST(&n);
					float l_length = VECTOR4D_LENGTH_FAST(&l);

					float cosTheta = VECTOR4D_DOT(&n, &l) / (n_length * l_length);
					if (cosTheta > 0)
					{
						float denominator = (lights[currentLight].kc + 
							lights[currentLight].kl * l_length +
							lights[currentLight].kq * l_length * l_length);
						float i = cosTheta / denominator;
						r_sum += (lights[currentLight].diffuse.r * r_base * i) / 256;
						g_sum += (lights[currentLight].diffuse.g * g_base * i) / 256;
						b_sum += (lights[currentLight].diffuse.b * b_base * i) / 256;
					}
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_SPOTLIGHT2)
				{

				}
			}  
			if (r_sum > 255) r_sum = 255;
			if (g_sum > 255) r_sum = 255;
			if (b_sum > 255) r_sum = 255;
			currentPoly->lightColor = RGB32BIT_8888(255, r_sum, g_sum, b_sum);
		}
		else  // A
		{
			currentPoly->lightColor = currentPoly->color;
		}
	}
	return 1;
}


int Light_RENDERLIST4D_32(LPRENDERLIST4D renderList,
						    LPCAM4D        cam,
						    LPLIGHT        lights,
						    int              maxLights)
{
	int a_base = 0;
	int r_base = 0;
	int g_base = 0;
	int b_base = 0;
	


	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		int a_sum  = 0;
		int r_sum  = 0;
		int g_sum  = 0;
		int b_sum  = 0;
		LPPOLYF4D currentPoly = renderList->polyPointer[poly];
		if (!(currentPoly->state & POLY4D_STATE_ACTIVE) ||
			 (currentPoly->state & POLY4D_STATE_CLIPPED) ||
			 (currentPoly->state & POLY4D_STATE_BACKFACE))
			continue;

		if (currentPoly->attr & POLY4D_ATTR_SHADE_MODE_FLAT||
			currentPoly->attr & POLY4D_ATTR_SHADE_MODE_GOURAUD)
		{
			BREAKRGB32(currentPoly->color, a_base, r_base, g_base,b_base);
			for (int currentLight = 0; currentLight < maxLights; currentLight++)
			{
				if (lights[currentLight].state == LIGHT_STATE_OFF)
					continue;
				if (lights[currentLight].attr & LIGHT_ATTR_AMBIENT)
				{
					r_sum += ((lights[currentLight].ambient.r * r_base) / 256);
					g_sum += ((lights[currentLight].ambient.g * g_base) / 256);
					b_sum += ((lights[currentLight].ambient.b * b_base) / 256);
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_INFINITE)
				{
					VECTOR4D u, v, n;

					VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[1], &u);
					VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[2], &v);
					VECTOR4D_CROSS(&u, &v, &n);


					float length = VECTOR4D_LENGTH_FAST(&n);
					float dot    = VECTOR4D_DOT(&n, &lights[currentLight].dir);

					if (dot > 0)
					{
						float i = dot / length;
						r_sum += (lights[currentLight].diffuse.r * r_base * i) / 256;
						g_sum += (lights[currentLight].diffuse.g * g_base * i) / 256;
						b_sum += (lights[currentLight].diffuse.b * b_base * i) / 256;
					}
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_POINT)
				{
					VECTOR4D u, v, n, l;

					VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[1], &u);
					VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[2], &v);
					VECTOR4D_BUILD(&currentPoly->vTranList[0], &lights[currentLight].pos, &l);
					VECTOR4D_CROSS(&u, &v, &n);

					float n_length = VECTOR4D_LENGTH_FAST(&n);
					float l_length = VECTOR4D_LENGTH_FAST(&l);

					float cosTheta = VECTOR4D_DOT(&n, &l) / (n_length * l_length);
					if (cosTheta > 0)
					{
						float denominator = (lights[currentLight].kc + 
							lights[currentLight].kl * l_length +
							lights[currentLight].kq * l_length * l_length);
						float i = cosTheta / denominator;
						r_sum += (lights[currentLight].diffuse.r * r_base * i) / 256;
						g_sum += (lights[currentLight].diffuse.g * g_base * i) / 256;
						b_sum += (lights[currentLight].diffuse.b * b_base * i) / 256;
					}
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_SPOTLIGHT1)
				{
					VECTOR4D u, v, n, l;

					VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[1], &u);
					VECTOR4D_BUILD(&currentPoly->vTranList[0], &currentPoly->vTranList[2], &v);
					VECTOR4D_BUILD(&currentPoly->vTranList[0], &lights[currentLight].pos, &l);
					VECTOR4D_CROSS(&u, &v, &n);

					float n_length = VECTOR4D_LENGTH_FAST(&n);
					float l_length = VECTOR4D_LENGTH_FAST(&l);

					float cosTheta = VECTOR4D_DOT(&n, &l) / (n_length * l_length);
					if (cosTheta > 0)
					{
						float denominator = (lights[currentLight].kc + 
							lights[currentLight].kl * l_length +
							lights[currentLight].kq * l_length * l_length);
						float i = cosTheta / denominator;
						r_sum += (lights[currentLight].diffuse.r * r_base * i) / 256;
						g_sum += (lights[currentLight].diffuse.g * g_base * i) / 256;
						b_sum += (lights[currentLight].diffuse.b * b_base * i) / 256;
					}
				}
				else if (lights[currentLight].attr & LIGHT_ATTR_SPOTLIGHT2)
				{

				}
			}  
			if (r_sum > 255) r_sum = 255;
			if (g_sum > 255) g_sum = 255;
			if (b_sum > 255) b_sum = 255;
			currentPoly->lightColor = RGB32BIT_8888(255, r_sum, g_sum, b_sum);
		}
		else  // A
		{
			//currentPoly->lightColor = currentPoly->color;
		}
	}
	return 1;
}

int Compare_AvgZ_POLYF4D(const void* arg1, const void* arg2)
{
	LPPOLYF4D poly1;
	LPPOLYF4D poly2;

	float z1;
	float z2;

	poly1 = *((LPPOLYF4D*)(arg1));
	poly2 = *((LPPOLYF4D*)(arg2));

	z1 = poly1->vTranList[0].z + poly1->vTranList[1].z + poly1->vTranList[2].z;
	z2 = poly2->vTranList[0].z + poly2->vTranList[1].z + poly2->vTranList[2].z;


	if (z1 > z2)
		return -1;
	else if (z1 < z2)
		return 1;
	else
		return 0;
}

int Compare_NearZ_POLYF4D(const void* arg1, const void* arg2)
{
	LPPOLYF4D poly1;
	LPPOLYF4D poly2;

	float z1;
	float z2;

	poly1 = *((LPPOLYF4D*)(arg1));
	poly2 = *((LPPOLYF4D*)(arg2));

	z1 = MIN(poly1->vTranList[0].z, poly1->vTranList[1].z);
	z1 = MIN(z1, poly1->vTranList[2].z);

	z2 = MIN(poly2->vTranList[0].z, poly2->vTranList[1].z);
	z2 = MIN(z2, poly2->vTranList[2].z);


	if (z1 > z2)
		return -1;
	else if (z1 < z2)
		return 1;
	else
		return 0;
}

int Compare_FarZ_POLYF4D(const void* arg1, const void* arg2)
{
	LPPOLYF4D poly1;
	LPPOLYF4D poly2;

	float z1;
	float z2;

	poly1 = *((LPPOLYF4D*)(arg1));
	poly2 = *((LPPOLYF4D*)(arg2));

	z1 = MAX(poly1->vTranList[0].z, poly1->vTranList[1].z);
	z1 = MAX(z1, poly1->vTranList[2].z);

	z2 = MAX(poly2->vTranList[0].z, poly2->vTranList[1].z);
	z2 = MAX(z2, poly2->vTranList[2].z);


	if (z1 > z2)
		return -1;
	else if (z1 < z2)
		return 1;
	else
		return 0;
}

void Sort_RENDERLIST4D(LPRENDERLIST4D rendList, int method)
{
	switch (method)
	{
	case SORT_POLYLIST_AVGZ:
		{
			qsort((void*)rendList->polyPointer, 
				rendList->numPolys, 
				sizeof(LPPOLYF4D), 
				Compare_AvgZ_POLYF4D);
		}
		break;
	case SORT_POLYLIST_NEARZ:
		{
			qsort((void*)rendList->polyPointer, 
				rendList->numPolys, 
				sizeof(LPPOLYF4D), 
				Compare_NearZ_POLYF4D);
		}
		break;
	case SORT_POLYLIST_FARZ:
		{
			qsort((void*)rendList->polyPointer, 
				rendList->numPolys, 
				sizeof(LPPOLYF4D), 
				Compare_FarZ_POLYF4D);
		}
		break;
	default:
		break;
	}
}