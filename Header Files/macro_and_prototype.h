#ifndef MACRO_AND_PROTOTYPE
#define MACRO_AND_PROTOTYPE

#include <wingdi.h>




//basic unsigned type
typedef unsigned short USHORT;
typedef unsigned short WORD;
typedef unsigned char  UCHAR;
typedef unsigned char  BYTE;
typedef unsigned int   QUAD;
typedef unsigned int   UINT;

//container structure for bitmaps .BMP file
typedef struct BITMAP_FILE_TAG
{
	BITMAPFILEHEADER bitmap_file_header;  //this contains the bitmapfile header
	BITMAPINFOHEADER bitmap_info_header;  //this is all the info including the palette
	PALETTEENTRY     palette[256];        //we will store the palette here
	UCHAR*           buffer;              //this is a pointer to the data
}BITMAP_FILE, *PBITMAP_FILE;

//the blitter object structure BOB
typedef struct BOB_TYP
{
	/* data */
};

//the simple bitmap image
typedef struct BITMAP_IMAGE_TYP
{
	int state;               //state of bitmap
	int attribute            //attributs of bitmap
	int x, y;                //position of bitmap
	int width, height;       //size of bitmap
	int num_bytes;           //total bytes of bitmap
	int bits_per_pixel       //bits per pixel
	UCHAR* buffer;           //pixels of bitmap
}BITMAP_IMAGE, *PBITMAP_IMAGE;




#endif