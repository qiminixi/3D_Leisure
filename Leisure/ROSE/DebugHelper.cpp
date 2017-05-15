#include <Windows.h>
#include <stdio.h>
#include <stdarg.h>
#include "DebugHelper.h"

static char msgbuffer[1024*5];

void ROSEDump_OutputWindow(const char *str, ... )
{	
	va_list ap;
	va_start( ap, str );
	vsprintf(msgbuffer, str, ap);
	va_end( ap );
	OutputDebugStr(msgbuffer);
}