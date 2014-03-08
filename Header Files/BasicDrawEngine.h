#ifndef BASIC_DRAW_ENGINE
#define BASIC_DRAW_ENGINE

#define SCREEN_WIDDTH      640
#define SCREEN_HEIGHT      480
#define SCREEN_BPP         8
#define MAX_COLORS_PALETEE 256


#define DEFAULT_PALETTE_FILE "PALDATA2.PAL"

#define SCREEN_FULLSCREEN  0
#define SCREEN_WINDOWED    1

#define BITMAP_ID                0x4D42
#define BITMAP_STATE_DEAD        0
#define BITMAP_STATE_ALIVE       1
#define BITMAP_STATE_DYING       2
#define BITMAP_ATTR_LOADED       128
#define BITMAP_EXTRACT_MODE_CELL 0
#define BITMAP_EXTRACT_MODE_ABS  1

#define DD_PIXEL_FORMAT8        8
#define DD_PIXEL_FORMAT555      15
#define DD_PIXEL_FORMAT565      16
#define DD_PIXEL_FORMAT888      24
#define DD_PIXEL_FORMATALPHA888 32


#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define _RGB16BIT_555(r, g, b) ((b & 31) + ((g & 31) << 5) + ((r & 31) << 10))
#define _RGB16BIT_565(r, g, b) ((b & 31) + ((g & 63) << 5) + ((r & 31) << 11))

#define _RGB24BIT_888(a, r, g, b)  ((b) + ((g) << 8) + ((r) << 16))
#define _RGB32BIT_8888(a, r, g, b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

#define SET_BIT(word, bit_flag) ((word) = ((word) | (bit_flag)))
#define RESET_BIT(word, bit_flag) ((word) = ((word) & (~bit_flag)))

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct, 0, sizeof(ddstruct)); ddstruct.dwSize = sizeof(ddstruct); }


typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;

typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmapfileheader;
	BITMAPINFOHEADER bitmapinfoheader;
	PALETTEENTRY     palette[256];
	UCHAR            *buffer;
}BITMAP_FILE, *LPBITMAP_FILE;

typedef struct BOB_TYP
{

};

typedef struct BITMAP_IMAGE_TYP
{
	int state;
	int attr;
	int x, y;
	int width, height;
	int numBytes;
	int bpp;
	UCHAR* buffer;
}BITMAP_IMAGE, *LPBITMAP_IMAGE;

typedef struct BLINKER_TYP
{
	int colorIndex;
	PALETTEENTRY onColor;
	PALETTEENTRY offColor;
	int onTime;
	int offTime;
	int counter;
	int state;
}BLINKER, *LPBLINKER;

typedef struct VERTEX2DI_TYP
{
	int x;
	int y;
}VERTEX2DI, *LPVERTEX2DI;

typedef struct VERTEX2DF_TYP
{
	float x;
	float y;
}VERTEX2DF, *LPVERTEX2DF;

typedef struct POLYGON2D_TYP
{
	int state;
	int numVertic;
	int x, y;
	int xv, yv;
	DWORD color;
	VERTEX2DF* vList;
}POLYGON2D, *LPPOLYGON2D;




int DDraw_Init(int width, int height, int bpp, int windowed = 0);
int DDraw_ShutDown();


LPDIRECTDRAWCLIPPER  DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds, int numRect, LPRECT clipList);
LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height, int memFlags = 0, USHORT colorKeyValue = 0);


/***************************************************************************
                       Function About DrectDraw
***************************************************************************/
int    DDraw_Flip();
int    DDraw_Wait_For_Vsync();
int    DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT* client = NULL);
UCHAR* DDraw_Lock_Surface(LPDIRECTDRAWSURFACE7 lpdds, int* lpitch);
int    DDraw_Unlock_Surface(LPDIRECTDRAWSURFACE7 lpdds);
UCHAR* DDraw_Lock_Primary_Surface();
int    DDraw_Unlock_Primary_Surface();
UCHAR* DDraw_Lock_Back_Surface();
int    DDraw_Unlock_Back_Surface();


/***************************************************************************
                                Utility Function
***************************************************************************/
DWORD Get_Clock();
DWORD Start_Clock();
DWORD Wait_Clock();
int   Collision_Test(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);
int   Collision_Scan(int x1, int y1, int x2, int y2, UCHAR scanStart, UCHAR scanEnd, UCHAR *scanBuffer, int scanLPitch);
int   Collision_Scan16(int x1, int y1, int x2, int y2, USHORT scanStart, USHORT scanEnd, UCHAR *scanBuffer, int scanLPitch);

int Draw_Pixel(int x, int y, int color, UCHAR* vedeoBuffer, int lPitch);
int Draw_Pixel16(int x, int y, int color, UCHAR* vedeoBuffer, int lPitch);

int Draw_Clip_Line(int x0, int y0, int x1, int y1, int color, UCHAR* destBuffer, int lPitch);
int Draw_Clip_Line16(int x0, int y0, int x1, int y1, int color, UCHAR* destBuffer, int lPitch);
int Draw_Clip_Line32(int x0, int y0, int x1, int y1, int color, UCHAR* destBuffer, int lPitch);

int Clip_Line(int& x1, int& y1, int& x2, int & y2);
int Draw_Line(int x0, int y0, int x1, int y1, int color, UCHAR* vbStart, int lPitch);
int Draw_Line16(int x0, int y0, int x1, int y1, int color, UCHAR* vbStart, int lPitch);
int Draw_Line32(int x0, int y0, int x1, int y1, int color, UCHAR* vbStart, int lPitch);

void HLine(int x1, int x2, int y, int color, UCHAR* vBuffer, int lPitch);
void VLine(int y1, int y2, int x, int color, UCHAR& vBuffer, int lPitch);
void HLine16(int x1, int x2, int y, int color, UCHAR* vBuffer, int lPitch);
void VLine16(int y1, int y2, int x, int color, UCHAR* vBuffer, int lPitch);

int Draw_Rectangle(int x1, int y1, int x2, int y2, int color, LPDIRECTDRAWSURFACE7 lpdds);
void Screen_Transitions(int effect, UCHAR* vBuffer, int lPitch);

int Set_Palette_Entry(int colorIndex, LPPALETTEENTRY color);
int Get_Palette_Entry(int colorIndex, LPPALETTEENTRY color);
int Load_Palette_From_File(char* filename, LPPALETTEENTRY palette);
int Save_Palette_To_File(char* filename, LPPALETTEENTRY palette);
int Save_Palette(LPPALETTEENTRY palette);
int Set_Palette(LPPALETTEENTRY palette);
int Rotate_Colors(int startIndex, int endStart);
int Blink_Colors(int command, LPBLINKER newLight, int id);


int Create_Bitmap(LPBITMAP_IMAGE image, int x, int y, int width, int height, int bpp = 8);
int Destory_Bitmap(LPBITMAP_IMAGE image);
//int Draw_Bitmap(LPBITMAP_IMAGE







#endif