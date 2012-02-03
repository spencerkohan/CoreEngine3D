//
//  ScriptObject.h
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef Game_iOS__ScriptObject_h
#define Game_iOS__ScriptObject_h

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
		CollisionMode_TriggerBox,
		CollisionMode_Num,
	};
	
	enum Action
	{
		Action_WaitForCollision,
		Action_WaitForTrigger,
		Action_Num,
	};
	
	enum ScriptStatus
	{
		ScriptStatus_On,
		ScriptStatus_Off,
		ScriptStatus_Num,
	};

	virtual bool Init(u32 type);
	virtual void Update(f32 timeElapsed);
	virtual void Uninit();
	virtual void ProcessMessage(u32 message);	//Pass in a hash value

	vec3* GetPosition();
	
	bool GetPositionIsInside(const vec2* pTouchPos);

	//TODO: make SpawnInit take a struct or something.  This is getting bad
	void SpawnInit(SpawnableEntity* pEntity, u32 triggerMessage, CoreObjectHandle triggerObject, u32 collisionType, ScriptStatus status);
	void AttemptTileTrigger(u32 objectType, u32 tileIndex_X, u32 tileIndex_Y);
	
	void Link();
private:	

	bool m_isFirstUpdate;

	vec3 m_position;

	CoreObjectHandle m_hCollisionBox;
	
	s32 m_numAllowedTriggers;
	u32 m_numTriggers;
	
	u32 m_tileIndex_X;
	u32 m_tileIndex_Y;
	
	u32 m_triggerMessage;
	
	CoreObjectHandle m_hTriggerObject;
	
	CoreObjectHandle m_hTriggerVolume;
	
	CollisionMode m_collMode;
	u32 m_collisionType;
	
	ScriptStatus m_scriptStatus;
};

void ScriptObject_Init();

#endif
