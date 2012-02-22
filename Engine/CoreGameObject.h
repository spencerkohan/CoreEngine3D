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

#define COREGAMEOBJECT_MAX_COLLISION_INFOS 16

struct Box2D_CollisionInfo
{
	CoreObject* pCollider;
	vec2 normal;
};

class CoreGameObject: public CoreObject
{
public:
	virtual void SetPosition(const vec3* pPosition){}
	virtual void SetUp(const vec3* pUp){};
	virtual const vec3* GetPosition(){return NULL;}
	virtual b2Body* Box2D_GetBody(){return NULL;}
	virtual bool Box2D_GetAnchorOffset(vec2* pOut_OffsetVec){return false;}
	
	virtual void CollisionResponseCallback(const Box2D_CollisionInfo& collInfo){};
	
	virtual CoreObjectType GetType(){return CoreObjectType_CoreGameObject;}

private:
	u32 m_Box2D_NumCollisionInfos;
	Box2D_CollisionInfo m_Box2D_CollisionInfo[COREGAMEOBJECT_MAX_COLLISION_INFOS];
};

#endif
