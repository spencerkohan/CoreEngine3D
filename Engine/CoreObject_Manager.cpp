//
//  CoreObject_Manager.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CoreObject_Manager.h"
#include "stddef.h" //for NULL -_-
#include <cstdio>

//TODO:  http://gamesfromwithin.com/managing-data-relationships

#define COREOBJECTMANAGER_DEBUG 0

//#if COREOBJECTMANAGER_DEBUG
#include <cassert>
//#endif

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
	
	//Handles start at 1
	for(u32 i=0; i<COREOBJECT_MAX_OBJECTS; ++i)
	{
		m_freeHandles[i] = i+1;
	}
	
	m_numFreeHandles = COREOBJECT_MAX_OBJECTS;
}


bool CoreObjectManager::AddObject(CoreObject *pCoreObject)
{
	//If the object is NULL, don't add it
	if(pCoreObject == NULL)
	{
		return false;
	}
	
	if(m_numObjects >= COREOBJECT_MAX_OBJECTS)
	{
#if COREOBJECTMANAGER_DEBUG
		COREDEBUG_PrintDebugMessage("ERROR: Out of core objects!\n");
#endif
		return false;
	}
	
	const u32 handle = GetUnusedHandle();

	if(handle == INVALID_COREOBJECT_HANDLE)
	{
#if COREOBJECTMANAGER_DEBUG
		COREDEBUG_PrintDebugMessage("ERROR: Failed to get handle!\n");
#endif
		return false;
	}
	
	CoreObjectHandleObject* pHandleObject = &m_objectArray[m_numObjects];
	pHandleObject->pObject = pCoreObject;
	pHandleObject->handle = handle;
	
	pCoreObject->handle = handle;
	
#if COREOBJECTMANAGER_DEBUG	
	assert(pCoreObject->handle == pHandleObject->handle);
#endif
	
	++m_numObjects;
	
	return true;
}


u32 CoreObjectManager::GetUnusedHandle()
{
	if(m_numFreeHandles == 0)
	{
#if COREOBJECTMANAGER_DEBUG
		COREDEBUG_PrintDebugMessage("ERROR: Out of object handles!\n");
#endif
		return 0;
	}
	
	if(m_numUsedHandles == COREOBJECT_MAX_OBJECTS)
	{
#if COREOBJECTMANAGER_DEBUG
		COREDEBUG_PrintDebugMessage("ERROR: Out of coreobjects!\n");
#endif
		return 0;
	}
	
	const u32 handle = m_freeHandles[m_numFreeHandles-1];
	--m_numFreeHandles;

	m_usedHandles[m_numUsedHandles] = handle;
	++m_numUsedHandles;

#if COREOBJECTMANAGER_DEBUG
	//COREDEBUG_PrintDebugMessage("Created handle: %d\n",handle);
#endif
	
	return handle;
}


CoreObject* CoreObjectManager::GetObjectByHandle(CoreObjectHandle handle)
{
	if(handle == INVALID_COREOBJECT_HANDLE)
	{
		return NULL;
	}
	
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObjectHandleObject* pHandleObject = &m_objectArray[i];
		if(pHandleObject->handle == handle)
		{
			return pHandleObject->pObject;
		}
	}
	
#if COREOBJECTMANAGER_DEBUG	
	COREDEBUG_PrintDebugMessage("ERROR: Tried to get object for handle %d but it could not be found!\n", handle);
#endif
	
	assert(0);
	
	return NULL;
}


//Gets called when a CoreObject changes it's memory location
//like when you compact an array
void CoreObjectManager::UpdateHandle(CoreObject* pCoreObject)
{
	//If the object is NULL, do nothing
	if(pCoreObject == NULL)
	{
		assert(0);
		return;
	}
	
	const u32 handle = pCoreObject->GetHandle();
	if(handle == INVALID_COREOBJECT_HANDLE)
	{
#if COREOBJECTMANAGER_DEBUG	
		COREDEBUG_PrintDebugMessage("Warning: UpdateHandle->Tried to update invalid object handle.");
#endif
		return;
	}
	
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObjectHandleObject* pHandleObject = &m_objectArray[i];
		if(pHandleObject->handle == handle)
		{
			pHandleObject->pObject = pCoreObject;
						
#if COREOBJECTMANAGER_DEBUG	
			assert(pCoreObject->GetHandle() == pHandleObject->handle);
#endif
			return;
		}
	}
	
	assert(0);
	
#if COREOBJECTMANAGER_DEBUG	
	COREDEBUG_PrintDebugMessage("ERROR: Tried to update handle %d but it could not be found!\n", handle);
#endif
}


void CoreObjectManager::FreeHandle(CoreObjectHandle handle)
{	
	for(u32 i=0; i<m_numUsedHandles; ++i)
	{
		if(m_usedHandles[i] == handle)
		{
			assert(m_numUsedHandles != 0);
			m_usedHandles[i] = m_usedHandles[m_numUsedHandles-1];
			m_usedHandles[m_numUsedHandles-1] = INVALID_COREOBJECT_HANDLE;
			--m_numUsedHandles;
#if COREOBJECTMANAGER_DEBUG			
			assert(m_numFreeHandles < COREOBJECT_MAX_OBJECTS);
#endif
			
			m_freeHandles[m_numFreeHandles] = handle;
			++m_numFreeHandles;
			
			return;
		}
	}
	
	assert(0);
	
#if COREOBJECTMANAGER_DEBUG	
	COREDEBUG_PrintDebugMessage("ERROR: Tried to free handle %d but it could not be found!\n",handle);
#endif
}


void CoreObjectManager::RemoveObjectByHandle(CoreObjectHandle handle)
{
	if(handle == INVALID_COREOBJECT_HANDLE)
	{
		
#if COREOBJECTMANAGER_DEBUG
		COREDEBUG_PrintDebugMessage("ERROR: Tried to remove invalid handle!\n");
#endif
		
		return;
	}
	
	s32 freeHandles = m_numFreeHandles;
	
	for(u32 i=0; i<m_numObjects; ++i)
	{
		CoreObjectHandleObject* pHandleObject = &m_objectArray[i];
		if(pHandleObject->handle == handle)
		{
			
			
			FreeHandle(handle);
			
			assert(freeHandles != m_numFreeHandles);
			
			pHandleObject->pObject->handle = INVALID_COREOBJECT_HANDLE;
			
			pHandleObject->handle = INVALID_COREOBJECT_HANDLE;
			pHandleObject->pObject = NULL;	//TODO: might be able to just remove this line
			
			
			m_objectArray[i] = m_objectArray[m_numObjects-1];
			
			assert(m_numObjects != 0);
			--m_numObjects;
			
			
			
#if COREOBJECTMANAGER_DEBUG
			//COREDEBUG_PrintDebugMessage("Removed handle: %d\n",handle);
#endif
			return;
		}
	}
	
	assert(freeHandles != m_numFreeHandles);
	
	//Should not get here
	assert(0);

#if COREOBJECTMANAGER_DEBUG	
	COREDEBUG_PrintDebugMessage("ERROR: Tried to remove handle %d but it could not be found!\n", handle);
#endif
	
}

void CoreObjectManager::PrintStatus()
{
	COREDEBUG_PrintDebugMessage("Handles: Num Used: %d, Num Free %d",m_numUsedHandles,m_numFreeHandles);
}

