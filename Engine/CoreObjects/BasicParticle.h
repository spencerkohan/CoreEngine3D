//
//  BasicParticle.h
//  CoreEngine3D(OSX)
//
//  Created by Jody McAdams on 2/26/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#ifndef CoreEngine3D_OSX__BasicParticle_h
#define CoreEngine3D_OSX__BasicParticle_h

#include "../CoreObject.h"
#include "RenderLayer.h"
#include "GraphicsTypes.h"

class BasicParticle: public CoreObject
{
public:
	struct ParticleSettings
	{
		f32 lifetime;
		f32 gravity;
		f32 moveSpeedMin;
		f32 moveSpeedMax;
		f32 spinSpeedMin;
		f32 spinSpeedMax;
		bool randomlyFlipSpin;
		f32 radiusMin;
		f32 radiusMax;
		ItemArtDescription* pItemArt;
		RenderLayer renderLayer;
		u32 categoryFlags;
	};
	
	//static void InitClass(){};
	
	virtual void UpdateHandle();	//Call when the memory location changes
	
	//virtual bool Init(u32 type);
	//virtual bool SpawnInit(void* pSpawnStruct){return true;}
	//virtual bool PostSpawnInit(void* pSpawnStruct){return true;}
	void InitParticle(BasicParticle::ParticleSettings *pSettings, const vec3* pPosition, const vec3* pDirection, u32 texIndex);
	u32 GetCategoryFlags();
	virtual void Uninit();
	virtual void Update(f32 timeElapsed);
	virtual const vec3* GetPosition() const;
	void AddVelocity(const vec3* pVelAdd);
	//virtual void ProcessMessage(u32 message){};	//Pass in a hash value
private:
	ParticleSettings* m_pSettings;
	f32 m_spinSpeed;
	f32 m_lifeTimer;
	f32 m_radius;
	vec3 m_velocity;
	vec3 m_position;
	vec4 m_diffuseColorStart;
	vec4 m_diffuseColor;
	vec2 m_texcoordOffset;
	f32 m_currSpinAngle;
	CoreObjectHandle m_hRenderable;
};

#endif
