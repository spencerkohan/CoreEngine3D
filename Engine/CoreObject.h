//
//  CoreObject.h
//  CoreEngine3D(iOS)
//
//  Created by JodyMcAdams on 1/3/12.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_iOS__CoreObject_h
#define CoreEngine3D_iOS__CoreObject_h

#include "MathTypes.h"
class CoreObject;
class CoreObjectManager;

typedef u32 CoreObjectHandle;
#define INVALID_COREOBJECT_HANDLE 0

struct CoreObjectHandleObject
{
	CoreObject* pObject;
	u32 handle;
};

class CoreObject
{
public:
	CoreObjectHandle GetHandle();
	virtual void UpdateHandle();	//Call when the memory location changes
	virtual void Init();
	virtual void Uninit();
	
private:
	CoreObjectHandle handle;
	friend class CoreObjectManager;
};

#endif
