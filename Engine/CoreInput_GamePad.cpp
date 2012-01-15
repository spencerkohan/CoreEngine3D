//
//  CoreInput_Joystick.cpp
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 12/28/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CoreInput_GamePad.h"

#include "MathUtil.h"
#include "matrix.h"

#include "OpenGLRenderer.h"

GamePadJoystick* GamePad::AddJoystick(GamePadJoystickType type, JoystickMode stickMode, const JoystickBoundingBox& boundingBox, f32 radius, f32 radiusBuffer, f32 deadZone, f32 minValue)
{
	if(numJoysticks == GamePadJoystickType_Num)
	{
		return NULL;
	}
	
	GamePadJoystick* pJoystick = &joysticks[numJoysticks];
	
	pJoystick->joystickType = type;
	
	pJoystick->boundingBox = boundingBox;
	
	pJoystick->stickCenter.x = (boundingBox.X0 + boundingBox.X1)/2.0f;
	pJoystick->stickCenter.y = (boundingBox.Y0 + boundingBox.Y1)/2.0f;
	
	CopyVec2(&pJoystick->stickJoystickPos, &pJoystick->stickCenter);
	pJoystick->touchIndex = -1;
	
	pJoystick->deadZone = deadZone;
	pJoystick->stickMode = stickMode;
	pJoystick->stickRadius = radius;
	pJoystick->stickRadiusBuffer = radiusBuffer;
	pJoystick->minValue = minValue;
	
	pJoystick->value.x = 0.0f;
	pJoystick->value.y = 0.0f;
	
	++numJoysticks;
	
	return pJoystick;
}


GamePadButton* GamePad::AddButton(GamePadButtonType type, const JoystickBoundingBox& boundingBox, f32 radius)
{
	if(numButtons == GamePadButtonType_Num)
	{
		return NULL;
	}
	
	GamePadButton* pButton = &buttons[numButtons];
	
	pButton->buttonType = type;
	
	pButton->boundingBox = boundingBox;
	
	pButton->buttonCenter.x = (boundingBox.X0 + boundingBox.X1)/2.0f;
	pButton->buttonCenter.y = (boundingBox.Y0 + boundingBox.Y1)/2.0f;
	pButton->touchIndex = -1;
	pButton->buttonRadius = radius;
	
	pButton->timePressed = 0.0f;
	pButton->touchIsInsideBoundingBox_INTERNAL = 0;
	pButton->value = 0;
	
	++numButtons;
	
	return pButton;
}

GamePadJoystick* GamePad::GetJoystickByType(GamePadJoystickType type)
{
	for(u32 joystickIDX=0; joystickIDX<numJoysticks; ++joystickIDX)
	{
		GamePadJoystick* pJoystick = &joysticks[joystickIDX];
		if(pJoystick->joystickType == type)
		{
			return pJoystick;
		}
	}
	
	return NULL;
}


GamePadButton* GamePad::GetGamePadButtonByType(GamePadButtonType type)
{
	for(u32 buttonIDX=0; buttonIDX<numButtons; ++buttonIDX)
	{
		GamePadButton* pButton = &buttons[buttonIDX];
		if(pButton->buttonType == type)
		{
			return pButton;
		}
	}
	
	return NULL;
}


