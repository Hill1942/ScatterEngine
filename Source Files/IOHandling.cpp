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
	fstream = NULL;
	Reset();
}

CParser_v1::~CParser_v1()
{
	Reset();
}

int CParser_v1::Reset()
{
	if (fstream)
		fclose(fstream);
	fstream = NULL;

	memset(buffer, 0, sizeof(buffer));
	length   = 0;
	numLines = 0;

	strcpy(comment, PARSER_DEFAULT_COMMENT);

	return 1;
}

int CParser_v1::Open(char* filename)
{
	Reset();
	//if ((fstream = fopen(filename, "r")) != NULL)
}