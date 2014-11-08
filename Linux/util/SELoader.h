#ifndef SELOADER_H
#define SELOADER_H

#define VERTEX_FLAGS_INVERT_X             1
#define VERTEX_FLAGS_INVERT_Y             2
#define VERTEX_FLAGS_INVERT_Z             4
#define VERTEX_FLAGS_SWAP_YZ              8
#define VERTEX_FLAGS_SWAP_XZ              16
#define VERTEX_FLAGS_SWAP_XY              32
#define VERTEX_FLAGS_INVERT_WINDING_ORDER 64

#define SELOADER_STRIP_EMPTY_LINES          1
#define SELOADER_LEAVE_EMPTY_LINES          2
#define SELOADER_STRIP_WS_ENDS              4   //strip ws space at ends of line
#define SELOADER_LEAVE_WS_ENDS              8
#define SELOADER_STRIP_COMMENTS             16
#define SELOADER_LEAVE_COMMENTS             32

#define SELOADER_BUFFER_SIZE                256
#define SELOADER_MAX_COMMENT                16

#define SELOADER_DEFAULT_COMMENT            "#"

#define PATTERN_TOKEN_FLOAT               'f'
#define PATTERN_TOKEN_INT                 'i'
#define PATTERN_TOKEN_STRING              's'
#define PATTERN_TOKEN_LITERAL             '\''

#define PATTERN_STATE_INIT                0
#define PATTERN_STATE_RESTART             1
#define PATTERN_STATE_FLOAT               2
#define PATTERN_STATE_INT                 3
#define PATTERN_STATE_LITERAL             4
#define PATTERN_STATE_STRING              5
#define PATTERN_STATE_NEXT                6
#define PATTERN_STATE_MATCH               7
#define PATTERN_STATE_END                 8

#define PATTERN_MAX_ARGS                  16
#define PATTERN_BUFFER_SIZE               256


class SELoader
{
public:
	SELoader();
	~SELoader();
	
public:
	int Reset();
	int Open(char* filename);
	int Close();
	char *Getline(int mode);
	int SetComment(char* string);
	int PatternMatch(char* string, char* pattern, ...);

public:

	FILE* file;
	char buffer[SELOADER_BUFFER_SIZE];
	int  length;
	int  numLines;
	char comment[SELOADER_MAX_COMMENT];

	char  pStrings[PATTERN_MAX_ARGS][PATTERN_BUFFER_SIZE];
	int   numPStrings;
	float pFloats[PATTERN_MAX_ARGS];
	int   numPFloats;
	int   pInts[PATTERN_MAX_ARGS];
	int   numPInts;
};

typedef SELoader *LPSELoader;


char* StringDel_StartWS(char* string);

char* StringDel_EndWS(char* string);

int StripChars(char* in_string, char* out_string, char* strip_chars, int case_on = 1);
int ReplaceChars(char* in_string, char* out_string, char* replace_chars, char replaceChar, int case_on = 1);

float IsFloat(char* string);

int IsInt(char* string);

int Load_OBJECT4D_3DSASC(LPOBJECT4D obj,
						   char*        filename,
						   LPVECTOR4D   scale,
						   LPVECTOR4D   postion,
						   LPVECTOR4D   rotation,
						   int          flags);

#endif