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

enum GamePadButtonType
{
	GamePadButtonType_Face_Left,
	GamePadButtonType_Face_Right,
	GamePadButtonType_Face_Down,
	GamePadButtonType_Face_Up,
	GamePadButtonType_DPAD_Left,
	GamePadButtonType_DPAD_Right,
	GamePadButtonType_DPAD_Down,
	GamePadButtonType_DPAD_Up,
	GamePadButtonType_AnalogClick_Left,
	GamePadButtonType_AnalogClick_Right,
	GamePadButtonType_Shoulder_L1,
	GamePadButtonType_Shoulder_L2,
	GamePadButtonType_Shoulder_R1,
	GamePadButtonType_Shoulder_R2,
	GamePadButtonType_Select,
	GamePadButtonType_Start,
	GamePadButtonType_Home,
	GamePadButtonType_Num,
};

enum GamePadJoystickType
{
	GamePadJoystickType_AnalogLeft,
	GamePadJoystickType_AnalogRight,
	GamePadJoystickType_Num,
};

enum GamePadButtonState
{
	GamePadButtonState_None,
    GamePadButtonState_FirstFrameOn,
	GamePadButtonState_On,
    GamePadButtonState_Released,
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
    JoystickMode_TouchRelative,
};


struct GamePadJoystick
{
	void Update(const vec2* pTouchPos);
	void ConstrainStick();
	void SetStickCenter(const vec2* pTouchPos);
	
	GamePadJoystickType joystickType;
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


struct GamePadButton
{
	GamePadButtonType buttonType;
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
	GamePadJoystick* AddJoystick(GamePadJoystickType type, JoystickMode stickMode, const JoystickBoundingBox& boundingBox, f32 radius, f32 radiusBuffer, f32 deadZone, f32 minValue);
	GamePadButton* AddButton(GamePadButtonType type, const JoystickBoundingBox& boundingBox, f32 radius);
	void DebugDraw();
	void Update();
	GamePadJoystick* GetJoystickByType(GamePadJoystickType type);
	GamePadButton* GetGamePadButtonByType(GamePadButtonType type);
	
	GamePadJoystick joysticks[GamePadJoystickType_Num];
	u32 numJoysticks;
	GamePadButton buttons[GamePadButtonType_Num];
	u32 numButtons;
	
private:
	
	DeviceInputState* m_pDeviceInputState;
};

//Helpers
bool TouchIsInsideBoundingBox(const vec2* pTouchPos, JoystickBoundingBox* pBox, f32 bufferArea);

#endif
