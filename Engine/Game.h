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

#define GAME_MAX_ART_DESCRIPTIONS 128

typedef struct
{
	GLuint			textureFilterMode;
	GLuint			wrapModeU;
	GLuint			wrapModeV;
    RenderMaterial	renderMaterial;
    u32				renderFlags;
} MaterialSettings;

typedef struct
{
    const char*		textureFileName;
    ImageType		imageType;
    GLuint			textureHandle;
	MaterialSettings	materialSettings;
    ModelData*		pModelData;
} ItemArtDescription;

class Game
{
public:
	virtual void Init(OpenGLRenderer* pRenderer);
	virtual void Update(f32 timeElapsed) = 0;
	virtual void CleanUp() = 0;
protected:
	OpenGLRenderer*  m_pRenderer;
	
	void AddArtDescriptionToLoad(ItemArtDescription* pArtDescription);
	bool WillArtDescriptionBeLoaded(ItemArtDescription* pArtDesc);
	void LoadItemArt();
private:
	ItemArtDescription* m_pLoadedArtDescriptions[GAME_MAX_ART_DESCRIPTIONS];
    u32 m_numLoadedArtDescriptions;
    
    ItemArtDescription* m_pArtDescriptionsToLoadTexturesFor[GAME_MAX_ART_DESCRIPTIONS];
    u32 m_numArtDescriptionsToLoadTexturesFor;
};


//UTIL
void DeleteTexture(GLuint* pTextureID);

#endif
