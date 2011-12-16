//
//  untitled.h
//  CoreEngine3D
//
//  Created by sam on 10/26/10.
//  Copyright 2010 GameJelly Online. All rights reserved.
//

#import "MathTypes.h"
#import "GraphicsTypes.h"


#import <Foundation/Foundation.h>

#define MAX_ANIMATION_EVENTS 12
#define MAX_ANIMATION_PLAYERS 32

#define MAX_ANIMATION_ATTACHPOINT_HEADER_ARRAYS 8

typedef enum
{
	AttachPointFileReadState_ReadNumNames,
	AttachPointFileReadState_ReadName,
	AttachPointFileReadState_ReadPoints,
	AttachPointFileReadState_None,
}AttachPointFileReadState;

typedef struct 
{
	char attachPointName[256];
	vec2* pAttachPointLocations;
	s32 numAttachPointLocations;
}AnimationAttachPointLocations;


typedef struct
{
	s32 frame;
	s32 eventID;
}AnimEvent;


typedef enum
{
	AnimPlayMode_Forward,
	AnimPlayMode_Backward,
	AnimPlayMode_PingPong,
	AnimPlayMode_Lerp,
}AnimPlayMode;


typedef struct
{
	NSString* animationName;
	int attachHeaderIndex;
    NSString* nextAnimName;
	GLuint* textureHandle;
	int	startFrame;
	int	endFrame;
	int	framesPerRow;
	int	framesPerColumn;
	int	sheetWidth;
	int	sheetHeight;
	float fps;
	int frameWidth;
	int frameHeight;
	int framesInSheet;
	AnimEvent animEvents[MAX_ANIMATION_EVENTS];
	s32 numAnimEvents;
	AnimPlayMode playMode;
}AnimationClip;


typedef struct
{
	AnimationAttachPointLocations* pAttachPointLocationHeaders;
	s32 numAttachPointLocationHeaders;
}AnimationAttachPointArray;

typedef struct 
{
	bool drawthisframe;
	bool animFinished;
	bool pingPong;
	
	RenderableObject3D * r3d;
	
	AnimationAttachPointArray m_pAttachPointHeaderArrays[MAX_ANIMATION_ATTACHPOINT_HEADER_ARRAYS];
	s32 m_numAttachPointHeaderArrays;
	
	vec2 wwlo3;
	int timesToPlay;
	NSMutableArray* clips;
	AnimationClip* currentClip;
	AnimationClip* nextClip;
	NSMutableDictionary* clipDictionary;
	f32 currentTime;
	s32 currentFrame;
	s32 timesPlayed;
	f32 lerpVal;
}AnimationInfo;


@class ES2Renderer;

@protocol AnimationSystem2D_Delegate <NSObject>

@required
-(void)AnimationSystem2D_Delegate_HandleAnimEvent:(s32)animEventID;
@end


@interface AnimationSystem2D : NSObject 
{
	ES2Renderer * renderer;
	s32 m_highestAnimationIndex;

	AnimationInfo m_animationInfos[MAX_ANIMATION_PLAYERS];
	s32 m_animPlayers[MAX_ANIMATION_PLAYERS];

	s32 m_lastAnimEventFrame;
}

@property (assign) id<AnimationSystem2D_Delegate>  animEventDelegate;


-(void) Init:(ES2Renderer*)_renderer;
-(void) UpdateAnimations : (f32) timeElapsed;
-(s32) CreateAnimationPlayer;
-(s32) AddAttachPointsToAnimationPlayer:(s32) animationInfoIndex:(const char*)attachPointFile;
-(void) ClearAllAnimationPlayers;
-(void) LinkAnimationPlayer:(RenderableObject3D*)_r3d: (s32)animationID;
-(void) AddClip:(AnimationClip *)_clip ToAnimation:(s32) _animation;
-(void) PlayClip:(NSString *) clipName ForAnimation:(s32) _animation timesToPlay:(int)_timestoplay;
-(void) AddAnimEventToClip:(NSString *) clipName ForAnimation:(s32) _animation OnFrame:(s32)frame WithID:(s32)eventID;
-(BOOL) GetAnimationAttachPointByName:(s32) animationInfoIndex:(const char*)attachPointName:(vec2*)pOut_vec;
-(bool) IsAnimationDone:(s32) _animationIndex;
-(bool) IsAnimationPlaying:(s32) _animationIndex:(NSString *) _animationName;
-(u32) GetCurrentFrame:(s32) _animationIndex;
-(void) SetLerpTimeForAnimation:(s32) _animation: (f32)lerpValue;

@end
