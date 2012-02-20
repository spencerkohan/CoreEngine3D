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

class CoreGameObject: public CoreObject
{
public:
	virtual void SetPosition(const vec3* pPosition){}//HACK: for now it's fine to have this here
	virtual void SetUp(const vec3* pUp){};//HACK: for now it's fine to have this here
	virtual const vec3* GetPosition(){return NULL;}//HACK: for now it's fine to have this here
	
	virtual void CollisionResponseCallback(CoreGameObject* pResponder, CoreGameObject* pCollider){};
};

#endif
