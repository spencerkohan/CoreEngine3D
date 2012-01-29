//
//  CoreObject.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CoreObject.h"
#include "CoreObject_Manager.h"

#include <cassert>

CoreObjectHandle CoreObject::GetHandle() const
{
	return handle;
}

void CoreObject::DeleteObject()
{
	m_markedForDeletion = true;
}

//Call when the memory location changes
void CoreObject::UpdateHandle()
{
	COREOBJECTMANAGER->UpdateHandle(this);
}

bool CoreObject::Init(s32 type)
{
	m_markedForDeletion = false;
	return COREOBJECTMANAGER->AddObject(this);
}

void CoreObject::Uninit()
{
	COREOBJECTMANAGER->RemoveObjectByHandle(handle);
	assert(handle == INVALID_COREOBJECT_HANDLE);
}