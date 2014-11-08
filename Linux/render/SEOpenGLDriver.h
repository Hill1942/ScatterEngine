#ifndef SEOPENGLDRIVER_H
#define SEOPENGLDRIVER_H


#define POLY4D_ATTR_2SIDED      0x0001
#define POLY4D_ATTR_TRANSPARENT 0x0002
#define POLY4D_ATTR_8BITCOLOR   0x0004
#define POLY4D_ATTRRGB16        0x0008
#define POLY4D_ATTRRGB24        0x0010


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

#define RGB32BIT_8888(a, r, g, b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

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


typedef struct RENDERLIST4D_TYP
{
	int state;
	int attr;

	LPPOLYF4D polyPointer[RENDERLIST4D_MAX_POLYS];
	POLYF4D   polyData[RENDERLIST4D_MAX_POLYS];

	int numPolys;
}RENDERLIST4D, *LPRENDERLIST4D;





inline void BREAKRGB32(int color, int& a, int& r, int& g, int& b)
{
	a = (color >> 24) & 0x000000ff;
	r = (color >> 16) & 0x000000ff;
	g = (color >> 8 ) & 0x000000ff;
	b = color & 0x000000ff;
}



void  Reset_RENDERLIST4D(LPRENDERLIST4D renderList);

void  Reset_OBJECT4D(LPOBJECT4D obj);

int   Insert_POLY4D_RENDERLIST4D(LPRENDERLIST4D renderList, LPPOLY4D poly);

int   Insert_POLYF4D_RENDERLIST4D(LPRENDERLIST4D renderList, LPPOLYF4D poly);

int   Insert_OBJECT4D_RENDERLIST4D(LPRENDERLIST4D renderList, LPOBJECT4D obj, 
									   int insertLocation = 0);

void  Draw_OBJECT4D_Wire(LPOBJECT4D obj, RenderContext* rcx);

void  Draw_RENDERLIST4D_Wire(LPRENDERLIST4D renderList, RenderContext* rcx);

void Draw_OBJECT4D_Solid(LPOBJECT4D obj, RenderContext* rcx);

void Draw_RENDERLIST4D_Solid(LPRENDERLIST4D renderList, RenderContext* rcx);


void Draw_Pixel(int x, int y, int color, RenderContext *rcx);

int Clip_Line(int &x1, int &y1, int &x2, int &y2, RenderContext* rcx);
int Draw_Line(int x0, int y0, int x1, int y1, int color, RenderContext* rcx);
int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color, RenderContext* rcx);

void Draw_Top_Triangle(int x1,    int y1,
					   int x2,    int y2, 
					   int x3,    int y3,
					   int color, RenderContext* rcx);
void Draw_Bottom_Triangle(int x1,    int y1,
					      int x2,    int y2, 
					      int x3,    int y3,
					      int color, RenderContext* rcx);
void Draw_2D_Triangle(int x1,    int y1,
					  int x2,    int y2, 
					  int x3,    int y3,
					  int color, RenderContext* rcx);


void InitShaders();
GLuint CompileShaders();




#endif