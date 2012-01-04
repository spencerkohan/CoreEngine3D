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

struct CoreObjectHandleObject
{
	CoreObject* pObject;
	s32 handle;
};

class CoreObject
{
public:
	s32 GetHandle();
	virtual void UpdateHandle();	//Call when the memory location changes
protected:
	virtual void Init();
	virtual void Uninit();
private:
	u32 handle;
	friend class CoreObjectManager;
};

#endif
