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
#import "TouchInputIOS.h"
#endif

#include "CoreInput_DeviceInputState.h"

#include "CoreUI_Button.h"

#define GAME_MAX_BUTTONS 16


#define GAME_MAX_ART_DESCRIPTIONS 128


class Game
{
public:
	virtual void Init();
	virtual void Update(f32 timeElapsed) = 0;
	virtual void CleanUp() = 0;
#if defined (PLATFORM_IOS)
	TouchInputIOS* m_pTouchInput;
#endif

	CoreUI_Button* AddUIButton(u32 width, u32 height, CoreUI_AttachSide attachSide, s32 offsetX, s32 offsetY, u32* textureHandle, s32 value, void (*callback)(s32));
	void UpdateButtons(TouchState touchState, vec2 *pTouchPos);
	void ClearAllButtons();
	void AddItemArt(ItemArtDescription* pArtDescription); //Adds are to art list
	
protected:
	
	void LoadItemArt();	//Call to load all the art in the list
	void ClearItemArt();	//Call when you're going to load more art and some of it might be the same
	void DeleteAllItemArt();	//Call to delete all the art in the list regardless
	
#if defined (PLATFORM_IOS) || defined (PLATFORM_ANDROID)
	DeviceInputState m_deviceInputState;
#endif
	
private:
	bool WillArtDescriptionBeLoaded(ItemArtDescription* pArtDesc);
	
	ItemArtDescription* m_pLoadedArtDescriptions[GAME_MAX_ART_DESCRIPTIONS];
    u32 m_numLoadedArtDescriptions;
    
    ItemArtDescription* m_pArtDescriptionsToLoadTexturesFor[GAME_MAX_ART_DESCRIPTIONS];
    u32 m_numArtDescriptionsToLoadTexturesFor;
	
	CoreUI_Button m_ui_buttons[GAME_MAX_BUTTONS];
	u32 m_ui_numButtons;
};

#endif
