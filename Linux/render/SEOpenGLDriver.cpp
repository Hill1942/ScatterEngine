#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <iostream>

#include "../SEConfig.h"
#include "../platform/SEWindow.h"
#include "../math/SEMath.h"
#include "SEOpenGLDriver.h"

GLuint program;

void InitShaders()
{
	program = CompileShaders();
}



GLuint CompileShaders()
{
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint program;

	static const GLchar* vertexShaderSource[] = 
	{
		"#version 140                               \n"
		"                                           \n"
		"in vec4 mypos;                             \n"
		"in vec4 mycolor;                           \n"
		"out vec4 vs_color;	                         \n"
		"void main()                                \n"
		"{                                          \n"
		"	gl_Position = mypos;                    \n"
		"   vs_color    = mycolor;                  \n"
		"}                                          \n"
	};
	static const GLchar* fragmentShaderSource[] = 
	{
		"#version 140                              \n"
		"                                          \n"
		"in vec4 vs_color;                         \n"
		"out vec4 color;                           \n"
		"                                          \n"
		"void main()                               \n"
		"{                                         \n"
		"	color = vs_color;                      \n"
		"}                                         \n"
	};

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, vertexShaderSource, NULL);
	glCompileShader(vertexShader);

#ifdef SE_DEBUG_ON
	char log[2048];
	int length = 0;
	glGetShaderInfoLog(vertexShader, 2048, &length, log);
	std::cout << log << std::endl;
#endif

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glBindAttribLocation(program, 0, "mypos");
	glBindAttribLocation(program, 1, "mycolor");
	glLinkProgram(program);
	
#ifdef SE_DEBUG_ON
	int a = glGetAttribLocation(program, "mypos");
	std::cout << a << std::endl;
#endif	

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return program;
}

void Draw_Pixel(int x, int y, int color, RenderContext *rcx)
{
	
	//GLuint vaos[1] = {0};
	//GLuint bufs[1] = {0};																															
	//glGenVertexArrays(2, vaos);
	//glBindVertexArray(vaos[0]);
/*
	GLfloat vertices[2][4] = 
	{
		{x,    y,    0.0f, 1.0f},
		{0.0f, 1.0f, 0.8f, 1.0f}
	}
	

	glGenBuffers(1, bufs);
	glBindBuffer(GL_ARRAY_BUFFER, bufs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);*/

	float alpha = 0.5 * rcx->nWindowWidth  - 0.5;
	float beta  = 0.5 * rcx->nWindowHeight - 0.5;
	
	GLfloat xpos = x / alpha - 1;
	GLfloat ypos = 1 - y / beta;
	GLfloat postion[] = {xpos, ypos, 0.0f, 1.0f};
	//std::cout << "(" << xpos << ", " << ypos << ")" << std::endl;

	//program = CompileShaders();

	int a, r, g, b;
	BREAKRGB32(color, a, r, g, b);

	GLfloat colors[]   = {r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f};

	glVertexAttrib4fv(0, postion);
	glVertexAttrib4fv(1, colors);
	
	//glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(1);
	glUseProgram(program);
	//glPointSize(10.0f);
	//glBindVertexArray(vaos[0]);
	glDrawArrays(GL_POINTS, 0, 1);
}


