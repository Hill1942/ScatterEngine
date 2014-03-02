#include <Windows.h>
#include <ddraw.h>

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   UINT;



#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define _RGB32BIT(a,r,g,b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))
#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct); }

// GLOBALS ////////////////////////////////////////////////
HWND      mainWindowHandle  = NULL; 
HINSTANCE hInstanceApp      = NULL; 


LPDIRECTDRAW7        lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsPrimary = NULL;
DDSURFACEDESC2       ddsd;
LPDIRECTDRAWPALETTE  lpddPalette = NULL;

int screenWidth  = 1366;
int screenHeight = 768;
int screenBpp    = 32;

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

inline void PlotPixel32(int x,
						int y,
						int alpha,
						int red,
						int green,
						int blue,
						UINT* buffer,
						int lPitch)
{
	UINT pixel = _RGB32BIT(alpha, red, green, blue);
	buffer[x + y * lPitch] = pixel;
}

int GameMain(int width, int height, int bpp)
{
	if (KEYDOWN(VK_ESCAPE))
		SendMessage(mainWindowHandle, WM_CLOSE, 0, 0);

	DDRAW_INIT_STRUCT(ddsd);

	if (FAILED(lpddsPrimary->Lock(NULL, 
		                          &ddsd, 
								  DDLOCK_SURFACEMEMORYPTR|DDLOCK_WAIT,
								  NULL)))
		return 0;


	int lPitch = (int)(ddsd.lPitch >> 2);
	UINT* buffer = (UINT*)ddsd.lpSurface;

	for (int i = 0; i < 1000; i++)
	{
		int red = rand() % 256;
		int green = rand() % 256;
		int blue = rand() & 256;

		int x = rand() % width;
		int y = rand() % height;


		PlotPixel32(x, y, 0, red, green, blue, buffer, lPitch);
	}

	if (FAILED(lpddsPrimary->Unlock(NULL)))
		return 0;

	return 1;

}

int GameInit(int width, int height, int screenBpp)
{
	if (FAILED(DirectDrawCreateEx(NULL, (void**)&lpdd, IID_IDirectDraw7, NULL)))
		return 0;

	if (FAILED(lpdd->SetCooperativeLevel(mainWindowHandle,
		                                DDSCL_FULLSCREEN|
										DDSCL_ALLOWMODEX|
										DDSCL_EXCLUSIVE|
										DDSCL_ALLOWREBOOT)))
		return 0;

	if (FAILED(lpdd->SetDisplayMode(width, height, screenBpp, 0, 0)))
		return 0;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsPrimary, NULL)))
		return 0;

	return 1;
}

int GameShutdown()
{
	if (lpddsPrimary)
	{
		lpddsPrimary->Release();
		lpddsPrimary = NULL;
	}

	if (lpdd)
	{
		lpdd->Release();
		lpdd = NULL;
	}

	return 1;
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevHinstance, LPSTR lpCmdLine, int nCmdShow)
{
	char szClassName[] = "MainWindow";

	WNDCLASSEX wndclass;
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW|CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WinProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hIconSm = NULL;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szClassName;

	hInstanceApp = hInstance;

	RegisterClassEx(&wndclass);
	
	HWND hwnd = CreateWindowEx(0,
		                       szClassName,
							   "Test Window",
							   WS_POPUP|WS_VISIBLE,
							   100,
							   100,
							   640,
							   480,
							   NULL,
							   NULL,
							   hInstance,
							   NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Error in Creating Window!", "Error", MB_OK);
		return -1;
	}

	mainWindowHandle = hwnd;

	GameInit(screenWidth, screenHeight, screenBpp);

	MSG msg;
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		GameMain(screenWidth, screenHeight, screenBpp);
	}

	GameShutdown();

	return msg.wParam;
}

LRESULT CALLBACK WinProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		{
			HDC hdc;
			PAINTSTRUCT ps;

			hdc = BeginPaint(hwnd, &ps);

			EndPaint(hwnd, &ps);
			
			return 0;
		}

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	default:
		break;
	}

	return DefWindowProc(hwnd, message, wParam, lParam);

}


