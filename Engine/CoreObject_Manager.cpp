//
//  CoreObject_Manager.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

// Thanks to http://gamesfromwithin.com/managing-data-relationships for the original code

#include "CoreObject_Manager.h"
#include "stddef.h" //for NULL -_-
#include <cstdio>

//TODO:  http://gamesfromwithin.com/managing-data-relationships

#define COREOBJECTMANAGER_DEBUG 0

#if COREOBJECTMANAGER_DEBUG
#include <cassert>
#endif

CoreObjectManager* COREOBJECTMANAGER = NULL;

CoreObjectManager::CoreObjectHandleEntry::CoreObjectHandleEntry() 
: m_nextFreeIndex(0)
, m_counter(1)
, m_active(0)
, m_endOfList(0)
, m_pObject(NULL)
{}

CoreObjectManager::CoreObjectHandleEntry::CoreObjectHandleEntry(u32 nextFreeIndex)
: m_nextFreeIndex(nextFreeIndex)
, m_counter(1)
, m_active(0)
, m_endOfList(0)
, m_pObject(NULL)
{}


CoreObjectManager::CoreObjectManager()
{
	COREOBJECTMANAGER = this;
	
	Clear();
}


void CoreObjectManager::Clear()
{
	m_activeEntryCount = 0;
	m_firstFreeEntry = 0;
	
	for (s32 i = 0; i < COREOBJECT_MAX_OBJECTS - 1; ++i)
	{
		m_entries[i] = CoreObjectHandleEntry(i + 1);
	}
	
	m_entries[COREOBJECT_MAX_OBJECTS - 1] = CoreObjectHandleEntry();
	
	m_entries[COREOBJECT_MAX_OBJECTS - 1].m_endOfList = true;
}


bool CoreObjectManager::AddObject(CoreObject *pCoreObject)
{
#if COREOBJECTMANAGER_DEBUG
	assert(pCoreObject->m_markedForDeletion == false);
	assert(m_activeEntryCount < COREOBJECT_MAX_OBJECTS - 1);
#endif
	const s32 newIndex = m_firstFreeEntry;
	
	if(newIndex >= COREOBJECT_MAX_OBJECTS)
	{
		return false;
	}

#if COREOBJECTMANAGER_DEBUG
	assert(newIndex < COREOBJECT_MAX_OBJECTS);
	assert(m_entries[newIndex].m_active == false);
	assert(!m_entries[newIndex].m_endOfList);
#endif
	
	m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
	
	m_entries[newIndex].m_nextFreeIndex = 0;
	
	//Increment useage counter for this entry
	//If it is above 2, then the entry has been used
	//multiple times and the system could be leaking.
	++m_entries[newIndex].m_counter;
	
	//Handle wrapping
	if (m_entries[newIndex].m_counter == 0)
	{
		m_entries[newIndex].m_counter = 1;
	}
	
	//The entry has been used so it is now active
	m_entries[newIndex].m_active = true;
	
	//Save the pointer to the CoreObject
	m_entries[newIndex].m_pObject = pCoreObject;
	
	//Increase the amount of active entries
	++m_activeEntryCount;
	
	//Create a handle
	const CoreObjectHandle newHandle = CoreObjectHandle(newIndex, m_entries[newIndex].m_counter, 0);
	
	//Copy the handle to the CoreObject
	pCoreObject->handle = newHandle;
	
	//COREDEBUG_PrintDebugMessage("Created Handle: %u, for type: %u", (u32)newHandle, pCoreObject->GetEntityType());

	return true;
}

CoreObject* CoreObjectManager::GetObjectByHandle(CoreObjectHandle handle)
{
	const s32 index = handle.m_index;
	
	//If the entry is no longer active or the counter
	//is not the same value, then the
	//handle is now invalid so return NULL
	if (m_entries[index].m_counter != handle.m_counter
		|| m_entries[index].m_active == false)
	{
		return NULL;
	}
    
    CoreObject* pObject = m_entries[index].m_pObject;
	   
	//Return the pointer to the object
	return pObject;
}


//Gets called when a CoreObject changes it's memory location
//like when you compact an array
void CoreObjectManager::UpdateHandle(CoreObject* pCoreObject)
{
	const CoreObjectHandle handle = pCoreObject->GetHandle();
	
	const s32 index = handle.m_index;
	
	//Make sure that the handle is valid.
	//If it's not, we could be leaking.
#if COREOBJECTMANAGER_DEBUG
	assert(m_entries[index].m_counter == handle.m_counter);
	assert(m_entries[index].m_active == true);
#endif
	//Save the new object pointer to the handle entry
	m_entries[index].m_pObject = pCoreObject;
}


void CoreObjectManager::RemoveObject(CoreObject* pCoreObject)
{
    if(m_activeEntryCount == 0)
    {
        COREDEBUG_PrintDebugMessage("INSANE ERROR: Tried to remove an object when there are no used handles.");
        return;
    }
    
#if COREOBJECTMANAGER_DEBUG
	assert(pCoreObject->m_markedForDeletion == true);
#endif
	const CoreObjectHandle handle = pCoreObject->GetHandle();
	
	//Invalidate the object's handle
	pCoreObject->handle = CoreObjectHandle();
	
	const s32 index = handle.m_index;
	
	//Make sure that the handle is valid.
	//If it's not, we could be leaking.
#if COREOBJECTMANAGER_DEBUG
	assert(m_entries[index].m_counter == handle.m_counter);
	assert(m_entries[index].m_active == true);
#endif	
	m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
	m_entries[index].m_active = false;
	
	m_firstFreeEntry = index;
	
	--m_activeEntryCount;
	
	//COREDEBUG_PrintDebugMessage("Removed Handle: %u, for type: %u", (u32)handle, pCoreObject->GetEntityType());
}

void CoreObjectManager::PrintStatus()
{
	COREDEBUG_PrintDebugMessage("Handles: Num Used: %d, Num Free %d",m_activeEntryCount,COREOBJECT_MAX_OBJECTS-m_activeEntryCount);
}

