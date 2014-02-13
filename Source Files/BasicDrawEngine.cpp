#include <Windows.h>
#include <cmath>
#include <iostream>
#include <cstring>

#include <ddraw.h>
#include "BasicDrawEngine.h"

using namespace std;

extern HWND mainWindowHandle;
extern HINSTANCE windowInstance;

FILE* fp_error;
char error_fillname[80];

LPDIRECTDRAW7         lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsPrimary = NULL;
LPDIRECTDRAWSURFACE7 lpddsBack = NULL;
LPDIRECTDRAWPALETTE  lpddPalette = NULL;
LPDIRECTDRAWCLIPPER  lpddClipper = NULL;
LPDIRECTDRAWCLIPPER  lpddClipperWin = NULL;

PALETTEENTRY palette[MAX_COLORS_PALETEE];
PALETTEENTRY savePalette[MAX_COLORS_PALETEE];
DDSURFACEDESC2 ddsd;
DDBLTFX ddBltfx;
DDSCAPS2 ddsCaps;
HRESULT ddRval;
UCHAR* primaryBuffer = NULL;
UCHAR* backBuffer = NULL;
int primaryLPitch = 0;
int backLPitch = 0;
BITMAP_FILE bitmap8Bit;
BITMAP_FILE bitmap16Bit;
BITMAP_FILE bitmap24Bit;

DWORD start_clock_count;
int windowed_mode = FALSE;

int minClipX = 0;
int maxClipX = SCREEN_WIDDTH - 1;
int minClipY = 0;
int maxClipY = SCREEN_HEIGHT - 1;

int screenWidth    = SCREEN_WIDDTH;
int screenHeight   = SCREEN_HEIGHT;
int screenBpp      = SCREEN_BPP;
int screenWindowed = SCREEN_WINDOWED;

int ddPixelFormat  = DD_PIXEL_FORMAT565;

int windowClientX = 0;
int windowClientY = 0;

USHORT (*RGB16BIT)(int r, int g, int b) = NULL;

USHORT RGB16BIT565(int r, int g, int b)
{
	r >>= 3;
	g >>= 2;
	b >>= 3;
	return _RGB16BIT_565((r), (g), (b));
}

USHORT RGB16BIT555(int r, int g, int b)
{
	r >>= 3;
	g >>= 3;
	b >>= 3;
	return _RGB16BIT_555((r), (g), (b));
}

int DDraw_Init(int width, int height, int bpp, int windowed = 0)
{
	int index;
	if (FAILED(DirectDrawCreateEx(NULL, (void**)&lpdd, IID_IDirectDraw7, NULL)))
		return 0;

	if (windowed)
	{
		if (FAILED(lpdd->SetCooperativeLevel(mainWindowHandle, DDSCL_NORMAL)))
			return 0;
	}
	else
	{
		if (FAILED(lpdd->SetCooperativeLevel(mainWindowHandle,
			DDSCL_ALLOWMODEX|DDSCL_FULLSCREEN|DDSCL_EXCLUSIVE|DDSCL_ALLOWREBOOT|DDSCL_MULTITHREADED)))
			return 0;
		if (FAILED(lpdd->SetDisplayMode(width, height, bpp, 0, 0)))
			return 0;
	}

	screenHeight = height;
	screenWidth = width;
	screenBpp = bpp;
	screenWindowed = windowed;
	
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (!screenWindowed)
	{
		ddsd.dwFlags = DDSD_CAPS|DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE|DDSCAPS_FLIP|DDSCAPS_COMPLEX;
		ddsd.dwBackBufferCount = 1;
	}
	else
	{
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		ddsd.dwBackBufferCount = 0;
	}

	lpdd->CreateSurface(&ddsd, &lpddsPrimary, NULL);
	DDPIXELFORMAT ddpf;
	DDRAW_INIT_STRUCT(ddpf);

	lpddsPrimary->GetPixelFormat(&ddpf);

	ddPixelFormat = ddpf.dwRGBBitCount;

	cerr << "pixel format" << ddPixelFormat <<endl;
	/*
	if (ddPixelFormat == DD_PIXEL_FORMAT555)
	{*/
		

	return 0;

}

int DDraw_ShutDown();

