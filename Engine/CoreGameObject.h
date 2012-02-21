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

class CoreGameObject: public CoreObject
{
public:
	virtual void SetPosition(const vec3* pPosition){}
	virtual void SetUp(const vec3* pUp){};
	virtual const vec3* GetPosition(){return NULL;}
	virtual const b2Body* Box2D_GetBody(){return NULL;}
	
	virtual void CollisionResponseCallback(CoreGameObject* pResponder, CoreGameObject* pCollider){};
	
	virtual CoreObjectType GetType(){return CoreObjectType_CoreGameObject;}
};

#endif
