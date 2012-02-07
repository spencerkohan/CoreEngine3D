#ifndef __PHYSICS
#define __PHYSICS

/*
 *  Physics.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 8/31/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "MathUtil.h"

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

//1D springs
void Spring_Update(Spring* spring, f32 timeElapsedSq);

//3D springs

/*** SPRING PHYSICS ***/
/* Here's what you do:
 1)	Create a bunch of attach points spaced out the distance they will be from
 each other in the physical rope.
 2)	Create a bunch of spring nodes that have pointers to the attach points.
 3)	You have to keep a previousDT in your moby so just set it to the same as DT the first frame
 and then correctly keep a previousDT every frame after that.
 4)	Make some variables you will use for passing into the inverse mass (1/mass) parameter of your
 GAMEUTIL_Physics_UpdateAttachPoint later.
 5)  Zero out all forces on your attach points.
 6)	Calculate DT/PreviousDT once and call GAMEUTIL_Physics_UpdateSpringNode on all your spring nodes every frame.
 Note there are two versions of the function and only one of them enforces a max length.
 7)	Right after this, call GAMEUTIL_Physics_UpdateAttachPoint on all your attach points.
 8)	*OPTIONAL* Enforce any extra constraints you want on the points.  Keep in mind that if you only
 move the position and not the old position, you're effectively adding an implied velocity
 because of the Verlet integration.
 9)	ENJOY YOUR NEW ROPE!
 
 Any questions?  Talk to Jody (Me!)
 
 Extra hints:
 -Scale your restLength and maxLength equally to change the length of your rope.
 -Use a damped spring to change the scale over time so you don't jerk the rope.
 -If you want any point to not move, simply don't call GAMEUTIL_Physics_UpdateAttachPoint on it.
 It's useful for hanging a rope from something.
 -I don't suggest hanging a rope from more than one point unless you make multiple ropes
 and start them from each end and connect them.  GOOD LUCK IF YOU TRY THIS.
 */

struct Physics_AttachPoint
{
	vec3 lastPosition;	//previous position of the point (set to same as position on init)
	vec3 position;		//current position of the point (set on init)
	vec3 force;			//force on the point.  Set to zero every frame and on init.
};


struct Physics_SpringNode
{
	Physics_AttachPoint* pointA;
	Physics_AttachPoint* pointB;
};

void Physics_UpdateSpringNode(Physics_SpringNode* springNode, f32 restLength, f32 springK);
void Physics_UpdateSpringNode_Clamped(Physics_SpringNode* springNode, f32 restLength, f32 maxLength, f32 springK);
void Physics_UpdateAttachPoint(Physics_AttachPoint* attachPoint, f32 inverseMass, const vec3* gravity, f32 DTSq, f32 DTOverPreviousDT, f32 damping);

#endif
