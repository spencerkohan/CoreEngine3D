//
//  CoreInput_PCInputState.h
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 12/29/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef CoreEngine3D_OSX__CoreInput_PCInputState_h
#define CoreEngine3D_OSX__CoreInput_PCInputState_h

#define MOUSESTATE_MAX_MOUSEBUTTONS 2

#include "MathTypes.h"

enum CoreInput_ButtonState
{
	CoreInput_ButtonState_None,
	CoreInput_ButtonState_Began,
	CoreInput_ButtonState_Held,
	CoreInput_ButtonState_Ended,
};


struct MouseInputState
{
	MouseInputState();
	CoreInput_ButtonState buttonState[MOUSESTATE_MAX_MOUSEBUTTONS];
	vec2 position;
};

struct KeyboardInputState
{
	KeyboardInputState();
	CoreInput_ButtonState buttonState[256];
};

#endif
