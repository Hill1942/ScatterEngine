#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>

#include <iostream>

#include "../SEConfig.h"
#include "../platform/SEWindow.h"
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
	glPointSize(10.0f);
	//glBindVertexArray(vaos[0]);
	glDrawArrays(GL_POINTS, 0, 1);
}

/*
int Clip_Line(int &x1, int &y1, int &x2, int &y2)
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

int Draw_Line32_Hard(int x0, 
	                 int y0,
	                 int x1, 
	                 int y1,
	                 int color,
	                 int lPitch)
{

}

int Draw_Line32(int    x0, 
			    int    y0, 
			    int    x1, 
			    int    y1,
			    int    color,
			    int    lPitch)
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

	if (dx > dy)
	{
		int error = 0;
		for (int i = 0; i <= dx; i++)
		{
			Draw_Pixel32(x, y, color);
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
			Draw_Pixel32(x, y, color);
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

int Draw_Clip_Line32(int    x0, 
					 int    y0, 
					 int    x1, 
					 int    y1,
					 int    color)
{
	int xc0 = x0;
	int yc0 = y0;
	int xc1 = x1;
	int yc1 = y1;

	if (Clip_Line(xc0, yc0, xc1, yc1))
		Draw_Line32(xc0, yc0, xc1, yc1, color);

	return 1;
}
/*
void Draw_RENDERLIST4D_Wire32(LPRENDERLIST4D renderList
{
	for (int poly = 0; poly < renderList->numPolys; poly++)
	{
		if (!(renderList->polyPointer[poly]->state & POLY4D_STATE_ACTIVE)  ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_CLIPPED) ||
			 (renderList->polyPointer[poly]->state & POLY4D_STATE_BACKFACE))
		    continue;
	
		Draw_Clip_Line32(renderList->polyPointer[poly]->vTranList[0].x,
			             renderList->polyPointer[poly]->vTranList[0].y,
					     renderList->polyPointer[poly]->vTranList[1].x,
					     renderList->polyPointer[poly]->vTranList[1].y,
					     renderList->polyPointer[poly]->color);

		Draw_Clip_Line32(renderList->polyPointer[poly]->vTranList[1].x,
			             renderList->polyPointer[poly]->vTranList[1].y,
					     renderList->polyPointer[poly]->vTranList[2].x,
					     renderList->polyPointer[poly]->vTranList[2].y,
					     renderList->polyPointer[poly]->color);

		Draw_Clip_Line32(renderList->polyPointer[poly]->vTranList[2].x,
			             renderList->polyPointer[poly]->vTranList[2].y,
					     renderList->polyPointer[poly]->vTranList[0].x,
					     renderList->polyPointer[poly]->vTranList[0].y,
					     renderList->polyPointer[poly]->color);		
	}
}*/



