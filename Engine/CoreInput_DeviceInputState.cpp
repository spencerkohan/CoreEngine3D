//
//  TouchState.cpp
//  CoreEngine3D
//
//  Created by JodyMcAdams on 12/19/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CoreInput_DeviceInputState.h"
#include "MathUtil.h"

void DeviceInputState::GetTouchVelocity(u32 touchIndex, vec2* out_velocity)
{
	TouchState touchState = GetTouchState(touchIndex);
	
	switch (touchState)
	{
		case TouchState_Moving:
		case TouchState_Ended:
		{
			SubVec2(out_velocity, &m_touchCurrentPoints[touchIndex],&m_touchPrevPoints[touchIndex]);
			const f32 time = static_cast<f32>(m_touchCurrTimes[touchIndex]-m_touchPrevTimes[touchIndex]);
			ScaleVec2_Self(out_velocity, 1.0f/time);
			
			break;
		}
		default:
		{
			SetVec2(out_velocity, 0.0f, 0.0f);
		}
	}
}

void DeviceInputState::GetTouchPos_Begin(u32 touchIndex, vec2* out_posCurr)
{
	CopyVec2(out_posCurr,&m_touchBeginPoints[touchIndex]);
}

void DeviceInputState::GetTouchPos_Curr(u32 touchIndex, vec2* out_posCurr)
{
	CopyVec2(out_posCurr,&m_touchCurrentPoints[touchIndex]);
}

TouchState DeviceInputState::GetTouchState(u32 touchIndex)
{
	return m_touchStates[touchIndex];
}

#if TARGET_IPHONE_SIMULATOR
const vec3 fakeAccel = {0.0f,-1.0f,0.0f};
vec3* DeviceInputState::GetAccelerometerValue()
{
    return &fakeAccel;
}
#else
vec3* DeviceInputState::GetAccelerometerValue()
{
    return &m_accelerometerValue;
}
#endif