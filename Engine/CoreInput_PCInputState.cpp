//
//  CoreInput_MouseState.cpp
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 12/29/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CoreInput_PCInputState.h"

MouseInputState::MouseInputState()
{
	position.x = 0;
	position.y = 0;
	
	for(u32 i=0; i<MOUSESTATE_MAX_MOUSEBUTTONS; ++i)
	{
		buttonState[i] = CoreInput_ButtonState_None;
	}
}

KeyboardInputState::KeyboardInputState()
{
	for(u32 i=0; i<256; ++i)
	{
		buttonState[i] = CoreInput_ButtonState_None;
	}
}