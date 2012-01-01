//
//  AnimationController.h
//  Crow-Regime(iOS)
//
//  Created by Jody McAdams on 12/17/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef Crow_Regime_iOS__AnimationController_h
#define Crow_Regime_iOS__AnimationController_h

class AnimationController;

extern AnimationController* ANIMCONTROLLER;

#include "MathTypes.h"

#define ANIMATION_MAX_PLAYERS 64
#define ANIMATION_MAX_ANIM_EVENTS 12
#define ANIMATION_MAX_ANIMATIONS 12
#define ANIMATION_MAX_ANIMATION_SETS 8

struct AnimEvent
{
	u32 frame;
	s32 eventID;
};


struct Animation
{
public:
	s32 animID;
	s32 animID_playWhenFinished;
	u32 startFrame;
	u32 endFrame;
	u32 FPS;
	bool isLooping;
	AnimEvent m_animationEvents[ANIMATION_MAX_ANIM_EVENTS];
};


struct AnimationSet
{
public:
	void AddAnimation(s32 animID, u32 startFrame, u32 endFrame, u32 FPS, bool isLooping, s32 animID_NextAnim);
	Animation* GetAnimationByID(s32 animID);
	Animation m_animations[ANIMATION_MAX_ANIMATIONS];
	u32 m_numAnimations;
};


struct AnimationPlayer
{
public:
	void Init(AnimationSet* pAnimSet);
	void Update(f32 timeElapsed);
	void PlayAnimation(u32 animID,u32 frameOffset, f32 playSpeed);
	bool GetAnimIsDone();
	f32 GetCurrentFrame();
	s32 GetCurrentAnimation();
	f32 m_currFrame;
	bool m_animIsDone;
	AnimationSet* m_pAnimSet;
	Animation* m_pCurrAnim;
	f32 m_playSpeed;
};


class AnimationController
{
public:
	AnimationController();
	void (*animEventCallback)(u32);
	
	AnimationSet* CreateAnimationSet();

private:
	AnimationPlayer m_animPlayers[ANIMATION_MAX_PLAYERS];
	u32 m_numAnimPlayers;
	AnimationSet m_animSets[ANIMATION_MAX_ANIMATION_SETS];
	u32 m_numAnimSets;
};

#endif
