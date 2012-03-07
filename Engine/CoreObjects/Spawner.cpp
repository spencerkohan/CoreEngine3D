//
//  Spawner.cpp
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "Spawner.h"

#include "../Hash.h"
#include "../matrix.h"
#include "../OpenGLRenderer.h"
#include "../CoreObject_Manager.h"
#include "CoreObjectFactories.h"

static const f32 g_Spawner_mass = 64.0f;

static const MaterialSettings g_Spawner_MaterialSettings1 =
{
	GL_LINEAR,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_EDGE,
	MT_TextureOnlySimple,
	RenderFlagDefaults_2DTexture_AlphaBlended_UseView,
	true,
};


bool Spawner::SpawnInit(void* pSpawnStruct)
{
	m_hSpawnObject = CoreObjectHandle();
	
	m_spawnTimer = -1.0f;
	m_spawnTimeMin = 5.0f;
	m_spawnTimeMax = 5.0f;
	m_requiresTrigger = false;
	
	m_launchSpeed = 0.0f;

	SpawnableEntity* pSpawnableEnt = (SpawnableEntity*)pSpawnStruct;
	if(pSpawnableEnt == NULL)
	{
		return false;
	}
	
	CopyVec3(&m_position,&pSpawnableEnt->position);
	
	for (pugi::xml_node property = pSpawnableEnt->pProperties.child("property"); property; property = property.next_sibling("property"))
	{
		const char* propNameString = property.attribute("name").value();
		const char* valueString = property.attribute("value").value();
		
		if(strcmp(propNameString, "SpawnObject") == 0)
		{
			const u32 uniqueID = atoi(valueString);
			m_pEntityToSpawn = GAME->GetSpawnableEntityByTiledUniqueID(uniqueID);
		}
		else if(strcmp(propNameString, "SpawnTimeMin") == 0)
		{
			m_spawnTimeMin = atof(valueString);
		}
		else if(strcmp(propNameString, "SpawnTimeMax") == 0)
		{
			m_spawnTimeMax = atof(valueString);
		}
		else if(strcmp(propNameString, "LaunchSpeed") == 0)
		{
			m_launchSpeed = atof(valueString);
		}
		else if(strcmp(propNameString, "RequiresTrigger") == 0)
		{
			if(strcmp(valueString, "true") == 0)
			{
				m_requiresTrigger = true;
			}
		}
	}
	
	m_spawnTimer = rand_FloatRange(m_spawnTimeMin, m_spawnTimeMax);

	return false;
}


bool Spawner::Init(u32 type)
{
	CoreObject::Init(type);

	m_isFirstUpdate = true;

	
	return true;
}


void Spawner::Update(f32 timeElapsed)
{
	if(m_isFirstUpdate == true)
	{
		m_isFirstUpdate = false;

		return;
	}

	if(m_requiresTrigger)
	{
		return;
	}
	
	vec2 posMeters;
	posMeters.x = m_position.x/GAME->GetPixelsPerMeter();
	posMeters.y = m_position.y/GAME->GetPixelsPerMeter();
	
	m_spawnTimer -= timeElapsed;
	if(m_spawnTimer < 0.0f)
	{
		m_spawnTimer = rand_FloatRange(m_spawnTimeMin, m_spawnTimeMax);
		
		CoreGameObject* pObject = (CoreGameObject*)GAME->CreateObject(m_pEntityToSpawn->type);
		if(pObject != NULL)
		{
			pObject->SpawnInit(m_pEntityToSpawn);
			
			if(m_launchSpeed > 0.0f)
			{
				b2Body* pBody = pObject->Box2D_GetBody();
				if(pBody != NULL)
				{
					vec2 launchDir;
					const b2Vec2& bodyPos = pBody->GetWorldCenter();
					const vec2 bodyPosVec = {bodyPos.x,bodyPos.y};
					SubVec2(&launchDir,&bodyPosVec,&posMeters);
					TryNormalizeVec2_Self(&launchDir);
					ScaleVec2_Self(&launchDir, m_launchSpeed);
					
					pBody->SetLinearVelocity(b2Vec2(launchDir.x,launchDir.y));
				}
			}
		}
	}
}


void Spawner::ProcessMessage(u32 message)
{
	/*if(message == Hash("AttachPlayer"))
	{
		g_Factory_AIPlayer.m_pObjectList[0].AttachToRope(this->GetHandle());
	}*/
}


void Spawner::Uninit()
{
	//Free the handle up
	CoreObject::Uninit();
}

