//
//  Game.cpp
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "Game.h"


void Game::Init(OpenGLRenderer* pRenderer)
{
	m_pRenderer = pRenderer;
	
	m_numLoadedArtDescriptions = 0;
	m_numArtDescriptionsToLoadTexturesFor = 0;
}


bool Game::WillArtDescriptionBeLoaded(ItemArtDescription* pArtDesc)
{
    //Load unloaded level item textures
    for(u32 i=0; i<m_numArtDescriptionsToLoadTexturesFor; ++i)
    {
        ItemArtDescription* pCurrArtDesc = m_pArtDescriptionsToLoadTexturesFor[i];
        if(pArtDesc == pCurrArtDesc)
        {
            return YES;
        }
    }
    
    return NO;
}


void Game::AddArtDescriptionToLoad(ItemArtDescription* pArtDescription)
{
    //Make sure this description is not already in the list
    for(u32 i=0; i<m_numArtDescriptionsToLoadTexturesFor; ++i)
    {
        
        if(m_pArtDescriptionsToLoadTexturesFor[i] == pArtDescription)
        {
            return;
        }
    }
    
    //It wasn't in the list, so add it
    m_pArtDescriptionsToLoadTexturesFor[m_numArtDescriptionsToLoadTexturesFor] = pArtDescription;
    ++m_numArtDescriptionsToLoadTexturesFor;
}


void Game::LoadItemArt()
{
    //Every time LoadLevel is called, m_numArtDescriptionsToLoadTexturesFor gets set to 0
    //Then add item gets called a bunch of times which increment m_numArtDescriptionsToLoadTexturesFor
    
    //You can call this function later after load if you add more things (like with the level editor)
    
    //Dump old level item textures that don't need to be loaded
    for(u32 i=0; i<m_numLoadedArtDescriptions; ++i)
    {
        ItemArtDescription* pCurrArtDesc = m_pArtDescriptionsToLoadTexturesFor[i];
        if(WillArtDescriptionBeLoaded(pCurrArtDesc) == false)
        {
            DeleteTexture(&pCurrArtDesc->textureHandle);
        }
    }
	
    //Reset the loaded description count to 0
    m_numLoadedArtDescriptions = 0;
	
    //Load unloaded level item textures
    //This list has only unique entries
    for(u32 i=0; i<m_numArtDescriptionsToLoadTexturesFor; ++i)
    {
		ItemArtDescription* pCurrArtDesc = m_pArtDescriptionsToLoadTexturesFor[i];
		MaterialSettings* pMaterialSettings = &pCurrArtDesc->materialSettings;
		
		m_pRenderer->LoadTexture(pCurrArtDesc->textureFileName, pCurrArtDesc->imageType, &pCurrArtDesc->textureHandle, pMaterialSettings->textureFilterMode, pMaterialSettings->wrapModeU, pMaterialSettings->wrapModeV);
        
        //Store the descriptions we've loaded
        m_pLoadedArtDescriptions[m_numLoadedArtDescriptions] = pCurrArtDesc;
        ++m_numLoadedArtDescriptions;
    }
}


//UTIL
void DeleteTexture(GLuint* pTextureID)
{
	if(*pTextureID != 0)
    {
        glDeleteTextures(1,pTextureID);
        *pTextureID = 0;
    }
}