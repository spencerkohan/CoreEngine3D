//
//  TouchState.h
//  CoreEngine3D
//
//  Created by JodyMcAdams on 12/19/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef CoreEngine3D_DeviceInputState_h
#define CoreEngine3D_DeviceInputState_h

#include "MathTypes.h"

#define MAX_MULTITOUCH 11

typedef enum
{
	TouchState_None,
	TouchState_Began,
	TouchState_Stationary,
	TouchState_Moving,
	TouchState_Ended,
	TouchState_Cancelled,
} TouchState;

struct DeviceInputState
{
	void GetTouchVelocity(u32 touchIndex, vec2* out_velocity);
	void GetTouchPos_Begin(u32 touchIndex, vec2* out_posCurr);
	void GetTouchPos_Curr(u32 touchIndex, vec2* out_posCurr);
	vec3* GetAccelerometerValue();
	TouchState GetTouchState(u32 touchIndex);
	
	bool m_touchStateWasUpdated[MAX_MULTITOUCH];
	vec2 m_touchBeginPoints[MAX_MULTITOUCH];
	double m_touchPrevTimes[MAX_MULTITOUCH];
	double m_touchCurrTimes[MAX_MULTITOUCH];
	vec2 m_touchPrevPoints[MAX_MULTITOUCH];
	vec2 m_touchCurrentPoints[MAX_MULTITOUCH];
	TouchState m_touchStates[MAX_MULTITOUCH];
	vec3 m_accelerometerValue;
};

#endif
