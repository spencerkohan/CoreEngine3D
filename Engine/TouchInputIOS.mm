//
//  TouchInputIOS.m
//  Game(iOS)
//
//  Created by JodyMcAdams on 12/16/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import "TouchInputIOS.h"
#include "MathUtil.h"
#include "OpenGLRenderer.h"

@implementation TouchInputIOS

- (id) init:(DeviceInputState*) pDeviceInputState
{	
	m_orientation = UIInterfaceOrientationPortrait;
	
	m_inputState = pDeviceInputState;
	
	for (u32 i=0; i<MAX_MULTITOUCH; ++i)
	{
		//m_pTouches[i] = nil;
		m_inputState->m_touchStates[i] = TouchState_None;
		m_inputState->m_touchStateWasUpdated[i] = false;
	}

    SetVec3(&m_inputState->m_accelerometerValue,0.0f,0.0f,0.0f);
	[[UIAccelerometer sharedAccelerometer] setDelegate:self];
    [self SetAccelerometerIsUsed:NO];

	m_inputScale = [[UIDevice currentDevice] userInterfaceIdiom] == UIUserInterfaceIdiomPhone ? 2:1;
	
	return self;
}


- (void)Update:(f32) timeElapsed
{
	
}

- (void)SetAcceleration:(const vec3*)pAccel
{
	CopyVec3(&m_inputState->m_accelerometerValue,pAccel);
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
		touchCoordinates.x *= m_inputScale;
		touchCoordinates.y *= m_inputScale;
		
		m_inputState->m_touchStateWasUpdated[touchIndex] = true;
		
		m_inputState->m_touchBeginPoints[touchIndex].x = touchCoordinates.x;
		m_inputState->m_touchBeginPoints[touchIndex].y = touchCoordinates.y;
		
		double currTime = CFAbsoluteTimeGetCurrent();
		m_inputState->m_touchPrevTimes[touchIndex] = currTime;
		m_inputState->m_touchCurrTimes[touchIndex] = currTime;
		
        m_inputState->m_touchCurrentPoints[touchIndex].x = touchCoordinates.x;
        m_inputState->m_touchCurrentPoints[touchIndex].y = touchCoordinates.y;
		m_inputState->m_touchPrevPoints[touchIndex].x = touchCoordinates.x;
		m_inputState->m_touchPrevPoints[touchIndex].y = touchCoordinates.y;
		
		m_inputState->m_touchStates[touchIndex] = TouchState_Began;
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
			touchCoordinates.x *= m_inputScale;
			touchCoordinates.y *= m_inputScale;
			
			m_inputState->m_touchStateWasUpdated[touchIndex] = true;
			
			m_inputState->m_touchPrevPoints[touchIndex].x = m_inputState->m_touchCurrentPoints[touchIndex].x;
			m_inputState->m_touchPrevPoints[touchIndex].y = m_inputState->m_touchCurrentPoints[touchIndex].y;
			m_inputState->m_touchCurrentPoints[touchIndex].x = touchCoordinates.x;
			m_inputState->m_touchCurrentPoints[touchIndex].y = touchCoordinates.y;
			
			m_inputState->m_touchPrevTimes[touchIndex] = m_inputState->m_touchCurrTimes[touchIndex];
			m_inputState->m_touchCurrTimes[touchIndex] = CFAbsoluteTimeGetCurrent();
			
			m_inputState->m_touchStates[touchIndex] = TouchState_Moving;
		}
#ifdef DEBUG_PRINTTOUCHEVENTS
		//else
		//{
		//printf("ERROR: touchesMoved-> Somehow your touch got lost...\n");
		//}
