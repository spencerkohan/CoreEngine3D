#include "CoreDebug.h"
#include <cstdio>

#if defined (PLATFORM_WIN)
#include <Windows.h>
#endif

void COREDEBUG_PrintDebugMessage(const char* debugMessage, ...)
{
	//HACK: this will crash one day and you will die from memory stomps
	static char coreDebugMessageBuffer[2048];
	va_list args;
	va_start(args,debugMessage);
	sprintf(coreDebugMessageBuffer,debugMessage,args);

	const size_t strLen = strlen(coreDebugMessageBuffer);
	sprintf(&coreDebugMessageBuffer[strLen],"\n");

#if defined(PLATFORM_IOS) || defined(PLATFORM_OSX)
	printf("%s",coreDebugMessageBuffer);
#endif

#if defined(PLATFORM_WIN)
	OutputDebugString(coreDebugMessageBuffer);
#endif
}