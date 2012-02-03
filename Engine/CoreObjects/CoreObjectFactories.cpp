#include "CoreObjectFactories.h"

CoreObjectFactory<CollisionBox> g_Factory_CollisionBox;
CoreObjectFactory<ScriptObject> g_Factory_ScriptObject;

void CoreObjectFactories_Init()
{
	g_Factory_CollisionBox.Init(16);
	g_Factory_ScriptObject.Init(16);
}

void CoreObjectFactories_Update(f32 timeElapsed)
{
	g_Factory_ScriptObject.UpdateObjectList(timeElapsed);
	g_Factory_CollisionBox.UpdateObjectList(timeElapsed);
}
