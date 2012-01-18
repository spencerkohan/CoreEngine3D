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

#define COREOBJECT_MAX_OBJECTS 4096

class CoreObjectManager
{
public:
	CoreObjectManager();
	void Clear();
	bool AddObject(CoreObject* pCoreObject);
	
	CoreObject* GetObjectByHandle(CoreObjectHandle handle);
private:
	void RemoveObjectByHandle(CoreObjectHandle handle);
	void UpdateHandle(CoreObject* pCoreObject);
	u32 GetUnusedHandle();
	void FreeHandle(CoreObjectHandle handle);
	CoreObjectHandleObject m_objectArray[COREOBJECT_MAX_OBJECTS];
	u32 m_numObjects;
	
	//Handles available for use
	CoreObjectHandle m_freeHandles[COREOBJECT_MAX_OBJECTS];
	u32 m_numFreeHandles;
	
	//Handles that are currently being used
	CoreObjectHandle m_usedHandles[COREOBJECT_MAX_OBJECTS];
	u32 m_numUsedHandles;
	
	friend class CoreObject;
};

#endif