void GamePad::DebugDraw()
{
	for(u32 i=0; i<numJoysticks; ++i)
	{
		GamePadJoystick* pJoystick = &joysticks[i];
		
		const vec3 topLeft = {pJoystick->boundingBox.X0,pJoystick->boundingBox.Y0,0.0f};
		const vec3 botRight = {pJoystick->boundingBox.X1,pJoystick->boundingBox.Y1,0.0f};
		const vec3 topRight = {botRight.x,topLeft.y,0.0f};
		const vec3 botLeft = {topLeft.x,botRight.y,0.0f};
		
		const vec4 boxColor = {1.0f,0.0f,1.0f,1.0f};
		
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&topLeft,&topRight,&boxColor);
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&topRight,&botRight,&boxColor);
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&botRight,&botLeft,&boxColor);
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&botLeft,&topLeft,&boxColor);
		
		const vec4 circleColor = {1.0f,1.0f,0.0f,1.0f};
		const vec3 circlePos = {pJoystick->stickCenter.x,pJoystick->stickCenter.y,0.0f};
		GLRENDERER->DEBUGDRAW_DrawCircleXY(DebugDrawMode_Screen2D, &circlePos, pJoystick->stickRadius, &circleColor);
		
		const vec4 circleColorStick = {1.0f,0.0f,0.0f,1.0f};
		const vec3 circlePosStick = {pJoystick->stickJoystickPos.x,pJoystick->stickJoystickPos.y,0.0f};
		
		GLRENDERER->DEBUGDRAW_DrawCircleXY(DebugDrawMode_Screen2D,&circlePosStick,pJoystick->stickRadius/4.0f,&circleColorStick);
	}
	
	for(u32 i=0; i<numButtons; ++i)
	{
		GamePadButton* pButton = &buttons[i];
		
		const vec3 topLeft = {pButton->boundingBox.X0,pButton->boundingBox.Y0,0.0f};
		const vec3 botRight = {pButton->boundingBox.X1,pButton->boundingBox.Y1,0.0f};
		const vec3 topRight = {botRight.x,topLeft.y,0.0f};
		const vec3 botLeft = {topLeft.x,botRight.y,0.0f};
		
		const vec4 boxColor = {1.0f,0.6f,1.0f,1.0f};
		
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&topLeft,&topRight,&boxColor);
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&topRight,&botRight,&boxColor);
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&botRight,&botLeft,&boxColor);
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_Screen2D,&botLeft,&topLeft,&boxColor);
		
		vec4 circleColor;
		f32 circleRadius;
		if(pButton->value & (1<<GamePadButtonState_On))
		{
			SetVec4(&circleColor,0,1,0,1);
			circleRadius = pButton->buttonRadius*1.5f;
		}
		else if(pButton->value & (1<<GamePadButtonState_Released))
		{
			SetVec4(&circleColor,1,1,0,1);
			circleRadius = pButton->buttonRadius*2.5f;
		}
		else
		{
			circleRadius = pButton->buttonRadius;
			SetVec4(&circleColor,1,0,0,1);
		}
		
		const vec3 circlePosStick = {pButton->buttonCenter.x,pButton->buttonCenter.y,0.0f};
		GLRENDERER->DEBUGDRAW_DrawCircleXY(DebugDrawMode_Screen2D,&circlePosStick, circleRadius, &circleColor);
	}
}


void GamePadJoystick::SetStickCenter(const vec2* pTouchPos)
{
    //Do not process touch if it's outside of the box
    if(pTouchPos->x < boundingBox.X0
       || pTouchPos->x > boundingBox.X1
       || pTouchPos->y < boundingBox.Y0
       || pTouchPos->y > boundingBox.Y1)
    {
        return;
    }
    
    CopyVec2(&stickCenter, pTouchPos);
    
    ConstrainStick();
}


void GamePadJoystick::ConstrainStick()
{
    //CONSTRAIN THE STICK
    const f32 radiusWithBuffer = stickRadius+stickRadiusBuffer;
    
    const f32 distFromTop = stickCenter.y-boundingBox.Y0;
    const f32 distFromLeft = stickCenter.x-boundingBox.X0;
    
    const f32 distFromBot = boundingBox.Y1-stickCenter.y;
    const f32 distFromRight = boundingBox.X1-stickCenter.x;
    
    
    if(distFromLeft < radiusWithBuffer)
    {
        stickCenter.x = boundingBox.X0+radiusWithBuffer;
    }
    
    if(distFromRight < radiusWithBuffer)
    {
        stickCenter.x = boundingBox.X1-radiusWithBuffer;
    }
    
    if(distFromTop < radiusWithBuffer)
    {
        stickCenter.y = boundingBox.Y0+radiusWithBuffer;
    }
    
    if(distFromBot < radiusWithBuffer)
    {
        stickCenter.y = boundingBox.Y1-radiusWithBuffer;
    }
}


