//
//  CoreUI_Button.cpp
//  CoreEngine3D
//
//  Created by JodyMcAdams on 12/19/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CoreUI_Button.h"
#include "matrix.h"

void CoreUI_Button::SetActive(bool isActive)
{
	m_isActive = isActive;
}

void CoreUI_Button::PressButton(TouchState touchState, vec2* pTouchPos)
{
	if(!m_isActive)
	{
		return;
	}
	
	const f32 x = pTouchPos->x;
	const f32 y = pTouchPos->y;
	
	
	const bool touchIsInside =  x > m_position.x-m_halfWidth && x < m_position.x+m_halfWidth
		&& y > m_position.y-m_halfHeight && y < m_position.y+m_halfHeight;
	
	if(touchIsInside)
	{
		switch(touchState)
		{
			case TouchState_Began:
			{
				m_buttonState = ButtonState_Pressed;
				
				break;
			}
			case TouchState_Moving:
			{
				m_buttonState = ButtonState_Pressed;
				
				break;
			}
			case TouchState_Ended:
			{
				if(m_buttonState == ButtonState_Pressed)
				{
					m_buttonState = ButtonState_Neutral;
					
					m_callback(m_value);
				}
				
				break;
			}
			case TouchState_Stationary:
			{
				m_buttonState = ButtonState_Pressed;
				
				break;
			}
			case TouchState_Cancelled:
			{
				m_buttonState = ButtonState_Neutral;
				
				break;
			}
			default:
			{
				break;
			}
		}
	}
	else
	{
		m_buttonState = ButtonState_Neutral;
	}
}


void CoreUI_Button::Init(u32 width, u32 height, CoreUI_AttachSide attachSide, s32 offsetX, s32 offsetY, u32* textureHandle, s32 value, void (*callback)(s32))
{
	m_isActive = true;
	
	m_width = width;
	m_height = height;
	
	m_halfWidth = width/2;
	m_halfHeight = height/2;
	
	m_attachSide = attachSide;

	m_value = value;
	
	m_callback = callback;
	
	switch(attachSide)
	{
		case CoreUI_AttachSide_None:
		{
			m_position.x = offsetX;
			m_position.y = offsetY;
			
			break;
		}
		case CoreUI_AttachSide_Left:
		{
			m_position.x = offsetX;
			m_position.y = offsetY;
			
			break;
		}
		case CoreUI_AttachSide_Right:
		{
			m_position.x = GLRENDERER->screenWidth_points+offsetX;
			m_position.y = offsetY;
			
			break;
		}
		case CoreUI_AttachSide_Top:
		{
			m_position.x = GLRENDERER->screenWidth_points/2+offsetX;
			m_position.y = offsetY;
			
			break;
		}
		case CoreUI_AttachSide_Bottom:
		{
			m_position.x = GLRENDERER->screenWidth_points/2+offsetX;
			m_position.y = GLRENDERER->screenHeight_points+offsetY;
			
			break;
		}
		case CoreUI_AttachSide_TopLeft:
		{
			m_position.x = offsetX;
			m_position.y = offsetY;
			
			break;
		}
		case CoreUI_AttachSide_BottomLeft:
		{
			m_position.x = offsetX;
			m_position.y = GLRENDERER->screenHeight_points+offsetY;
			
			break;
		}
		case CoreUI_AttachSide_TopRight:
		{
			m_position.x = GLRENDERER->screenWidth_points+offsetX;
			m_position.y = offsetY;
			
			break;
		}
		case CoreUI_AttachSide_BottomRight:
		{
			m_position.x = GLRENDERER->screenWidth_points+offsetX;
			m_position.y = GLRENDERER->screenHeight_points+offsetY;
			
			break;
		}
		default:
		{
			break;
		}
	}
	
	GLRENDERER->InitRenderableObject3D(&m_r3D_button, &g_Square1x1_modelData, MT_TextureOnlySimple, textureHandle, NULL, RenderLayer_UI, View_0, RenderFlagDefaults_2DTexture_NoAlpha);
	
	GLRENDERER->AddRenderableObject3DToList(&m_r3D_button);

	mat4f_LoadScaleFromFloats(m_r3D_button.worldMat,width,height,1.0f);
	
	vec3* pPos = mat4f_GetPos(m_r3D_button.worldMat);
	pPos->x = m_position.x;
	pPos->y = m_position.y;
	pPos->z = 0.0f;
}


void CoreUI_Button::Show()
{
	m_r3D_button.geom.material.flags |= RenderFlag_Visible;
}


void CoreUI_Button::Hide()
{
	m_r3D_button.geom.material.flags &= ~RenderFlag_Visible;
}
