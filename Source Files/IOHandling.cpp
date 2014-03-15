#include <iostream>
#include <Windows.h>

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

