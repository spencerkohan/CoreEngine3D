//
//  TouchInputIOS.m
//  Game(iOS)
//
//  Created by JodyMcAdams on 12/16/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "TouchInputIOS.h"
#include "MathUtil.h"

@implementation TouchInputIOS

- (id) init
{	
	for (u32 i=0; i<MAX_MULTITOUCH; ++i)
	{
		//m_pTouches[i] = nil;
		m_touchStates[i] = TouchState_None;
		m_touchStateWasUpdated[i] = false;
	}

    SetVec3(&m_accelerometerValue,0.0f,0.0f,0.0f);
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
    [self SetAccelerometerIsUsed:NO];

	return self;
}


- (void)Update:(f32) timeElapsed
{
	
}

- (TouchState)GetTouchState:(u32)touchIndex
{
	return m_touchStates[touchIndex];
}


- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView
{
#ifdef DEBUG_PRINTTOUCHEVENTS
	NSLog(@"EAGLVIEW: Touches began\n");
#endif
	
	for (UITouch *touch in touches)
	{
		s32 touchIndex = -1;
		UITouch* pFoundTouch = nil;
		
		//Find an existing touch or a spot for a new one
		for (u32 i=0; i<MAX_MULTITOUCH; ++i)
		{
			//If found
			if (m_pTouches[i] == touch)
			{
				touchIndex = i;
				pFoundTouch = m_pTouches[i];
#ifdef DEBUG_PRINTTOUCHEVENTS
				//printf("**Previous touch found! %d\n",(int)pFoundTouch);
#endif
				
				break;
			}
			//Else find a free index if we need one
			else if(touchIndex == -1 && m_pTouches[i] == nil)
			{
				touchIndex = i;
			}
		}
		
		//No existing touch found so add it in
		if(pFoundTouch == nil && touchIndex != -1)
		{
			if(m_pTouches[touchIndex])
			{
				[m_pTouches[touchIndex] release];
			}
			m_pTouches[touchIndex] = touch;
			[m_pTouches[touchIndex] retain];
			
#ifdef DEBUG_PRINTTOUCHEVENTS
			//printf("*New touch created!	%d\n",(int)touch);
#endif
		}
		
		//Should be impossible for touchIndex to be -1 by now
		CGPoint touchCoordinates = [touch locationInView:pView];
		
		m_touchStateWasUpdated[touchIndex] = true;
		
		m_touchBeginPoints[touchIndex].x = touchCoordinates.x;
		m_touchBeginPoints[touchIndex].y = touchCoordinates.y;
		
		double currTime = CFAbsoluteTimeGetCurrent();
		m_touchPrevTimes[touchIndex] = currTime;
		m_touchCurrTimes[touchIndex] = currTime;
		
        m_touchCurrentPoints[touchIndex].x = touchCoordinates.x;
        m_touchCurrentPoints[touchIndex].y = touchCoordinates.y;
		m_touchPrevPoints[touchIndex].x = touchCoordinates.x;
		m_touchPrevPoints[touchIndex].y = touchCoordinates.y;
		
		m_touchStates[touchIndex] = TouchState_Began;
	}
}


- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView
{
#ifdef DEBUG_PRINTTOUCHEVENTS
	NSLog(@"EAGLVIEW: Touches moved\n");
#endif
	
	for (UITouch *touch in touches)
	{
		s32 touchIndex = -1;
		UITouch* pFoundTouch = nil;
		
		//Find an existing touch or a spot for a new one
		for (u32 i=0; i<MAX_MULTITOUCH; ++i)
		{
			//If found
			if (m_pTouches[i] == touch)
			{
				touchIndex = i;
				pFoundTouch = m_pTouches[i];
#ifdef DEBUG_PRINTTOUCHEVENTS
				//printf("**Previous touch found! %d\n",(int)pFoundTouch);
#endif
				
				break;
			}
		}
		
		if(touchIndex == -1)
		{
			return;
		}
		
		if (pFoundTouch != nil)
		{
			CGPoint touchCoordinates = [touch locationInView:pView];
			
			m_touchStateWasUpdated[touchIndex] = true;
			
			m_touchPrevPoints[touchIndex].x = m_touchCurrentPoints[touchIndex].x;
			m_touchPrevPoints[touchIndex].y = m_touchCurrentPoints[touchIndex].y;
			m_touchCurrentPoints[touchIndex].x = touchCoordinates.x;
			m_touchCurrentPoints[touchIndex].y = touchCoordinates.y;
			
			m_touchPrevTimes[touchIndex] = m_touchCurrTimes[touchIndex];
			m_touchCurrTimes[touchIndex] = CFAbsoluteTimeGetCurrent();
			
			m_touchStates[touchIndex] = TouchState_Moving;
		}
#ifdef DEBUG_PRINTTOUCHEVENTS
		//else
		//{
		//printf("ERROR: touchesMoved-> Somehow your touch got lost...\n");
		//}
#endif
	}
}


- (void) GetTouchVelocity:(u32)touchIndex:(vec2*)out_velocity
{
	TouchState touchState = [self GetTouchState:touchIndex];
	
	switch (touchState)
	{
		case TouchState_Moving:
		case TouchState_Ended:
		{
			SubVec2(out_velocity, &m_touchCurrentPoints[touchIndex],&m_touchPrevPoints[touchIndex]);
			const f32 time = m_touchCurrTimes[touchIndex]-m_touchPrevTimes[touchIndex];
			ScaleVec2_Self(out_velocity, 1.0f/time);

			break;
		}
		default:
		{
			SetVec2(out_velocity, 0.0f, 0.0f);
		}
	}
}


