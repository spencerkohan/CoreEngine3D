//
//  Box2DContactListener.cpp
//  Game(iOS)
//
//  Created by Jody McAdams on 2/19/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "Box2DContactListener.h"

#include "CoreGameObject.h"

void Box2DContactListener::BeginContact(b2Contact* contact)
{
	CoreGameObject* pObjectA = (CoreGameObject*)contact->GetFixtureA()->GetBody()->GetUserData();
	
	CoreGameObject* pObjectB = (CoreGameObject*)contact->GetFixtureB()->GetBody()->GetUserData();

	Box2D_CollisionInfo collInfo;
	
	const b2Vec2& normal = contact->GetManifold()->localNormal;
	collInfo.normal.x = normal.x;
	collInfo.normal.y = normal.y;
	
	const b2Vec2& point = contact->GetManifold()->localPoint;
	collInfo.point.x = point.x;
	collInfo.point.y = point.y;
	
	if(pObjectA != NULL)
	{
		collInfo.pCollider = pObjectB;
		collInfo.pFixtureCollider = contact->GetFixtureB();
		collInfo.pFixtureReceiver = contact->GetFixtureA();
		pObjectA->CollisionResponseCallback(collInfo);
	}

	
	if(pObjectB != NULL)
	{
		collInfo.pCollider = pObjectA;
		collInfo.pFixtureCollider = contact->GetFixtureA();
		collInfo.pFixtureReceiver = contact->GetFixtureB();
		pObjectB->CollisionResponseCallback(collInfo);
	}

}