void GamePadJoystick::Update(const vec2* pTouchPos)
{    
    const bool insideStickArea = TouchIsInsideBoundingBox(pTouchPos,&boundingBox,stickRadiusBuffer);
    
    CopyVec2(&stickJoystickPos, pTouchPos);
    
    vec2 distVec;
    SubVec2(&distVec,pTouchPos,&stickCenter);
    
    //Only move stick around if the user is touching inside the stick area, else
    //prevent it from moving but still use the input
    
    const f32 distMagSq = MagnitudeSqVec2(&distVec);
    const f32 epsilon = 0.01f;
    if(distMagSq > epsilon && distMagSq > stickRadius*stickRadius)
    {
        if(insideStickArea)
        {
            //printf("Exited the radius of the stick\n");
            
            if(stickMode == JoystickMode_SquareAnalog)
            {
                if(distVec.x > stickRadius)
                {
                    distVec.x = stickRadius;
					
                    stickCenter.x = pTouchPos->x - stickRadius;
                }
                else if(distVec.x < -stickRadius)
                {
                    distVec.x = -stickRadius;
                    
                    stickCenter.x = pTouchPos->x + stickRadius;
                }
                
                if(distVec.y > stickRadius)
                {
                    distVec.y = stickRadius;
                    
                    stickCenter.y = pTouchPos->y - stickRadius;
                }
                else if(distVec.y < -stickRadius)
                {
                    distVec.y = -stickRadius;
                    
                    stickCenter.y = pTouchPos->y + stickRadius;
                }
            }
            else
            {
                const f32 distMag = sqrtf(distMagSq);
                vec2 distVecNorm;
                ScaleVec2(&distVecNorm,&distVec,-stickRadius/distMag);
                AddVec2(&stickCenter, pTouchPos, &distVecNorm);
            }
            
        }
        else
        {
            if(distVec.x > stickRadius)
            {
                stickCenter.x += distVec.x-stickRadius; 
            }
            else if(distVec.x < -stickRadius)
            {
                stickCenter.x -= -(distVec.x+stickRadius);
            }
            
            if(distVec.y > stickRadius)
            {
                stickCenter.y += distVec.y-stickRadius; 
            }
            else if(distVec.y < -stickRadius)
            {
                stickCenter.y -= -(distVec.y+stickRadius);
            }
        }
    }
    
    
    ConstrainStick();
    
    switch(stickMode)
    {
        case JoystickMode_DPAD:
        {
            //Update stick values
            f32 distX = distVec.x/stickRadius;
            f32 distY = distVec.y/stickRadius;
            
            const f32 xDir = distX > 0.0f ? 1.0f:-1.0f;
            const f32 yDir = distY > 0.0f ? 1.0f:-1.0f;
            
            distX = fabsf(distX);
            distY = fabsf(distY);

            if(distX > deadZone)
            {
                distX = 1.0f;
            }
            else
            {
                distX = 0.0f;
            }
            
            if(distY > deadZone)
            {
                distY = 1.0f;
            }
            else
            {
                distY = 0.0f;
            }
            
            
            value.x = distX*xDir;
            value.y = distY*yDir;
            
            break;
        }
        case JoystickMode_Analog:
        {
            //Update stick values
            
            vec2 stickVec;
            
            f32 distMag = NormalizeVec2(&stickVec, &distVec);
            
            if(distMag > 0.0f)
            {
                distMag = MinF(distMag/stickRadius,1.0f);

                const f32 oneMinDeadZone = 1.0f-deadZone;
                
                if(distMag < deadZone)
                {
                    distMag = 0.0f;
                }
                else
                {
                    const f32 stickT = (distMag-deadZone)/oneMinDeadZone;
                    distMag = Lerp(minValue, 1.0f, stickT);
                }
                
                ScaleVec2(&value,&stickVec,distMag);
            }
            else
            {
                value.x = 0.0f;
                value.y = 0.0f;
            }
            
            break;
        }
        case JoystickMode_SquareAnalog:
        {
            //Update stick values
            f32 distX = distVec.x/stickRadius;
            f32 distY = distVec.y/stickRadius;
            
            const f32 xDir = distX > 0.0f ? 1.0f:-1.0f;
            const f32 yDir = distY > 0.0f ? 1.0f:-1.0f;
            
            distX = MinF(fabsf(distX),1.0f);
            distY = MinF(fabsf(distY),1.0f);
            
            const f32 oneMinDeadZone = 1.0f-deadZone;
            
            if(distX < deadZone)
            {
                distX = 0.0f;
            }
            else
            {
                const f32 stickT = (distX-deadZone)/oneMinDeadZone;
                distX = xDir*Lerp(minValue, 1.0f, stickT);
            }
            
            if(distY < deadZone)
            {
                distY = 0.0f;
            }
            else
            {
                const f32 stickT = (distY-deadZone)/oneMinDeadZone;
                distY = yDir*Lerp(minValue, 1.0f, stickT);
            }
            
            value.x = distX;
            value.y = distY;
            
            break;
        }
        case JoystickMode_NonAnalog:
        {
            //Update stick values
            
            vec2 stickVec;
            
            f32 distMag =  MinF(NormalizeVec2(&stickVec, &distVec)/stickRadius,1.0f);
            
            printf("distMag: %f, deadZone: %f\n",distMag,deadZone);
            if(distMag > deadZone)
            {
                CopyVec2(&value,&stickVec);
            }
            else
            {
                value.x = 0.0f;
                value.y = 0.0f;
            }
            
            break;
        }
    }
    
    
    //DEBUG
    //printf("Stick Value: <%f,%f>\n",m_stickLeft.value.x,m_stickLeft.value.y);
}


