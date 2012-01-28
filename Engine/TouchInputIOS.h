//
//  TouchInputIOS.h
//  Game(iOS)
//
//  Created by JodyMcAdams on 12/16/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef __TOUCHINPUT_H
#define __TOUCHINPUT_H

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#include "MathTypes.h"
#include "CoreInput_DeviceInputState.h"

@interface TouchInputIOS : NSObject<UIAccelerometerDelegate>
{
	UITouch* m_pTouches[MAX_MULTITOUCH];
	DeviceInputState* m_inputState;
	u32 m_inputScale;
}

- (id) init:(DeviceInputState*) pDeviceInputState;
- (void)Update:(f32) timeElapsed;
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*) pView;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event: (UIView*)pVIew;
- (void) PostGameUpdate;
- (void) SetAccelerometerIsUsed:(BOOL)useAccelerometer;
- (void)accelerometer:(UIAccelerometer*)accelerometer didAccelerate:(UIAcceleration*)acceleration;

@end

#endif