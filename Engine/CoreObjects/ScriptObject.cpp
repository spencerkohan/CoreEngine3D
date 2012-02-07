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

#include "Hash.h"

vec3* ScriptObject::GetPosition()
{
	return &m_position;
}


bool ScriptObject::SpawnInit(void* pSpawnStruct)
{
	m_isFirstUpdate = true;
	
	SpawnableEntity* pSpawnableEnt = (SpawnableEntity*)pSpawnStruct;
	if(pSpawnableEnt == NULL)
	{
		return false;
	}
	
	CopyVec3(&m_position, &pSpawnableEnt->position);
	
	m_triggerMessage = 0;
	m_hTriggerObject = INVALID_COREOBJECT_HANDLE;
	
	m_collisionType = CollisionBoxType_Ghost;
	m_scriptStatus = ScriptObject::ScriptStatus_On;
	m_collMode = CollisionMode_Tile;
	
	m_collMode = pSpawnableEnt->tileID == -1 ? ScriptObject::CollisionMode_Box:ScriptObject::CollisionMode_Tile;

	for (pugi::xml_node property = pSpawnableEnt->pProperties.child("property"); property; property = property.next_sibling("property"))
	{
		const char* propNameString = property.attribute("name").value();
		const char* valueString = property.attribute("value").value();
		
		if(strcmp(propNameString,"TriggerMessage") == 0)
		{
			m_triggerMessage = Hash(valueString);
		}
		else if(strcmp(propNameString,"TriggerObject") == 0)
		{
			const u32 triggerObject = Hash(valueString);
			SpawnableEntity* pEnt = GAME->GetSpawnableEntityByNameHash(triggerObject);
			if(pEnt->pObject != NULL)
			{
				m_hTriggerObject = pEnt->pObject->GetHandle();
			}
		}
		else if(strcmp(propNameString,"CollisionType") == 0)
		{
			m_collisionType = Hash(valueString);
		}
		else if(strcmp(propNameString,"ScriptStatus") == 0)
		{
			if(strcmp(valueString,"Off") == 0)
			{
				m_scriptStatus = ScriptObject::ScriptStatus_Off;
			}
		}
	}


	m_hCollisionBox = INVALID_COREOBJECT_HANDLE;
	
	//Will init the box using our own self!
	if(pSpawnableEnt->tileID == -1)
	{
		const f32 halfWidth = pSpawnableEnt->scale.x/2.0f;
		const f32 halfHeight = pSpawnableEnt->scale.y/2.0f;

		CollisionBox* pBox = g_Factory_CollisionBox.CreateObject(CollisionBoxType_Trigger);
		pBox->SpawnInit(&m_position, -halfWidth+m_position.x, halfWidth+m_position.x, halfHeight+m_position.y, -halfHeight+m_position.y);

		m_hCollisionBox = pBox->GetHandle();
	}

	m_tileIndex_X = pSpawnableEnt->tileIndexX;
	m_tileIndex_Y = pSpawnableEnt->tileIndexY;

	m_numTriggers = 0;
	m_numAllowedTriggers = 1;	//TODO: load this

	m_initialScriptStatus = m_scriptStatus;
	
	return true;
}

void ScriptObject::AttemptBoxTrigger(u32 objectType, const vec3* pPosition)
{
	if(m_numTriggers == m_numAllowedTriggers)
	{
		return;
	}
	
	if(m_scriptStatus == ScriptStatus_Off)
	{
		return;
	}
	
	if(m_collMode != CollisionMode_Box)
	{
		return;
	}

	if(m_collisionType != 0 && m_collisionType != objectType)
	{
		return;
	}
	
	CollisionBox* pBox = (CollisionBox*)COREOBJECTMANAGER->GetObjectByHandle(m_hCollisionBox);
	if(pBox == NULL)
	{
		return;
	}
	
	if(!pBox->GetPositionIsInside((vec2*)pPosition))
	{
		return;
	}
	
	if(m_triggerMessage == Hash("SetCamera"))
	{
		vec3 camPos;
		CopyVec3(&camPos,pBox->GetBottomLeft());
		GAME->SetCameraPosition(&camPos,0.0f);
	}
	else if(m_triggerMessage == Hash("MoveCamera"))
	{
		vec3 camPos;
		CopyVec3(&camPos,pBox->GetBottomLeft());
		GAME->SetCameraPosition(&camPos,1.5f);
	}
	else
	{
		CoreObject* pObject = COREOBJECTMANAGER->GetObjectByHandle(m_hTriggerObject);
		
		if(pObject != NULL)
		{
			pObject->ProcessMessage(m_triggerMessage);
		}
	}
	
	
	++m_numTriggers;
	
	if(m_numTriggers == m_numAllowedTriggers)
	{
		//TODO: not sure if I should leave this
		m_scriptStatus = ScriptStatus_Off;
	}
}

void ScriptObject::Reset()
{
	m_scriptStatus = m_initialScriptStatus;
	m_numTriggers = 0;
	m_numAllowedTriggers = 1;	//TODO: save initial
}


void ScriptObject::AttemptTileTrigger(u32 objectType, u32 tileIndex_X, u32 tileIndex_Y)
{
	if(m_numTriggers == m_numAllowedTriggers)
	{
		return;
	}
	
	if(m_scriptStatus == ScriptStatus_Off)
	{
		return;
	}
	
	if(m_collMode != CollisionMode_Tile)
	{
		return;
	}
	
	if(!(m_tileIndex_X == tileIndex_X
		  && m_tileIndex_Y == tileIndex_Y))
	{
		return;
	}

	if(m_collisionType != 0 && m_collisionType != objectType)
	{
		return;
	}
	

	CoreObject* pObject = COREOBJECTMANAGER->GetObjectByHandle(m_hTriggerObject);
	
	if(pObject != NULL)
	{
		pObject->ProcessMessage(m_triggerMessage);
	}
	
	++m_numTriggers;
	
	if(m_numTriggers == m_numAllowedTriggers)
	{
		//TODO: not sure if I should leave this
		m_scriptStatus = ScriptStatus_Off;
	}
	
}

void ScriptObject::ProcessMessage(u32 message)	//Pass in a hash value
{
	if(message == Hash("On"))
	{
		m_scriptStatus = ScriptStatus_On;
	}
	else if(message == Hash("Off"))
	{
		m_scriptStatus = ScriptStatus_Off;
	}
}

bool ScriptObject::GetPositionIsInside(const vec2* pTouchPos)
{
	return true;
}


void ScriptObject::Uninit()
{
	
}

