

#ifndef LIGHTANDSHADING
#define LIGHTANDSHADING

/** @defgroup ScatterEngine Light and Shading
 *  @author  Yang Kaidi
 *  @version 1.0
 *  @date    2013-2014
 *  @{
 */

/** @name Macro Constant
 *  @{
 */

#define LIGHT_ATTR_AMBIENT     0x0001
#define LIGHT_ATTR_INFINITE    0x0002
#define LIGHT_ATTR_DIRECTIONAL 0x0002
#define LIGHT_ATTR_POINT       0x0004
#define LIGHT_ATTR_SPOTLIGHT1  0x0008
#define LIGHT_ATTR_SPOTLIGHT2  0x0010

#define LIGHT_STATE_ON         1
#define LIGHT_STATE_OFF        0

#define MAX_LIGHTS               8

#define MATERIAL_ATTR_2SIDED              0x0001
#define MATERIAL_ATTR_TRANSPARENT         0x0002
#define MATERIAL_ATTR_8BITCOLOR           0x0004
#define MATERIAL_ATTRRGB16               0x0008
#define MATERIAL_ATTRRGB24               0x0010
#define MATERIAL_ATTR_SHADEMODE_CONSTANT  0x0020
#define MATERIAL_ATTR_SHADEMODE_EMMISIVE  0x0020
#define MATERIAL_ATTR_SHADEMODE_FLAT      0x0040
#define MATERIAL_ATTR_SHADEMODE_GOURAUD   0x0080
#define MATERIAL_ATTR_SHADEMODE_FASTPHONG 0x0100
#define MATERIAL_ATTR_SHADEMODE_TEXTURE   0x0200

#define MATERIAL_STATE_ACTIVE             0x0001
#define MATERIAL_STATE_INACTIVE           0x0002

#define MAX_MATERIAL                      256

#define SORT_POLYLIST_AVGZ   0
#define SORT_POLYLIST_NEARZ  1
#define SORT_POLYLIST_FARZ   2

/** @}*/ // Macro Constant


/** @name Struct Define
 *  @{
 */

typedef struct ARGB_TYP
{
	union
	{
		int   argb;
		UCHAR ARGB[4];
		struct
		{
			UCHAR b, g, r, a;
		};
	};
}ARGB, *LPARGB;

typedef struct MATERIAL_TYP
{
	int    state;
	int    id;
	char   name[64];
	int    attr;

	ARGB color;
	ARGB ra;
	ARGB rd;
	ARGB rs;
	float  ka;
	float  kd;
	float  ks;
	float  power;

	char   texturFile[80];
	BITMAP_IMAGE texture;
}MATERIAL, *LPMATERIAL;

typedef struct LIGHT_TYP
{
	int state;
	int id;
	int attr;

	ARGB ambient;
	ARGB diffuse;
	ARGB specular;

	POINT4D  pos;
	VECTOR4D dir;

	float kc;
	float kl;
	float kq;
	
	float spotInnerTheta;
	float spotOuterTheta;

	float pf;
}LIGHT, *LPLIGHT;

/** @}*/ // Struct Define


void Draw_OBJECT4D_Solid(LPOBJECT4D obj, UCHAR* buffer, int lPitch);

void Draw_RENDERLIST4D_Solid(LPRENDERLIST4D renderList, UCHAR* buffer, int lPitch);

void Draw_OBJECT4D_Solid16(LPOBJECT4D obj, UCHAR* buffer, int lPitch);

void Draw_RENDERLIST4D_Solid16(LPRENDERLIST4D renderList, UCHAR* buffer, int lPitch);

void Draw_OBJECT4D_Solid32(LPOBJECT4D obj, UCHAR* buffer, int lPitch);

void Draw_RENDERLIST4D_Solid32(LPRENDERLIST4D renderList, UCHAR* buffer, int lPitch);

/** Initialize a light, version 1
 *  @param[in] index the index of the light, based on zero
 *  @param[in] _state the state of the light
 *  @param[in] _attr the attribute of the light
 *  @param[in] _ambient
 *  @param[in] lPitch the video card pitch
 *  @return the symbol indicating success
 *  --1 success
 */
int Init_Light(int        index,
			     int        _state,
			     int        _attr,
			     ARGB     _ambient,
			     ARGB     _diffuse,
			     ARGB     _specular,
			     LPPOINT4D  _pos,
			     LPVECTOR4D _dir,
			     float      _kc,
			     float      _kl,
			     float      _kq,
			     float      _spotInnerTheta,
			     float      _spotOuterTheta,
			     float      _pf);

int Reset_Lights();

int Reset_Materials();

int Insert_OBJECT4D_RENDERLIST4DV2(LPRENDERLIST4D  renderList,
									 LPOBJECT4D      obj,
									 int               isLocal = 0,
									 int               isLightOn = 0);

int Light_OBJECT4D(LPOBJECT4D obj,
					 LPCAM4D    cam,
					 LPLIGHT    lights,
					 int          maxLights);

int Light_OBJECT4D_16(LPOBJECT4D obj,
					    LPCAM4D    cam,
					    LPLIGHT    lights,
					    int          maxLights);

int Light_OBJECT4D_32(LPOBJECT4D obj,
					    LPCAM4D    cam,
					    LPLIGHT    lights,
					    int          maxLights);

int Light_RENDERLIST4D(LPRENDERLIST4D renderList,
						 LPCAM4D        cam,
						 LPLIGHT        lights,
						 int              maxLights);

int Light_RENDERLIST4D_16(LPRENDERLIST4D renderList,
						    LPCAM4D        cam,
						    LPLIGHT        lights,
						    int              maxLights);

int Light_RENDERLIST4D_32(LPRENDERLIST4D renderList,
						    LPCAM4D        cam,
						    LPLIGHT        lights,
						    int              maxLights);

int Compare_AvgZ_POLYF4D(const void* arg1, const void* arg2);

int Compare_NearZ_POLYF4D(const void* arg1, const void* arg2);

int Compare_FarZ_POLYF4D(const void* arg1, const void* arg2);

void Sort_RENDERLIST4D(LPRENDERLIST4D rendList, int method = SORT_POLYLIST_AVGZ);



/** @}*/ // ScatterEngien Basic Drawing

#endif LIGHTANDSHADING


