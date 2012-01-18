//
//  Game.h
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OSX__Game_h
#define InfiniSurv_OSX__Game_h

#include "MathTypes.h"
#include "OpenGLRenderer.h"

#if defined (PLATFORM_IOS)
#include "TouchInputIOS.h"
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
#include "CoreInput_DeviceInputState.h"
#endif

#if defined (PLATFORM_OSX) || defined (PLATFORM_WIN)
#include "CoreInput_MouseState.h"
#endif

#include "CoreUI_Button.h"

#include "CoreAudio_OpenAL.h"

#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
#include <AVFoundation/AVFoundation.h>
#endif

#include "CoreObject_Manager.h"

class Game;
extern Game* GAME;

#define GAME_MAX_BUTTONS 16


#define GAME_MAX_ART_DESCRIPTIONS 128
#define GAME_MAX_SOUND_DESCRIPTIONS 32
#define GAME_MAX_SONGS_IN_PLAYLIST 16

#define GAME_MAX_BREAKABLES 256

extern const MaterialSettings g_Game_BlobShadowSettings;
extern ItemArtDescription g_Game_BlobShadowDesc;

struct BreakableSettings
{
    f32 lifetime;
    f32 gravity;
    f32 bounceDamping;
    f32 moveSpeedMin;
    f32 moveSpeedMax;
    f32 spinSpeedMin;
    f32 spinSpeedMax;
    bool doesBounce;
};

struct BreakableData
{
    const BreakableSettings* pSettings;
    ItemArtDescription itemArt;
    const char* breakSoundName;
    f32 radius;
    u8 scaleWithZ;
};

struct Breakable
{
    BreakableData* pBreakableData;
    f32 spinSpeed;
    f32 lifeTimer;
    vec3 velocity;
    vec4 diffuseColorStart;
    vec4 diffuseColor;
    CoreObjectHandle handleRenderable;
    vec2 texcoordOffset;
    f32 currSpinAngle;
};


class Game
{
public:
	virtual void Init();
	virtual void Update(f32 timeElapsed);
	virtual void CleanUp();
#if defined (PLATFORM_IOS)
	TouchInputIOS* m_pTouchInput;
#endif

#if defined (PLATFORM_OSX) || defined(PLATFORM_WIN)
	MouseInputState m_mouseState;
#endif
	CoreUI_Button* AddUIButton(u32 width, u32 height, CoreUI_AttachSide attachSide, s32 offsetX, s32 offsetY, u32* textureHandle, s32 value, void (*callback)(s32));
	void UpdateButtons(TouchState touchState, vec2 *pTouchPosBegin, vec2* pTouchPosCurr);
	void ClearAllButtons();
	void AddItemArt(ItemArtDescription* pArtDescription);
	void AddItemSound(ItemSoundDescription* pSoundDescription);
	void UpdateBreakables(f32 timeElapsed);
	void SpawnBreakable(BreakableData* pData, const vec3* pPosition, const vec3* pDirection, u32 breakableIndex, const vec4* diffuseColor, RenderLayer renderLayer);
	s32 AddSongToPlaylist(const char* songFilenameMP3);
	void PlaySongByID(s32 songID, f32 volume, bool isLooping);
	std::string GetPathToFile(const char* filename);
protected:
	
	void LoadItemArt();	//Call to load all the art in the list
	void LoadItemSounds();
	void ClearItemArt();	//Call when you're going to load more art and some of it might be the same
	void ClearItemSounds();
	void DeleteAllItemArt();	//Call to delete all the art in the list regardless
	void DeleteAllItemSounds();
#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
	DeviceInputState m_deviceInputState;
#endif
	
private:
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
	
	Breakable m_updatingBreakables[GAME_MAX_BREAKABLES];
	u32 m_numBreakables;
	CoreAudioOpenAL* m_pCoreAudioOpenAL;

	s32 m_currSongID;
	char* m_songPlaylist[GAME_MAX_SONGS_IN_PLAYLIST];
	u32 m_numSongsInPlaylist;
	
#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
	AVAudioPlayer* m_pAudioPlayer;
#endif
	
	CoreObjectManager* m_coreObjectManager;

#if defined (PLATFORM_WIN)
	std::string m_path;
#endif
};

#endif
