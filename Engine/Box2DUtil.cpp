//
//  Box2DUtil.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 2/24/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "Box2DUtil.h"

vec2* AsVec2(const b2Vec2& box2DVec)
{
	return (vec2*)&box2DVec;
}

const b2Vec2& AsBox2DVec2(const vec2& vec0)
{
    return (b2Vec2&)vec0;
}