#endif
	}
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
			touchCoordinates.x *= m_inputScale;
			touchCoordinates.y *= m_inputScale;
			
			m_inputState->m_touchStateWasUpdated[touchIndex] = true;
			
			//m_touchPrevPoints[touchIndex].x = m_touchCurrentPoints[touchIndex].x;
			//m_touchPrevPoints[touchIndex].y = m_touchCurrentPoints[touchIndex].y;
			m_inputState->m_touchCurrentPoints[touchIndex].x = touchCoordinates.x;
			m_inputState->m_touchCurrentPoints[touchIndex].y = touchCoordinates.y;
			
			//m_touchPrevTimes[touchIndex] = m_touchCurrTimes[touchIndex];
			m_inputState->m_touchCurrTimes[touchIndex] = CFAbsoluteTimeGetCurrent();
			
			m_inputState->m_touchStates[touchIndex] = TouchState_Ended;
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
		m_inputState->m_touchStates[touchIndex] = TouchState_Cancelled;
		
		[m_pTouches[touchIndex] release];
		m_pTouches[touchIndex] = nil;
		
	}
}

- (void)SetOrientation:(UIInterfaceOrientation)orientation
{
	m_orientation = orientation;
}


-(void)PostGameUpdate
{
	//Complete input processing after updating the game
	for(u32 i=0; i<MAX_MULTITOUCH; ++i)
	{
		if (m_pTouches[i] != nil)
		{
			if(m_inputState->m_touchStateWasUpdated[i] == true)
			{
				m_inputState->m_touchStateWasUpdated[i] = false;
				if(m_inputState->m_touchStates[i] == TouchState_Began)
				{
					m_inputState->m_touchStates[i] = TouchState_Stationary;
				}
			}
			else
			{
				m_inputState->m_touchStates[i] = TouchState_Stationary;
			}
			
			switch(m_inputState->m_touchStates[i])
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
		if (m_inputState->m_touchStates[i] ==  TouchState_Ended || m_inputState->m_touchStates[i] == TouchState_Cancelled)
		{
			m_inputState->m_touchStates[i] = TouchState_None;
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
	
    switch(m_orientation)
	{
		case UIInterfaceOrientationPortrait:
		{
			//Use a basic low-pass filter to only keep the gravity in the accelerometer values
			m_inputState->m_accelerometerValue.x = acceleration.x * kFilteringFactor + m_inputState->m_accelerometerValue.x * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.y = acceleration.y * kFilteringFactor + m_inputState->m_accelerometerValue.y * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.z = acceleration.z * kFilteringFactor + m_inputState->m_accelerometerValue.z * (1.0 - kFilteringFactor);
			
			break;
		}
		case UIInterfaceOrientationPortraitUpsideDown:
		{
			//Use a basic low-pass filter to only keep the gravity in the accelerometer values
			m_inputState->m_accelerometerValue.x = -acceleration.x * kFilteringFactor + m_inputState->m_accelerometerValue.x * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.y = -acceleration.y * kFilteringFactor + m_inputState->m_accelerometerValue.y * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.z = -acceleration.z * kFilteringFactor + m_inputState->m_accelerometerValue.z * (1.0 - kFilteringFactor);
			
			break;
		}
		case UIInterfaceOrientationLandscapeLeft:
		{
			//Use a basic low-pass filter to only keep the gravity in the accelerometer values
			m_inputState->m_accelerometerValue.x = acceleration.y * kFilteringFactor + m_inputState->m_accelerometerValue.x * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.y = acceleration.x * kFilteringFactor + m_inputState->m_accelerometerValue.y * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.z = acceleration.z * kFilteringFactor + m_inputState->m_accelerometerValue.z * (1.0 - kFilteringFactor);
			
			break;
		}
		case UIInterfaceOrientationLandscapeRight:
		{
			//Use a basic low-pass filter to only keep the gravity in the accelerometer values
			m_inputState->m_accelerometerValue.x = -acceleration.y * kFilteringFactor + m_inputState->m_accelerometerValue.x * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.y = -acceleration.x * kFilteringFactor + m_inputState->m_accelerometerValue.y * (1.0 - kFilteringFactor);
			m_inputState->m_accelerometerValue.z = -acceleration.z * kFilteringFactor + m_inputState->m_accelerometerValue.z * (1.0 - kFilteringFactor);
			
			break;
		}
		default:
		{
			break;
		}
	}
	
	
	
}


@end
