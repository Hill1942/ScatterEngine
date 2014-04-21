/** @defgroup ScatterEngine Light and Shading
 *  @author  Yang Kaidi
 *  @version 1.0
 *  @date    2013-2014
 *  @{
 */

#ifndef LIGHTANDSHADING
#define LIGHTANDSHADING

/** @name Macro Constant
 *  @{
 */

#define LIGHTV1_ATTR_AMBIENT     0x0001
#define LIGHTV1_ATTR_INFINITE    0x0002
#define LIGHTV1_ATTR_DIRECTIONAL 0x0002
#define LIGHTV1_ATTR_POINT       0x0004
#define LIGHTV1_ATTR_SPOTLIGHT1  0x0008
#define LIGHTV1_ATTR_SPOTLIGHT2  0x0010

#define LIGHTV1_STATE_ON         1
#define LIGHTV2_STATE_OFF        0

#define MAX_LIGHTS               8

#define MATERIALV1_ATTR_2SIDED              0x0001
#define MATERIALV1_ATTR_TRANSPARENT         0x0002
#define MATERIALV1_ATTR_8BITCOLOR           0x0004
#define MATERIALV1_ATTR_RGB16               0x0008
#define MATERIALV1_ATTR_RGB24               0x0010
#define MATERIALV1_ATTR_SHADEMODE_CONSTANT  0x0020
#define MATERIALV1_ATTR_SHADEMODE_EMMISIVE  0x0020
#define MATERIALV1_ATTR_SHADEMODE_FLAT      0x0040
#define MATERIALV1_ATTR_SHADEMODE_GOURAUD   0x0080
#define MATERIALV1_ATTR_SHADEMODE_FASTPHONG 0x0100
#define MATERIALV1_ATTR_SHADEMODE_TEXTURE   0x0200

#define MATERIALV1_STATE_ACTIVE             0x0001
#define MATERIALV1_STATE_INACTIVE           0x0002

#define MAX_MATERIALV1                      256

/** @}*/ // Macro Constant


/** @name Struct Define
 *  @{
 */

typedef struct RGBAV1_TYP
{
	union
	{
		int   rgba;
		UCHAR RGBA[4];
		struct
		{
			UCHAR a, b, g, r;
		};
	};
}RGBAV1, *LPRGBAV1;

typedef struct MATERIALV1_TYP
{
	int    state;
	int    id;
	char   name[64];
	int    attr;

	RGBAV1 color;
	RGBAV1 ra;
	RGBAV1 rd;
	RGBAV1 rs;
	float  ka;
	float  kd;
	float  ks;
	float  power;

	char   texturFile[80];
	BITMAP_IMAGE texture;
}MATERIALV1, *LPMATERIALV1;

typedef struct LIGHTV1_TYP
{
	int state;
	int id;
	int attr;

	RGBAV1 ambient;
	RGBAV1 diffuse;
	RGBAV1 specular;

	POINT4D  pos;
	VECTOR4D dir;

	float kc;
	float kl;
	float kq;
	
	float spotInnerTheta;
	float spotOuterTheta;

	float pf;
}LIGHTV1, *LPLIGHTV1;

/** @}*/ // Struct Define


void Draw_OBJECT4DV1_Solid(LPOBJECT4DV1 obj, UCHAR* buffer, int lPitch);

void Draw_RENDERLIST4DV1_Solid(LPRENDERLIST4DV1 renderList, UCHAR* buffer, int lPitch);

void Draw_OBJECT4DV1_Solid16(LPOBJECT4DV1 obj, UCHAR* buffer, int lPitch);

void Draw_RENDERLIST4DV1_Solid16(LPRENDERLIST4DV1 renderList, UCHAR* buffer, int lPitch);

void Draw_OBJECT4DV1_Solid32(LPOBJECT4DV1 obj, UCHAR* buffer, int lPitch);

void Draw_RENDERLIST4DV1_Solid32(LPRENDERLIST4DV1 renderList, UCHAR* buffer, int lPitch);


int Init_Light(int        index,
			   int        _state,
			   int        _attr,
			   RGBAV1     _ambient,
			   RGBAV1     _diffuse,
			   RGBAV1     _specular,
			   LPPOINT    _pos,
			   LPVECTOR4D _dir,
			   float      _kc,
			   float      _kl,
			   float      _kq,
			   float      _spotInnerTheta,
			   float      _spotOuterTheta,
			   float      _pf);

int Reset_Lights();

int Reset_Materials();

int Insert_OBJECT4DV1_RENDERLIST4DV2(LPRENDERLIST4DV1  renderList,
									 LPOBJECT4DV1      obj,
									 int               isLocal = 0,
									 int               isLightOn = 0);

int Light_OBJECT4DV1(LPOBJECT4DV1 obj,
					 LPCAM4DV1    cam,
					 LPLIGHTV1    lights,
					 int          maxLights);

int Light_OBJECT4DV1_16(LPOBJECT4DV1 obj,
					    LPCAM4DV1    cam,
					    LPLIGHTV1    lights,
					    int          maxLights);

int Light_OBJECT4DV1_32(LPOBJECT4DV1 obj,
					    LPCAM4DV1    cam,
					    LPLIGHTV1    lights,
					    int          maxLights);

int Light_RENDERLIST4DV1(LPRENDERLIST4DV1 renderList,
						 LPCAM4DV1        cam,
						 LPLIGHTV1        lights,
						 int              maxLights);

int Light_RENDERLIST4DV1_16(LPRENDERLIST4DV1 renderList,
						    LPCAM4DV1        cam,
						    LPLIGHTV1        lights,
						    int              maxLights);

int Light_RENDERLIST4DV1_32(LPRENDERLIST4DV1 renderList,
						    LPCAM4DV1        cam,
						    LPLIGHTV1        lights,
						    int              maxLights);







#endif LIGHTANDSHADING


/** @}*/ // ScatterEngien Basic Drawing