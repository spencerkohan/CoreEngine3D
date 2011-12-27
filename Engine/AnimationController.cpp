//
//  AnimationController.cpp
//  Crow-Regime(iOS)
//
//  Created by Jody McAdams on 12/17/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "AnimationController.h"
#include "stddef.h" //for NULL -_-
#include "Hash.h"

//ANIMATION SET
void AnimationSet::AddAnimation(s32 animID, u32 startFrame, u32 endFrame, u32 FPS, bool isLooping, s32 animID_NextAnim)
{
	if(m_numAnimations == ANIMATION_MAX_ANIMATIONS)
	{
		return;
	}
	
	Animation* pAnim = &m_animations[m_numAnimations];
	
	pAnim->startFrame = startFrame;
	pAnim->endFrame = endFrame;
	pAnim->FPS = FPS;
	pAnim->isLooping = isLooping;
	pAnim->animID_playWhenFinished = animID_NextAnim;
	pAnim->animID = animID;

	++m_numAnimations;
}


Animation* AnimationSet::GetAnimationByID(s32 animID)
{
	for(u32 i=0; i<m_numAnimations; ++i)
	{
		Animation* pCurrAnim = &m_animations[i];
		if(pCurrAnim->animID == animID)
		{
			return pCurrAnim;
		}
	}
	
	return NULL;
}


//ANIMATION PLAYER

void AnimationPlayer::PlayAnimation(u32 animID)
{
	if(m_pAnimSet == NULL)
	{
		return;
	}
	
	m_pCurrAnim = m_pAnimSet->GetAnimationByID(animID);
	
	if(!m_pCurrAnim)
	{
		return;
	}
	
	m_animIsDone = false;
	
	m_currFrame = m_pCurrAnim->startFrame;
}

bool AnimationPlayer::GetAnimIsDone()
{
	return m_animIsDone;
}

f32 AnimationPlayer::GetCurrentFrame()
{
	return m_currFrame;
}

void AnimationPlayer::Update(f32 timeElapsed)
{
	if(m_animIsDone)
	{
		return;
	}
	
	if(m_pCurrAnim == NULL)
	{
		return;
	}
	
	m_currFrame += timeElapsed*(f32)m_pCurrAnim->FPS;
	
	if(m_currFrame > m_pCurrAnim->endFrame)
	{
		if(m_pCurrAnim->isLooping)
		{
			m_currFrame -= (m_pCurrAnim->endFrame-m_pCurrAnim->startFrame);
		}
		else if(m_pCurrAnim->animID_playWhenFinished)
		{
			PlayAnimation(m_pCurrAnim->animID_playWhenFinished);
		}
		else
		{
			m_animIsDone = true;
		}
	}
}


//ANIMATION CONTROLLER

AnimationController::AnimationController()
{
	m_numAnimPlayers = 0;
	m_numAnimSets = 0;
}


AnimationPlayer* AnimationController::CreateAnimationPlayer(AnimationSet* pAnimSet)
{
	if(m_numAnimPlayers == ANIMATION_MAX_PLAYERS)
	{
		return NULL;
	}
	
	AnimationPlayer* pAnimPlayer = &m_animPlayers[m_numAnimPlayers];
	pAnimPlayer->m_pAnimSet = pAnimSet;
	
	
	++m_numAnimPlayers;
	
	return pAnimPlayer;
}

AnimationSet* AnimationController::CreateAnimationSet()
{
	if(m_numAnimSets == ANIMATION_MAX_ANIMATION_SETS)
	{
		return NULL;
	}
	
	AnimationSet* pAnimSet = &m_animSets[m_numAnimSets];
	++m_numAnimSets;
	
	return pAnimSet;
}


void AnimationController::DestroyAnimationPlayer(AnimationPlayer* pAnimPlayer)
{
	if(m_numAnimPlayers == 0)
	{
		return;
	}
	
	if(m_numAnimPlayers > 1)
	{
		u32 playerIndex = -1;
		for(u32 i=0; i<m_numAnimPlayers; ++i)
		{
			AnimationPlayer* pCurrPlayer = &m_animPlayers[i];
			if(pCurrPlayer == pAnimPlayer)
			{
				playerIndex = i;
				break;
			}
		}
		
		if(playerIndex != -1)
		{
			m_animPlayers[playerIndex] = m_animPlayers[m_numAnimPlayers-1];
		}
		
	}
	
	--m_numAnimPlayers;
}


void AnimationController::Update(f32 timeElapsed)
{
	for(u32 i=0; i<m_numAnimPlayers; ++i)
	{
		m_animPlayers[i].Update(timeElapsed);
	}
}
