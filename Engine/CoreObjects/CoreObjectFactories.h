#ifndef __COREOBJECTFACTORIES_H
#define __COREOBJECTFACTORIES_H

#include "CoreObject_Manager.h"
#include "CollisionBox.h"
#include "ScriptObject.h"

extern CoreObjectFactory<CollisionBox> g_Factory_CollisionBox;
extern CoreObjectFactory<ScriptObject> g_Factory_ScriptObject;

void CoreObjectFactories_Init();
void CoreObjectFactories_Update(f32 timeElapsed);

#endif