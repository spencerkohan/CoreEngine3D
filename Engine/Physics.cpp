/*
 *  Physics.c
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 8/31/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "Physics.h"
#include "GameDefines.h"

//--------------------------------------------------------------------------------------
//Update a spring using verlet integration
//--------------------------------------------------------------------------------------
void Spring_Update(Spring* spring)
{
	//TODO: use time-corrected Verlet integration
	
	f32 posMinusOld = spring->currPos-spring->oldPos;
	
	f32 springForce = spring->springK*(spring->goalPos-spring->currPos) - spring->springDamping*posMinusOld;
	f32 springAccel = springForce*ONE_OVER_FPS*ONE_OVER_FPS;
	
	spring->oldPos = spring->currPos;
	spring->currPos += posMinusOld + springAccel;
}
