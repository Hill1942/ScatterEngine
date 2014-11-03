#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>


#include <iostream>
#include <cstdlib>
#include <cstring>

#include "SEConfig.h"
#include "platform/SEWindow.h"
#include "render/SEOpenGLDriver.h"


#define SE_DEBUG_ON

void Draw(RenderContext *rcx);


int main(int argc, char* argv[])
{	
	RenderContext       rcx;
	rcx.nWindowWidth  = 640;
	rcx.nWindowHeight = 480;
	rcx.nMousePosX    = 0;
	rcx.nMousePosY    = 0;

	CreateWindow(&rcx);
	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Unable to initilize GLEW..." << std::endl;
		Cleanup(&rcx);
		return 0;
	}

	InitGL();

	Loop(&rcx, Draw);
	Cleanup(&rcx);
	return 0;
}

void Draw(RenderContext *rcx)
{
	const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);


	DrawPixel(0.0f, -0.6f, 10.0f);
    glXSwapBuffers(rcx->dpy, rcx->win);    
}






