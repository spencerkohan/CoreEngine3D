/*
 *  Physics.c
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 8/31/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "Physics.h"

//--------------------------------------------------------------------------------------
//Update a spring using verlet integration
//--------------------------------------------------------------------------------------
//1D springs
void Spring_Update(Spring* spring, f32 timeElapsedSq)
{
	//TODO: use time-corrected Verlet integration
	
	const f32 posMinusOld = spring->currPos-spring->oldPos;
	
	const f32 springForce = spring->springK*(spring->goalPos-spring->currPos) - spring->springDamping*posMinusOld;
	const f32 springAccel = springForce*timeElapsedSq;
	
	spring->oldPos = spring->currPos;
	spring->currPos += posMinusOld + springAccel;
}


//3D springs

void Physics_UpdateAttachPoint(Physics_AttachPoint* attachPoint, f32 mass, const vec3* gravity, f32 DTSq, f32 DTOverPreviousDT, f32 damping)
{
	//Time-Corrected Verlet integration
	//http://archive.gamedev.net/archive/reference/articles/article2200.html
	
	vec3 xNeg1;
	CopyVec3(&xNeg1,&attachPoint->lastPosition);
	
	//Add damping force
	vec3 dampingForce;
	SubVec3(&dampingForce,&attachPoint->position,&attachPoint->lastPosition);
	ScaleVec3_Self(&dampingForce,damping);
	SubVec3_Self(&attachPoint->force,&dampingForce);
	
	//Acceleration
	vec3 acceleration;
	ScaleVec3(&acceleration,&attachPoint->force,1.0f/mass);
	AddVec3_Self(&acceleration,gravity);
	
	vec3 x0;
	CopyVec3(&x0,&attachPoint->position);
	
	//Save current position into last position before updating new position
	CopyVec3(&attachPoint->lastPosition,&attachPoint->position);
	
	SubVec3(&attachPoint->position,&x0,&xNeg1);
	ScaleVec3_Self(&attachPoint->position,DTOverPreviousDT);
	
	AddVec3_Self(&attachPoint->position,&x0);
	
	AddScaledVec3_Self(&attachPoint->position,&acceleration,DTSq);
}


void Physics_UpdateSpringNode(Physics_SpringNode* springNode, f32 restLength, f32 springK)
{
	Physics_AttachPoint* attachPointA = springNode->pointA;
	Physics_AttachPoint* attachPointB = springNode->pointB;
	
	//Get stretch
	//The direction points from pointA to pointB
	vec3 stretch;
	SubVec3(&stretch,&attachPointB->position,&attachPointA->position);
	
	//Get magnitude of stretch
	const f32 stretchLength = TryNormalizeVec3_Self(&stretch);
	
	//Calculate force
	const f32 forceF = -springK * (stretchLength-restLength);
	
	//Add forces to attach points
	AddScaledVec3_Self(&attachPointA->force, &stretch, -forceF);
	AddScaledVec3_Self(&attachPointB->force, &stretch, forceF);
}
