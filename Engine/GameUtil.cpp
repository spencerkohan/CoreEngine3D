/*
 *  GameUtil.c
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 1/6/11.
 *  Copyright 2011 GameJelly Online. All rights reserved.
 *
 */

#include "GameUtil.h"
#include <math.h>
#include "MathUtil.h"
#include "GameDefines.h"

//*****************************************************************************
// Creator: JAM
// Calculates velocity needed for simple ballistic motion so that the object
// goes from pStartPos to pEndPos with the given gravity. The top of the arc
// is at MAX(in_startPos->y,in_endPos->y) + in_jumpHeight
// Note: Takes a positive gravity.
// Note: If you count up the timeElapsed, the jump will end when
// (totalTimeElapsed == out_totalJumpTime)
//
// Note: your update code should look like this:
// VEC_AddScaledXYZ(&pPvars->boulderShrapnel.pMoby->matrix.pos,&pContent->jumpVelocity,G_dtMultiplier);
// pContent->jumpVelocity.y -= gravity*G_dtMultiplier;
// pContent->elapsedJumpTime += timeElapsed;
//
// Note: For extra accuracy with landing, make sure you subtract off your timeElapsed when/if it
// goes above out_totalJumpTime
//
// Returns NOTHING!
//*****************************************************************************
void GU_LaunchToPoint(vec3* out_jumpVel, f32* out_totalJumpTime, const vec3* in_startPos, const vec3* in_endPos, f32 in_jumpHeight, f32 in_gravity)
{
	const f32 highestY = MaxF(in_startPos->y, in_endPos->y);
	const f32 apexY = highestY + in_jumpHeight;
	
	const f32 s1 = apexY - in_startPos->y;
	const f32 s2 = apexY - in_endPos->y;
	const f32 s3 = in_endPos->z-in_startPos->z;
	const f32 s4 = in_endPos->x-in_startPos->x;
	
	const f32 t1 = sqrtf(2.0f * s1 / in_gravity);
	const f32 t2 = sqrtf(2.0f * s2 / in_gravity);
	
	const f32 totalTime = t1 + t2;
	
	
	if(out_jumpVel)
	{
		out_jumpVel->y = in_gravity * t1;
		out_jumpVel->z = s3/totalTime;
		out_jumpVel->x = s4/totalTime;
	}
	
	if(out_totalJumpTime)
	{
		*out_totalJumpTime = totalTime;
	}
}

//*****************************************************************************
//*****************************************************************************
void GU_InsertPositiveValueAsBits(u32* pOriginalNumber, u32 value, u32 bitPosition0to31, u32 numBitsToWrite)
{
	u32 insertNumberShifted = value << (32-bitPosition0to31-numBitsToWrite);
	*pOriginalNumber |= insertNumberShifted;
}
