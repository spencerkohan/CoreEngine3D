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

void CoreObject::InvalidateHandle()
{
	handle = CoreObjectHandle();
}


u32 CoreObject::GetEntityType()
{
	return m_entityTypeHash;
}


//Call when the memory location changes
void CoreObject::UpdateHandle()
{
	//assert(m_markedForDeletion == false);
	
	COREOBJECTMANAGER->UpdateHandle(this);
}


bool CoreObject::Init(u32 type)
{
	m_markedForDeletion = false;
	m_entityTypeHash = type;

	return COREOBJECTMANAGER->AddObject(this);
}


void CoreObject::Uninit()
{
	//assert(m_markedForDeletion == true);
	
	COREOBJECTMANAGER->RemoveObject(this);
}
