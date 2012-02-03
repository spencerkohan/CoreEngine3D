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
	enum Action
	{
		Action_WaitForCollision,
		Action_WaitForTrigger,
		Action_Num,
	};

	virtual bool Init(s32 type);
	virtual void Update(f32 timeElapsed);
	virtual void Uninit();

	vec3* GetPosition();
	
	bool GetPositionIsInside(const vec2* pTouchPos);

	void SpawnInit(SpawnableEntity* pEntity);

private:	

	s32 m_type;
	
	bool m_isFirstUpdate;

	vec3 m_position;

	CoreObjectHandle m_hCollisionBox;
	
	s32 m_numAllowedTriggers;
	u32 m_numTriggers;
	
	CoreObjectHandle m_hTriggerObject;
	CoreObjectHandle m_hTriggerVolume;
};

void ScriptObject_Init();

#endif
