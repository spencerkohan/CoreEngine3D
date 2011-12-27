//
//  Hash.c
//  InfiniSurv(iOS)
//
//  Created by Jody McAdams on 12/16/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "Hash.h"

//djb2 hash
u32 Hash(const char* str)
{
	u32 hash = 5381;
	u32 c;
	
	while ((c = *str++))
	{
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	}
	
	return hash;
}
