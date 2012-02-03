//
//  ScriptObject.cpp
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "ScriptObject.h"

#include "Game.h"

#include "matrix.h"
#include "OpenGLRenderer.h"
#include "CoreObject_Manager.h"
#include "CoreObjectFactories.h"
#include "CollisionBox.h"

vec3* ScriptObject::GetPosition()
{
	return &m_position;
}

void ScriptObject::Link()
{
	//Convert name hash to object handle
	const CoreObjectHandle objectHandle = GAME->SpawnableEntityHandleByNameHash(m_hTriggerObject);
	m_hTriggerObject = objectHandle;
}


void ScriptObject::SpawnInit(SpawnableEntity* pEntity, u32 triggerMessage, CoreObjectHandle triggerObject)
{
	CopyVec3(&m_position, &pEntity->position);
	
	//TODO: make the bounding box based on the art
	CollisionBox* pBox = g_Factory_CollisionBox.CreateObject(CollisionBoxType_Trigger);
	m_hCollisionBox = pBox->GetHandle();
	
	const f32 halfWidth = pEntity->scale.x/2.0f;
	const f32 halfHeight = pEntity->scale.y/2.0f;
	
	pBox->SpawnInit(-halfWidth+m_position.x, halfWidth+m_position.x, halfHeight+m_position.y, -halfHeight+m_position.y, &m_position);
	
	m_tileIndex_X = pEntity->tileIndexX;
	m_tileIndex_Y = pEntity->tileIndexY;
	
	m_triggerMessage = triggerMessage;
	m_collType = CollisionType_Tile;	//TODO: load a type
	
	m_numTriggers = 0;
	m_numAllowedTriggers = 1;	//TODO: load this
	
	m_hTriggerObject = triggerObject;
}


bool ScriptObject::Init(s32 type)
{
	CoreObject::Init();
	
	m_type = type;

	m_isFirstUpdate = true;

	return true;
}

void ScriptObject::AttemptTileTrigger(u32 tileIndex_X, u32 tileIndex_Y)
{
	if(m_collType != CollisionType_Tile)
	{
		return;
	}
	
	if(m_numAllowedTriggers == 0)
	{
		return;
	}
	
	if(m_tileIndex_X == tileIndex_X
	   && m_tileIndex_Y == tileIndex_Y)
	{
		CoreObject* pObject = COREOBJECTMANAGER->GetObjectByHandle(m_hTriggerObject);
		
		if(pObject != NULL)
		{
			pObject->ProcessMessage(m_triggerMessage);
		}
		
		--m_numAllowedTriggers;
	}
}

bool ScriptObject::GetPositionIsInside(const vec2* pTouchPos)
{
	return true;
}


void ScriptObject::Update(f32 timeElapsed)
{
	
}

void ScriptObject::Uninit()
{
	
}

