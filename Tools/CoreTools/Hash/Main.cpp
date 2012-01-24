// Hash.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "conio.h"

#include "Engine\Hash.h"

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc > 1)
	{
		printf("const u32 myHash = %u; //%s\n",Hash(argv[1]),argv[1]);
	}
	else
	{
		printf("ERROR: no string specified...\n");
	}

#ifdef _DEBUG
	getch();
#endif

	return 0;
}

