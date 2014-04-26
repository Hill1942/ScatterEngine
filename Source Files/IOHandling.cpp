#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <Windows.h>
#include <ddraw.h>


#include "BasicDrawEngine.h"
#include "MathEngine.h"
#include "3DPipeLine.h"
#include "IOHandling.h"

/*

int Open_Error_File(char* filename, FILE* fp_override = NULL)
{

}

int Close_Error_File()
{

}
int Write_Error(char* string, ...)
{

}

*/

CParser_v1::CParser_v1()
{
	file = NULL;
	Reset();
}

CParser_v1::~CParser_v1()
{
	Reset();
}

int CParser_v1::Reset()
{
	if (file)
		fclose(file);
	file = NULL;

	memset(buffer, 0, sizeof(buffer));
	length   = 0;
	numLines = 0;

	strcpy(comment, PARSER_DEFAULT_COMMENT);

	return 1;
}

int CParser_v1::Open(char* filename)
{
	Reset();
	if ((file = fopen(filename, "r")) != NULL)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CParser_v1::Close()
{
	return Reset();
}

int CParser_v1::SetComment(char* string)
{
	if (strlen(string) < PARSER_MAX_COMMENT)
	{
		strcpy(comment, string);
		return 1;
	}
	else
		return 0;
}

char* CParser_v1::Getline(int mode)
{
	char* string;

	if (file)
	{
		if (mode & PARSER_STRIP_EMPTY_LINES)
		{
			while (1)
			{
				if ((string = fgets(buffer, PARSER_BUFFER_SIZE, file)) == NULL)
					break;

				int slength = strlen(string);
				int sindex  = 0;
				while (isspace(string[sindex]))
					sindex++;
				if ((slength - sindex ) > 0)
				{
					memmove((void*)buffer, 
						    (void*)&string[sindex], 
							(slength - sindex) + 1);
					string = buffer;
					slength = strlen(string);
					if (mode & PARSER_STRIP_COMMENTS)
					{
						char* commentStr = strstr(string, comment);

						if (commentStr == NULL)
							break;
						int cindex = (int)(commentStr - string);

						if (cindex == 0)  //This line is a comment
							continue;
						else
						{
							commentStr[0] = 0;
							break;
						}
					}
					break;
				}
			}
		}
		else
		{
			string = fgets(buffer, PARSER_BUFFER_SIZE, file);
		}

		if (string)
		{
			numLines++;
			if (mode & PARSER_STRIP_WS_ENDS)
			{
				StringDel_StartWS(buffer);
				StringDel_EndWS(buffer);
			}
			length = strlen(buffer);
			return string;
		}
		else
		{
			return NULL;   //NO string in file
		}
	}
	else
	{
		return NULL;  // NO file
	}
}

int CParser_v1::PatternMatch(char* string, char* pattern, ...)
{
	char tokenType[PATTERN_MAX_ARGS];
	char tokenString[PATTERN_MAX_ARGS][PATTERN_BUFFER_SIZE];
	char tokenOperator[PATTERN_MAX_ARGS];
	int  tokenNumric[PATTERN_MAX_ARGS];

	char buffer[PARSER_BUFFER_SIZE];

	if (!string || strlen(string) == 0
		|| (!pattern || strlen(pattern) == 0))
		return 0;

	strcpy(buffer, string);

	int tokenStart     = 0;
	int tokenEnd       = 0;
	int tokenRestart   = 0;
	int tokenFirstPass = 0;
	int tokenNum       = 0;

	while (1)
	{
		while (isspace(pattern[tokenStart]))
			tokenStart++;

		if (tokenStart >= strlen(pattern))
			break;
		if (pattern[tokenStart] == '[')
		{
			switch (pattern[tokenStart + 1])		
			{
			case PATTERN_TOKEN_FLOAT:
				{
					if (pattern[tokenStart + 2] != ']')
						return 0;
					tokenStart += 3;
					tokenType[tokenNum]     = PATTERN_TOKEN_FLOAT;
					strcpy(tokenString[tokenNum], "");
					tokenOperator[tokenNum] = 0;
					tokenNumric[tokenNum]   = 0;
					tokenNum++;
#ifdef PARSER_DEBUG_ON
					printf("\nFound Float token");
#endif
				}
				break;
			case PATTERN_TOKEN_INT:
				{
					if (pattern[tokenStart + 2] != ']')
						return 0;
					tokenStart += 3;
					tokenType[tokenNum]     = PATTERN_TOKEN_INT;
					strcpy(tokenString[tokenNum], "");
					tokenOperator[tokenNum] = 0;
					tokenNumric[tokenNum]   = 0;
					tokenNum++;
#ifdef PARSER_DEBUG_ON
					printf("\nFound Int token");
#endif
				}
				break;
			case PATTERN_TOKEN_LITERAL:
				{
					tokenStart += 2;
					tokenEnd    = tokenStart;
					while (pattern[tokenEnd] != PATTERN_TOKEN_LITERAL)
						tokenEnd++;
					memcpy(tokenString[tokenNum],
						   &pattern[tokenStart],
						   (tokenEnd - tokenStart));
					tokenString[tokenNum][(tokenEnd - tokenStart)] = 0;
					tokenType[tokenNum] = PATTERN_TOKEN_LITERAL;
					tokenOperator[tokenNum] = 0;
					tokenNumric[tokenNum]   = 0;
#ifdef PARSER_DEBUG_ON
					printf("\nFound Literal token = %s", tokenString[tokenNum]);
#endif
					tokenStart              = tokenEnd + 2;
					tokenNum++;
				}
				break;
			case PATTERN_TOKEN_STRING:
				{
					if (pattern[tokenStart + 2] == '=' ||
						pattern[tokenStart + 2] == '>' ||
						pattern[tokenStart + 2] == '<' )
					{
						tokenEnd = tokenStart + 3;
						while (isdigit(pattern[tokenEnd]))
							tokenEnd++;
						if (pattern[tokenEnd] != ']')
							return 0;
						memcpy(buffer,
							   &pattern[tokenStart + 3], 
							   (tokenEnd - tokenStart));
						buffer[tokenEnd - tokenStart] = 0;

						tokenType[tokenNum] = PATTERN_TOKEN_STRING;
						strcpy(tokenString[tokenNum], "");
						tokenOperator[tokenNum] = pattern[tokenStart + 2];
						tokenNumric[tokenNum] = atoi(buffer);
					}
					else
						return 0;
#ifdef PARSER_DEBUG_ON
					printf("\nFound String token, comparator: %c, characters: %d", 
						tokenOperator[tokenNum], tokenNumric[tokenNum]);
#endif
					tokenStart = tokenEnd + 1;
					tokenNum++;
				}
				break;

			default:
				break;
			}
		}  // end if

		if (tokenStart >= strlen(pattern))
			break;
	}  // end while

#ifdef PARSER_DEBUG_ON
	printf("\nstring to parse: %s", string);
	printf("\nPattern to scan for: %s", pattern);
	printf("\number of tokens found: %d", tokenNum);
#endif

	int patternState = PATTERN_STATE_INIT;
	int currToken    = 0;
	char token[PATTERN_BUFFER_SIZE];

	while (1)
	{
		switch (patternState)
		{
		case PATTERN_STATE_INIT:
			{
				strcpy(buffer, string);
				tokenStart     = 0;
				tokenEnd       = 0;
				tokenRestart   = 0;
				tokenFirstPass = 1;
				currToken      = 0;

				numPInts = numPFloats = numPStrings = 0;
				patternState = PATTERN_STATE_RESTART;
			}
			break;

		case PATTERN_STATE_RESTART:
			{
				currToken      = 0;
				tokenFirstPass = 1;
				if (tokenEnd >= strlen(buffer))
					return 0;
				tokenStart = tokenEnd = tokenRestart;
				patternState = PATTERN_STATE_NEXT;
			}
			break;

		case PATTERN_STATE_NEXT:
			{
				if (currToken >= tokenNum)
				{
					patternState = PATTERN_STATE_MATCH;
				}
				else
				{
					if (tokenEnd >= strlen(buffer))
						return 0;
					tokenStart = tokenEnd;
					while (isspace(buffer[tokenStart]))
						tokenStart++;
					tokenEnd = tokenStart;
					while (!isspace(buffer[tokenEnd]) && tokenEnd < strlen(buffer))
						tokenEnd++;
					memcpy(token, &buffer[tokenStart] , tokenEnd  - tokenStart);
					token[tokenEnd - tokenStart] = 0;
					if (strlen(token) == 0)
						return 0;

					if (tokenFirstPass)
					{
						tokenFirstPass = 0;
						tokenRestart = tokenEnd;
					}

					switch (tokenType[currToken])
					{
					case PATTERN_TOKEN_FLOAT:
						{
							patternState = PATTERN_STATE_FLOAT;
						}
						break;
					case PATTERN_TOKEN_INT:
						{
							patternState = PATTERN_STATE_INT;
						}
						break;
					case PATTERN_TOKEN_STRING:
						{
							patternState = PATTERN_STATE_STRING;
						}
						break;
					case PATTERN_TOKEN_LITERAL:
						{
							patternState = PATTERN_STATE_LITERAL;
						}
						break;
					default:
						break;
					}
				}// end else

			}
			break;

		case PATTERN_STATE_FLOAT:
			{
				float f = IsFloat(token);
				if (f != FLT_MIN)
				{
					pFloats[numPFloats] = f;
					numPFloats++;
					currToken++;
					patternState = PATTERN_STATE_NEXT;
				}
				else
				{
					patternState = PATTERN_STATE_RESTART;
				}
			}
			break;

		case PATTERN_STATE_INT:
			{
				int i = IsInt(token);
				if (i != INT_MIN)
				{
					pInts[numPInts] = i;
					numPInts++;
					currToken++;
					patternState = PATTERN_STATE_NEXT;
				}
				else
				{
					patternState = PATTERN_STATE_RESTART;
				}
			}
			break;

		case PATTERN_STATE_LITERAL:
			{
				if (strcmp(token, tokenString[currToken]) == 0)
				{
					strcpy(pStrings[numPStrings], token);
					numPStrings++;
					currToken++;
					patternState = PATTERN_STATE_NEXT;
				}
				else
				{
					patternState = PATTERN_STATE_RESTART;
				}
			}
			break;

		case PATTERN_STATE_STRING:
			{
				switch (tokenOperator[currToken])
				{
				case '=':
					{
						if (strlen(token) == tokenNumric[currToken])
						{
							strcpy(pStrings[numPStrings], token);
							numPStrings++;
							currToken++;
							patternState = PATTERN_STATE_NEXT;
						}
						else
						{
							patternState = PATTERN_STATE_RESTART;
						}
					}
					break;
				case '<':
					{
						if (strlen(token) < tokenNumric[currToken])
						{
							strcpy(pStrings[numPStrings], token);
							numPStrings++;
							currToken++;
							patternState = PATTERN_STATE_NEXT;
						}
						else
						{
							patternState = PATTERN_STATE_RESTART;
						}
					}
					break;
				case '>':
					{
						if (strlen(token) > tokenNumric[currToken])
						{
							strcpy(pStrings[numPStrings], token);
							numPStrings++;
							currToken++;
							patternState = PATTERN_STATE_NEXT;
						}
						else
						{
							patternState = PATTERN_STATE_RESTART;
						}
					}
					break;
				default:
					break;
				}
			}
			break;

		case PATTERN_STATE_MATCH:
			{
#ifdef PARSER_DEBUG_ON
				printf("\nPattern: %s matched", pattern);
#endif
				return 1;
			}
			break;
		case PATTERN_STATE_END:
			{
			}
			break;
		default:
			break;
		}  // end pattern state switch
	} // end while
}

char* StringDel_StartWS(char* string)
{
	int index = 0;
	int length = strlen(string);
	if (!string || length == 0)
		return string;

	index = 0;
	while (isspace(string[index] && index < length))
		string[index++] = 0;

	return 0;
}

char* StringDel_EndWS(char* string)
{
	int index = 0;
	int length = strlen(string);
	if (!string || length == 0)
		return string;

	index = length - 1;
	while (isspace(string[index] && index >= 0))
		string[index--] = 0;

	return 0;
}

int StripChars(char* in_string, char* out_string, char* strip_chars, int case_on)
{
	int index_in  = 0;
	int index_out = 0;
	int index_strip;
	int length_strip = strlen(strip_chars);
	int strip_char_num = 0;

	if (in_string == 0 || out_string == 0 || strlen(in_string) == 0)
		return 0;

	if (strip_chars == 0 || strlen(strip_chars) == 0)
	{
		strcpy(out_string, in_string);
		return 0;
	}

	if (case_on)
	{
		while (in_string[index_in])
		{
			for (index_strip = 0; index_strip < length_strip; index_strip++)
			{
				if (in_string[index_in] == strip_chars[index_strip])
				{
					index_in++;
					strip_char_num++;
					break;
				}
			}
			if (index_strip >= length_strip)
			{
				out_string[index_out] = in_string[index_in];
				index_out++;
				index_in++;
			}
		}
	}
	else
	{
		while (in_string[index_in])
		{
			for (index_strip = 0; index_strip < length_strip; index_strip++)
			{
				if (toupper(in_string[index_in]) == toupper(strip_chars[index_strip]))
				{
					index_in++;
					strip_char_num++;
					break;
				}
			}
			if (index_strip >= length_strip)
			{
				out_string[index_out] = in_string[index_in];
				index_out++;
				index_in++;
			}
		}
	}
	out_string[index_out] = 0;
	return strip_char_num;
}

int ReplaceChars(char* in_string, 
				 char* out_string, 
				 char* replace_chars, 
				 char replaceChar,
				 int case_on)
{
	int index_in  = 0;
	int index_out = 0;
	int index_replace;
	int length_replace = strlen(replace_chars);
	int replace_char_num = 0;

	if (in_string == 0 || out_string == 0 || strlen(in_string) == 0)
		return 0;

	if (replace_chars == 0 || strlen(replace_chars) == 0)
	{
		strcpy(out_string, in_string);
		return 0;
	}

	if (case_on)
	{
		while (in_string[index_in])
		{
			for (index_replace = 0; index_replace < length_replace; index_replace++)
			{
				if (in_string[index_in] == replace_chars[index_replace])
				{
					out_string[index_out] = replaceChar;
					index_out++;
					index_in++;
					replace_char_num++;
					break;
				}
			}
			if (index_replace >= length_replace)
			{
				out_string[index_out] = in_string[index_in];
				index_out++;
				index_in++;
			}
		}
	}
	else
	{
		while (in_string[index_in])
		{
			for (index_replace = 0; index_replace < length_replace; index_replace++)
			{
				if (toupper(in_string[index_in]) == toupper(replace_chars[index_replace]))
				{
					out_string[index_out] = replaceChar;
					index_out++;
					index_in++;
					replace_char_num++;
					break;
				}
			}
			if (index_replace >= length_replace)
			{
				out_string[index_out] = in_string[index_in];
				index_out++;
				index_in++;
			}
		}
	}
	out_string[index_out] = 0;
	return replace_char_num;
}

float IsFloat(char* string)
{
	char* str = string;
	while (isspace(*str))
		str++;
	if (*str == '+' || *str == '-')
		str++;
	while (isdigit(*str))
		str++;
	if (*str == '.')
	{
		str++;
		while (isdigit(*str))
			str++;
	}
	if (*str == 'e' || *str == 'E' || *str == 'd' || *str == 'D')
	{
		str++;
		if (*str == '+' || *str == '-')
			str++;
		while (isdigit(*str)) 
			str++;
	}
	if (strlen(string) == (int)(str - string))
		return (atof(string));
	else
		return FLT_MIN;
}


/*************************************************************


***************************************************************/
int IsInt(char* string)
{
	char* str = string;
	while (isspace(*str))
		str++;
	if (*str == '+'|| *str == '-')
		str++;
	while (isdigit(*str))
		str++;

	if (strlen(string) == (int)(str - string))
		return (atoi(string));
	else
		return INT_MIN;
}


int Load_OBJECT4D_3DSASC(LPOBJECT4D obj,
						   char*        filename,
						   LPVECTOR4D   scale,
						   LPVECTOR4D   postion,
						   LPVECTOR4D   rotation,
						   int          flags)
{
	CParser_v1 parser;

	char seperators[16];
	char tokenBuffer[256];
	char* token;

	int r;
	int g;
	int b;

	memset(obj, 0, sizeof(OBJECT4D));

	obj->state = OBJECT4D_STATE_ACTIVE | OBJECT4D_STATE_VISIBLE;

	if (postion)
	{
		obj->worldPos.x = postion->x;
		obj->worldPos.y = postion->y;
		obj->worldPos.z = postion->z;
		obj->worldPos.w = postion->w;
	}
	else
	{
		obj->worldPos.x = 0;
		obj->worldPos.y = 0;
		obj->worldPos.z = 0;
		obj->worldPos.w = 1;
	}

	if (!parser.Open(filename))
	{
		MessageBox(NULL, "Couldn't open .asc file", "Error", MB_OK);
		return 0;
	}
	while (1)
	{
		if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
		{
			MessageBox(NULL, "No name in .asc file", "Error", MB_OK);
			return 0;
		}
		if (parser.PatternMatch(parser.buffer, "['Named'] ['object:']"))
		{
			strcpy(tokenBuffer, parser.buffer);
			strcpy(seperators, "\"");
			strtok(tokenBuffer, seperators);
			token = strtok(NULL, seperators);
			strcpy(obj->name, token);
			break;
		}
	}

	while (1)
	{
		if (!parser.Getline(PARSER_STRIP_COMMENTS | PARSER_STRIP_WS_ENDS))
		{
			MessageBox(NULL, "No Tri-mesh line in .asc file", "Error", MB_OK);
			return 0;
		}
		if (parser.PatternMatch(parser.buffer, "['Tri-mesh,'] ['Vertices:'] [i] ['Faces:'] [i]"))
		{
			obj->numVertics = parser.pInts[0];
			obj->numPolys   = parser.pInts[1];
			//MessageBox(NULL, "No Tri-mesh line in .asc file", "Error", MB_OK);
			break;
		}
	}

	while (1)
	{
		if (!parser.Getline(PARSER_STRIP_COMMENTS | PARSER_STRIP_WS_ENDS))
		{
			MessageBox(NULL, "No 'Vertex list' line in .asc file", "Error", MB_OK);
			return 0;
		}
		if (parser.PatternMatch(parser.buffer, "['Vertex'] ['list:']"))
		{
			break;
		}
	}

	for (int vertex = 0; vertex < obj->numVertics; vertex++)
	{
		while (1)
		{
			if (!parser.Getline(PARSER_STRIP_COMMENTS | PARSER_STRIP_WS_ENDS))
			{
				MessageBox(NULL, "Vertex list end abruptly", "Error", MB_OK);
				return 0;
			}
			StripChars(parser.buffer, parser.buffer, ":XYZ");
			if (parser.PatternMatch(parser.buffer, "['Vertex'] [i] [f] [f] [f]"))
			{
				obj->vLocalList[vertex].x = parser.pFloats[0];
				obj->vLocalList[vertex].y = parser.pFloats[1];
				obj->vLocalList[vertex].z = parser.pFloats[2];
				obj->vLocalList[vertex].w = 1;
			

				float temp;
			
				if (flags & VERTEX_FLAGS_INVERT_X)
					obj->vLocalList[vertex].x = -obj->vLocalList[vertex].x;

				if (flags & VERTEX_FLAGS_INVERT_Y)
					obj->vLocalList[vertex].y = -obj->vLocalList[vertex].y;

				if (flags & VERTEX_FLAGS_INVERT_Z)
					obj->vLocalList[vertex].z = -obj->vLocalList[vertex].z;

				if (flags & VERTEX_FLAGS_SWAP_XY)
					SWAP(obj->vLocalList[vertex].x, obj->vLocalList[vertex].y, temp);

				if (flags & VERTEX_FLAGS_SWAP_YZ)
					SWAP(obj->vLocalList[vertex].y, obj->vLocalList[vertex].z, temp);

				if (flags & VERTEX_FLAGS_SWAP_XZ)
					SWAP(obj->vLocalList[vertex].x, obj->vLocalList[vertex].z, temp);
				
				if (scale)
				{
					obj->vLocalList[vertex].x *= scale->x;
					obj->vLocalList[vertex].y *= scale->y;
					obj->vLocalList[vertex].z *= scale->z;
				}
				break;
			}
		}  // end while
	}  // end for

	Compute_OBJECT4D_Radius(obj);

	while (1)
	{
		if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
		{
			MessageBox(NULL, "Face list not found", "Error", MB_OK);
			return 0;
		}
		if (parser.PatternMatch(parser.buffer, "['Face'] ['list:']"))
		{
			//MessageBox(NULL, "Face list found", "OK", MB_OK);
			break;
		}
	}

	for (int poly = 0; poly < obj->numPolys; poly++)
	{
		while (1)
		{
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				MessageBox(NULL, "Face list end abruptly", "Error", MB_OK);
				return 0;
			}
			StripChars(parser.buffer, parser.buffer, ":ABC");

			if (parser.PatternMatch(parser.buffer, "['Face'] [i] [i] [i] [i]"))
			{
				if (flags & VERTEX_FLAGS_INVERT_WINDING_ORDER)
				{
					obj->polyList[poly].vert[0] = parser.pInts[3];
					obj->polyList[poly].vert[1] = parser.pInts[2];
					obj->polyList[poly].vert[2] = parser.pInts[1];
				}
				else
				{
					obj->polyList[poly].vert[0] = parser.pInts[1];
					obj->polyList[poly].vert[1] = parser.pInts[2];
					obj->polyList[poly].vert[2] = parser.pInts[3];
				}
				obj->polyList[poly].vList = obj->vLocalList;
				break;
			}
		}  // end while

		while (1)
		{
			if (!parser.Getline(PARSER_STRIP_EMPTY_LINES | PARSER_STRIP_WS_ENDS))
			{
				MessageBox(NULL, "Material list end abruptly", "Error", MB_OK);
				return 0;
			}
			ReplaceChars(parser.buffer, parser.buffer, ":\"rgba", ' ');
			if (parser.PatternMatch(parser.buffer, "[i] [i] [i]"))
			{
				r = parser.pInts[0];
				g = parser.pInts[1];
				b = parser.pInts[2];

				obj->polyList[poly].color = RGB24BIT_888(255, r, g, b);
				SET_BIT(obj->polyList[poly].attr, POLY4D_ATTR_SHADE_MODE_FLAT);
				obj->polyList[poly].state = POLY4D_STATE_ACTIVE;
				break;
			}
		} // end while
	} // end for poly

	return 1;
}






