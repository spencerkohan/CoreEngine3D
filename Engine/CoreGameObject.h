//
//  CoreGameObject.h
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 2/19/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_OSX__CoreGameObject_h
#define CoreEngine3D_OSX__CoreGameObject_h

#include "CoreObject.h"
#include <Box2D/Box2D.h>
#include "MathTypes.h"
#include "MathUtil.h"

#define COREGAMEOBJECT_MAX_COLLISION_INFOS 16

class CoreGameObject;

struct Box2D_CollisionInfo
{
	CoreGameObject* pCollider;
	b2Fixture* pFixtureCollider;
	b2Fixture* pFixtureReceiver;
	vec2 normal;
};

class CoreGameObject: public CoreObject
{
public:
	virtual void SetPosition(const vec3* pPosition){}
	virtual const vec3* GetPosition() const{return NULL;}
	virtual b2Body* Box2D_GetBody(){return NULL;}
	virtual bool Box2D_GetAnchorOffset(vec2* pOut_OffsetVec){return false;}
	virtual bool Box2D_GetLinkOffset(vec2* pOut_OffsetVec){return false;}
	
	virtual void CollisionResponseCallback(const Box2D_CollisionInfo& collInfo){};
	
	virtual CoreObjectType GetType(){return CoreObjectType_CoreGameObject;}

	//void Box2D_SetVelocity(const vec2* pVelocity);
	
private:
	u32 m_Box2D_NumCollisionInfos;
	Box2D_CollisionInfo m_Box2D_CollisionInfo[COREGAMEOBJECT_MAX_COLLISION_INFOS];
};

#endif
