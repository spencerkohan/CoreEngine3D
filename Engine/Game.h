//
//  Game.h
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OSX__Game_h
#define InfiniSurv_OSX__Game_h

#include <Box2D/Box2D.h>
#include "Box2DContactListener.h"

#include "MathTypes.h"
#include "OpenGLRenderer.h"

#include "CoreObjects/Tile.h"

#if defined (PLATFORM_IOS)
#include "TouchInputIOS.h"
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
#include "CoreInput_DeviceInputState.h"
#endif

#if defined (PLATFORM_OSX) || defined (PLATFORM_WIN)
#include "CoreInput_PCInputState.h"
#endif

#include "CoreUI_Button.h"

#include "CoreAudio_OpenAL.h"

#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
#include <AVFoundation/AVFoundation.h>
#endif

#include "pugixml/src/pugixml.hpp"

#include "CoreObject_Manager.h"

class Game;
extern Game* GAME;

class Box2DDebugDraw;

#define GAME_MAX_BUTTONS 16


#define GAME_MAX_ART_DESCRIPTIONS 128
#define GAME_MAX_SOUND_DESCRIPTIONS 32
#define GAME_MAX_SONGS_IN_PLAYLIST 16

#define GAME_MAX_TILESET_DESCRIPTIONS 8
#define GAME_MAX_SPAWNABLE_ENTITIES 256

#define GAME_MAX_STORED_DELETABLE_TILES 64

extern const MaterialSettings g_Game_BlobShadowSettings;
extern ItemArtDescription g_Game_BlobShadowDesc;

enum CollisionFilter
{
	CollisionFilter_Ground,
	CollisionFilter_Breakable,
	CollisionFilter_Player,
	CollisionFilter_Enemy,
	CollisionFilter_PlayerProjectile,
	CollisionFilter_EnemyProjectile,
	CollisionFilter_Touch,
	CollisionFilter_Rope,
	CollisionFilter_Balloon, //sketchy balloon filter
	CollisionFilter_MovingPlatform, //even sketchier?
	CollisionFilter_Connector, //Reasonable?
	CollisionFilter_Spikey, //Reasonable?
	CollisionFilter_Bouncy,
	CollisionFilter_Ghost,
};


enum LevelLayer
{
	LevelLayer_Invalid = -1,
	LevelLayer_Parallax4,
	LevelLayer_Parallax3,
	LevelLayer_Parallax2,
	LevelLayer_Parallax1,
	LevelLayer_Parallax0,
	LevelLayer_Main1,
	LevelLayer_Main0,
	LevelLayer_Collision,
	LevelLayer_TileObjectArt,
	LevelLayer_CameraExtents,
	NumLevelLayers,
};

struct TiledLevelDescription
{
	s32 tileDisplaySizeX;
	s32 tileDisplaySizeY;
	s32 halfTileDisplaySizeX;
	s32 halfTileDisplaySizeY;
};

struct SpawnableEntity
{
	u32 type;
	u32 tiledUniqueID;
	pugi::xml_node pProperties;
	vec3 position;
	vec2 scale;
	s32 tileID;
	TileSetDescription* pDesc;
	u32 tileIndexX;
	u32 tileIndexY;
	bool autospawn;
	
	CoreObject* pObject;
};

struct Layer
{
	u32 numTilesX;
	u32 numTilesY;
	s32* pLevelData;
	vec3 position;
	
	RenderMaterial material;
	vec4 fogColor;
	f32	blurAmount;
	
	Tile* tiles;
};

enum CameraMode
{
	CameraMode_FollowCam,
	CameraMode_Anchor,
};

class Game
{
public:
	virtual bool Init();
	virtual void Update(f32 timeElapsed);
	virtual void CleanUp();
	virtual CoreObject* CreateObject(u32 objectType){return NULL;};
	virtual void LoadLevel(s32 levelNumber){};
	virtual void ReloadLevel(){};
	virtual void FinishedCurrentLevel(){};
	virtual u32 Box2D_GetCollisionFlagsForTileIndex(s32 tileIndex){return 1<<CollisionFilter_Ground;}
	virtual void TileDestructionCallback(s32 tileIndexX, s32 tileIndexY){};
#if defined (PLATFORM_IOS)
	TouchInputIOS* m_pTouchInput;
#endif

#if defined (PLATFORM_OSX) || defined(PLATFORM_WIN)
	MouseInputState m_mouseState;
	KeyboardInputState m_keyboardState;
#endif
	void Box2D_TogglePhysicsDebug(bool allowPhysicsDebugDraw);
	void ResetCamera();
	void SetCameraMode(CameraMode mode);
	void Box2D_Init(bool continuousPhysicsEnabled,bool allowObjectsToSleep);
	b2World* Box2D_GetWorld();
	b2Body* Box2D_GetGroundBody();
	void Box2D_SetGravity(f32 x, f32 y);
	void Box2D_SetContactListener(b2ContactListener* pContactListener);
	void Box2D_ResetWorld();
	SpawnableEntity* GetSpawnableEntityByTiledUniqueID(u32 tiledUniqueID);
	CoreUI_Button* AddUIButton(u32 width, u32 height, CoreUI_AttachSide attachSide, s32 offsetX, s32 offsetY, u32* textureHandle, s32 value, void (*callback)(s32));
	void UpdateButtons(TouchState touchState, vec2 *pTouchPosBegin, vec2* pTouchPosCurr);
	void ClearAllButtons();
	void AddItemArt(ItemArtDescription* pArtDescription);
	void AddItemSound(ItemSoundDescription* pSoundDescription);
	s32 AddSongToPlaylist(const char* songFilenameMP3);
	void PlaySongByID(s32 songID, f32 volume, bool isLooping);
	std::string GetPathToFile(const char* filename, bool fromEngine=false);
	void GetTileIndicesFromScreenPosition(const vec2* pPosition, u32* pOut_X, u32* pOut_Y);
	void GetTileIndicesFromPosition(const vec2* pPosition, u32* pOut_X, u32* pOut_Y);
	void GetPositionFromTileIndices(s32 index_X, s32 index_Y, vec3* pOut_position);
	s32 GetCollisionFromTileIndices(s32 index_X, s32 index_Y);
	void DestroyTile(s32 index_x, s32 index_Y);
	f32 GetTileSize();
	f32 GetHalfTileSize();
	f32 GetPixelsPerMeter();
	const vec3* GetCameraPosition();
	void SetCameraPosition(const vec3* pCamPos, f32 lerpTime);	//use with caution
	void SetParallaxPosition(const vec3* pParallaxPos);
	void SetParallaxScale(f32 parallaxScale);
	void SetFollowCamTarget(const vec3* pFollowCamPos);
	void ToggleTileVisibility(LevelLayer levelLayer,u32 tileIndex_X,u32 tileIndex_Y,bool isVisible);
	Layer* GetLayer(LevelLayer layer);
#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
	DeviceInputState* GetDeviceInputState();
#endif
protected:	//Only stuff that can be called from the game.cpp goes here
	
