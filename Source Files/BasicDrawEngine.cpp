#include <Windows.h>

#include <cmath>
#include <iostream>
#include <cstring>

#include <ddraw.h>

#include "BasicDrawEngine.h"

#pragma comment(lib, "ddraw.lib")
#pragma comment(lib, "dxguid.lib")

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

int DDraw_Init(int width, int height, int bpp, int windowed)
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
	/*
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
	}*/

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
	/*
	if (screenBpp == DD_PIXEL_FORMAT8)
	{
		memset(palette, 0, MAX_COLORS_PALETEE * sizeof(PALETTEENTRY));
		Load_Palette_From_File(DEFAULT_PALETTE_FILE, palette);
		if (screenWindowed)
		{
			for (int index = 0; index < 10; index++)
			{
				palette[index].peFlags = palette[index + 246].peFlags = PC_EXPLICIT;
			}
			if (FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE,
				                           palette,
										   &lpddPalette,
										   NULL)))
				return 0;
		}
		else
		{
			if(FAILED(lpdd->CreatePalette(DDPCAPS_8BIT | DDPCAPS_INITIALIZE,
				                          palette,
										  &lpddPalette,
										  NULL)))
				return 0;
		}
		if (FAILED(lpddsPrimary->SetPalette(lpddPalette)))
			return 0;
	}*/

	if (screenWindowed)
	{
		DDraw_Fill_Surface(lpddsBack, 0);
	}
	else
	{
		DDraw_Fill_Surface(lpddsPrimary, 0);
		DDraw_Fill_Surface(lpddsBack,    0);
	}

	minClipX = 0;
	maxClipX = screenWidth - 1;
	minClipY = 0;
	maxClipY = screenHeight - 1;

	RECT screenRect = 
	{
		0,
		0,
		screenWidth,
		screenHeight
	};

	lpddClipper = DDraw_Attach_Clipper(lpddsBack, 1, &screenRect);

	if (screenWindowed)
	{
		if (FAILED(lpdd->CreateClipper(0, &lpddClipperWin, NULL)))
			return 0;
		if (FAILED(lpddClipperWin->SetHWnd(0, mainWindowHandle)))
			return 0;
		if (FAILED(lpddsPrimary->SetClipper(lpddClipper)))
			return 0;
	}

	return 1;

}

int DDraw_ShutDown()
{
	if (lpddClipper)
		lpddClipper->Release();

	if (lpddClipperWin)
		lpddClipperWin->Release();

	if (lpddPalette)
		lpddPalette->Release();

	if (lpddsBack)
		lpddsBack->Release();

	if (lpddsPrimary)
		lpddsPrimary->Release();

	if (lpdd)
		lpdd->Release();

	return 0;
}

LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
										 int                  numRects,
										 LPRECT               clipList)
{
	int index;
	LPDIRECTDRAWCLIPPER lpddClipper;
	LPRGNDATA regionData;

	if (FAILED(lpdd->CreateClipper(0, &lpddClipper, NULL)))
		return NULL;

	regionData = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + numRects * sizeof(RECT));

	memcpy(regionData->Buffer, clipList, sizeof(RECT) * numRects);

	regionData->rdh.dwSize         = sizeof(RGNDATAHEADER);
	regionData->rdh.iType          = RDH_RECTANGLES;
	regionData->rdh.nCount         = numRects;
	regionData->rdh.nRgnSize       = numRects * sizeof(RECT);

	regionData->rdh.rcBound.left   =  64000;
	regionData->rdh.rcBound.top    =  64000;
	regionData->rdh.rcBound.right  = -64000;
	regionData->rdh.rcBound.bottom = -64000;

	for (int index = 0; index < numRects; index++)
	{
		if(clipList[index].left < regionData->rdh.rcBound.left)
			regionData->rdh.rcBound.left = clipList[index].left;

		if(clipList[index].right < regionData->rdh.rcBound.right)
			regionData->rdh.rcBound.right = clipList[index].right;

		if(clipList[index].top < regionData->rdh.rcBound.top)
			regionData->rdh.rcBound.top = clipList[index].top;

		if(clipList[index].bottom < regionData->rdh.rcBound.bottom)
			regionData->rdh.rcBound.bottom = clipList[index].bottom;
	}

	if (FAILED(lpddClipper->SetClipList(regionData, 0)))
	{
		free(regionData);
		return NULL;
	}

	if (FAILED(lpdds->SetClipper(lpddClipper)))
	{
		free(regionData);
		return NULL;
	}

	free(regionData);

	return lpddClipper;
}

LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int    width,
										  int    height, 
										  int    flags,
										  USHORT colorKeyValue)
{
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAWSURFACE7 lpdds;
	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize   = sizeof(ddsd);
	ddsd.dwFlags  = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth  = width;
	ddsd.dwHeight = height;

	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | flags;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return NULL;

	DDCOLORKEY colorKey;
	colorKey.dwColorSpaceLowValue  = colorKeyValue;
	colorKey.dwColorSpaceHighValue = colorKeyValue;

	lpdds->SetColorKey(DDCKEY_SRCBLT, &colorKey);

	return lpdds;
}

