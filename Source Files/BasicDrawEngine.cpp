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

LPDIRECTDRAW7        lpdd = NULL;
LPDIRECTDRAWSURFACE7 lpddsPrimary = NULL;
LPDIRECTDRAWSURFACE7 lpddsBack = NULL;
LPDIRECTDRAWPALETTE  lpddPalette = NULL;
LPDIRECTDRAWCLIPPER  lpddClipper = NULL;
LPDIRECTDRAWCLIPPER  lpddClipperWin = NULL;

PALETTEENTRY   palette[MAX_COLORS_PALETEE];
PALETTEENTRY   savePalette[MAX_COLORS_PALETEE];
DDSURFACEDESC2 ddsd;
DDBLTFX        ddBltfx;
DDSCAPS2       ddsCaps;
HRESULT        ddRval;
UCHAR*         primaryBuffer = NULL;
UCHAR*         backBuffer = NULL;
int            primaryLPitch = 0;
int            backLPitch = 0;
BITMAP_FILE    bitmap8Bit;
BITMAP_FILE    bitmap16Bit;
BITMAP_FILE    bitmap24Bit;

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

int minClipX = 0;
int maxClipX = SCREEN_WIDDTH - 1;
int minClipY = 0;
int maxClipY = SCREEN_HEIGHT - 1;


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
	
	if (ddPixelFormat == DD_PIXEL_FORMAT555)
	{
		RGB16BIT = RGB16BIT555;
		cerr << "pixel format = 5.5.5" << endl;
	}
	else
	{
		RGB16BIT = RGB16BIT565;
		cerr << "pixel format = 5.6.5" << endl;
	}

	if (!screenWindowed)
	{
		ddsCaps.dwCaps = DDSCAPS_BACKBUFFER;
		if (FAILED(lpddsPrimary->GetAttachedSurface(&ddsCaps, &lpddsBack)))
			return 0;
	}
	else
	{
		lpddsBack = DDraw_Create_Surface(width, height, DDSCAPS_SYSTEMMEMORY);
	}

	if (screenBpp == DD_PIXEL_FORMAT8)
	{
		memset(palette, 0, MAX_COLORS_PALETEE * sizeof(PALETTEENTRY));
		Load_Palette_From_File(DEFAULT_PALETTE_FILE, palette);
		if (screenWindowed)
		{
			//for (int index = 0; 
		}
	}

	return 0;

}

int DDraw_ShutDown()
{

}

DWORD Get_Clock()
{
	return GetTickCount();
}

DWORD Start_Clock()
{
	return (start_clock_count = Get_Clock());
}

DWORD Wait_Clock(DWORD count)
{
	while ((Get_Clock() - start_clock_count) < count);

	return Get_Clock();
}

int Draw_Pixel(int    x,
			   int    y,
			   int    color,
			   UCHAR* buffer,
			   int    lPitch)
{
	buffer[x + y * lPitch] = color;

	return 1;
}

int Draw_Pixel16(int    x,
			   int    y,
			   int    color,
			   UCHAR* buffer,
			   int    lPitch)
{
	((USHORT*) buffer)[x + y * (lPitch >> 1)] = color;

	return 1;
}


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

int Draw_Line(int    x0, 
			  int    y0, 
			  int    x1, 
			  int    y1,
			  int    color,
			  UCHAR* buffer,
			  int    lPitch)
{
	int dx;
	int dy;
	int dx2;
	int dy2;
	int xinc;
	int yinc;
	int error;
	int index;

	buffer = buffer + x0 + y0 * lPitch;

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx >= 0)
	{
		xinc = 1;
	}
	else
	{
		xinc = -1;
		dx   = -dx;
	}

	if (dy >=0)
	{
		yinc = lPitch;	
	}
	else
	{
		yinc = -lPitch;
		dy   = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)
	{
		error = dy2 - dx;
		for (int i = 0; i < dx; i++)
		{
			*buffer = color;
			if (error >= 0)
			{
				error -= dx2;
				buffer += yinc;
			}
			error += dy2;
			buffer += xinc;
		}
	}
	else
	{
		error = dx2 - dy;
		for (int i = 0; i < dy; i++)
		{
			*buffer = color;
			if (error >= 0)
			{
				error -= dy2;
				buffer += xinc;
			}
			error += dx2;
			buffer += xinc;
		}
	}

	return 1;
}

int Draw_Line16(int    x0, 
			  int    y0, 
			  int    x1, 
			  int    y1,
			  int    color,
			  UCHAR* buffer,
			  int    lPitch)
{
	int dx;
	int dy;
	int dx2;
	int dy2;
	int xinc;
	int yinc;
	int error;
	int index;

	int lPitch_2 = lPitch >> 1;
	USHORT* buffer2 = (USHORT*)buffer + x0 + y0 * lPitch_2;

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx >= 0)
	{
		xinc = 1;
	}
	else
	{
		xinc = -1;
		dx   = -dx;
	}

	if (dy >=0)
	{
		yinc = lPitch_2;
	}
	else
	{
		yinc = -lPitch_2;
		dy   = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)
	{
		error = dy2 - dx;
		for (int i = 0; i < dx; i++)
		{
			*buffer = (USHORT)color;
			if (error >= 0)
			{
				error -= dx2;
				buffer += yinc;
			}
			error += dy2;
			buffer += xinc;
		}
	}
	else
	{
		error = dx2 - dy;
		for (int i = 0; i < dy; i++)
		{
			*buffer = (USHORT)color;
			if (error >= 0)
			{
				error -= dy2;
				buffer += xinc;
			}
			error += dx2;
			buffer += xinc;
		}
	}

	return 1;
}

int Draw_Clip_Line16(int    x0, 
					 int    y0, 
					 int    x1, 
					 int    y1,
					 int    color,
					 UCHAR* buffer, 
					 int    lPitch)
{
	int xc0 = x0;
	int yc0 = y0;
	int xc1 = x1;
	int yc1 = y1;

	if (Clip_Line(xc0, yc0, xc1, yc1))
		Draw_Line16(xc0, yc0, xc1, yc1, color, buffer, lPitch);

	return 1;
}

int Draw_Clip_Line(int    x0, 
					 int    y0, 
					 int    x1, 
					 int    y1,
					 int    color,
					 UCHAR* buffer, 
					 int    lPitch)
{
	int xc0 = x0;
	int yc0 = y0;
	int xc1 = x1;
	int yc1 = y1;

	if (Clip_Line(xc0, yc0, xc1, yc1))
		Draw_Line(xc0, yc0, xc1, yc1, color, buffer, lPitch);

	return 1;
}