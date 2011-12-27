//
//  TouchInputIOS.h
//  Game(iOS)
//
//  Created by JodyMcAdams on 12/16/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "MathTypes.h"
#include "TouchState.h"

#define MAX_MULTITOUCH 11

@interface TouchInputIOS : NSObject<UIAccelerometerDelegate>
{
	UITouch* m_pTouches[MAX_MULTITOUCH];
	bool m_touchStateWasUpdated[MAX_MULTITOUCH];
	vec2 m_touchBeginPoints[MAX_MULTITOUCH];
	double m_touchPrevTimes[MAX_MULTITOUCH];
	double m_touchCurrTimes[MAX_MULTITOUCH];
	vec2 m_touchPrevPoints[MAX_MULTITOUCH];
	vec2 m_touchCurrentPoints[MAX_MULTITOUCH];
	TouchState m_touchStates[MAX_MULTITOUCH];
	vec3 m_accelerometerValue;
}

- (id) init;
- (void)Update:(f32) timeElapsed;
- (TouchState)GetTouchState:(u32)touchIndex;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*)pVIew;
- (void) GetTouchVelocity:(u32)touchIndex:(vec2*)out_velocity;
- (void) GetTouchPos:(u32)touchIndex:(vec2*)out_posCurr;
- (void) PostGameUpdate;
- (void) SetAccelerometerIsUsed:(BOOL)useAccelerometer;
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;
- (const vec3*) GetAccelerometerValue;

@end