int DDraw_Flip()
{
	if (primaryBuffer || backBuffer)
		return 0;

	if (!screenWindowed)
		while(FAILED(lpddsPrimary->Flip(NULL, DDFLIP_WAIT)));
	else
	{
		RECT rect;
		GetWindowRect(mainWindowHandle, &rect);

		rect.left  += windowClientX;
		rect.top   += windowClientY;

		rect.right  = rect.left + screenWidth  - 1;
		rect.bottom = rect.top  + screenHeight - 1;

		if (FAILED(lpddsPrimary->Blt(&rect, lpddsBack, NULL, DDBLT_WAIT, NULL)))
			return 0;
	}

	return 1;
}

int DDraw_Wait_For_Vsync()
{
	lpdd->WaitForVerticalBlank(DDWAITVB_BLOCKBEGIN, 0);

	return 1;
}

int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client)
{
	DDBLTFX ddbltfx;

	DDRAW_INIT_STRUCT(ddbltfx);

	ddbltfx.dwFillColor = color;

	lpdds->Blt(client,
		       NULL,
			   NULL,
			   DDBLT_COLORFILL | DDBLT_WAIT,
			   &ddbltfx);

	return 1;
}

UCHAR* DDraw_Lock_Surface(LPDIRECTDRAWSURFACE7 lpdds, int *lPitch)
{
	if (!lpdds)
		return NULL;

	DDRAW_INIT_STRUCT(ddsd);
	lpdds->Lock(NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR, NULL);

	if (lPitch)
		*lPitch = ddsd.lPitch;

	return (UCHAR*)ddsd.lpSurface;
}

int DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE7 lpdds)
{
	if (!lpdds)
		return 0;

	lpdds->Unlock(NULL);

	return 1;
}

UCHAR* DDraw_Lock_Primary_Surface()
{
	if (primaryBuffer)
		return primaryBuffer;

	DDRAW_INIT_STRUCT(ddsd);
	lpddsPrimary->Lock(NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR, NULL);

	primaryBuffer = (UCHAR*)ddsd.lpSurface;
	primaryLPitch = ddsd.lPitch;

	return primaryBuffer;
}

int DDraw_Unlock_Primary_Surface()
{
	if (!primaryBuffer)
		return 0;

	lpddsPrimary->Unlock(NULL);

	primaryBuffer = NULL;
	primaryLPitch = 0;

	return 1;
}

UCHAR* DDraw_Lock_Back_Surface()
{
	if (backBuffer)
		return backBuffer;

	DDRAW_INIT_STRUCT(ddsd);
	lpddsBack->Lock(NULL, &ddsd, DDLOCK_WAIT|DDLOCK_SURFACEMEMORYPTR, NULL);

	backBuffer = (UCHAR*)ddsd.lpSurface;
	backLPitch = ddsd.lPitch;

	return backBuffer;
}

int DDraw_Unlock_Back_Surface()
{
	if (!backBuffer)
		return 0;

	lpddsBack->Unlock(NULL);
	
	backBuffer = NULL;
	backLPitch = 0;

	return 1;
}

int Set_Palette_Entry(int color_index, LPPALETTEENTRY color)
{
	lpddPalette->SetEntries(0, color_index, 1, color);
	memcpy(&palette[color_index], color, sizeof(PALETTEENTRY));

	return 1;
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
			*buffer2 = (USHORT)color;
			if (error >= 0)
			{
				error -= dx2;
				buffer2 += yinc;
			}
			error += dy2;
			buffer2 += xinc;
		}
	}
	else
	{
		error = dx2 - dy;
		for (int i = 0; i < dy; i++)
		{
			*buffer2 = (USHORT)color;
			if (error >= 0)
			{
				error -= dy2;
				buffer2 += xinc;
			}
			error += dx2;
			buffer2 += xinc;
		}
	}

	return 1;
}


int Draw_Line32(int    x0, 
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

	int lPitch_2 = lPitch >> 2;
	UINT* buffer2 = (UINT*)buffer + x0 + y0 * lPitch_2;

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
			*buffer2 = (UINT)color;
			if (error >= 0)
			{
				error -= dx2;
				buffer2 += yinc;
			}
			error += dy2;
			buffer2 += xinc;
		}
	}
	else
	{
		error = dx2 - dy;
		for (int i = 0; i < dy; i++)
		{
			*buffer2 = (UINT)color;
			if (error >= 0)
			{
				error -= dy2;
				buffer2 += xinc;
			}
			error += dx2;
			buffer2 += xinc;
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

int Draw_Clip_Line32(int    x0, 
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
		Draw_Line32(xc0, yc0, xc1, yc1, color, buffer, lPitch);

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

int Draw_Rectangle(int                  x1,
				   int                  y1,
				   int                  x2,
				   int                  y2,
				   int                  color,
				   LPDIRECTDRAWSURFACE7 lpdds)
{

	DDBLTFX ddbltfx;
	RECT    rect;

	DDRAW_INIT_STRUCT(ddbltfx);

	ddbltfx.dwFillColor = color;

	rect.top    = y1;
	rect.left   = x1;
	rect.bottom = y2;
	rect.right  = x2;

	lpdds->Blt(&rect,
		       NULL, 
			   NULL,
			   DDBLT_COLORFILL|DDBLT_WAIT,
			   &ddbltfx);

	return 1;
}