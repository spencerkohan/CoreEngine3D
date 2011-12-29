//
//  CoreInput_Joystick.h
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 12/28/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef CoreEngine3D_OSX__CoreInput_Joystick_h
#define CoreEngine3D_OSX__CoreInput_Joystick_h

#include "MathTypes.h"
#include "CoreInput_DeviceInputState.h"

#define COREINPUT_MAX_JOYSTICKS 2
#define COREINPUT_MAX_JOYSTICKBUTTONS 16


enum JoystickButtonState
{
	JoystickButtonState_None,
    JoystickButtonState_FirstFrameOn,
	JoystickButtonState_On,
    JoystickButtonState_Released,
};


struct JoystickBoundingBox
{
    f32 X0;
    f32 X1;
    f32 Y0;
    f32 Y1;
};


enum JoystickMode
{
    JoystickMode_Analog,
    JoystickMode_SquareAnalog,
    JoystickMode_NonAnalog,
    JoystickMode_DPAD,
};


struct Joystick
{
	void Update(const vec2* pTouchPos);
	void ConstrainStick();
	void SetStickCenter(const vec2* pTouchPos);
	
    vec2 stickCenter;
    vec2 stickJoystickPos;
    f32 stickRadius;
    f32 stickRadiusBuffer;
    f32 deadZone;
    JoystickBoundingBox boundingBox;
    f32 minValue;
    
    vec2 value;
    JoystickMode stickMode;
    
    s32 touchIndex;
};


struct JoystickButton
{
    u32 value;
    vec2 buttonCenter;
    f32 buttonRadius;
    JoystickBoundingBox boundingBox;
    f32 timePressed;
    
    s32 touchIndex;
    u8 touchIsInsideBoundingBox_INTERNAL;
};


class GamePad
{
public:
	void Init(DeviceInputState* pDeviceInputState);
	void AddJoystick(JoystickMode stickMode, const JoystickBoundingBox& boundingBox, f32 radius, f32 radiusBuffer, f32 deadZone, f32 minValue);
	void AddButton(const JoystickBoundingBox& boundingBox, f32 radius);
	void DebugDraw();
	void Update();
	
private:
	Joystick m_joysticks[COREINPUT_MAX_JOYSTICKS];
	u32 m_numJoysticks;
	JoystickButton m_buttons[COREINPUT_MAX_JOYSTICKBUTTONS];
	u32 m_numButtons;
	DeviceInputState* m_pDeviceInputState;
};

//Helpers
bool TouchIsInsideBoundingBox(const vec2* pTouchPos, JoystickBoundingBox* pBox, f32 bufferArea);

#endif
