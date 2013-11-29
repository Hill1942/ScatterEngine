#ifndef IO_HANDLING
#define IO_HANDLING

#include <io.h>
#include <iostream>
#include <Windows.h>


int Open_Error_File(char* filename, FILE *fp_override = NULL);
int Close_Error_File();
int Write_Error(char* string, ...);






#endif

