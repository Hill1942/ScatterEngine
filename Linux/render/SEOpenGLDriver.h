#ifndef SEOPENGLDRIVER_H
#define SEOPENGLDRIVER_H

#define RGB32BIT_8888(a, r, g, b) ((b) + ((g) << 8) + ((r) << 16) + ((a) << 24))

inline void BREAKRGB32(int color, int& a, int& r, int& g, int& b)
{
	a = (color >> 24) & 0x000000ff;
	r = (color >> 16) & 0x000000ff;
	g = (color >> 8 ) & 0x000000ff;
	b = color & 0x000000ff;
}



void InitShaders();
GLuint CompileShaders();

void Draw_Pixel(int x, int y, int color, RenderContext *rcx);


#endif