#include <X11/Xlib.h>
#include <GL/glx.h>

#include <iostream>
#include <cstdlib>

#include "SEWindow.h"



void CreateWindow(RenderContext* rcx)
{
	XSetWindowAttributes winAttribs;
	GLint                winmask;
	GLint                nMajorVerion = 0;
	GLint                nMinorVerion = 0;
	XVisualInfo*         visualInfo;
	GLXFBConfig*         fbConfigs;
	int                  numConfigs   = 0;
	static int           fbAttribs[] = 
	{
		GLX_RENDER_TYPE, 
		GLX_RGBA_BIT,
		GLX_X_RENDERABLE,
		True,
		GLX_DRAWABLE_TYPE,
		GLX_WINDOW_BIT,
		GLX_DOUBLEBUFFER,
		True,
		GLX_RED_SIZE,
		8,
		GLX_BLUE_SIZE,
		8,
		GLX_GREEN_SIZE,
		8,
		0
	};

	rcx->dpy = XOpenDisplay(NULL);

	glXQueryVersion(rcx->dpy, &nMajorVerion, &nMinorVerion);
#ifdef SE_DEBUG_ON
	std::cout << "Supported GLX version - " << nMajorVerion << "." << nMinorVerion << std::endl;
#endif
	if (nMajorVerion == 1 && nMinorVerion < 2) 
	{
		std::cout << "Error: GLX 1.2 or greater is necessary" << std::endl;
		XCloseDisplay(rcx->dpy);
		exit(0);
	}
	fbConfigs               = glXChooseFBConfig(rcx->dpy,
		                                        DefaultScreen(rcx->dpy),
		                                        fbAttribs,
		                                        &numConfigs);
	visualInfo              = glXGetVisualFromFBConfig(rcx->dpy,
		                                               fbConfigs[0]);
	winAttribs.event_mask   = ExposureMask         |
	                          VisibilityChangeMask |
	                          KeyPressMask         |
	                          PointerMotionMask    |
	                          StructureNotifyMask;
	winAttribs.border_pixel = 0;
	winAttribs.bit_gravity  = StaticGravity;
	winAttribs.colormap     = XCreateColormap(rcx->dpy,
		                                      RootWindow(rcx->dpy, visualInfo->screen),
		                                      visualInfo->visual,
		                                      AllocNone);

	winmask                 = CWBorderPixel |
	                          CWEventMask   |
	                          CWColormap;
	rcx->win                = XCreateWindow(rcx->dpy, 
		                                    DefaultRootWindow(rcx->dpy),
		                                    0, 
		                                    0,
		                                    rcx->nWindowWidth,
		                                    rcx->nWindowHeight,
		                                    0,
		                                    visualInfo->depth,
		                                    InputOutput,
		                                    visualInfo->visual,
		                                    winmask,
		                                    &winAttribs);            
	XMapWindow(rcx->dpy, rcx->win);

	GLint attribs[] = 
	{
		GLX_CONTEXT_MAJOR_VERSION_ARB,
		3,
		GLX_CONTEXT_MINOR_VERSION_ARB,
		1,
		GLX_CONTEXT_PROFILE_MASK_ARB, 
		GLX_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB = (glXCreateContextAttribsARBProc)
           glXGetProcAddressARB( (const GLubyte *) "glXCreateContextAttribsARB" );
	rcx->ctx = glXCreateContextAttribsARB(rcx->dpy,
		                                  fbConfigs[0],
		                                  0,
		                                  True,
		                                  attribs);

	glXMakeCurrent(rcx->dpy, rcx->win, rcx->ctx);

	//const GLubyte *s = glGetString(GL_VERSION);
	//std::cout << "GL Version = " << s << std::endl;
}

void Loop(RenderContext* rcx, RenderCallback render)
{
	bool bWinMapped = false;
	for(;;)
	{
		XEvent newEvent;
		XWindowAttributes winData;

		XNextEvent(rcx->dpy, &newEvent);

		switch(newEvent.type)
		{
			case UnmapNotify:
			    bWinMapped = False;
			    break;
			case MapNotify:
				bWinMapped = True;
				break;
			case ConfigureNotify:
				XGetWindowAttributes(rcx->dpy, rcx->win, &winData);
				rcx->nWindowHeight = winData.height;
				rcx->nWindowWidth  = winData.width;
				break;
			case MotionNotify:
				rcx->nMousePosX = newEvent.xmotion.x;
				rcx->nMousePosY = newEvent.xmotion.y;
				break;
			case KeyPress:
				break;
			case DestroyNotify:
				Cleanup(rcx);
				exit(0);
				break;
		}
		if (bWinMapped) 
		{
			render(rcx);
		}
	}
}

void Cleanup(RenderContext* rcx)
{
	glXMakeCurrent(rcx->dpy, None, NULL);
	glXDestroyContext(rcx->dpy, rcx->ctx);
	rcx->ctx = NULL;

	glXDestroyWindow(rcx->dpy, rcx->win);
	rcx->win = (Window)NULL;

	XCloseDisplay(rcx->dpy);
	rcx->dpy = 0;
}