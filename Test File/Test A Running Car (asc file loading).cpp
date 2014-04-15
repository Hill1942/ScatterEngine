#define _CRT_SECURE_NO_WARNINGS

#define PARSER_DEBUG_ON

#include <iostream>
#include <windows.h>

#include <ddraw.h>
#include <dinput.h>


#include "BasicDrawEngine.h"
#include "MathEngine.h"
#include "InputEngine.h"
#include "3DPipeLine.h"
#include "IOHandling.h"


#define WINDOW_CLASS_NAME "WIN3DCLASS"

#define WINDOW_TITLE      "Scatter Engine"
#define WINDOW_WIDTH      800
#define WINDOW_HEIGHT     600
 
#define WINDOW_BPP        32

#define WINDOWED_APP      1

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1:0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0:1)


//Function Prototype
int Game_Init(void *param = NULL);
int Game_Shutdown(void* param = NULL);
int Game_Main(void* param = NULL);

//Global Variable
HWND mainWindowHandle = NULL;
HINSTANCE windowInstance = NULL;
char buffer[256];



POINT4D  cam_pos = {0,0,-100,1};
POINT4D  cam_target = {0,0,0,1};
VECTOR4D cam_dir = {0,0,0,1};

// all your initialization code goes here...
VECTOR4D vscale={20,20,20,1}, 
         vpos = {0,0,0,1}, 
         vrot = {0,0,0,1};

CAM4DV1        cam;                     // the single camera
RENDERLIST4DV1 rend_list;               // the single renderlist
POLYF4DV1      poly1;                   // our lonely polygon
POINT4D        poly1_pos = {0,0,100,1}; // world position of polygon

OBJECT4DV1 myobj;


extern LPDIRECTDRAWSURFACE7 lpddsBack;
extern UCHAR* backBuffer;

extern int backLPitch;

extern int windowClientX;
extern int windowClientY;

LRESULT WINAPI WinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (msg)
	{
	case WM_CREATE:
		return(0);
		break;

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        return(0);
		break;

	case WM_DESTROY: 
		PostQuitMessage(0);
		return(0);
	    break;

	default:
		break;
	}
	return (DefWindowProc(hWnd, msg, wParam, lParam));
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR sCmdLine, int nCmdShow)
{
	WNDCLASSEX wnd = {};
	wnd.cbSize = sizeof(WNDCLASSEX);
	wnd.lpfnWndProc = WinProc;
	wnd.hInstance = hInstance;
	wnd.lpszClassName = WINDOW_CLASS_NAME;

	RegisterClassEx(&wnd);

	mainWindowHandle = CreateWindowEx(
		0, 
		WINDOW_CLASS_NAME,
		WINDOW_TITLE,
		 (WINDOWED_APP ? (WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION) : (WS_POPUP | WS_VISIBLE)),
		200, 100, WINDOW_WIDTH, WINDOW_HEIGHT,
		NULL,
		NULL,
		hInstance,
		NULL);

	//SetWindowLong(, GWL_EXSTYLE, WS_EX_TOOLWINDOW);

	if (mainWindowHandle == NULL)
		return 0;

	windowInstance = hInstance;

	if (WINDOWED_APP)
	{
		RECT windowRect = {0, 0, WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1};
		AdjustWindowRectEx(&windowRect,
			               GetWindowLong(mainWindowHandle, GWL_STYLE),
						   NULL,
						   GetWindowLong(mainWindowHandle, GWL_EXSTYLE));

		windowClientX = -windowRect.left;
		windowClientY = -windowRect.top;
		

		MoveWindow(mainWindowHandle,
			       0,
				   0,
				   windowRect.right - windowRect.left,
				   windowRect.bottom - windowRect.top,
				   FALSE);

		ShowWindow(mainWindowHandle, SW_SHOW);
	}

	Game_Init();

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			Game_Main();
		}
	}

	Game_Shutdown();
	return 0;
}

int Game_Init(void *param)
{
	DDraw_Init(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP, WINDOWED_APP);

	DInput_Init();
	DInput_Init_Keyboard();

	Build_Sin_Cos_Tables();




	Load_OBJECT4DV1_3DSASC(&myobj, "car.asc", &vscale, &vpos, &vrot, VERTEX_FLAGS_INVERT_WINDING_ORDER | VERTEX_FLAGS_SWAP_YZ);

    
    // initialize the camera with 90 FOV, normalized coordinates
    Init_CAM4DV1(&cam,      // the camera object
                 CAM_MODEL_EULER, // euler camera model
                 &cam_pos,  // initial camera position
                 &cam_dir,  // initial camera angles
                 NULL,      // no initial target
                 50.0,      // near and far clipping planes
                 500.0,
                 90.0,      // field of view in degrees
                 WINDOW_WIDTH,   // size of final screen viewport
                 WINDOW_HEIGHT);
    

	
	return 1;
}

int Game_Shutdown(void *param)
{
	DInput_Release_Keyboard();

    DInput_Shutdown();

// shutdown directdraw last
	DDraw_ShutDown();

	return 1;
} 

int Game_Main(void *param)
{
	static MATRIX_4X4 mrot; // general rotation matrix

    static float ang_y = 0;      // rotation angle
    
    int index; // looping var
    
    // start the timing clock
    Start_Clock();
    
    // clear the drawing surface 
    DDraw_Fill_Surface(lpddsBack, 0);
    
    // read keyboard and other devices here
    DInput_Read_Keyboard();
    
    // game logic here...
    
    // initialize the renderlist
    Reset_RENDERLIST4DV1(&rend_list);
    

	//Reset_OBJECT4DV1(&myobj);

	Insert_OBJECT4DV1_RENDERLIST4DV1(&rend_list, &myobj, 1);

	Build_XYZ_Rotation_Matrix4X4(0, ang_y, 0, &mrot);
   
    if (++ang_y >= 360.0) ang_y = 0;

    Transform_RENDERLIST4DV1(&rend_list, &mrot, TRANSFORM_LOCAL_ONLY);
    
	Model_To_World_RENDERLIST4DV1(&rend_list, &poly1_pos);

	Build_CAM4DV1_Matrix_Euler(&cam, CAM_ROT_SEQ_ZYX);

    // apply world to camera transform
    World_To_Camera_RENDERLIST4DV1(&rend_list, &cam);
    
    // apply camera to perspective transformation
    Camera_To_Perspective_RENDERLIST4DV1(&rend_list, &cam);
    
    // apply screen transform
    Perspective_To_Screen_RENDERLIST4DV1(&rend_list, &cam);
    
    // draw instructions
   // Draw_Text_GDI("Press ESC to exit.", 0, 0, RGB(0,255,0), lpddsback);
    
    // lock the back buffer
    DDraw_Lock_Back_Surface();
    
    // render the polygon list
    Draw_RENDERLIST4DV1_Wire32(&rend_list, backBuffer, backLPitch);
    
    // unlock the back buffer
    DDraw_Unlock_Back_Surface();
    
    // flip the surfaces
    DDraw_Flip();
    
   
    Wait_Clock(17);
    
    // check of user is trying to exit
	if (KEY_DOWN(VK_ESCAPE))
	{
		PostMessage(mainWindowHandle, WM_DESTROY, 0, 0);
    } 


	return 1;
} 