#ifndef GAMEUTIL_H__
#define GAMEUTIL_H__

/*
 *  GameUtil.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 1/6/11.
 *  Copyright 2011 GameJelly Online. All rights reserved.
 *
 */

#include "MathTypes.h"

void GU_LaunchToPoint(vec3* out_jumpVel, f32* out_totalJumpTime, const vec3* in_startPos, const vec3* in_endPos, f32 in_jumpHeight, f32 in_gravity);
void GU_InsertPositiveValueAsBits(u32* pOriginalNumber, u32 value, u32 bitPosition0to31, u32 numBitsToWrite);

#endif
