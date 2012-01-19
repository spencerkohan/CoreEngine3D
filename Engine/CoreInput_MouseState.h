//
//  CoreInput_MouseState.h
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 12/29/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef CoreEngine3D_OSX__CoreInput_MouseState_h
#define CoreEngine3D_OSX__CoreInput_MouseState_h

#define MOUSESTATE_MAX_MOUSEBUTTONS 2

#include "MathTypes.h"

enum MouseButtonState
{
	MouseButtonState_None,
	MouseButtonState_Began,
	MouseButtonState_Held,
	MouseButtonState_Ended,
};


struct MouseInputState
{
	MouseInputState();
	MouseButtonState buttonState[MOUSESTATE_MAX_MOUSEBUTTONS];
	vec2 position;
};

#endif
