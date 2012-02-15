#include "CoreObjectFactories.h"
#include "OpenGLRenderer.h"

CoreObjectFactory<CollisionBox> g_Factory_CollisionBox;
CoreObjectFactory<ScriptObject> g_Factory_ScriptObject;
CoreObjectFactory<ObjectGroup> g_Factory_ObjectGroup;
CoreObjectFactory<SoundPlayer> g_Factory_SoundPlayer;
CoreObjectFactory<TileAffector> g_Factory_TileAffector;
CoreObjectFactory<RenderableGeometry3D> g_Factory_Geometry_Normal;
CoreObjectFactory<RenderableGeometry3D> g_Factory_Geometry_UI;
CoreObjectFactory<RenderableSceneObject3D> g_Factory_RenderableSceneObject;

void CoreObjectFactories_Init()
{
	g_Factory_CollisionBox.Init(64);
	g_Factory_ScriptObject.Init(128);
	g_Factory_ObjectGroup.Init(32);
	g_Factory_TileAffector.Init(8);
	g_Factory_SoundPlayer.Init(16);
	g_Factory_Geometry_Normal.Init(2048);
	g_Factory_Geometry_UI.Init(64);
	g_Factory_RenderableSceneObject.Init(64);
}

void CoreObjectFactories_Update(f32 timeElapsed)
{
	g_Factory_CollisionBox.UpdateObjectList(timeElapsed);
	g_Factory_ScriptObject.UpdateObjectList(timeElapsed);
	g_Factory_ObjectGroup.UpdateObjectList(timeElapsed);
	g_Factory_TileAffector.UpdateObjectList(timeElapsed);
	g_Factory_SoundPlayer.UpdateObjectList(timeElapsed);
	
	if(g_Factory_Geometry_Normal.UpdateObjectList(timeElapsed))
	{
		GLRENDERER->ForceRenderablesNeedSorting_Normal();
	}
	
	if(g_Factory_Geometry_UI.UpdateObjectList(timeElapsed))
	{
		GLRENDERER->ForceRenderablesNeedSorting_UI();
	}
	
	g_Factory_RenderableSceneObject.UpdateObjectList(timeElapsed);
}

void CoreObjectFactories_Clear()
{
	g_Factory_CollisionBox.Clear();
	g_Factory_ScriptObject.Clear();
	g_Factory_ObjectGroup.Clear();
	g_Factory_SoundPlayer.Clear();
	g_Factory_TileAffector.Clear();
	g_Factory_Geometry_Normal.Clear();
	g_Factory_Geometry_UI.Clear();
	g_Factory_RenderableSceneObject.Clear();
}
