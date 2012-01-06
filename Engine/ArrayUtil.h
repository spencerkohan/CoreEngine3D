/*
 *  ArrayUtil.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#ifndef __ARRAY_UTIL_H
#define __ARRAY_UTIL_H

#include "MathTypes.h"

/* This macro allows it to be accessed as if it was 2D. */
#define ARRAY2D(THEARRAY, X, Y, WIDTH) ((THEARRAY)[(X) + (Y) * (WIDTH)])

template <typename T>
void Array_InsertionSort(T* array, s32 length, bool (*compareFunc)(const T& lhs, const T& rhs))
{
	//INSERTION SORT
	
	// Sort an array of integers
	for (s32 i=0; i < length; ++i)
	{
		// Insert a[i] into the sorted sublist
		const T v = array[i];
		
		s32 j;
		for (j = i - 1; j > -1; --j)
		{
			if (compareFunc(array[j],v))
			{
				break;
			}
			
			array[j + 1] = array[j];
		}
		
		array[j + 1] = v;			
	}
}

//void Array_Shuffle(s32* array, s32 length);   //TODO: implement

#endif
