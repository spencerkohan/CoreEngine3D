//
//  CollisionBox.h
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef Game_iOS__CollisionBox_h
#define Game_iOS__CollisionBox_h

#include "MathTypes.h"
#include "GraphicsTypes.h"

#include "CoreObject.h"

#define CollisionBox_MAX_COLLISION_RESULTS 16

enum CollisionBoxType
{
	CollisionBoxType_Player,
	CollisionBoxType_Moving,
	CollisionBoxType_Static,
	CollisionBoxType_Ghost,
	CollisionBoxType_Trigger,
	CollisionBoxType_Num,
};


class CollisionBox: public CoreObject
{
public:
	struct CollisionResult
	{
		CollisionBox* pBoxList[CollisionBox_MAX_COLLISION_RESULTS];
		u32 numBoxes;
	};
	
	virtual bool Init(u32 type);
	virtual void Update(f32 timeElapsed);
	virtual void Uninit();
	
	bool GetPositionIsInside(const vec2* pPos);
	
	const vec3* GetTopLeft();
	const vec3* GetBottomLeft();
	const vec3* GetBottomRight();
	const vec3* GetBoxCenter();
	
	void SpawnInit(f32 xLeft, f32 yTop, f32 xRight, f32 yBottom, const vec3* pReferencePos);
	void UpdatePosition(const vec3* pPos);
	
	bool CollideWithWorld(u32 collisionTypeFlags, CollisionResult* pOut_Result);
private:	
	
	f32 m_xLeft;
	f32 m_yTop;
	f32 m_xRight;
	f32 m_yBottom;
	
	vec3 m_refPos;
	vec3 m_currPos;
	
	vec3 m_boxTL;
	vec3 m_boxTR;
	vec3 m_boxBL;
	vec3 m_boxBR;
	
	vec3 m_boxCenter;
	
	CollisionBoxType m_type;
	
	CollisionResult m_depenResult;
	
	void UpdateCornerPositions();
};

void CollisionBox_Init();

#endif
