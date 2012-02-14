//
//  ScriptObject.h
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef __ScriptObject_h
#define __ScriptObject_h

#include "MathTypes.h"
#include "GraphicsTypes.h"

#include "CoreObject.h"
#include "Game.h"

class ScriptObject: public CoreObject
{
public:
	enum CollisionMode
	{
		CollisionMode_Tile,
		CollisionMode_Box,
		CollisionMode_Num,
	};
	
	enum Action
	{
		Action_WaitForCollision,
		Action_TimerToggle,
		Action_Kill,
		Action_Num,
	};
	
	enum ScriptStatus
	{
		ScriptStatus_On,
		ScriptStatus_Off,
		ScriptStatus_Num,
	};

	virtual void Uninit();
	virtual void ProcessMessage(u32 message);	//Pass in a hash value

	vec3* GetPosition();
	
	bool GetPositionIsInside(const vec2* pTouchPos);

	//TODO: make SpawnInit take a struct or something.  This is getting bad
	virtual bool SpawnInit(void* pSpawnStruct);
	void AttemptTileTrigger(u32 objectType, u32 tileIndex_X, u32 tileIndex_Y);
	void AttemptBoxTrigger(u32 objectType, const vec3* pPosition);
	void Reset();
	virtual void SetPosition(const vec3* pPosition);
	virtual void Update(f32 timeElapsed);
private:	

	bool m_isFirstUpdate;

	vec3 m_position;

	CoreObjectHandle m_hCollisionBox;
	
	s32 m_numAllowedTriggers;
	u32 m_numTriggers;
	
	u32 m_tileIndex_X;
	u32 m_tileIndex_Y;
	
	u32 m_triggerMessage;
	u32 m_untriggerMessage;
	
	CoreObjectHandle m_hTriggerObject;

	CollisionMode m_collMode;
	u32 m_collisionType;
	
	ScriptStatus m_scriptStatus;
	
	ScriptStatus m_initialScriptStatus;
	
	Action m_action;
	f32 m_toggleTimeOn;
	f32 m_toggleTimeOff;
	f32 m_toggleTimer;
	bool m_toggleIsOn;
};

void ScriptObject_Init();

#endif
