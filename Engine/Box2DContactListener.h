//
//  Box2DContactListener.h
//  Game(iOS)
//
//  Created by Jody McAdams on 2/19/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#ifndef Game_iOS__Box2DContactListener_h
#define Game_iOS__Box2DContactListener_h

#include <Box2D/Box2D.h>

class Box2DContactListener: public b2ContactListener
{
	public:
	
	/// Called when two fixtures begin to touch.
	virtual void BeginContact(b2Contact* contact);
};

#endif
