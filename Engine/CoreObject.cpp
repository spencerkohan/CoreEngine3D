//
//  CoreObject.cpp
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "CoreObject.h"
#include "CoreObject_Manager.h"

s32 CoreObject::GetHandle()
{
	return handle;
}

void CoreObject::Init()
{
	COREOBJECTMANAGER->AddObject(this);
}

void CoreObject::Uninit()
{
	COREOBJECTMANAGER->RemoveObjectByHandle(handle);
}