void GamePad::Init(DeviceInputState* pDeviceInputState)
{
	m_pDeviceInputState = pDeviceInputState;
	
	numButtons = 0;
	numJoysticks = 0;
}


void GamePad::Update()
{
	//Reset flags for all buttons that need resetting
    for(u32 buttonIDX=0; buttonIDX<numButtons; ++buttonIDX)
    {
        //Clear out one-frame button flags from last update
        buttons[buttonIDX].value &= ~(1<<GamePadButtonState_Released);
         buttons[buttonIDX].value &= ~(1<<GamePadButtonState_FirstFrameOn);
    }
    
    
	for (s32 i=0; i<MAX_MULTITOUCH; ++i)
	{
		
		TouchState touchState = m_pDeviceInputState->GetTouchState(i);
		
		vec2 posCurr;
		m_pDeviceInputState->GetTouchPos_Curr(i,&posCurr);
		
		vec2 velocity;
        m_pDeviceInputState->GetTouchVelocity(i,&velocity);
        
		//TODO: expand to support more than one joystick
		GamePadJoystick* pJoystick = &joysticks[0];
		
		const bool touchIsLeftStick = TouchIsInsideBoundingBox(&posCurr,&pJoystick->boundingBox,0.0f);
        
        //Find button
        
        GamePadButton* pButton = NULL;
        
        //Only look for buttons if we are not using the stick
        if(touchIsLeftStick == false)
        {
            for(u32 buttonIDX=0; buttonIDX<numButtons; ++buttonIDX)
            {
                if(buttons[buttonIDX].touchIndex == i)
                {
                    //If we found the button with this touch index and it's inside the bounding box
                    if(TouchIsInsideBoundingBox(&posCurr,&buttons[buttonIDX].boundingBox,0.0f))
                    {
                        pButton = &buttons[buttonIDX];
                        
                    }
                    //else the touch is outside the bounding box so reset the button
                    else
                    {
                        //printf("Force reset button: %d\n",i);
                        buttons[buttonIDX].value = 0;
                        buttons[buttonIDX].touchIndex = -1;
                    }
                    
                    break;
                }
            }
            
            //If there was not an existing button
            if(pButton == NULL)
            {
                //If we didn't already have a button, find the closest
                //one in the button group
                for(u32 buttonIDX=0; buttonIDX<numButtons; ++buttonIDX)
                {
                    if(TouchIsInsideBoundingBox(&posCurr,&buttons[buttonIDX].boundingBox,0.0f))
                    {
                        pButton = &buttons[buttonIDX];
						
                        break;
                    }
                }
            }
        }
        //No matter what we're going to have to clear out these buttons
        //This can probably be cleaned up a bit
        else
        {
            for(u32 buttonIDX=0; buttonIDX<numButtons; ++buttonIDX)
            {
                if(buttons[buttonIDX].touchIndex == i)
                {
                    //If we found the button with this touch index and it's inside the bounding box
                    if(TouchIsInsideBoundingBox(&posCurr,&buttons[buttonIDX].boundingBox,0.0f) == false)
                    {
                        //printf("Force reset button: %d\n",i);
                        buttons[buttonIDX].value = 0;
                        buttons[buttonIDX].touchIndex = -1;
                    }
                    
                    break;
                }
            }
        }
        
		switch (touchState)
		{
			case TouchState_Began:
                //Set touch index for button
                
            case TouchState_Moving:
            {
                
                //printf("Touches: Began -> %d\n",i);
                
                //Set touch index for stick
                if(touchIsLeftStick
                   && pJoystick->touchIndex == -1)
                {
                    pJoystick->touchIndex = i;
                    //[self SetLeftStickCenter:&posCurr];
                }  
                
                
            }
                //fall through
            case TouchState_Stationary:
                //fall through
				
			{
                if(pButton && pButton->touchIndex == -1)
                {    
                    //printf("Pressed button\n");
                    pButton->touchIndex = i;
                    pButton->value |= 1<<GamePadButtonState_On;
                    pButton->value |= 1<<GamePadButtonState_FirstFrameOn;
                    pButton->timePressed = 0.0f;
                }
                
                if(pButton && pButton->touchIndex == i)
                {    
                    //printf("Pressed button\n");
                    pButton->value |= 1<<GamePadButtonState_On;
                }
                
                //Only check the stick if the touch index is the assigned index of the stick
                //Only update player stuff is stick input is within the stick box
                if(pJoystick->touchIndex == i)
                {
                    pJoystick->Update(&posCurr);
                    
                    //g_game_Joystick_Left_Deadzone
                    
                }
                
                if(pButton && pButton->touchIndex == i)
                {    
                    //printf("Pressed button\n");
                    pButton->value |= 1<<GamePadButtonState_On;
                }
                
                break;
			}
			case TouchState_Cancelled:
                //fall through
			case TouchState_Ended:
			{		
                //printf("Touches: Cancelled/Ended -> %d\n",i);
                //Reset stick
                if(pJoystick->touchIndex == i)
                {
                    pJoystick->value.x = 0.0f;
                    pJoystick->value.y = 0.0f;
                    
                    pJoystick->touchIndex = -1;
                    //CopyVec3(&m_playerData[0].velocity, &vec3_zero);
                }
                
                //Reset button
                if(pButton && pButton->touchIndex == i)
                {
                    //printf("Released button\n");
                    pButton->touchIndex = -1;
                    pButton->value &= ~(1<<GamePadButtonState_On);
                    pButton->value |= 1<<GamePadButtonState_Released;
                }
				
				//	printf("Touch %d -Ended- <%f,%f>\n",i+1,posCurr.x,posCurr.y);
				
				// Break links
				
				//const BOOL isATap = [pEAGLView GetTouchIsTap:i:TOUCH_MAX_FRAMECOUNT_FOR_TAP];
				
				
                //m_playerData[0].velocity.x = velocity.x*g_game_tossSpeedMultX;
                //m_playerData[0].velocity.y = velocity.y*g_game_tossSpeedMultY;
                
                //printf("Tossed! Velocity: %f, %f\n",m_playerData[0].velocity.x,m_playerData[0].velocity.y);
                
                
                
				break;
			}
			case TouchState_None:
			{
				//Let's not spam ok?
				//printf("Touch %d -None-\n",i+1);
				break;
			}
		}
	}
}


//Helpers
bool TouchIsInsideBoundingBox(const vec2* pTouchPos, JoystickBoundingBox* pBox, f32 bufferArea)
{
    //Do not process touch if it's outside of the box
    if(pTouchPos->x < pBox->X0+bufferArea
       || pTouchPos->x > pBox->X1-bufferArea
       || pTouchPos->y < pBox->Y0+bufferArea
       || pTouchPos->y > pBox->Y1-bufferArea)
    {
        return false;
    }
    
    return true;
}


