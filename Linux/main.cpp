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
#include "math/SEMath.h"
//#include "SEPerspective.h"




/*
POINT4D  cam_pos = {0,0,-100,1};
VECTOR4D cam_dir = {0,0,0,1};

// all your initialization code goes here...
VECTOR4D vscale={.5,.5,.5,1}, 
         vpos = {0,0,0,1}, 
         vrot = {0,0,0,1};

CAM4D        cam;                     // the single camera
RENDERLIST4D rend_list;               // the single renderlist
POLYF4D      poly1;                   // our lonely polygon
POINT4D        poly1_pos = {0,0,100,1}; // world position of polygon

*/

void Draw(RenderContext *rcx);
void InitGame(RenderContext *rcx);
void InitGL(RenderContext *rcx);

int main(int argc, char* argv[])
{	
	RenderContext       rcx;
	rcx.nWindowWidth  = 640;
	rcx.nWindowHeight = 480;
	rcx.nMousePosX    = 0;
	rcx.nMousePosY    = 0;

	CreateWindow(&rcx);
	
	InitGL(&rcx);

	InitGame(&rcx);

	Loop(&rcx, Draw);

	Cleanup(&rcx);

	return 0;
}

void Draw(RenderContext *rcx)
{
	const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);
/*
	static float ang_y = 0;      // rotation angle

	Reset_RENDERLIST4D(&rend_list);
    
    // insert polygon into the renderlist
    Insert_POLYF4D_RENDERLIST4D(&rend_list, &poly1);
    
    // generate rotation matrix around y axis
    Build_XYZ_Rotation_Matrix4X4(0, ang_y, 0, &mrot);
   
    // rotate polygon slowly
    if (++ang_y >= 360.0) ang_y = 0;
    
    // rotate the local coords of single polygon in renderlist
    Transform_RENDERLIST4D(&rend_list, &mrot, TRANSFORM_LOCAL_ONLY);
    
    // perform local/model to world transform
    Model_To_World_RENDERLIST4D(&rend_list, &poly1_pos);
    
    // generate camera matrix
    Build_CAM4D_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);
    
    // apply world to camera transform
    World_To_Camera_RENDERLIST4D(&rend_list, &cam);
    
    // apply camera to perspective transformation
    Camera_To_Perspective_RENDERLIST4D(&rend_list, &cam);
    
    // apply screen transform
    Perspective_To_Screen_RENDERLIST4D(&rend_list, &cam);
    
    // draw instructions
   // Draw_Text_GDI("Press ESC to exit.", 0, 0, RGB(0,255,0), lpddsback);
    
    // lock the back buffer
   // DDraw_Lock_Back_Surface();
    
    // render the polygon list
    Draw_RENDERLIST4D_Wire32(&rend_list, backBuffer, backLPitch);
    
    // unlock the back buffer
   // DDraw_Unlock_Back_Surface();
    
    // flip the surfaces
   // DDraw_Flip();
    
    // sync to 30ish fps
   // Wait_Clock(30);*/

    
	Draw_Pixel(100, 200, 0x00ff0000, rcx);
    glXSwapBuffers(rcx->dpy, rcx->win);    
}

void InitGame(RenderContext *rcx)
{
	//Build_Sin_Cos_Tables();

    // initialize a single polygon
    /*poly1.state  = POLY4D_STATE_ACTIVE;
    poly1.attr   =  0; 
    poly1.color  = _RGB32BIT_8888(255,255,0, 0);
      
	poly1.vList[0].x = 0;
    poly1.vList[0].y = 50;
    poly1.vList[0].z = 0;
    poly1.vList[0].w = 1;

    poly1.vList[1].x = 50;
    poly1.vList[1].y = -50;
    poly1.vList[1].z = 0;
    poly1.vList[1].w = 1;

    poly1.vList[2].x = -50;
    poly1.vList[2].y = -50;
    poly1.vList[2].z = 0;
    poly1.vList[2].w = 1;

    poly1.next = poly1.prev = NULL;
    
    // initialize the camera with 90 FOV, normalized coordinates
    Init_CAM4D(&cam,      // the camera object
                 CAM_MODEL_EULER, // euler camera model
                 &cam_pos,  // initial camera position
                 &cam_dir,  // initial camera angles
                 NULL,      // no initial target
                 50.0,      // near and far clipping planes
                 500.0,
                 90.0,      // field of view in degrees
                 rcx->nWindowWidth,   // size of final screen viewport
                 rdx->nWindowHeight);*/
}

void InitGL(RenderContext *rcx)
{
	glewExperimental = GL_TRUE; 
	if (glewInit() != GLEW_OK) 
	{
		std::cout << "Unable to initilize GLEW..." << std::endl;
		Cleanup(rcx);
		exit(0);
	}
    InitShaders();
}






