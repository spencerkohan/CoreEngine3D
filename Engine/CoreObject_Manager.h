//
//  CoreObject_Manager.h
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_iOS__CoreObject_Manager_h
#define CoreEngine3D_iOS__CoreObject_Manager_h

#include "CoreObject.h"
#include "MathTypes.h"
#include "CoreDebug.h"
#include "stddef.h" //for NULL -_-
#include "ArrayUtil.h"
#include <cassert>

class CoreObjectManager;
extern CoreObjectManager* COREOBJECTMANAGER;

#define COREOBJECT_MAX_OBJECTS 4096


class CoreObjectManager
{
	friend class CoreObject;
public:
	CoreObjectManager();
	void Clear();
	bool AddObject(CoreObject* pCoreObject);	//use outside this class is deprecated
	void PrintStatus();
	CoreObject* GetObjectByHandle(CoreObjectHandle handle);
	s32 GetNumEntries(){return m_activeEntryCount;}
private:
	void RemoveObject(CoreObject* pCoreObject);
	void UpdateHandle(CoreObject* pCoreObject);
	
	struct CoreObjectHandleEntry
	{
		CoreObjectHandleEntry();
		explicit CoreObjectHandleEntry(u32 nextFreeIndex);
		
		u32 m_nextFreeIndex : 12;
		u32 m_counter : 15;
		u32 m_active : 1;
		u32 m_endOfList : 1;
		CoreObject* m_pObject;
	};
	
	CoreObjectHandleEntry m_entries[COREOBJECT_MAX_OBJECTS];
	
	s32 m_activeEntryCount;
	u32 m_firstFreeEntry;
};

template <class T>
class CoreObjectFactory
{
	friend class CoreObjectManager;
public:
	CoreObjectFactory()
	{
		m_pObjectList = NULL;
		m_numObjects = 0;
		m_maxObjects = 0;
		m_objectsCanUpdate = true;
	}
	
	
	void Sort(bool (*compareFunc)(const T& lhs, const T& rhs))
	{
		Array_InsertionSort(m_pObjectList, m_numObjects, compareFunc);
		
		for(u32 i=0; i<m_numObjects; ++i)
		{
			m_pObjectList[i].UpdateHandle();
		}
	}
	
	
	void Sort(u32 startIndex, u32 count, bool (*compareFunc)(const T& lhs, const T& rhs))
	{
		Array_InsertionSort(&m_pObjectList[startIndex], count, compareFunc);
		
		for(u32 i=startIndex; i<startIndex+count; ++i)
		{
			m_pObjectList[i].UpdateHandle();
		}
	}
	
	
	void Clear()
	{
		for(u32 i=0; i<m_maxObjects; ++i)
		{
			m_pObjectList[i].m_markedForDeletion = false;
			m_pObjectList[i].InvalidateHandle();
		}
		
		m_numObjects = 0;
	};

	
	T* CreateObject(u32 type)
	{
		if(m_numObjects == m_maxObjects)
		{
			COREDEBUG_PrintDebugMessage("INSANE ERROR: You can't make any more objects!");

			return NULL;
		}

		T* pObject = &m_pObjectList[m_numObjects];

		if(pObject->Init(type))
		{
			++m_numObjects;

			return pObject;
		}
		
		COREDEBUG_PrintDebugMessage("INSANE ERROR! CoreObjectFactory: Failed to create object!  Could not initialize object.\n");
		
		return NULL;
	}
	

	//Returns true if an object was deleted in case you need
	//to respond to that sort of thing
	bool UpdateObjectList(f32 timeElapsed)
	{
		if(m_numObjects == 0)
		{
			return false;
		}
		
		//Delete dead objects
		bool deletedSomething = false;
		for(u32 i=0; i<m_numObjects;)
		{
			T* pCurrObject = &m_pObjectList[i];
			if(pCurrObject->m_markedForDeletion)
			{
				deletedSomething = true;

				pCurrObject->Uninit();

				T* pLastObject = &m_pObjectList[m_numObjects-1];

				if(pCurrObject != pLastObject)
				{
					//overwrite current enemy with last enemy
					*pCurrObject = *pLastObject;	

					//Memory location of the object has moved so update the handle
					//to point to the new memory location
					pCurrObject->UpdateHandle();
				}
				
				//Last object should now be an invalid object
				pLastObject->m_markedForDeletion = false;
				pLastObject->InvalidateHandle();

				--m_numObjects;
			}
			else
			{
				++i;
			}
		}

		if(m_objectsCanUpdate)
		{
			//Update remaining objects
			for(u32 i=0; i<m_numObjects; ++i)
			{
				T* pCurrObject = &m_pObjectList[i];
				pCurrObject->Update(timeElapsed);
			}
		}
		
		return deletedSomething;
	}

	
	void Init(u32 maxObjects)
	{
		m_pObjectList = new T[maxObjects];
		m_maxObjects = maxObjects;
		
		Clear();

		T::InitClass();
	}
	
	
	void SetObjectsCanUpdate(bool objectsCanUpdate)
	{
		m_objectsCanUpdate = objectsCanUpdate;
	}
	//private:

	T* m_pObjectList;
	u32 m_numObjects;
	u32 m_maxObjects;
	bool m_objectsCanUpdate;
};


#endif
