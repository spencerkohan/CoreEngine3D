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

#include "OpenGLRenderer.h"

struct Animationvert2 { float x, y; };
struct Animationcoord2{ float tu, tv; };

#define MAX_ANIMATION_EVENTS 5
#define MAX_ANIMATION_PLAYERS 32

#define MAX_ANIMATION_ATTACHPOINT_HEADER_ARRAYS 8

enum AttachPointFileReadState
{
	AttachPointFileReadState_ReadNumNames,
	AttachPointFileReadState_ReadName,
	AttachPointFileReadState_ReadPoints,
	AttachPointFileReadState_None,
};

typedef struct
{
	char attachPointName[256];
	vec2* pAttachPointLocations;
	s32 numAttachPointLocations;
}AnimationAttachPointLocations;

typedef struct 
{
	NSString* animationName;
	int attachHeaderIndex;
    NSString* nextAnimName;
	GLuint* textureHandle;
	int	beginingFrame;
	int	endingFrame;
	int	framesPerRow;
	int	framesPerColumn;
	int	sheetWidth;
	int	sheetHeight;
	float fps;
	int currentFrame;
	int frameWidth;
	int frameHeight;
	int framesInSheet;
	SEL cs[MAX_ANIMATION_EVENTS];
	u32 cf[MAX_ANIMATION_EVENTS];
	BOOL cb_Done[MAX_ANIMATION_EVENTS];
	u32 numAnimEvents;
	BOOL usesCallBacks;
	BOOL stopAtEnd;
}AnimationClip;


typedef struct 
{
	AnimationAttachPointLocations* pAttachPointLocationHeaders;
	s32 numAttachPointLocationHeaders;
}AnimationAttachPointArray;

typedef struct 
{
	bool drawthisframe;
	RenderableObject3D * r3d;
	
	AnimationAttachPointArray m_pAttachPointHeaderArrays[MAX_ANIMATION_ATTACHPOINT_HEADER_ARRAYS];
	s32 m_numAttachPointHeaderArrays;
	
	vec2 wwlo3;
	int timesToPlay;
	NSMutableArray * clips;
	AnimationClip * currentClip;
	AnimationClip * nextClip;
	NSMutableDictionary * clipDictionary;
	BOOL startOfPlay;
	double beginningTime;
	double endTime;
	double localTime;
	double theTimeOffset;
}AnimationInfo;


@interface AnimationSystem2D : NSObject 
{
	OpenGLRenderer* renderer;
	s32 m_highestAnimationIndex;

	AnimationInfo m_animationInfos[MAX_ANIMATION_PLAYERS];
	s32 m_animPlayers[MAX_ANIMATION_PLAYERS];
}

-(void) Init:(OpenGLRenderer*)_renderer;
-(void) UpdateAnimations : (f32) elapsedTime;
-(void) setRenderer :(OpenGLRenderer*)_renderer;
-(s32) CreateAnimationPlayer;
-(s32) AddAttachPointsToAnimationPlayer:(s32) animationInfoIndex:(const char*)attachPointFile;
-(void) ClearAllAnimationPlayers;
-(void) LinkAnimationPlayer:(RenderableObject3D*)_r3d: (s32)animationID;
-(void) addClip:(AnimationClip *)_clip ToAnimation:(s32) _animation;
-(void) playclip:(NSString *) _nsstring ForAnimation:(s32) _animation timesToPlay:(int)_timestoplay;
//-(s32) GetAnimationAttachPointIndexByName:(s32)animationInfoIndex:(const char*)attachPointName;
//-(BOOL) GetAnimationAttachPointByIndex:(s32) animationInfoIndex:(s32)attachPointIndex:(vec2*)pOut_vec;
-(BOOL) GetAnimationAttachPointByName:(s32) animationInfoIndex:(const char*)attachPointName:(vec2*)pOut_vec;
-(bool) IsAnimationDone:(s32) _animationIndex;
-(bool) IsAnimationPlaying:(s32) _animationIndex:(NSString *) _animationName;
-(u32) GetCurrentFrame:(s32) _animationIndex;

@end
