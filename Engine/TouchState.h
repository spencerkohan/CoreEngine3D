//
//  TouchState.h
//  CoreEngine3D
//
//  Created by JodyMcAdams on 12/19/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_TouchState_h
#define CoreEngine3D_TouchState_h

typedef enum
{
	TouchState_None,
	TouchState_Began,
	TouchState_Stationary,
	TouchState_Moving,
	TouchState_Ended,
	TouchState_Cancelled,
} TouchState;

typedef enum
{
	SwipeType_Left,
	SwipeType_Right,
	SwipeType_None,
} SwipeType;

#endif
