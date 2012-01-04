//
//  CoreObject_Manager.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CoreObject_Manager.h"
#include "stddef.h" //for NULL -_-

CoreObjectManager* COREOBJECTMANAGER = NULL;

CoreObjectManager::CoreObjectManager()
{
	COREOBJECTMANAGER = this;
	
	Clear();
}


void CoreObjectManager::Clear()
{
	m_numObjects = 0;
	m_numUsedHandles = 0;
	
	for(u32 i=0; i<COREOBJECT_MAX_OBJECTS; ++i)
	{
		m_freeHandles[i] = i;
	}
	
	m_numFreeHandles = COREOBJECT_MAX_OBJECTS;
}


bool CoreObjectManager::AddObject(CoreObject *pCoreObject)
{
	if(m_numObjects >= COREOBJECT_MAX_OBJECTS)
	{
		return false;
	}
	
	const s32 handle = GetUnusedHandle();
	if(handle == -1)
	{
		return false;
	}
	
	m_objectArray[m_numObjects] = pCoreObject;
	pCoreObject->handle = handle;
	
	++m_numObjects;
	
	return true;
}


s32 CoreObjectManager::GetUnusedHandle()
{
	if(m_numFreeHandles == 0)
	{
		return -1;
	}
	
	if(m_numUsedHandles == COREOBJECT_MAX_OBJECTS)
	{
		return -1;
	}
	
	const s32 handle = m_freeHandles[0];
	m_freeHandles[0] = m_freeHandles[m_numFreeHandles-1];
	--m_numFreeHandles;
	
	m_usedHandles[m_numUsedHandles] = handle;
	++m_numUsedHandles;
	
	return handle;
}


CoreObject* CoreObjectManager::GetObjectByHandle(s32 handle)
{
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObject* pCurrObject = m_objectArray[i];
		if(pCurrObject->handle == handle)
		{
			return pCurrObject;
		}
	}
	
	return NULL;
}


void CoreObjectManager::FreeHandle(s32 handle)
{
	for(u32 i=0; i<m_numUsedHandles; ++i)
	{
		if(m_usedHandles[i] == handle)
		{
			m_usedHandles[i] = m_usedHandles[m_numUsedHandles-1];
			--m_numUsedHandles;
			
			return;
		}
	}
}


void CoreObjectManager::RemoveObjectByHandle(s32 handle)
{
	if(handle == -1)
	{
		return;
	}
	
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObject* pCurrObject = m_objectArray[i];
		if(pCurrObject->handle == handle)
		{
			m_objectArray[i] = m_objectArray[m_numObjects-1];
			--m_numObjects;
			
			FreeHandle(handle);
			
			return;
		}
	}
}
