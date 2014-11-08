#include <X11/Xlib.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glx.h>


#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/time.h>


#include "SEConfig.h"
#include "platform/SEWindow.h"
#include "math/SEMath.h"
#include "render/SEOpenGLDriver.h"
#include "util/SELoader.h"
#include "SEPerspective.h"






POINT4D  cam_pos = {0,0,-100,1};
POINT4D  cam_target = {0,0,0,1};
VECTOR4D cam_dir = {0,0,0,1};

// all your initialization code goes here...
VECTOR4D vscale={20,20,20,1}, 
         vpos = {0,0,0,1}, 
         vrot = {0,0,0,1};

CAM4D        cam;                     // the single camera
RENDERLIST4D rend_list;               // the single renderlist
POLYF4D      poly1;                   // our lonely polygon
POINT4D        poly1_pos = {0,0,100,1}; // world position of polygon

OBJECT4D myobj;


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
    struct timeval start;
    gettimeofday(&start, NULL);

	const GLfloat color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	glClearBufferfv(GL_COLOR, 0, color);

    static MATRIX_4X4 mrot; // general rotation matrix

	static float ang_y = 0;      // rotation angle

	Reset_RENDERLIST4D(&rend_list);
    
    Insert_OBJECT4D_RENDERLIST4D(&rend_list, &myobj, 1);

    
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
    Draw_RENDERLIST4D_Solid(&rend_list, rcx);
    
    // unlock the back buffer
   // DDraw_Unlock_Back_Surface();
    
    // flip the surfaces
   // DDraw_Flip();
    
    // sync to 30ish fps
   // Wait_Clock(30);

    
	//Draw_Pixel(100, 200, 0x00ff0000, rcx);
    glXSwapBuffers(rcx->dpy, rcx->win);   

    struct timeval end;
    gettimeofday(&end, NULL);

    //std::cout << (start.tv_usec) << std::endl;

    //std::cout << (end.tv_usec) << std::endl;

    std::cout << (1000000 / (end.tv_usec - start.tv_usec)) << std::endl;
    
}

void InitGame(RenderContext *rcx)
{
	Build_Sin_Cos_Tables();

    Load_OBJECT4D_3DSASC(&myobj, 
                         "car.asc", 
                         &vscale, 
                         &vpos, 
                         &vrot, 
                         VERTEX_FLAGS_INVERT_WINDING_ORDER | VERTEX_FLAGS_SWAP_YZ);
    
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
                 rcx->nWindowHeight);
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