	void ConstrainCameraToTiledLevel();
	bool LoadTiledLevel(std::string& path, std::string& filename, u32 tileWidthPixels, f32 tileSizeMeters);
	void UpdateTiledLevelPosition(vec3* pPosition);
	void LoadItemArt();	//Call to load all the art in the list
	void LoadItemSounds();
	void ClearItemArt();	//Call when you're going to load more art and some of it might be the same
	void ClearItemSounds();
	void DeleteAllItemArt();	//Call to delete all the art in the list regardless
	void DeleteAllItemSounds();
	void ConvertTileID(s32* p_InOut_tileID, TileSetDescription** pOut_tileDesc);
	CoreObjectHandle CreateRenderableTile(s32 tileID, TileSetDescription* pDesc, RenderableGeometry3D** pGeom, RenderLayer renderLayer, RenderMaterial material, vec2* pOut_texCoordOffset, bool usesViewMatrix);
#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
	DeviceInputState m_deviceInputState;
#endif
	SpawnableEntity m_spawnableEntities[GAME_MAX_SPAWNABLE_ENTITIES];
	u32 m_numSpawnableEntities;
	TileSetDescription m_tileSetDescriptions[GAME_MAX_TILESET_DESCRIPTIONS];
	u32 m_numTileSetDescriptions;
	Layer m_layers[NumLevelLayers];
	TiledLevelDescription m_tiledLevelDescription;
	f32 m_view[16];
	
	vec3 m_camPos;
	vec3 m_startCamPos;
	vec3 m_desiredCamPos;
	vec3 m_followCamPos;
	
	f32 m_parallaxScale;
	
	vec3 m_parallaxBasePos;
	
	f32 m_camLerpTimer;
	f32 m_camLerpTotalTime;
	
	bool m_levelHasCamRestraints;
	s32 m_camExtentTL_X;
	s32 m_camExtentTL_Y;
	s32 m_camExtentBR_X;
	s32 m_camExtentBR_Y;
	
	bool m_touchIsDisabled[MAX_MULTITOUCH];
	bool m_paused;
	
	
	
private:
	pugi::xml_document m_TMXDoc;
	
	CameraMode m_cameraMode;
	
	bool m_Box2D_ContinuousPhysicsEnabled;
	bool m_Box2D_allowObjectsToSleep;
	
	Box2DDebugDraw* m_Box2D_pDebugDraw;
	Box2DContactListener* m_Box2D_pContactListener;
	
	b2World* m_Box2D_pWorld;
	b2Body* m_Box2D_pGroundBody;
	
	f32 m_pixelsPerMeter;
	bool WillArtDescriptionBeLoaded(ItemArtDescription* pArtDesc);
	bool WillSoundDescriptionBeLoaded(ItemSoundDescription* pArtDesc);
	
	ItemArtDescription* m_pLoadedArtDescriptions[GAME_MAX_ART_DESCRIPTIONS];
    u32 m_numLoadedArtDescriptions;
    
    ItemArtDescription* m_pArtDescriptionsToLoadTexturesFor[GAME_MAX_ART_DESCRIPTIONS];
    u32 m_numArtDescriptionsToLoadTexturesFor;
	
	ItemSoundDescription* m_pLoadedSoundDescriptions[GAME_MAX_SOUND_DESCRIPTIONS];
    u32 m_numLoadedSoundDescriptions;
    
    ItemSoundDescription* m_pSoundDescriptionsToLoadWavsFor[GAME_MAX_SOUND_DESCRIPTIONS];
    u32 m_numSoundDescriptionsToLoadWavsFor;
	
	CoreUI_Button m_ui_buttons[GAME_MAX_BUTTONS];
	u32 m_ui_numButtons;
	
	CoreAudioOpenAL* m_pCoreAudioOpenAL;

	s32 m_currSongID;
	u32 m_currSongBuffer;
	u32 m_currSongSource;
	char* m_songPlaylist[GAME_MAX_SONGS_IN_PLAYLIST];
	u32 m_numSongsInPlaylist;
	
#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
	AVAudioPlayer* m_pAudioPlayer;
#endif
	
	CoreObjectManager* m_coreObjectManager;

#if defined (PLATFORM_WIN)
	std::string m_path;
	std::string m_enginePath;
#endif

	intVec2 m_tilesToDelete[GAME_MAX_STORED_DELETABLE_TILES];
	s32 m_numTilesToDelete;
};

#endif
