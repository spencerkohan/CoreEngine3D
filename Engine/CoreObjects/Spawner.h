//
//  Spawner.h
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef __Spawner_h
#define __Spawner_h

#include "../MathTypes.h"
#include "../GraphicsTypes.h"

#include "../CoreAudio_OpenAL.h"
#include "../CoreGameObject.h"

#include "../Game.h"

class Spawner: public CoreGameObject
{
public:

	virtual bool Init(u32 type);
	virtual void Update(f32 timeElapsed);
	virtual void Uninit();
	virtual void ProcessMessage(u32 message);
	virtual bool SpawnInit(void* pSpawnStruct);
	//virtual b2Body* Box2D_GetBody(){return m_pBody;}
	virtual const vec3* GetPosition(){return &m_position;}//HACK: for now it's fine to have this here
	
	void AttachObject(CoreObjectHandle objectHandle, const vec3* pPos);

private:	
	CoreObjectHandle m_hSpawnObject;
	
	bool m_isFirstUpdate;

	vec3 m_position;
	
	SpawnableEntity* m_pEntityToSpawn;
	
	f32 m_spawnTimer;
	f32 m_spawnTimeMin;
	f32 m_spawnTimeMax;
	bool m_requiresTrigger;
	
	//b2Body* m_pBody;
};

//void Spawner_Init();

#endif
