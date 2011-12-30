//
//  CoreInput_MouseState.cpp
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 12/29/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CoreInput_MouseState.h"

MouseInputState::MouseInputState()
{
	position.x = 0;
	position.y = 0;
	
	for(u32 i=0; i<MOUSESTATE_MAX_MOUSEBUTTONS; ++i)
	{
		buttonState[i] = MouseButtonState_None;
	}
}
