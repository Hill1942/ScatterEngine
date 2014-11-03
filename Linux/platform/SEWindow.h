#ifndef SEWINDOW_H
#define SEWINDOW_H

typedef struct RenderContextRec
{
	GLXContext ctx;
	Display*   dpy;
	Window     win;
	int        nWindowWidth;
	int        nWindowHeight;
	int        nMousePosX;
	int        nMousePosY;
} RenderContext;

typedef void (*RenderCallback)(RenderContext*);
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

void CreateWindow(RenderContext* rcx);

void Loop(RenderContext* rcx, RenderCallback render);

void Cleanup(RenderContext* rcx);

#endif