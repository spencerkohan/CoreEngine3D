#include "CoreDebug.h"
#include <cstdio>
#include <cstdarg>
#include <cstring>

#if defined (PLATFORM_WIN)
#include <Windows.h>
#endif

void COREDEBUG_PrintDebugMessage(const char* debugMessage, ...)
{
//#if defined (_DEBUG) || defined(DEBUG)


#if defined(PLATFORM_IOS) || defined(PLATFORM_OSX)
	va_list args;
	va_start(args,debugMessage);
	vprintf(debugMessage,args);
	printf("\n");
	va_end(args);
#endif

#if defined(PLATFORM_WIN)
	//HACK: this will crash one day and you will die from memory stomps
	static char coreDebugMessageBuffer[2048];
	va_list args;
	va_start(args,debugMessage);
	vsprintf(coreDebugMessageBuffer,debugMessage,args);
	va_end(args);
	
	const size_t strLen = strlen(coreDebugMessageBuffer);
	sprintf(&coreDebugMessageBuffer[strLen],"\n");
	
	OutputDebugString(coreDebugMessageBuffer);
#endif

//#endif
}