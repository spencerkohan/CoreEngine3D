//
//  BasicParticle.cpp
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 2/26/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "BasicParticle.h"
#include "../OpenGLRenderer.h"
#include "../matrix.h"
#include "../MathUtil.h"
#include "../Game.h"

void BasicParticle::InitParticle(ParticleSettings *pSettings, const vec3* pPosition, const vec3* pDirection, u32 texIndex)
{
	m_pSettings = pSettings;
	
	ItemArtDescription* pArtDesc = pSettings->pItemArt;
	const MaterialSettings* pMaterial = pArtDesc->materialSettings;
	
	RenderableGeometry3D* pGeom = NULL;
	m_hRenderable = GLRENDERER->CreateRenderableGeometry3D(RenderableObjectType_Normal,&pGeom);
	
	if(pGeom == NULL)
	{
		COREDEBUG_PrintDebugMessage("ERROR: Could not create renderable for particle!");
		return;
	}
	
	GLRENDERER->InitRenderableGeometry3D(pGeom, pArtDesc->pModelData, pMaterial->renderMaterial, &pArtDesc->textureHandle, NULL, pSettings->renderLayer, View_0, pMaterial->renderFlags|RenderFlag_Visible);
	pGeom->material.uniqueUniformValues[0] = (u8*)&m_texcoordOffset;
	pGeom->material.uniqueUniformValues[1] = (u8*)&m_diffuseColor;
	
	m_radius = rand_FloatRange(pSettings->radiusMin, pSettings->radiusMax);
	
	mat4f_LoadScale(pGeom->worldMat, m_radius);
	
	vec3* pPos = mat4f_GetPos(pGeom->worldMat);
	CopyVec3(pPos, pPosition);
	CopyVec3(&m_position,pPosition);
	
	const f32 speed = rand_FloatRange(pSettings->moveSpeedMin, pSettings->moveSpeedMax);
	ScaleVec3(&m_velocity,pDirection,speed);
	f32 spinSpeed = rand_FloatRange(pSettings->spinSpeedMin, pSettings->spinSpeedMax);
	if(pSettings->randomlyFlipSpin)
	{
		spinSpeed *= -1.0f;
	}
	m_spinSpeed = spinSpeed*(rand_Bool() ? -1.0f : 1.0f);
	m_currSpinAngle = 0.0f;
	m_lifeTimer = rand_FloatRange(pSettings->lifetimeMin,pSettings->lifetimeMax);
	
	SetVec4(&m_diffuseColor,1.0f,1.0f,1.0f,1.0f);
	CopyVec4(&m_diffuseColorStart,&vec4_one);
	
	switch (texIndex)
	{
		case 0:
		{
			SetVec2(&m_texcoordOffset, 0.0f, 0.0f);
			break;
		}
		case 1:
		{
			SetVec2(&m_texcoordOffset, 0.5f, 0.0f);
			break;
		}
		case 2:
		{
			SetVec2(&m_texcoordOffset, 0.0f, 0.5f);
			break;
		}
		case 3:
		{
			SetVec2(&m_texcoordOffset, 0.5f, 0.5f);
			break;
		}
		default:
		{
			break;
		}
	}
	
	m_pBody = NULL;
	if(pSettings->pBox2D_ParticleSettings != NULL)
	{
		Box2D_ParticleSettings* pCurrSettings = &pSettings->pBox2D_ParticleSettings[texIndex];
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(m_position.x/GAME->GetPixelsPerMeter(), m_position.y/GAME->GetPixelsPerMeter());
		
		b2FixtureDef fixtureDef;
		fixtureDef.filter.maskBits = pCurrSettings->maskBits;
		fixtureDef.filter.categoryBits = pCurrSettings->categoryBits;
		fixtureDef.filter.groupIndex = pCurrSettings->groupIndex;
		fixtureDef.restitution = pCurrSettings->restitution;
		fixtureDef.density = pCurrSettings->density;
		fixtureDef.friction = pCurrSettings->friction;
		
		
		b2CircleShape circleShape;


		circleShape.m_radius = pCurrSettings->collisionRadiusPixels/GAME->GetPixelsPerMeter();

		fixtureDef.shape = &circleShape;
		
		m_pBody = GAME->Box2D_GetWorld()->CreateBody(&bodyDef);
		m_pBody->CreateFixture(&fixtureDef);
		
		m_pBody->SetAngularVelocity(m_spinSpeed);
		m_pBody->SetLinearVelocity(b2Vec2(pDirection->x,pDirection->y));
	}
}


u32 BasicParticle::GetCategoryFlags()
{
	return m_pSettings->categoryFlags;
}


void BasicParticle::Update(f32 timeElapsed)
{
    RenderableGeometry3D* pGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(m_hRenderable);
    
    if(pGeom == NULL)
	{
		return;
	}
    
	m_lifeTimer -= timeElapsed;
	
	const f32 breakableAlpha = ClampF(m_lifeTimer/0.15f,0.0f,1.0f);
    
    ScaleVec4(&m_diffuseColor,&m_diffuseColorStart,breakableAlpha);
    
    if(m_lifeTimer <= 0.0f)
	{
		this->DeleteObject();
	}
    
	vec3* pPos = mat4f_GetPos(pGeom->worldMat);
	
	if(m_pBody != NULL)
	{
		const b2Vec2& posVec = m_pBody->GetPosition();
		pPos->x = posVec.x*GAME->GetPixelsPerMeter();
		pPos->y = posVec.y*GAME->GetPixelsPerMeter();
		pPos->z = 0.0f;
		
		mat4f_LoadScaledZRotation_IgnoreTranslation(pGeom->worldMat, m_pBody->GetAngle(), m_radius);
	}
	else
	{
		m_currSpinAngle += m_spinSpeed*timeElapsed;

		m_velocity.y -= m_pSettings->gravity*timeElapsed;
		AddScaledVec3_Self(&m_position,&m_velocity,timeElapsed);
		
		CopyVec3(pPos,&m_position);
		
		vec3 velNorm;
		TryNormalizeVec3(&velNorm,&m_velocity);
		
		mat4f_LoadScaledZRotation_IgnoreTranslation(pGeom->worldMat, m_currSpinAngle, m_radius);
	}
	
	
	if(m_lifeTimer <= 0.0f)
	{
		this->DeleteObject();
	}
    
    GAME->TiledLevel_DeleteObjectIfOffscreen_X(this,pPos,m_radius,0.0f);
    GAME->TiledLevel_DeleteObjectIfOffscreen_Y(this,pPos,m_radius,0.0f);
}


void BasicParticle::Uninit()
{
	RenderableGeometry3D* pGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(m_hRenderable);
	if(pGeom != NULL)
	{
		pGeom->DeleteObject();
	}
	
	if(m_pBody != NULL)
	{
		GAME->Box2D_GetWorld()->DestroyBody(m_pBody);
	}
	
	CoreObject::Uninit();
}


const vec3* BasicParticle::GetPosition() const
{
	return &m_position;
}

void BasicParticle::AddVelocity(const vec3* pVelAdd)
{
	AddVec3_Self(&m_velocity, pVelAdd);
}


void BasicParticle::UpdateHandle()	//Call when the memory location changes
{	
	CoreObject::UpdateHandle();
    
    RenderableGeometry3D* pGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(m_hRenderable);
    pGeom->material.uniqueUniformValues[0] = (u8*)&m_texcoordOffset;
	pGeom->material.uniqueUniformValues[1] = (u8*)&m_diffuseColor;
}