- (void) GetTouchPos:(u32)touchIndex:(vec2*)out_posCurr
{
	CopyVec2(out_posCurr,&m_touchCurrentPoints[touchIndex]);
}


- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView
{
#ifdef DEBUG_PRINTTOUCHEVENTS
	NSLog(@"EAGLVIEW: Touches moved\n");
#endif
	
	for (UITouch *touch in touches)
	{
		s32 touchIndex = -1;
		UITouch* pFoundTouch = nil;
		
		//Find an existing touch or a spot for a new one
		for (u32 i=0; i<MAX_MULTITOUCH; ++i)
		{
			//If found
			if (m_pTouches[i] == touch)
			{
				touchIndex = i;
				pFoundTouch = m_pTouches[i];
#ifdef DEBUG_PRINTTOUCHEVENTS
				//printf("**Previous touch found! %d\n",(int)pFoundTouch);
#endif
				
				break;
			}
		}
		
		if(touchIndex == -1)
		{
			return;
		}
		
		if (pFoundTouch != nil)
		{
			CGPoint touchCoordinates = [touch locationInView:pView];
			
			m_touchStateWasUpdated[touchIndex] = true;
			
			//m_touchPrevPoints[touchIndex].x = m_touchCurrentPoints[touchIndex].x;
			//m_touchPrevPoints[touchIndex].y = m_touchCurrentPoints[touchIndex].y;
			m_touchCurrentPoints[touchIndex].x = touchCoordinates.x;
			m_touchCurrentPoints[touchIndex].y = touchCoordinates.y;
			
			//m_touchPrevTimes[touchIndex] = m_touchCurrTimes[touchIndex];
			m_touchCurrTimes[touchIndex] = CFAbsoluteTimeGetCurrent();
			
			m_touchStates[touchIndex] = TouchState_Ended;
		}
#ifdef DEBUG_PRINTTOUCHEVENTS
		//else
		//{
		//printf("ERROR: touchesMoved-> Somehow your touch got lost...\n");
		//}
#endif
	}
}


- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*)pVIew
{
#ifdef DEBUG_PRINTTOUCHEVENTS
	NSLog(@"EAGLVIEW: Touches cancelled\n");
#endif
	
	for (UITouch *touch in touches)
	{
		s32 touchIndex = -1;
		//UITouch* pFoundTouch = nil;
		
		//Find an existing touch or a spot for a new one
		for (u32 i=0; i<MAX_MULTITOUCH; ++i)
		{
			//If found
			if (m_pTouches[i] == touch)
			{
				touchIndex = i;
				//pFoundTouch = m_pTouches[i];
#ifdef DEBUG_PRINTTOUCHEVENTS
				//printf("**Previous touch found! %d\n",(int)pFoundTouch);
#endif
				
				break;
			}
		}
		
		//Should be impossible for touchIndex to be -1 by now
		m_touchStates[touchIndex] = TouchState_Cancelled;
		
		[m_pTouches[touchIndex] release];
		m_pTouches[touchIndex] = nil;
		
	}
}


-(void)PostGameUpdate
{
	//Complete input processing after updating the game
	for(u32 i=0; i<MAX_MULTITOUCH; ++i)
	{
		if (m_pTouches[i] != nil)
		{
			if(m_touchStateWasUpdated[i] == true)
			{
				m_touchStateWasUpdated[i] = false;
				if(m_touchStates[i] == TouchState_Began)
				{
					m_touchStates[i] = TouchState_Stationary;
				}
			}
			else
			{
				m_touchStates[i] = TouchState_Stationary;
			}
			
			switch(m_touchStates[i])
			{
				case TouchState_Ended:
				case TouchState_Cancelled:
				{
					if(m_pTouches[i])
					{
						[m_pTouches[i] release];
						m_pTouches[i] = nil;
					}	
				}
				default:
				{
					break;
				}
			}
		}
		
		//If found
		if (m_touchStates[i] ==  TouchState_Ended || m_touchStates[i] == TouchState_Cancelled)
		{
			m_touchStates[i] = TouchState_None;
		}
	}
}

- (void) SetAccelerometerIsUsed:(BOOL)useAccelerometer
{
	if(useAccelerometer)
    {
        const f32 kAccelerometerFrequency = 30.0f;
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
    }
    else
    {
        const f32 kAccelerometerFrequency = 1.0f;
        [[UIAccelerometer sharedAccelerometer] setUpdateInterval:(1.0 / kAccelerometerFrequency)];
    }
}


- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration
{
	const f32 kFilteringFactor	= 0.1f;
    
	//Use a basic low-pass filter to only keep the gravity in the accelerometer values
	m_accelerometerValue.x = acceleration.x * kFilteringFactor + m_accelerometerValue.x * (1.0 - kFilteringFactor);
	m_accelerometerValue.y = acceleration.y * kFilteringFactor + m_accelerometerValue.y * (1.0 - kFilteringFactor);
	m_accelerometerValue.z = acceleration.z * kFilteringFactor + m_accelerometerValue.z * (1.0 - kFilteringFactor);
}

#if TARGET_IPHONE_SIMULATOR
const vec3 fakeAccel = {0.0f,-1.0f,0.0f};
- (const vec3*) GetAccelerometerValue
{
    return &fakeAccel;
}
#else
- (const vec3*) GetAccelerometerValue
{
    return &m_accelerometerValue;
}
#endif

@end
