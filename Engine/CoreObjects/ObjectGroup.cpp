//
//  ObjectGroup.cpp
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "ObjectGroup.h"

#include "Game.h"

#include "matrix.h"
#include "OpenGLRenderer.h"
#include "CoreObject_Manager.h"
#include "CoreObjectFactories.h"

#include "Hash.h"


bool ObjectGroup::SpawnInit(void* pSpawnStruct)
{
	m_numObjects = 0;
	
	SpawnableEntity* pSpawnableEnt = (SpawnableEntity*)pSpawnStruct;
	if(pSpawnableEnt == NULL)
	{
		return false;
	}
	
	for (pugi::xml_node property = pSpawnableEnt->pProperties.child("property"); property; property = property.next_sibling("property"))
	{
		const u32 tiledUniqueID = atoi(property.attribute("value").value());
		
		SpawnableEntity* pEnt = GAME->GetSpawnableEntityByTiledUniqueID(tiledUniqueID);
		if(pEnt != NULL
		   && pEnt->pObject != NULL)
		{
			m_objectHandles[m_numObjects] = pEnt->pObject->GetHandle();
			++m_numObjects;
		}
	}
	
	return true;
}


bool ObjectGroup::ObjectsAreContained(CollisionBox* pContainer)
{
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObject* pObject = COREOBJECTMANAGER->GetObjectByHandle(m_objectHandles[i]);
		if(pObject == NULL)
		{
			return false;
		}
		
		const vec3* pPos = pObject->GetPosition();
		if(pPos == NULL)
		{
			return false;
		}
		
		if(!pContainer->GetPositionIsInside((vec2*)pPos))
		{
			return false;
		}
	}
	
	return true;
}

void ObjectGroup::SetPosition(const vec3* pPosition)
{
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObject* pObject = COREOBJECTMANAGER->GetObjectByHandle(m_objectHandles[i]);
		if(pObject != NULL)
		{
			pObject->SetPosition(pPosition);
		}
	}
}


void ObjectGroup::ProcessMessage(u32 message)	//Pass in a hash value
{
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObject* pObject = COREOBJECTMANAGER->GetObjectByHandle(m_objectHandles[i]);
		if(pObject != NULL)
		{
			pObject->ProcessMessage(message);
		}
	}
}


void ObjectGroup::Uninit()
{
	
}

