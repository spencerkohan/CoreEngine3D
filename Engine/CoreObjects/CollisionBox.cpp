//
//  CollisionBox.cpp
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "CollisionBox.h"

#include "Game.h"

#include "matrix.h"
#include "OpenGLRenderer.h"
#include "CoreObject_Manager.h"

#include "CoreObjectFactories.h"

bool CollisionBox::SpawnInit(void* pSpawnStruct)
{
	SpawnableEntity* pSpawnableEnt = (SpawnableEntity*)pSpawnStruct;
	if(pSpawnableEnt == NULL)
	{
		return false;
	}
	
	//vec3* pPos = &pSpawnableEnt->position;
	
	const f32 scaleX = pSpawnableEnt->scale.x;
	const f32 scaleY = pSpawnableEnt->scale.y;
	
	SpawnInit(&pSpawnableEnt->position,-scaleX,scaleX,-scaleY,scaleY);
	
	return false;
}

void CollisionBox::SpawnInit(const vec3* pRefPos, f32 xLeft, f32 xRight, f32 yTop, f32 yBottom)
{
	CopyVec3(&m_refPos,pRefPos);
	CopyVec3(&m_currPos,pRefPos);
	 
	m_xLeft = xLeft;
	m_xRight = xRight;
	m_yTop = yTop;
	m_yBottom = yBottom;
	
	UpdateCornerPositions();
}


void CollisionBox::UpdatePosition(const vec3* pPos)
{
	CopyVec3(&m_currPos,pPos);
	
	UpdateCornerPositions();
}


void CollisionBox::UpdateCornerPositions()
{
	//All we can do here is maybe draw something
	vec3 offset;
	SubVec3(&offset,&m_currPos,&m_refPos);
	
	m_boxTL.z = 0.0f;
	m_boxTR.z = 0.0f;
	m_boxBL.z = 0.0f;
	m_boxBR.z = 0.0f;
	
	m_boxTL.x = m_xLeft+offset.x;
	m_boxTL.y = m_yTop+offset.y;
	
	m_boxTR.x = m_xRight+offset.x;
	m_boxTR.y = m_boxTL.y;
	
	m_boxBL.x = m_boxTL.x;
	m_boxBL.y = m_yBottom+offset.y;
	
	m_boxBR.x = m_boxTR.x;
	m_boxBR.y = m_boxBL.y;
	
	//TODO: rename this function?
	
	m_boxCenter.x = (m_boxBL.x+m_boxBR.x) * 0.5f;
	m_boxCenter.y = (m_boxTL.y+m_boxBR.y) * 0.5f;
	m_boxCenter.z = 0.0f;
}


bool CollisionBox::Init(u32 type)
{
	CoreObject::Init(type);
	
	m_type = (CollisionBoxType)type;
	
	return true;
}


bool CollisionBox::GetPositionIsInside(const vec2* pPos)
{
	if(pPos->x > m_boxTL.x
	   && pPos->x < m_boxBR.x
	   && pPos->y > m_boxBR.y
	   && pPos->y < m_boxTL.y)
	{
		return true;
	}
	
	return false;
}

const vec3* CollisionBox::GetBottomLeft()
{
	return &m_boxBL;
}

const vec3* CollisionBox::GetTopLeft()
{
	return &m_boxTL;
}

const vec3* CollisionBox::GetBottomRight()
{
	return &m_boxBR;
}

const vec3* CollisionBox::GetBoxCenter()
{
	return &m_boxCenter;
}

bool CollisionBox::CollideWithWorld(u32 collisionTypeFlags, CollisionResult* pOut_Result)
{
	//NOTE: This function is probably bad so you probably
	//shouldn't use it very much
	
	bool foundSomething = false;
	
	pOut_Result->numBoxes = 0;
	
	for(u32 i=0; i<g_Factory_CollisionBox.m_numObjects; ++i)
	{
		CollisionBox* pBox = &g_Factory_CollisionBox.m_pObjectList[i];
		if(pBox == this)
		{
			continue;
		}
		
		if(collisionTypeFlags & (1 << pBox->m_type))
		{
			bool isInside = false;
			
			if(pBox->GetPositionIsInside((vec2*)&m_boxTL))
			{
				isInside = true;
			}
			else if(pBox->GetPositionIsInside((vec2*)&m_boxTR))
			{
				isInside = true;
			}
			else if(pBox->GetPositionIsInside((vec2*)&m_boxBL))
			{
				isInside = true;
			}
			else if(pBox->GetPositionIsInside((vec2*)&m_boxBR))
			{
				isInside = true;
			}
			
			if(isInside)
			{
				pOut_Result->pBoxList[pOut_Result->numBoxes] = pBox;
				++pOut_Result->numBoxes;
				
				foundSomething = true;
			}
			
			//Stop when we run out of boxes
			if(pOut_Result->numBoxes == CollisionBox_MAX_COLLISION_RESULTS)
			{
				break;
			}
		}
	}
	
	return foundSomething;
}


void CollisionBox::Update(f32 timeElapsed)
{
	
#ifdef _DEBUG	
	
	//DEBUG DRAW!
	
	vec4 color;
	
	switch((CollisionBoxType)m_type)
	{
		case CollisionBoxType_Player:
		{
			CopyVec4(&color,&color4f_green);
			
			break;
		}
		case CollisionBoxType_Moving:
		{
			CopyVec4(&color,&color4f_yellow);
			
			break;
		}
		case CollisionBoxType_Static:
		{
			CopyVec4(&color,&color4f_red);
			
			break;
		}
		case CollisionBoxType_Ghost:
		{
			CopyVec4(&color,&color4f_pink);
			
			break;
		}
		case CollisionBoxType_Trigger:
		{
			CopyVec4(&color,&color4f_white);
			
			break;
		}
		default:
		{
			CopyVec4(&color,&color4f_blue);
			
			break;
		}
	}
	
	GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &m_boxTL, &m_boxTR, &color);
	
	GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &m_boxBL, &m_boxBR, &color);
	
	GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &m_boxTL, &m_boxBL, &color);
	
	GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &m_boxTR, &m_boxBR, &color);
#endif
	
}

void CollisionBox::Uninit()
{
	
}

