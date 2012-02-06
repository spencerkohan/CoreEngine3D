// Hash.cpp : Defines the entry point for the console application.
//

#include <cstdio>

#if defined (PLATFORM_WIN)
#include "conio.h"
#endif

#include "Engine/Hash.h"

int main(int argc, char* argv[])
{
	if(argc > 1)
	{
		printf("const u32 myHash = %u; //%s\n",Hash(argv[1]),argv[1]);
	}
	else
	{
		printf("ERROR: no string specified...\n");
	}

#if defined (PLATFORM_WIN)
#ifdef _DEBUG
	getch();
#endif
#endif

	return 0;
}