int Clip_Line(int &x1, int &y1, int &x2, int &y2, RenderContext* rcx)
{
#define CLIP_CODE_C  0x0000
#define CLIP_CODE_N  0x0008
#define CLIP_CODE_S  0x0004
#define CLIP_CODE_E  0x0002
#define CLIP_CODE_W  0x0001
					 
#define CLIP_CODE_NE 0x000a
#define CLIP_CODE_SE 0x0006
#define CLIP_CODE_NW 0x0009
#define CLIP_CODE_SW 0x0005

	int xc1 = x1;
	int yc1 = y1;
	int xc2 = x2;
	int yc2 = y2;

	int directCode1 = 0;
	int directCode2 = 0;

	int minClipX = 0;
	int maxClipX = rcx->nWindowWidth - 1;
	int minClipY = 0;
	int maxClipY = rcx->nWindowHeight - 1;

	if (y1 < minClipY)
		directCode1 |= CLIP_CODE_N;
	else if (y1 > maxClipY)
		directCode1 |= CLIP_CODE_S;

	if (x1 < minClipX)
		directCode1 |= CLIP_CODE_W;
	else if (x1 > maxClipX)
		directCode1 |= CLIP_CODE_E;

	if (y2 < minClipY)
		directCode2 |= CLIP_CODE_N;
	else if (y2 > maxClipY)
		directCode2 |= CLIP_CODE_S;

	if (x2 < minClipX)
		directCode2 |= CLIP_CODE_W;
	else if (x2 > maxClipX)
		directCode2 |= CLIP_CODE_E;

	//totally out
	if ((directCode1 & directCode2))
		return 0;

	//totoally in
	if (directCode1 == 0 && directCode2 == 0)
		return 1;

	switch (directCode1)
	{
	case CLIP_CODE_C:
		break;

	case CLIP_CODE_N:
		xc1 = 0.5 + x1 + (minClipY - y1) * (x2 - x1) / (y2 - y1);
		yc1 = minClipY;
		break;

	case CLIP_CODE_S:
		xc1 = 0.5 + x1 + (maxClipY - y1) * (x2 - x1) / (y2 - y1);
		yc1 = maxClipY;
		break;

	case CLIP_CODE_W:
		xc1 = minClipX;
		yc1 = 0.5 + y1 + (minClipX - x1) * (y2 - y1) / (x2 - x1);
		break;

	case CLIP_CODE_E:
		xc1 = maxClipX;
		yc1 = 0.5 + y1 + (maxClipX - x1) * (y2 - y1) / (x2 - x1);
		break;

	case CLIP_CODE_NE:
		xc1 = 0.5 + x1 + (minClipY - y1) * (x2 - x1) / (y2 - y1);
		yc1 = minClipY;
		if (xc1 < minClipX || xc1 > maxClipX)
		{
			xc1 = maxClipX;
			yc1 = 0.5 + y1 + (maxClipX - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case CLIP_CODE_SE:
		xc1 = 0.5 + x1 + (maxClipY - y1) * (x2 - x1) / (y2 - y1);
		yc1 = maxClipY;
		if (xc1 < minClipX || xc1 > maxClipX)
		{
			xc1 = maxClipX;
			yc1 = 0.5 + y1 + (maxClipX - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case CLIP_CODE_NW:
		xc1 = 0.5 + x1 + (minClipY - y1) * (x2 - x1) / (y2 - y1);
		yc1 = minClipY;
		if (xc1 < minClipX || xc1 > maxClipX)
		{
			xc1 = minClipX;
			yc1 = 0.5 + y1 + (minClipX - x1) * (y2 - y1) / (x2 - x1);
		}

	case CLIP_CODE_SW:
		xc1 = 0.5 + x1 + (maxClipY - y1) * (x2 - x1) / (y2 - y1);
		yc1 = maxClipY;
		if (xc1 < minClipX || xc1 > maxClipX)
		{
			xc1 = minClipX;
			yc1 = 0.5 + y1 + (minClipX - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	default:
		break;
	}

	switch (directCode2)
	{
	case CLIP_CODE_C:
		break;

	case CLIP_CODE_N:
		xc2 = 0.5 + x2 + (minClipY - y1) * (x2 - x1) / (y2 - y1);
		yc2 = minClipY;
		break;

	case CLIP_CODE_S:
		xc2 = 0.5 + x2 + (maxClipY - y1) * (x2 - x1) / (y2 - y1);
		yc2 = maxClipY;
		break;

	case CLIP_CODE_W:
		xc2 = minClipX;
		yc2 = 0.5 + y2 + (minClipX - x1) * (y2 - y1) / (x2 - x1);
		break;

	case CLIP_CODE_E:
		xc2 = maxClipX;
		yc2 = 0.5 + y2 + (maxClipX - x1) * (y2 - y1) / (x2 - x1);
		break;

	case CLIP_CODE_NE:
		xc2 = 0.5 + x2 + (minClipY - y1) * (x2 - x1) / (y2 - y1);
		yc2 = minClipY;
		if (xc2 < minClipX || xc2 > maxClipX)
		{
			xc2 = maxClipX;
			yc2 = 0.5 + y2 + (maxClipX - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case CLIP_CODE_SE:
		xc2 = 0.5 + x2 + (maxClipY - y1) * (x2 - x1) / (y2 - y1);
		yc2 = maxClipY;
		if (xc2 < minClipX || xc2 > maxClipX)
		{
			xc2 = maxClipX;
			yc2 = 0.5 + y2 + (maxClipX - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	case CLIP_CODE_NW:
		xc2 = 0.5 + x2 + (minClipY - y1) * (x2 - x1) / (y2 - y1);
		yc2 = minClipY;
		if (xc2 < minClipX || xc2 > maxClipX)
		{
			xc2 = minClipX;
			yc2 = 0.5 + y2 + (minClipX - x1) * (y2 - y1) / (x2 - x1);
		}

	case CLIP_CODE_SW:
		xc2 = 0.5 + x2 + (maxClipY - y1) * (x2 - x1) / (y2 - y1);
		yc2 = maxClipY;
		if (xc2 < minClipX || xc2 > maxClipX)
		{
			xc2 = minClipX;
			yc2 = 0.5 + y2 + (minClipX - x1) * (y2 - y1) / (x2 - x1);
		}
		break;

	default:
		break;
	}

	if ((xc1 < minClipX) || (xc1 > maxClipX) ||
		(yc1 < minClipY) || (yc1 > maxClipY) ||
		(xc2 < minClipX) || (xc2 > maxClipX) ||
		(yc2 < minClipY) || (yc2 > maxClipY) )
	    return 0;

	x1 = xc1;
	y1 = yc1;
	x2 = xc2;
	y2 = yc2;

	return 1;
}



int Draw_Line(int    x0, 
			    int    y0, 
			    int    x1, 
			    int    y1,
			    int    color,
			    RenderContext* rcx)
{
	int dx;
	int dy;
	int xinc;
	int yinc;

	int x = x0;
	int y = y0;
	
	dx = x1 - x0;
	dy = y1 - y0;

	if (dx > 0)
	{
		xinc = 1;
	}
	else 
	{
		dx   = -dx;
		xinc = -1;
	}

	if (dy > 0)
	{
		yinc = 1;		
	}
	else
	{
		dy   = -dy;
		yinc = -1;
	}

	int dx2 = dx << 1;
	int dy2 = dy << 1;

	if (dx > dy)
	{
		int error = 0;
		for (int i = 0; i <= dx; i++)
		{
			Draw_Pixel(x, y, color, rcx);
			error += dy2;
			if (error >= dx)
			{
				y += yinc;
				error -= dx2;
			}
			x += xinc;
		}
	}
	else
	{
		int error = 0;
		for (int i = 0; i <= dy; i++)
		{
			Draw_Pixel(x, y, color, rcx);
			error += dx2;
			if (error >= dy)
			{
				x += xinc;
				error -= dy2;
			}
			y += yinc;
		}
	}

	return 1;
}

int Draw_Clip_Line(int    x0, 
					 int    y0, 
					 int    x1, 
					 int    y1,
					 int    color,
					 RenderContext* rcx)
{
	int xc0 = x0;
	int yc0 = y0;
	int xc1 = x1;
	int yc1 = y1;

	if (Clip_Line(xc0, yc0, xc1, yc1, rcx))
		Draw_Line(xc0, yc0, xc1, yc1, color, rcx);

	return 1;
}



/************************************************
for triangle like this:
     ______
     \    /
      \  /
       \/
**************************************************/
void Draw_Top_Triangle(int x1,    int y1,
					   int x2,    int y2, 
					   int x3,    int y3,
					   int color, RenderContext* rcx)
{
	float k_right;
	float k_left;
	float xStart;
	float xEnd;

	int   clip_right;
	int   clip_left;

	int minClipX = 0;
	int maxClipX = rcx->nWindowWidth - 1;
	int minClipY = 0;
	int maxClipY = rcx->nWindowHeight - 1;

	int temp;
	if (x2 < x1)
	{
		temp = x2;
		x2 = x1;
		x1 = temp;
	}

	k_left   = (float)(x3 - x1) / (float)(y3 - y1);
	k_right  = (float)(x3 - x2) / (float)(y3 - y1);

	xStart = (float)x1;
	xEnd   = (float)x2 + 0.5f;

	if (y1 < minClipY)
	{
		xStart = xStart + k_left  * (float)(minClipY - y1);
		xEnd   = xEnd   + k_right * (float)(minClipY - y1);
		y1 = minClipY;
	}

	if (y3 > maxClipY)
		y3 = maxClipY;

	if (x1 >= minClipX && x1 <= maxClipX &&
		x2 >= minClipX && x2 <= maxClipX &&
		x3 >= minClipX && x3 <= maxClipX)
	{
		for (int i = y1; i <= y3; i++)
		{
			for (int s = xStart; s<= xEnd; s++) 
			{
				Draw_Pixel(s, i, color, rcx);
			}
			xStart += k_left;
			xEnd   += k_right;
		}
	}
	else
	{
		for (int i = y1; i <= y3; i++)
		{
			clip_left  = (int)xStart;
			clip_right = (int)xEnd; 

			xStart += k_left;
			xEnd   += k_right;

			if (clip_left < minClipX)
			{
				clip_left = minClipX;
				if (clip_right < minClipX)
					continue;
			}
			if (clip_right > maxClipX)
			{
				clip_right = maxClipX;
				if (clip_left > maxClipX)
					continue;
			}
			for (int s = xStart; s<= xEnd; s++) 
			{
				Draw_Pixel(s, i, color, rcx);
			}
		}	
	}
}

/************************************************
for triangle like this:
      /\
     /  \
    /    \
   /______\
**************************************************/
void Draw_Bottom_Triangle(int x1,    int y1,
					      int x2,    int y2, 
					      int x3,    int y3,
					      int color, RenderContext* rcx)
{
	float k_right;
	float k_left;
	float xStart;
	float xEnd;

	int   clip_right;
	int   clip_left;

	int minClipX = 0;
	int maxClipX = rcx->nWindowWidth - 1;
	int minClipY = 0;
	int maxClipY = rcx->nWindowHeight - 1;

	int temp;
	if (x3 < x2)
	{
		temp = x3;
		x3 = x2;
		x2 = temp;
	}

	k_left  = (float)(x2 - x1) / (float)(y3 - y1);
	k_right = (float)(x3 - x1) / (float)(y3 - y1);

	xStart = (float)x1;
	xEnd   = (float)x1;

	if (y1 < minClipY)
	{
		xStart = xStart + k_left  * (float)(minClipY - y1);
		xEnd   = xEnd   + k_right * (float)(minClipY - y1);
		y1 = minClipY;
	}

	if (y3 > maxClipY)
		y3 = maxClipY;


	if (x1 >= minClipX && x1 <= maxClipX &&
		x2 >= minClipX && x2 <= maxClipX &&
		x3 >= minClipX && x3 <= maxClipX)
	{
		for (int i = y1; i <= y3; i++)
		{
			for (int s = xStart; s<= xEnd; s++) 
			{
				Draw_Pixel(s, i, color, rcx);
			}
			xStart += k_left;
			xEnd   += k_right;
		}
	}
	else
	{
		for (int i = y1; i <= y3; i++)
		{
			clip_left  = (int)xStart;
			clip_right = (int)xEnd;

			xStart += k_left;
			xEnd   += k_right;

			if (clip_left < minClipX)
			{
				clip_left = minClipX;
				if (clip_right < minClipX)
					continue;
			}
			if (clip_right > maxClipX)
			{
				clip_right = maxClipX;
				if (clip_left > maxClipX)
					continue;
			}
			for (int s = xStart; s<= xEnd; s++) 
			{
				Draw_Pixel(s, i, color, rcx);
			}
		}		
	}
}

void Draw_2D_Triangle(int x1,    int y1,
					  int x2,    int y2, 
					  int x3,    int y3,
					  int color, RenderContext* rcx)
{
	int minClipX = 0;
	int maxClipX = rcx->nWindowWidth - 1;
	int minClipY = 0;
	int maxClipY = rcx->nWindowHeight - 1;
	
	if ((x1 == x2 && x2 == x3) || (y1 == y2 && y2 == y3))
		return;

	int tempX;
	int tempY;
	if (y2 < y1)
	{
		tempX = x2;
		tempY = y2;
		x2    = x1;
		y2    = y1;
		x1    = tempX;
		y1    = tempY;
	}

	if (y3 < y1)
	{
		tempX = x3;
		tempY = y3;
		x3    = x1;
		y3    = y1;
		x1    = tempX;
		y1    = tempY;
	}

	if (y3 < y2)
	{
		tempX = x2;
		tempY = y2;
		x2    = x3;
		y2    = y3;
		x3    = tempX;
		y3    = tempY;
	}

	if ( y3 < minClipY || y1 > maxClipY ||
		(x1 < minClipX && x2 < minClipX && x3 < minClipX) ||
		(x1 > maxClipX && x2 > maxClipX && x3 > maxClipX))
		return;

	if (y1 == y2)
	{
		Draw_Top_Triangle(x1, y1, x2, y2, x3, y3, color, rcx);
	}
	else if (y2 == y3)
	{
		Draw_Bottom_Triangle(x1, y1, x2, y2, x3, y3, color, rcx);
	}
	else
	{
		int newX = x1 + (int)(0.5 + (float)(y2 - y1) * (float)(x3 - x1) / (float)(y3 - y1));
		Draw_Bottom_Triangle(x1, y1, newX, y2, x2, y2, color, rcx);
		Draw_Top_Triangle(x2, y2, newX, y2, x3, y3, color, rcx);
	}
}


void Draw_OBJECT4D_Wire(LPOBJECT4D obj, RenderContext* rcx)
{
	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		if (!(obj->polyList[poly].state & POLY4D_STATE_ACTIVE)  ||
			 (obj->polyList[poly].state & POLY4D_STATE_CLIPPED) ||
			 (obj->polyList[poly].state & POLY4D_STATE_BACKFACE))
		    continue;

		int v0 = obj->polyList[poly].vert[0];
		int v1 = obj->polyList[poly].vert[1];
		int v2 = obj->polyList[poly].vert[2];

		Draw_Clip_Line(obj->vTransList[v0].x,
			           obj->vTransList[v0].y,
					   obj->vTransList[v1].x,
					   obj->vTransList[v1].y,
					   obj->polyList[poly].color,
					   rcx);

		Draw_Clip_Line(obj->vTransList[v1].x,
			           obj->vTransList[v1].y,
					   obj->vTransList[v2].x,
					   obj->vTransList[v2].y,
					   obj->polyList[poly].color,
					   rcx);

		Draw_Clip_Line(obj->vTransList[v2].x,
			           obj->vTransList[v2].y,
					   obj->vTransList[v0].x,
					   obj->vTransList[v0].y,
					   obj->polyList[poly].color,
					   rcx);
	}
}


void Draw_RENDERLIST4D_Wire(LPRENDERLIST4D renderList, RenderContext* rcx)
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4D_STATE_ACTIVE)  ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_BACKFACE))
		    continue;
	
		Draw_Clip_Line(renderList->polyPointer[poly]->vTranList[0].x,
			             renderList->polyPointer[poly]->vTranList[0].y,
					     renderList->polyPointer[poly]->vTranList[1].x,
					     renderList->polyPointer[poly]->vTranList[1].y,
					     renderList->polyPointer[poly]->color,
					     rcx);

		Draw_Clip_Line(renderList->polyPointer[poly]->vTranList[1].x,
			             renderList->polyPointer[poly]->vTranList[1].y,
					     renderList->polyPointer[poly]->vTranList[2].x,
					     renderList->polyPointer[poly]->vTranList[2].y,
					     renderList->polyPointer[poly]->color,
					     rcx);

		Draw_Clip_Line(renderList->polyPointer[poly]->vTranList[2].x,
			             renderList->polyPointer[poly]->vTranList[2].y,
					     renderList->polyPointer[poly]->vTranList[0].x,
					     renderList->polyPointer[poly]->vTranList[0].y,
					     renderList->polyPointer[poly]->color,
					     rcx);		
	}
}

void Draw_OBJECT4D_Solid(LPOBJECT4D obj, RenderContext* rcx)
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
	
		Draw_2D_Triangle(obj->vTransList[vindex_0].x,
		                 obj->vTransList[vindex_0].y,
			             obj->vTransList[vindex_1].x,
			             obj->vTransList[vindex_1].y,
			             obj->vTransList[vindex_2].y, 
			             obj->vTransList[vindex_2].z,
			             obj->polyList[poly].lightColor,
			             rcx);
	}
}

void Draw_RENDERLIST4D_Solid(LPRENDERLIST4D renderList, RenderContext* rcx) 
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4D_STATE_ACTIVE) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_BACKFACE))
			 continue;

		Draw_2D_Triangle(
			renderList->polyPointer[poly]->vTranList[0].x,
			renderList->polyPointer[poly]->vTranList[0].y, 
			renderList->polyPointer[poly]->vTranList[1].x, 
			renderList->polyPointer[poly]->vTranList[1].y, 
			renderList->polyPointer[poly]->vTranList[2].x, 
			renderList->polyPointer[poly]->vTranList[2].y,
			renderList->polyPointer[poly]->lightColor,
			rcx);
	}
}

