/*
 *  ArrayUtil.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "MathTypes.h"

/* This macro allows it to be accessed as if it was 2D. */
#define ARRAY2D(THEARRAY, X, Y, WIDTH) ((THEARRAY)[(X) + (Y) * (WIDTH)])

void Array_InsertionSort(void** array, s32 length, s32 (*compareFunc)(const void* lhs, const void* rhs));
//void Array_Shuffle(s32* array, s32 length);   //TODO: implement
