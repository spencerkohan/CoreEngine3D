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
	friend class CoreObjectManager;
public:
	static void InitClass(){};

	CoreObjectHandle GetHandle() const;
	virtual void UpdateHandle();	//Call when the memory location changes
	void DeleteObject();			//Use with new system

	virtual bool Init(s32 type = 0);
	virtual void Uninit();
	virtual void Update(f32 timeElapsed){};
	bool m_markedForDeletion;
	virtual void ProcessMessage(u32 message){};	//Pass in a hash value
private:
	

	CoreObjectHandle handle;
	
};

#endif
