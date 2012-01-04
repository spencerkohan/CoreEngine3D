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
class CoreObjectManager;

class CoreObject
{
public:
	s32 GetHandle();
protected:
	virtual void Init();
	virtual void Uninit();
private:
	s32 handle;
	
	friend CoreObjectManager;
};

#endif
