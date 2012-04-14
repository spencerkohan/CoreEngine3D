#ifndef __COREOBJECTFACTORIES_H
#define __COREOBJECTFACTORIES_H

#include "../CoreObject_Manager.h"
#include "CollisionBox.h"
#include "ScriptObject.h"
#include "ObjectGroup.h"
#include "SoundPlayer.h"
#include "TileAffector.h"
#include "BasicParticle.h"
#include "Spawner.h"

extern CoreObjectFactory<CollisionBox> g_Factory_CollisionBox;
extern CoreObjectFactory<ScriptObject> g_Factory_ScriptObject;
extern CoreObjectFactory<ObjectGroup> g_Factory_ObjectGroup;
extern CoreObjectFactory<SoundPlayer> g_Factory_SoundPlayer;
extern CoreObjectFactory<TileAffector> g_Factory_TileAffector;
extern CoreObjectFactory<BasicParticle> g_Factory_BasicParticle;
extern CoreObjectFactory<RenderableGeometry3D> g_Factory_Geometry_Normal;
extern CoreObjectFactory<RenderableGeometry3D> g_Factory_Geometry_UI;
extern CoreObjectFactory<RenderableGeometry3D> g_Factory_Geometry_Light;
extern CoreObjectFactory<RenderableSceneObject3D> g_Factory_RenderableSceneObject;
extern CoreObjectFactory<Spawner> g_Factory_Spawner;

void CoreObjectFactories_Init();
void CoreObjectFactories_Update(f32 timeElapsed);
void CoreObjectFactories_Clear();

#endif