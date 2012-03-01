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
private:
	void RemoveObjectByHandle(CoreObjectHandle handle);
	void UpdateHandle(CoreObject* pCoreObject);
	
	struct CoreObjectHandleEntry
	{
		CoreObjectHandleEntry();
		explicit CoreObjectHandleEntry(u32 nextFreeIndex);
		
		u32 m_nextFreeIndex : 12;
		u32 m_counter : 15;
		u32 m_active : 1;
		u32 m_endOfList : 1;
		CoreObject* m_entry;
	};
	
	CoreObjectHandleEntry m_entries[COREOBJECT_MAX_OBJECTS];
	
	int m_activeEntryCount;
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
	
	void Clear(){m_numObjects = 0;};

	T* CreateObject(u32 type)
	{
		if(m_numObjects == m_maxObjects)
		{
			COREDEBUG_PrintDebugMessage("INSANE ERROR: You can't make any more objects!");

			return NULL;
		}
		
		//COREDEBUG_PrintDebugMessage("(O_O) Create, Status before:");
		//COREOBJECTMANAGER->PrintStatus();
		
		T* pObject = &m_pObjectList[m_numObjects];
		if(pObject->Init(type))
		{
			++m_numObjects;
			//COREDEBUG_PrintDebugMessage("CoreObjectFactory: Created an object!\n");

			//COREDEBUG_PrintDebugMessage("(O_O) Create, Status after:");
			//COREOBJECTMANAGER->PrintStatus();
			//COREDEBUG_PrintDebugMessage("");
			
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
				//COREDEBUG_PrintDebugMessage("(X_X) Delete, Status before:");
				//COREOBJECTMANAGER->PrintStatus();
				
				deletedSomething = true;
				
				pCurrObject->Uninit();

				T* pLastObject = &m_pObjectList[m_numObjects-1];

				if(m_numObjects > 1)
				{
					//overwrite current enemy with last enemy
					*pCurrObject = *pLastObject;

					//Memory location of the object has moved so update the handle
					//to point to the new memory location
					pCurrObject->UpdateHandle();
				}

				--m_numObjects;
				
				/*COREDEBUG_PrintDebugMessage("(X_X) Delete, Status after:");
				COREOBJECTMANAGER->PrintStatus();
				COREDEBUG_PrintDebugMessage("");*/

				//COREDEBUG_PrintDebugMessage("CoreObjectFactory: Deleted an object!\n");
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
