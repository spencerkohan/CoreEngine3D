//
//  CoreGameObject.cpp
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 2/21/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "CoreGameObject.h"

//Not convinced this is useful yet
/*void CoreGameObject::Box2D_SetVelocity(const vec2* pVelocity)
{
	b2Body* pBody = Box2D_GetBody();
	if(pBody == NULL)
	{
		return;
	}
	
	const b2Vec2& vel = pBody->GetLinearVelocity();
	const vec2 velVec = {vel.x,vel.y};
	
	vec2 velChange;
	SubVec2(&velChange,pVelocity,&velVec);
	vec2 impulse;
	ScaleVec2(&impulse,&velChange,pBody->GetMass());
	
	pBody->ApplyLinearImpulse( b2Vec2(impulse.x,impulse.y), pBody->GetWorldCenter() );
}*/
