/*
 *  TouchInput.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/14/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#ifndef __TOUCHINPUT_H
#define __TOUCHINPUT_H

typedef enum
{
	TouchState_None,
	TouchState_Began,
	TouchState_Stationary,
	TouchState_Moving,
	TouchState_Ended,
	TouchState_Cancelled,
} TouchState;

#endif
