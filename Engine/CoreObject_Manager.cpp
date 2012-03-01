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

CoreObjectManager::CoreObjectHandleEntry::CoreObjectHandleEntry() 
: m_nextFreeIndex(0)
, m_counter(1)
, m_active(0)
, m_endOfList(0)
, m_entry(NULL)
{}

CoreObjectManager::CoreObjectHandleEntry::CoreObjectHandleEntry(u32 nextFreeIndex)
: m_nextFreeIndex(nextFreeIndex)
, m_counter(1)
, m_active(0)
, m_endOfList(0)
, m_entry(NULL)
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
	
	for (int i = 0; i < COREOBJECT_MAX_OBJECTS - 1; ++i)
		m_entries[i] = CoreObjectHandleEntry(i + 1);
	m_entries[COREOBJECT_MAX_OBJECTS - 1] = CoreObjectHandleEntry();
	m_entries[COREOBJECT_MAX_OBJECTS - 1].m_endOfList = true;
}


bool CoreObjectManager::AddObject(CoreObject *pCoreObject)
{
	assert(m_activeEntryCount < COREOBJECT_MAX_OBJECTS - 1);
	//assert(type >= 0 && type <= 31);
	
	const int newIndex = m_firstFreeEntry;
	assert(newIndex < COREOBJECT_MAX_OBJECTS);
	assert(m_entries[newIndex].m_active == false);
	assert(!m_entries[newIndex].m_endOfList);
	
	m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
	m_entries[newIndex].m_nextFreeIndex = 0;
	m_entries[newIndex].m_counter = m_entries[newIndex].m_counter + 1;
	if (m_entries[newIndex].m_counter == 0)
		m_entries[newIndex].m_counter = 1;
	m_entries[newIndex].m_active = true;
	m_entries[newIndex].m_entry = pCoreObject;
	
	++m_activeEntryCount;
	
	pCoreObject->handle = CoreObjectHandle(newIndex, m_entries[newIndex].m_counter, 0);

	return true;
}


CoreObject* CoreObjectManager::GetObjectByHandle(CoreObjectHandle handle)
{
	const int index = handle.m_index;
	if (m_entries[index].m_counter != handle.m_counter ||
	    m_entries[index].m_active == false)
		return NULL;
	
	return m_entries[index].m_entry;
}


//Gets called when a CoreObject changes it's memory location
//like when you compact an array
void CoreObjectManager::UpdateHandle(CoreObject* pCoreObject)
{
	const CoreObjectHandle handle = pCoreObject->GetHandle();
	
	const int index = handle.m_index;
	assert(m_entries[index].m_counter == handle.m_counter);
	assert(m_entries[index].m_active == true);
	
	m_entries[index].m_entry = pCoreObject;
}


void CoreObjectManager::RemoveObjectByHandle(CoreObjectHandle handle)
{
	const u32 index = handle.m_index;
	assert(m_entries[index].m_counter == handle.m_counter);
	assert(m_entries[index].m_active == true);
	
	m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
	m_entries[index].m_active = false;
	m_firstFreeEntry = index;
	
	--m_activeEntryCount;
}

void CoreObjectManager::PrintStatus()
{
	//COREDEBUG_PrintDebugMessage("Handles: Num Used: %d, Num Free %d",m_numUsedHandles,m_numFreeHandles);
}

