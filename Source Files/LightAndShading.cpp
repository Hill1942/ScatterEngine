#include <iostream>
#include <Windows.h>
#include <ddraw.h>


#include "BasicDrawEngine.h"
#include "MathEngine.h"
#include "3DPipeLine.h"
#include "LightAndShading.h"

MATERIALV1 materials[MAX_MATERIALV1];
int        numMaterials;

LIGHTV1    lights[MAX_LIGHTS];
int        numLights;


void Draw_OBJECT4DV1_Solid(LPOBJECT4DV1 obj, 
						   UCHAR* buffer, 
						   int lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4DV1_STATE_ACTIVE) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_BACKFACE))
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

void Draw_OBJECT4DV1_Solid16(LPOBJECT4DV1 obj, 
						   UCHAR* buffer, 
						   int lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4DV1_STATE_ACTIVE) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_BACKFACE))
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

void Draw_OBJECT4DV1_Solid32(LPOBJECT4DV1 obj, 
						   UCHAR* buffer, 
						   int lPitch)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4DV1_STATE_ACTIVE) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4DV1_STATE_BACKFACE))
			 continue;

		int vindex_0 = obj->polyList[poly].vert[0];
		int vindex_1 = obj->polyList[poly].vert[1];
		int vindex_2 = obj->polyList[poly].vert[2];
	
		Draw_2D_Triangle32(obj->vTransList[vindex_0].x, obj->vTransList[vindex_0].y,
			obj->vTransList[vindex_1].x, obj->vTransList[vindex_1].y,
			obj->vTransList[vindex_2].y, obj->vTransList[vindex_2].z,
			obj->polyList[poly].color, buffer, lPitch);
	}
}

void Draw_RENDERLIST4DV1_Solid(LPRENDERLIST4DV1 renderList, 
							   UCHAR* buffer,
							   int lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4DV1_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_BACKFACE))
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

void Draw_RENDERLIST4DV1_Solid16(LPRENDERLIST4DV1 renderList, 
							   UCHAR* buffer,
							   int lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4DV1_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_BACKFACE))
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

void Draw_RENDERLIST4DV1_Solid32(LPRENDERLIST4DV1 renderList, 
							   UCHAR* buffer,
							   int lPitch)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4DV1_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4DV1_STATE_BACKFACE))
			 continue;

		Draw_2D_Triangle32(renderList->polyPointer[poly]->vTranList[0].x,
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