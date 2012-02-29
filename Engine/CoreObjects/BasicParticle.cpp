//
//  BasicParticle.cpp
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 2/26/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "BasicParticle.h"
#include "OpenGLRenderer.h"
#include "matrix.h"
#include "MathUtil.h"

void BasicParticle::InitParticle(ParticleSettings *pSettings, const vec3* pPosition, const vec3* pDirection, u32 texIndex)
{
	m_pSettings = pSettings;
	
	ItemArtDescription* pArtDesc = pSettings->pItemArt;
	const MaterialSettings* pMaterial = pArtDesc->materialSettings;
	
	RenderableGeometry3D* pRenderable = NULL;
	m_hRenderable = GLRENDERER->CreateRenderableGeometry3D_Normal(&pRenderable);
	
	GLRENDERER->InitRenderableGeometry3D(pRenderable, pArtDesc->pModelData, pMaterial->renderMaterial, &pArtDesc->textureHandle, NULL, pSettings->renderLayer, View_0, pMaterial->renderFlags|RenderFlag_Visible);
	pRenderable->material.uniqueUniformValues[0] = (u8*)&m_texcoordOffset;
	pRenderable->material.uniqueUniformValues[1] = (u8*)&m_diffuseColor;
	
	m_radius = rand_FloatRange(pSettings->radiusMin, pSettings->radiusMax);
	
	mat4f_LoadScale(pRenderable->worldMat, m_radius);
	
	vec3* pPos = mat4f_GetPos(pRenderable->worldMat);
	CopyVec3(pPos, pPosition);
	CopyVec3(&m_position,pPosition);
	
	const f32 speed = rand_FloatRange(pSettings->moveSpeedMin, pSettings->moveSpeedMax);
	ScaleVec3(&m_velocity,pDirection,speed);
	f32 spinSpeed = rand_FloatRange(pSettings->spinSpeedMin, pSettings->spinSpeedMax);
	if(pSettings->randomlyFlipSpin)
	{
		spinSpeed *= -1.0f;
	}
	m_spinSpeed = spinSpeed*((rand_FloatRange(0.0f, 1.0f) > 0.5f) ? -1.0f : 1.0f);
	m_currSpinAngle = 0.0f;
	m_lifeTimer = pSettings->lifetime;
	
	SetVec4(&m_diffuseColor,1.0f,1.0f,1.0f,0.0f);
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
	
	
	m_currSpinAngle += m_spinSpeed*timeElapsed;
	
	
	vec3* pPos = mat4f_GetPos(pGeom->worldMat);
	
	m_velocity.y -= m_pSettings->gravity*timeElapsed;
	AddScaledVec3_Self(&m_position,&m_velocity,timeElapsed);
	
	CopyVec3(pPos,&m_position);
	
	vec3 velNorm;
	TryNormalizeVec3(&velNorm,&m_velocity);

	mat4f_LoadScaledZRotation_IgnoreTranslation(pGeom->worldMat, m_currSpinAngle, m_radius);
	
	if(m_lifeTimer <= 0.0f)
	{
		this->DeleteObject();
	}
}


void BasicParticle::Uninit()
{
	RenderableGeometry3D* pGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(m_hRenderable);
	if(pGeom != NULL)
	{
		pGeom->DeleteObject();
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
	RenderableGeometry3D* pGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(m_hRenderable);
	if(pGeom == NULL)
	{
		return;
	}
	
	pGeom->material.uniqueUniformValues[0] = (u8*)&m_texcoordOffset;
	pGeom->material.uniqueUniformValues[1] = (u8*)&m_diffuseColor;
}
