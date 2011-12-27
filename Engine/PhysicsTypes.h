#ifndef __PHYSICS_TYPES
#define __PHYSICS_TYPES

/*
 *  PhysicsTypes.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 8/31/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "MathTypes.h"

//AttachPoint
typedef struct
{
	vec3 oldPosition;
	vec3 force;
} AttachPoint;

//SpringNode
typedef struct
{
	s32 attachPointA_Idx;
	s32 attachPointB_Idx;
} SpringNode;



//Spring
typedef struct
{
	f32 springK;
	f32 springDamping;
	f32 oldPos;
	f32 currPos;
	f32 goalPos;
}Spring;

#endif
