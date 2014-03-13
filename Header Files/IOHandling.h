#ifndef IO_HANDLING
#define IO_HANDLING



/*

int Open_Error_File(char* filename, FILE *fp_override = NULL);
int Close_Error_File();
int Write_Error(char* string, ...);

*/


#define VERTEX_FLAGS_INVERT_X             1
#define VERTEX_FLAGS_INVERT_Y             2
#define VERTEX_FLAGS_INVERT_Z             4
#define VERTEX_FLAGS_SWAP_YZ              8
#define VERTEX_FLAGS_SWAP_XZ              16
#define VERTEX_FLAGS_SWAP_XY              32
#define VERTEX_FLAGS_INVERT_WINDING_ORDER 64

#define PARSER_STRIP_EMPTY_LINES          1
#define PARSER_LEAVE_EMPTY_LINES          2
#define PARSER_STRIP_WS_ENDS              4   //strip ws space at ends of line
#define PARSER_LEAVE_WS_ENDS              8
#define PARSER_STRIP_COMMENTS             16
#define PARSER_LEAVE_COMMENTS             32

#define PARSER_BUFFER_SIZE                256
#define PARSER_MAX_COMMENT                16

#define PARSER_DEFAULT_COMMENT            "#"

#define PATTERN_TOKEN_FLOAT               'f'
#define PATTERN_TOKEN_INT                 'i'
#define PATTERN_TOKEN_STRING              's"
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


class CParser_v1
{
public:
	CParser_v1();
	~CParser_v1();
	
public:
	int Reset();
	int Open(char* filename);
	int Close();
	char *Getline(int mode);
	int SetComment(char* string);
	int PatternMatch(char* string, char* pattern, ...);

public:
	FILE* fstream;
	char buffer[PARSER_BUFFER_SIZE];
	int  length;
	int  numLines;
	char comment[PARSER_MAX_COMMENT];

	char  pStrings[PATTERN_MAX_ARGS][PATTERN_BUFFER_SIZE];
	int   numPStrings;
	float pFloats[PATTERN_MAX_ARGS];
	int   numPFloats;
	int   pInts[PATTERN_MAX_ARGS];
	int   numPInts;
};

typedef CParser_v1 *LPCParse_v1;

int Load_OBJECT4DV1_3DSASC(LPOBJECT4DV1 obj,
						   char*        filename,
						   LPVECTOR4D   scale,
						   LPVECTOR4D   postion,
						   LPVECTOR4D   rotation,
						   int          flags);



#endif

