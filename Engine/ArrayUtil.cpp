/*
 *  ArrayUtil.c
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "ArrayUtil.h"

//--------------------------------------------------------------------------------------
//This function is used to sort arrays of pointers to whatever object
//--------------------------------------------------------------------------------------
void Array_InsertionSort(void** array, s32 length, s32 (*compareFunc)(const void* lhs, const void* rhs))
{
	//INSERTION SORT
	
	// Sort an array of integers
	for (s32 i=0; i < length; ++i)
	{
		// Insert a[i] into the sorted sublist
		void* v = array[i];
		 
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


//--------------------------------------------------------------------------------------
//This function is used to shuffle an array of ints.
//TODO: implement
//--------------------------------------------------------------------------------------
/*void Array_Shuffle(s32* array, s32 length)
{
    //Shuffle!
    // for i from n − 1 downto 1 do
    for(s32 i=m_blockColorBucketCount-1; i > 0; --i)
    {
        //j ← random integer with 0 ≤ j ≤ i
        int randIndex = [replayRand rand32]%(i+1);
        
        //exchange a[j] and a[i]
        s32 tempPair = m_blockColorBucket[i];
        array[i] = m_blockColorBucket[randIndex];
        array[randIndex] = tempPair;
    }
}*/
