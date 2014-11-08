#ifndef SELIGHTING_H
#define SELIGHTING_H


#define SORT_POLYLIST_AVGZ   0
#define SORT_POLYLIST_NEARZ  1
#define SORT_POLYLIST_FARZ   2

#define LIGHT_ATTR_AMBIENT     0x0001
#define LIGHT_ATTR_INFINITE    0x0002
#define LIGHT_ATTR_DIRECTIONAL 0x0002
#define LIGHT_ATTR_POINT       0x0004
#define LIGHT_ATTR_SPOTLIGHT1  0x0008
#define LIGHT_ATTR_SPOTLIGHT2  0x0010

#define LIGHT_STATE_ON         1
#define LIGHT_STATE_OFF        0

#define MAX_LIGHTS               8

typedef struct ARGB_TYP
{
	union
	{
		int   argb;
		unsigned char ARGB[4];
		struct
		{
			unsigned char b, g, r, a;
		};
	};
}ARGB, *LPARGB;


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


int Light_RENDERLIST4D(LPRENDERLIST4D renderList,
						 LPCAM4D        cam,
						 LPLIGHT        lights,
						 int              maxLights);


int Compare_AvgZ_POLYF4D(const void* arg1, const void* arg2);

int Compare_NearZ_POLYF4D(const void* arg1, const void* arg2);

int Compare_FarZ_POLYF4D(const void* arg1, const void* arg2);

void Sort_RENDERLIST4D(LPRENDERLIST4D rendList, int method = SORT_POLYLIST_AVGZ);


#endif