//
//  CoreGameObject.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 2/21/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "CoreGameObject.h"
#include "CoreObject_Manager.h"

void CoreGameObject::Box2D_Init()
{
	m_Box2D_NumCollisionInfos = 0;
}

void CoreGameObject::Box2D_AddCollisionInfo(const Box2D_CollisionInfo& collInfo)
{
	if(m_Box2D_NumCollisionInfos < COREGAMEOBJECT_MAX_COLLISION_INFOS)
	{
		m_Box2D_CollisionInfo[m_Box2D_NumCollisionInfos] = collInfo;
		++m_Box2D_NumCollisionInfos;
	}
}

void CoreGameObject::Box2D_ProcessCollisions()
{
	for(u32 i=0; i<m_Box2D_NumCollisionInfos; ++i)
	{
		//const CoreObjectHandle handle = m_Box2D_CollisionData[m_Box2D_NumCollisionHandles];
		const Box2D_CollisionInfo& collInfo = m_Box2D_CollisionInfo[m_Box2D_NumCollisionInfos];

		this->CollisionResponseCallback(collInfo);
	}
	
	m_Box2D_NumCollisionInfos = 0;
}
