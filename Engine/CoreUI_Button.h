//
//  CoreUI_Button.h
//  CoreEngine3D
//
//  Created by JodyMcAdams on 12/19/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef CoreEngine3D_CoreUI_Button_h
#define CoreEngine3D_CoreUI_Button_h

#include "GraphicsTypes.h"
#include "OpenGLRenderer.h"
#include "CoreInput_DeviceInputState.h"

enum ButtonState
{
	ButtonState_Neutral,
	ButtonState_Pressed,
	ButtonState_Num,
};

enum CoreUI_AttachSide
{
	CoreUI_AttachSide_None,
	CoreUI_AttachSide_Left,
	CoreUI_AttachSide_Right,
	CoreUI_AttachSide_Top,
	CoreUI_AttachSide_Bottom,
	CoreUI_AttachSide_TopLeft,
	CoreUI_AttachSide_BottomLeft,
	CoreUI_AttachSide_TopRight,
	CoreUI_AttachSide_BottomRight,
	CoreUI_AttachSide_Num,
};

struct CoreUI_Button
{
public:
	void Init(u32 width, u32 height, CoreUI_AttachSide attachSide, s32 offsetX, s32 offsetY, u32* textureHandle, s32 value, void (*callback)(s32));
	void PressButton(TouchState touchState, vec2* pTouchPos);
	
	void Show();
	void Hide();
	void SetActive(bool isActive);
	
	u32 m_width;
	u32 m_height;
	
	u32 m_halfWidth;
	u32 m_halfHeight;
	
	vec3 m_position;

	ButtonState m_buttonState;
	CoreUI_AttachSide m_attachSide;
	
	RenderableObject3D m_r3D_button;
	
	s32 m_value;
	
	
	
	void (*m_callback)(s32);
	
private:
	bool m_isActive;
};

#endif
