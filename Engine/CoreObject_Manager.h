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

class CoreObjectManager;
extern CoreObjectManager* COREOBJECTMANAGER;

#define COREOBJECT_MAX_OBJECTS 256

class CoreObjectManager
{
public:
	CoreObjectManager();
	void Clear();
	bool AddObject(CoreObject* pCoreObject);
	void RemoveObjectByHandle(s32 handle);
	CoreObject* GetObjectByHandle(s32 handle);
private:
	void UpdateHandle(CoreObject* pCoreObject);
	s32 GetUnusedHandle();
	void FreeHandle(s32 handle);
	CoreObjectHandleObject m_objectArray[COREOBJECT_MAX_OBJECTS];
	u32 m_numObjects;
	
	//Handles available for use
	s32 m_freeHandles[COREOBJECT_MAX_OBJECTS];
	u32 m_numFreeHandles;
	
	//Handles that are currently being used
	s32 m_usedHandles[COREOBJECT_MAX_OBJECTS];
	u32 m_numUsedHandles;
	
	friend class CoreObject;
};

#endif
