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
	vec2 point;
};

class CoreGameObject: public CoreObject
{
public:
	virtual void SetPosition(const vec3* pPosition){}
	virtual const vec3* GetPosition() const{return NULL;}
	
    //This function will be called for each object but by the nature
    //of how my loading functions work, you will never load the same
    //texture, sound buffer, etc. more than once.  This method
    //of loading art makes it super easy.
	static bool LoadResourcesForType(u32 type){return true;}

	//TODO: separate Box2D functionality out so you are not forced to have it
	virtual b2Body* Box2D_GetBody(){return NULL;}
	virtual b2Body* Box2D_GetBodyInRadius(const vec2* pCenterMeters, f32 radiusMeters){return NULL;}
	virtual bool Box2D_GetAnchorOffset(vec2* pOut_OffsetVec){return false;}
	virtual bool Box2D_GetLinkOffset(vec2* pOut_OffsetVec){return false;}
	virtual f32 Box2D_GetInteractionInfluenceX(){return 1.0f;}
	virtual f32 Box2D_GetInteractionInfluenceY(){return 1.0f;}
	virtual void Box2D_HandleDetachmentFromType(u32 entityType){};
	
	virtual void CollisionResponseCallback(const Box2D_CollisionInfo& collInfo){};
	
	virtual CoreObjectType GetType(){return CoreObjectType_CoreGameObject;}

	//void Box2D_SetVelocity(const vec2* pVelocity);
	
private:
	u32 m_Box2D_NumCollisionInfos;
	Box2D_CollisionInfo m_Box2D_CollisionInfo[COREGAMEOBJECT_MAX_COLLISION_INFOS];
};

#endif
