//
//  Game.cpp
//  InfiniSurv(OSX)
//
//  Created by Jody McAdams on 11/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "Game.h"
#include "MathUtil.h"
#include "matrix.h"

#include "zlib/zlib.h"
#include "pugixml/src/pugixml.hpp"
#include "base64.h"
#include "Hash.h"

#if defined (PLATFORM_WIN)
#include <direct.h>
#include <stdlib.h>
#endif

Game* GAME = NULL;

const MaterialSettings g_Game_BlobShadowSettings =
{
	GL_LINEAR,//GLuint			textureFilterMode;
	GL_CLAMP_TO_EDGE,//GLuint			wrapModeU;
	GL_CLAMP_TO_EDGE,//GLuint			wrapModeV;
    MT_TextureOnlySimple,//RenderMaterial	renderMaterial;
    RenderFlagDefaults_CardWithAlpha,//u32				renderFlags;
	true,
};

ItemArtDescription g_Game_BlobShadowDesc =
{
	"ArtResources/Textures/shadowblob.png",//const char*		textureFileName;
	ImageType_PNG,//ImageType		imageType;
	0,//GLuint			textureHandle;
	&g_Game_BlobShadowSettings,//MaterialSettings	materialSettings;
	&g_Square1x1_modelData//ModelData*		pModelData;
};

bool Game::Init()
{
	//Register the common models people will use
	GLRENDERER->RegisterModel(&g_Square1x1_modelData);
	GLRENDERER->RegisterModel(&g_Square_Tiled_2_modelData);
	GLRENDERER->RegisterModel(&g_Square_Tiled_4_modelData);
	GLRENDERER->RegisterModel(&g_Square_Tiled_8_modelData);
	GLRENDERER->RegisterModel(&g_Square_Tiled_16_modelData);

#if defined (PLATFORM_WIN)
	char currentPath[_MAX_PATH];
	//GetCurrentDirectory(_MAX_PATH,currentPath);
	GetModuleFileName(0,currentPath,_MAX_PATH);
	std::string pathString(currentPath);

	s32 lastSlashIndex = 0;
	for(u32 i=0; i<pathString.size(); ++i)
	{
		if(pathString[i] == '\\')
		{
			lastSlashIndex = i;
		}
	}

	m_path = pathString.substr(0,lastSlashIndex+1);

	OutputDebugString(m_path.c_str());
#endif

	m_currSongSource = 0;
	m_currSongBuffer = 0;

	m_numLoadedArtDescriptions = 0;
	m_numArtDescriptionsToLoadTexturesFor = 0;
	m_numLoadedSoundDescriptions = 0;
	m_numSoundDescriptionsToLoadWavsFor = 0;
	m_ui_numButtons = 0;
	m_numBreakables = 0;
	
	m_numSongsInPlaylist = 0;
	m_currSongID = -1;
	
#if defined (PLATFORM_IOS)
	m_pTouchInput = [[[TouchInputIOS alloc]init:&m_deviceInputState]retain];
#endif
	
	m_pCoreAudioOpenAL = new CoreAudioOpenAL;
	m_pCoreAudioOpenAL->Init();
	
#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
	m_pAudioPlayer = nil;
#endif

#if defined (PLATFORM_IOS)
	AVAudioSession *session = [AVAudioSession sharedInstance];
	
	NSError* error;
	[session setCategory:AVAudioSessionCategorySoloAmbient error:&error];
#endif
	
	m_coreObjectManager = new CoreObjectManager;
	
	GAME = this;

	return true;
}

void Game::CleanUp()
{
	delete m_pCoreAudioOpenAL;
	delete m_coreObjectManager;
}

void Game::Update(f32 timeElapsed)
{
	UpdateBreakables(timeElapsed);

#if defined (PLATFORM_OSX) || defined(PLATFORM_WIN)
	
	for(u32 i=0; i<MOUSESTATE_MAX_MOUSEBUTTONS; ++i)
	{
		const MouseButtonState moustState = m_mouseState.buttonState[i];

		if(moustState == MouseButtonState_Began)
		{
			m_mouseState.buttonState[i] = MouseButtonState_Held;
		}
		else if(moustState == MouseButtonState_Ended)
		{
			m_mouseState.buttonState[i] = MouseButtonState_None;
		}
	}
#endif

	//Lazy so constantly load new resources
	//It can't be THAT bad
	LoadItemArt();
	LoadItemSounds();
}

s32 Game::AddSongToPlaylist(const char* songFilenameMP3)
{
	const u32 songID = m_numSongsInPlaylist;
	
	m_songPlaylist[songID] = new char[strlen(songFilenameMP3+1)];
	strcpy(m_songPlaylist[songID], songFilenameMP3);
	
	++m_numSongsInPlaylist;
	
	return songID;
}


std::string Game::GetPathToFile(const char* filename)
{
#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
	NSString* fileString = [NSString stringWithCString:filename encoding:NSUTF8StringEncoding];
	NSString *fullPath = [[NSBundle mainBundle] pathForResource:[fileString lastPathComponent] ofType:nil inDirectory:[fileString stringByDeletingLastPathComponent]];
	
	return [fullPath UTF8String];
#endif

#if defined (PLATFORM_WIN)
	return m_path + std::string(filename);
#endif
}


void Game::PlaySongByID(s32 songID, f32 volume, bool isLooping)
{
	if(songID >= (s32)m_numSongsInPlaylist)
	{
		return;
	}
	
	if(m_currSongID == songID)
	{
		return;
	}
	
#if defined (PLATFORM_OSX) || defined (PLATFORM_IOS)
	[m_pAudioPlayer stop];
	[m_pAudioPlayer release];
	
	NSString* fileString = [NSString stringWithCString:m_songPlaylist[songID] encoding:NSUTF8StringEncoding];
	NSString *fullPath = [[NSBundle mainBundle] pathForResource:[fileString lastPathComponent] ofType:nil inDirectory:[fileString stringByDeletingLastPathComponent]];
	NSURL *soundURL = [NSURL fileURLWithPath:fullPath];
	
	//m_pAudioPlayer = CreateAudioPlayer(fullPath,@"",YES,1.0f);
	NSError* error;
	m_pAudioPlayer = [[AVAudioPlayer alloc] initWithContentsOfURL:soundURL error:&error];
	m_pAudioPlayer.volume = volume;
	m_pAudioPlayer.numberOfLoops = isLooping?-1:0;
	
	[m_pAudioPlayer play];
#endif

#if defined (PLATFORM_WIN)
	/*if(m_currSongSource != 0)
	{
		OPENALAUDIO->DeleteSoundSource(&m_currSongSource);
	}

	if(m_currSongBuffer != 0)
	{
		OPENALAUDIO->DeleteSoundBuffer(&m_currSongBuffer);
	}

	std::string filePath = GetPathToFile(m_songPlaylist[songID]);
	OPENALAUDIO->CreateSoundBufferFromFile(filePath.c_str(),&m_currSongBuffer);
	m_currSongSource = OPENALAUDIO->CreateSoundSourceFromBuffer(m_currSongBuffer);
	OPENALAUDIO->PlaySoundSource(m_currSongSource,1.0f,1.0f,true);*/
#endif
	
	m_currSongID = songID;
}


CoreUI_Button* Game::AddUIButton(u32 width, u32 height, CoreUI_AttachSide attachSide, s32 offsetX, s32 offsetY, u32* textureHandle, s32 value, void (*callback)(s32))
{
	if(m_ui_numButtons == GAME_MAX_BUTTONS)
	{
		return NULL;
	}
	
	CoreUI_Button* pButton = &m_ui_buttons[m_ui_numButtons];
	pButton->Init(width, height, attachSide, offsetX, offsetY, textureHandle, value, callback);
	
	++m_ui_numButtons;
	
	return pButton;
}

void Game::ClearAllButtons()
{
	m_ui_numButtons = 0;
}


void Game::UpdateButtons(TouchState touchState, vec2 *pTouchPosBegin, vec2* pTouchPosCurr)
{
	for(u32 i=0; i<m_ui_numButtons; ++i)
	{
		if(touchState != TouchState_None)
		{
			m_ui_buttons[i].PressButton(touchState, pTouchPosBegin, pTouchPosCurr);
		}
	}
}


//Checks if the art will be loaded next time LoadItemArt gets called
bool Game::WillArtDescriptionBeLoaded(ItemArtDescription* pArtDesc)
{
    for(u32 i=0; i<m_numArtDescriptionsToLoadTexturesFor; ++i)
    {
        ItemArtDescription* pCurrArtDesc = m_pArtDescriptionsToLoadTexturesFor[i];
        if(pArtDesc == pCurrArtDesc)
        {
            return true;
        }
    }
    
    return false;
}


//Call many times to prepare art to be loaded later
void Game::AddItemArt(ItemArtDescription* pArtDescription)
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


//Call after all your art has been added to the list of things to load gunna die
//You can call this multiple times if you want and nothing bad will happen
void Game::LoadItemArt()
{
    //Dump old textures that don't need to be loaded
    for(u32 i=0; i<m_numLoadedArtDescriptions; ++i)
    {
        ItemArtDescription* pCurrArtDesc = m_pLoadedArtDescriptions[i];
        if(m_numArtDescriptionsToLoadTexturesFor == 0
		   || WillArtDescriptionBeLoaded(pCurrArtDesc) == false)
        {
            GLRENDERER->DeleteTexture(&pCurrArtDesc->textureHandle);
        }
    }
	
    //Reset the loaded description count to 0
    m_numLoadedArtDescriptions = 0;
	
    //Load unloaded level item textures
    //This list has only unique entries
    for(u32 i=0; i<m_numArtDescriptionsToLoadTexturesFor; ++i)
    {
		ItemArtDescription* pCurrArtDesc = m_pArtDescriptionsToLoadTexturesFor[i];
		const MaterialSettings* pMaterialSettings = pCurrArtDesc->materialSettings;
		
		//This will load the texture but if it's already loaded, it will do nothing
		GLRENDERER->LoadTexture(pCurrArtDesc->textureFileName, pCurrArtDesc->imageType, &pCurrArtDesc->textureHandle, pMaterialSettings->textureFilterMode, pMaterialSettings->wrapModeU, pMaterialSettings->wrapModeV,pMaterialSettings->flipY);
        
        //Store the descriptions we've loaded
		//Some of them were already loaded but now we can keep track!
        m_pLoadedArtDescriptions[m_numLoadedArtDescriptions] = pCurrArtDesc;
        ++m_numLoadedArtDescriptions;
    }
}


//Call this at init, then call AddArtDescriptionToLoad many times.
//The art you're not using anymore will get deleted when you call LoadItemArt
void Game::ClearItemArt()
{
	m_numArtDescriptionsToLoadTexturesFor = 0;
}


//Just deletes all the loaded art PERIOD
//You might never have to call this
void Game::DeleteAllItemArt()
{
	//Dump old textures that don't need to be loaded
    for(u32 i=0; i<m_numLoadedArtDescriptions; ++i)
    {
        ItemArtDescription* pCurrArtDesc = m_pLoadedArtDescriptions[i];
		GLRENDERER->DeleteTexture(&pCurrArtDesc->textureHandle);
    }
	
	m_numLoadedArtDescriptions = 0;
	m_numArtDescriptionsToLoadTexturesFor = 0;
}


//SOUND

//Checks if the art will be loaded next time LoadItemArt gets called
bool Game::WillSoundDescriptionBeLoaded(ItemSoundDescription* pSoundDesc)
{
    for(u32 i=0; i<m_numSoundDescriptionsToLoadWavsFor; ++i)
    {
        ItemSoundDescription* pCurrSoundDesc = m_pSoundDescriptionsToLoadWavsFor[i];
        if(pSoundDesc == pCurrSoundDesc)
        {
            return true;
        }
    }
    
    return false;
}


//Call many times to prepare sound to be loaded later
void Game::AddItemSound(ItemSoundDescription* pSoundDescription)
{
    //Make sure this description is not already in the list
    for(u32 i=0; i<m_numSoundDescriptionsToLoadWavsFor; ++i)
    { 
        if(m_pSoundDescriptionsToLoadWavsFor[i] == pSoundDescription)
        {
            return;
        }
    }
    
    //It wasn't in the list, so add it
    m_pSoundDescriptionsToLoadWavsFor[m_numSoundDescriptionsToLoadWavsFor] = pSoundDescription;
    ++m_numSoundDescriptionsToLoadWavsFor;
}


//Call after all your sound has been added to the list of things to load gunna die
//You can call this multiple times if you want and nothing bad will happen
void Game::LoadItemSounds()
{
    //Dump old sounds that don't need to be loaded
    for(u32 i=0; i<m_numLoadedSoundDescriptions; ++i)
    {
        ItemSoundDescription* pCurrSoundDesc = m_pLoadedSoundDescriptions[i];
        if(m_numSoundDescriptionsToLoadWavsFor == 0
		   || WillSoundDescriptionBeLoaded(pCurrSoundDesc) == false)
        {
			OPENALAUDIO->DeleteSoundBuffer(&pCurrSoundDesc->soundBufferID);
        }
    }
	
    //Reset the loaded description count to 0
    m_numLoadedSoundDescriptions = 0;
	
    //Load unloaded level item textures
    //This list has only unique entries
    for(u32 i=0; i<m_numSoundDescriptionsToLoadWavsFor; ++i)
    {
		ItemSoundDescription* pCurrSoundDesc = m_pSoundDescriptionsToLoadWavsFor[i];
	
		OPENALAUDIO->CreateSoundBufferFromFile(pCurrSoundDesc->soundFileName, &pCurrSoundDesc->soundBufferID);
		
        //Store the descriptions we've loaded
		//Some of them were already loaded but now we can keep track!
        m_pLoadedSoundDescriptions[m_numLoadedSoundDescriptions] = pCurrSoundDesc;
        ++m_numLoadedSoundDescriptions;
    }
}


//Call this at init, then call AddArtDescriptionToLoad many times.
//The art you're not using anymore will get deleted when you call LoadItemArt
void Game::ClearItemSounds()
{
	m_numSoundDescriptionsToLoadWavsFor = 0;
}


//Just deletes all the loaded art PERIOD
//You might never have to call this
void Game::DeleteAllItemSounds()
{
	//Dump old sounds that don't need to be loaded
    for(u32 i=0; i<m_numLoadedSoundDescriptions; ++i)
    {
        ItemSoundDescription* pCurrSoundDesc = m_pLoadedSoundDescriptions[i];
		OPENALAUDIO->DeleteSoundBuffer(&pCurrSoundDesc->soundBufferID);
    }
	
	m_numLoadedSoundDescriptions = 0;
	m_numSoundDescriptionsToLoadWavsFor = 0;
}


void Game::UpdateBreakables(f32 timeElapsed)
{
	//TODO: this is probably BAD
	
	//Delete old breakables
	for(u32 i=0; i<m_numBreakables; )
    {
        Breakable* pCurrBreakable = &m_updatingBreakables[i];
        
        //Kill off old breakables
        if(pCurrBreakable->lifeTimer < 0.0f)
        {
			Breakable* pLastBreakable = &m_updatingBreakables[m_numBreakables-1];
			
			//Get handles to both renderables
			RenderableGeometry3D* pCurrGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(pCurrBreakable->handleRenderable);
			//Delete the current geom because it's getting overwritten
			pCurrGeom->Uninit();

			if(m_numBreakables > 1)
			{
				//Overwrite the breakable
				*pCurrBreakable = *pLastBreakable;
			}
			
			--m_numBreakables;
        }
		else
		{
			++i;
		}
	}
	
	//Update breakables
    for(u32 i=0; i<m_numBreakables; ++i)
    {
        Breakable* pCurrBreakable = &m_updatingBreakables[i];
        
		RenderableGeometry3D* pCurrGeom = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(pCurrBreakable->handleRenderable);
		
		pCurrBreakable->lifeTimer -= timeElapsed;
		
        const f32 breakableAlpha = ClampF(pCurrBreakable->lifeTimer/0.15f,0.0f,1.0f);
        ScaleVec4(&pCurrBreakable->diffuseColor,&pCurrBreakable->diffuseColorStart,breakableAlpha);
        
        BreakableData* pData = pCurrBreakable->pBreakableData;
        
        pCurrBreakable->currSpinAngle += pCurrBreakable->spinSpeed*timeElapsed;
        
        
        vec3* pBreakablePos = mat4f_GetPos(pCurrGeom->worldMat);
        
        pCurrBreakable->velocity.y -= pData->pSettings->gravity*timeElapsed;
        AddScaledVec3_Self(pBreakablePos,&pCurrBreakable->velocity,timeElapsed);
        
        vec3 velNorm;
        TryNormalizeVec3(&velNorm,&pCurrBreakable->velocity);
        
        const f32 maxZForScale = 40.0f;
        const f32 maxZScale = 3.5f;
        
        f32 radius = pData->radius;
        if(pData->scaleWithZ)
        {
            const f32 zScaleT = MinF(pBreakablePos->z/maxZForScale,1.0f);
            radius = Lerp(pData->radius,maxZScale,zScaleT);
        }
        
        mat4f_LoadScaledZRotation_IgnoreTranslation(pCurrGeom->worldMat, pCurrBreakable->currSpinAngle, radius);
        
										
		//Have to relink up the uniform values because they're basicaly gone
		//TODO: make this better
		pCurrGeom->material.uniqueUniformValues[0] = (u8*)&pCurrBreakable->texcoordOffset;
		pCurrGeom->material.uniqueUniformValues[1] = (u8*)&pCurrBreakable->diffuseColor;

		//Bouncing disabled for now
		
        /*if(pData->pSettings->doesBounce && pBreakablePos->y <= 0.0f)
        {
            pBreakablePos->y = 0.0f;
            
            const f32 dotProd = -DotVec3(&g_GameBox_normal_Floor,&velNorm);
            //printf("DotProd: %f\n",dotProd);
            
            const f32 finalDamp = Lerp(1.0f,pDesc->pSettings->bounceDamping,dotProd);
            //printf("Damping mult: %f\n",finalDamp);
            ScaleVec3_Self(&pCurrBreakable->velocity,finalDamp);
            
            pCurrBreakable->velocity.y *= -1.0f;
        }*/
    }
}


void Game::SpawnBreakable(BreakableData* pData, const vec3* pPosition, const vec3* pDirection, u32 breakableIndex, const vec4* diffuseColor, RenderLayer renderLayer)
{
	if(m_numBreakables == GAME_MAX_BREAKABLES)
	{
		return;
	}
	
	//printf("Spawned breakable!\n");
	
	Breakable* pCurrBreakable = &m_updatingBreakables[m_numBreakables];
	
	pCurrBreakable->pBreakableData = pData;
	ItemArtDescription* pArtDesc = &pData->itemArt;
	const MaterialSettings* pMaterial = pArtDesc->materialSettings;
	
	//[self PlaySoundByFilename:pCurrBreakable->pBreakableDescription->breakSoundName:pPosition:0.0f:FALSE];
	
	RenderableGeometry3D* pRenderable = NULL;
	pCurrBreakable->handleRenderable = GLRENDERER->CreateRenderableGeometry3D_Normal(&pRenderable);
	
	GLRENDERER->InitRenderableGeometry3D(pRenderable, pArtDesc->pModelData, pMaterial->renderMaterial, &pArtDesc->textureHandle, NULL, renderLayer, View_0, pMaterial->renderFlags|RenderFlag_Visible);
	pRenderable->material.uniqueUniformValues[0] = (u8*)&pCurrBreakable->texcoordOffset;
	pRenderable->material.uniqueUniformValues[1] = (u8*)&pCurrBreakable->diffuseColor;
	
	f32 radius = pData->radius;
	
	mat4f_LoadScale(pRenderable->worldMat, radius);
	
	vec3* pPos = mat4f_GetPos(pRenderable->worldMat);
	CopyVec3(pPos, pPosition);
	
	const f32 speed = rand_FloatRange(pData->pSettings->moveSpeedMin, pData->pSettings->moveSpeedMax);
	ScaleVec3(&pCurrBreakable->velocity,pDirection,speed);
	const f32 spinSpeed = rand_FloatRange(pData->pSettings->spinSpeedMin, pData->pSettings->spinSpeedMax);
	pCurrBreakable->spinSpeed = spinSpeed*((rand_FloatRange(0.0f, 1.0f) > 0.5f) ? -1.0f : 1.0f);
	pCurrBreakable->currSpinAngle = 0.0f;
	pCurrBreakable->lifeTimer = pData->pSettings->lifetime;
	
	CopyVec4(&pCurrBreakable->diffuseColor,diffuseColor);
	CopyVec4(&pCurrBreakable->diffuseColorStart,diffuseColor);
	
	switch (breakableIndex)
	{
		case 0:
		{
			SetVec2(&pCurrBreakable->texcoordOffset, 0.0f, 0.0f);
			break;
		}
		case 1:
		{
			SetVec2(&pCurrBreakable->texcoordOffset, 0.5f, 0.0f);
			break;
		}
		case 2:
		{
			SetVec2(&pCurrBreakable->texcoordOffset, 0.0f, 0.5f);
			break;
		}
		case 3:
		{
			SetVec2(&pCurrBreakable->texcoordOffset, 0.5f, 0.5f);
			break;
		}
		default:
		{
			break;
		}
	}
	
	++m_numBreakables;
}

void Game::UpdateTiledLevelPosition(vec3* pPosition)
{
	for(s32 i=0; i<NumLevelLayers; ++i)
	{
#ifndef _DEBUG
		if(i == (s32)LevelLayer_Collision)
		{
			continue;
		}
#endif

		Layer* pCurrLayer = &m_layers[i];
		if(pCurrLayer->tiles == NULL)
		{
			continue;
		}
		
		RenderLayer renderLayer = (RenderLayer)(RenderLayer_Background0+i);

		//If this is the collision layer, it should move at the same rate as the main layer
		const s32 adjustedIndex = (i==(s32)LevelLayer_Collision)?(s32)LevelLayer_Main:i;
		const s32 scrollIndex = 1+(s32)LevelLayer_Main-adjustedIndex;	//TODO: index into an array of values maybe

		//pCurrLayer->position.x -= timeElapsed*(f32)(scrollIndex*scrollIndex*scrollIndex)*scrollSpeed;
		ScaleVec3(&pCurrLayer->position,pPosition,1.0f/(f32)scrollIndex);

		const u32 numTextureTilesX = pCurrLayer->description->textureSizeX/pCurrLayer->description->tileSizeX;
		const u32 numTextureTilesY = pCurrLayer->description->textureSizeY/pCurrLayer->description->tileSizeY;

		const f32 uIncrement = 1.0f/(f32)numTextureTilesX;
		const f32 vIncrement = 1.0f/(f32)numTextureTilesY;

		ModelData* pModelData = NULL;
		
		switch(numTextureTilesX)
		{
			case 2:
			{
				pModelData = &g_Square_Tiled_2_modelData;
				break;
			}
			case 4:
			{
				pModelData = &g_Square_Tiled_4_modelData;
				break;
			}
			case 8:
			{
				pModelData = &g_Square_Tiled_8_modelData;

				break;
			}
			case 16:
			{
				pModelData = &g_Square_Tiled_16_modelData;

				break;
			}
		}

		const s32 tileDisplaySizeX = pCurrLayer->description->tileSizeX/m_tileSizeScaleDiv;
		const s32 tileDisplaySizeY = pCurrLayer->description->tileSizeY/m_tileSizeScaleDiv;

		const s32 halfTileSizeX = tileDisplaySizeX/2;
		const s32 halfTileSizeY = tileDisplaySizeY/2;
		const s32 width = pCurrLayer->numTilesX;
		const s32 height = pCurrLayer->numTilesY;
		
		const s32 distCheckRightAdd = GLRENDERER->screenWidth_points+halfTileSizeX;
		
		for(int y=0; y<height; ++y)
		{
			for(int x=0; x<width; ++x)
			{
				Tile* pTile = &ARRAY2D(pCurrLayer->tiles, x, y, width);
				if(pTile->tileID == -1)
				{
					continue;
				}

				const s32 tileBasePos = x*tileDisplaySizeX+halfTileSizeX;
				
				if(-pCurrLayer->position.x > tileBasePos+halfTileSizeX
				   || -pCurrLayer->position.x+distCheckRightAdd < tileBasePos)
				{
					if(pTile->hRenderable != INVALID_COREOBJECT_HANDLE)
					{
						RenderableGeometry3D* pCurrRenderable = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(pTile->hRenderable);
						pCurrRenderable->Uninit();
						pTile->hRenderable = INVALID_COREOBJECT_HANDLE;
					}
				}
				else
				{
					RenderableGeometry3D* pCurrRenderable;

					if(pTile->hRenderable == INVALID_COREOBJECT_HANDLE)
					{
						f32 tileMat[16];
						mat4f_LoadScale(tileMat, (f32)tileDisplaySizeX);

						pTile->hRenderable = GLRENDERER->CreateRenderableGeometry3D_Normal(&pCurrRenderable);
	
						assert(pTile->hRenderable != INVALID_COREOBJECT_HANDLE);

						GLRENDERER->InitRenderableGeometry3D(pCurrRenderable, pModelData, MT_TextureOnlyWithTexcoordOffset, &pCurrLayer->loadedTextureID, tileMat, renderLayer, View_0, RenderFlagDefaults_2DTexture_AlphaBlended|RenderFlag_Visible);
	
						const s32 tileID_X = pTile->tileID%numTextureTilesX;
						const s32 tileID_Y = pTile->tileID/numTextureTilesX;
					
						pTile->texCoordOffset.x = uIncrement*(f32)tileID_X;
						pTile->texCoordOffset.y = vIncrement*(f32)tileID_Y;
					}
					else
					{
						pCurrRenderable = (RenderableGeometry3D*)COREOBJECTMANAGER->GetObjectByHandle(pTile->hRenderable);
					}

					vec3* pCurrPos = mat4f_GetPos(pCurrRenderable->worldMat);
					pCurrPos->x = 0.5f+tileBasePos+((s32)pCurrLayer->position.x);
					pCurrPos->y = (f32)(y*tileDisplaySizeY+halfTileSizeY);

					//TODO: do something better than this if possible
					pCurrRenderable->material.uniqueUniformValues[0] = (u8*)&pTile->texCoordOffset;
				}
			}
		}
	}
}


bool Game::LoadTiledLevel(std::string& path, std::string& filename, u32 tileSizeScaleDiv)
{
	m_tileSizeScaleDiv = tileSizeScaleDiv;

	m_numSpawnableEntities = 0;

	std::string filenameWithPath(path+filename);
	
    pugi::xml_document doc;
	
	pugi::xml_parse_result result = doc.load_file(GetPathToFile(filenameWithPath.c_str()).c_str());
	
	if(result)
	{
		COREDEBUG_PrintDebugMessage("Parsing map file was successful!\n");

		pugi::xml_node map = doc.child("map");
		
		s32 layerCount = 0;
		for (pugi::xml_node layer = map.child("tileset"); layer; layer = layer.next_sibling("tileset"),++layerCount)
		{
			LayerDescription* pDesc = &m_layerDescriptions[layerCount];

			const char* descName = layer.attribute("name").value();
			const size_t descNameSize = strlen(descName);
			pDesc->name = new char[descNameSize+1];
			strcpy(pDesc->name, descName);
			
			pDesc->firstGID = atoi(layer.attribute("firstgid").value());
			pDesc->tileSizeX = atoi(layer.attribute("tilewidth").value());
			pDesc->tileSizeY = atoi(layer.attribute("tileheight").value());
			
			pugi::xml_node textureNode = layer.child("image");
			
			const char* textureName = textureNode.attribute("source").value();
			const size_t textureNameSize = strlen(textureName);
			pDesc->textureFileName = new char[textureNameSize+1];
			strcpy(pDesc->textureFileName, textureName);
			
			pDesc->textureSizeX = atoi(textureNode.attribute("width").value());
			pDesc->textureSizeY = atoi(textureNode.attribute("height").value());
		}
		
		for (pugi::xml_node layer = map.child("layer"); layer; layer = layer.next_sibling("layer"))
		{
			const char* layerName = layer.attribute("name").value();
			
			LayerDescription* pCurrDesc = NULL;
			
			for(s32 layerIDX=0; layerIDX<layerCount; ++layerIDX)
			{
				LayerDescription* pDescToCheck = &m_layerDescriptions[layerIDX];
				if(strcmp(pDescToCheck->name, layerName) == 0)
				{
					pCurrDesc = pDescToCheck;
					break;
				}
			}
			
			if(pCurrDesc == NULL)
			{
				continue;
			}

			s32* pData = NULL;

			const u32 width = atoi(layer.attribute("width").value());
			const u32 height = atoi(layer.attribute("height").value());

			COREDEBUG_PrintDebugMessage("\nLayer %s, width: %d, height, %d",layerName,width,height);
			
			LevelLayer currLayer = LevelLayer_Invalid;
			
			if(strcmp(layerName, "Main") == 0)
			{
				currLayer = LevelLayer_Main;
			}
			else if(strcmp(layerName, "Parallax0") == 0)
			{
				currLayer = LevelLayer_Parallax0;
			}
			else if(strcmp(layerName, "Parallax1") == 0)
			{
				currLayer = LevelLayer_Parallax1;
			}
			else if(strcmp(layerName, "Parallax2") == 0)
			{
				currLayer = LevelLayer_Parallax2;
			}
			else if(strcmp(layerName, "Parallax3") == 0)
			{
				currLayer = LevelLayer_Parallax3;
			}
			else if(strcmp(layerName, "Parallax4") == 0)
			{
				currLayer = LevelLayer_Parallax4;
			}
			else if(strcmp(layerName, "collision") == 0)
			{
				currLayer = LevelLayer_Collision;
			}
			
			if(currLayer == LevelLayer_Invalid)
			{
				COREDEBUG_PrintDebugMessage("Invalid Layer: Skipping...");

				continue;
			}
			
			Layer* pCurrLayer = &m_layers[currLayer];
			
			const u32 numTiles = width*height;
	
			switch(currLayer)
			{
				case LevelLayer_Parallax4:
				case LevelLayer_Parallax3:
				case LevelLayer_Parallax2:
				case LevelLayer_Parallax1:
				case LevelLayer_Parallax0:
				case LevelLayer_Main:
				case LevelLayer_Collision:
				{
					pCurrLayer->numTilesX = width;
					pCurrLayer->numTilesY = height;
					
					pData = new s32[numTiles];
					
					pCurrLayer->pLevelData = pData;
					
					CopyVec3(&pCurrLayer->position,&vec3_zero);
					
					pCurrLayer->description = pCurrDesc;
					
					//Loads the collision texture if you're in debug mode
					//Loads all other textures regardless
#ifndef _DEBUG
				if(currLayer != LevelLayer_Collision)
				{
#endif
					std::string textureFileName(pCurrDesc->textureFileName);
					std::string texFilenameWithPath(path+textureFileName);
					
					GLRENDERER->LoadTexture(texFilenameWithPath.c_str(), ImageType_PNG, &pCurrLayer->loadedTextureID, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE,true);
#ifndef _DEBUG
				}
#endif
					break;
				}
				default:
				{
					break;
				}
			}
			
			if(pData == NULL)
			{
				//If we have no data to write into, we might as well skip
				COREDEBUG_PrintDebugMessage("This layer will not be saved. Skipping...");

				continue;
			}
			
			pugi::xml_node data = layer.child("data");
			//std::cout << "data: " << data.first_child().value() << '\n';
			
			char* dataToDecode = (char*)data.first_child().value();
			
			const int BUFFER_SIZE = 4096;
			
			unsigned char decodedData[BUFFER_SIZE];
			
			size_t outputLength = base64_decode(dataToDecode,decodedData, BUFFER_SIZE);

			COREDEBUG_PrintDebugMessage("base64_decode...");
			
			const u32 dataSize = numTiles*sizeof(u32);
			unsigned long bufferSize = dataSize;
			
			switch(uncompress((Bytef*)pData, &bufferSize, (Bytef*)decodedData, outputLength))
			{
				case Z_OK:
				{
					COREDEBUG_PrintDebugMessage("ZLIB uncompress was successful!\n");
					
					break;
				}
				case Z_MEM_ERROR:
				{
					COREDEBUG_PrintDebugMessage("ZLIB ERROR: Uncompress failed due to MEMORY ERROR.  Exiting program...\n");
					return false;
				}
				case Z_BUF_ERROR:
				{
					COREDEBUG_PrintDebugMessage("ZLIB ERROR: Uncompress failed due to BUFFER ERROR.  Exiting program...\n");
					return false;
				}
				case Z_DATA_ERROR:
				{
					COREDEBUG_PrintDebugMessage("ZLIB ERROR: Uncompress failed due to DATA ERROR.  Exiting program...\n");
					return false;
				}
			}
			
			for(u32 y=0; y<height; ++y)
			{
				for(u32 x=0; x<width; ++x)
				{
					s32* currInt = &ARRAY2D(pData, x, y, width);
					*currInt &= 0x00FFFFFF;

					*currInt -= pCurrLayer->description->firstGID;
					
					//std::cout << *currInt << ", ";
					
				}
				//std::cout << '\n';
			}
			
			//Make a matrix scaled to the size of the tile
			
			const s32 tileDisplaySizeX = pCurrDesc->tileSizeX/tileSizeScaleDiv;
			//const s32 tileDisplaySizeY = pCurrDesc->tileSizeY/tileSizeScaleDiv;
			
			f32 tileMat[16];
			mat4f_LoadScale(tileMat, (f32)tileDisplaySizeX);
			vec3* pTilePos = mat4f_GetPos(tileMat);
			
			pTilePos->z = 0.0f;
			
			pCurrLayer->tiles = new Tile[width*height];
			
			for(u32 y=0; y<height; ++y)
			{
				for(u32 x=0; x<width; ++x)
				{
					Tile* pTile = &ARRAY2D(pCurrLayer->tiles, x, y, width);
					pTile->hRenderable = INVALID_COREOBJECT_HANDLE;

					s32 tileID = ARRAY2D(pData, x, y, width);
					
					pTile->tileID = tileID;

					if(tileID == -1)
					{
						continue;
					}
				}
			}
		}
		
		for (pugi::xml_node layer = map.child("objectgroup"); layer; layer = layer.next_sibling("objectgroup"))
		{
			const char* layerName = layer.attribute("name").value();

			COREDEBUG_PrintDebugMessage("Layer: %s",layerName);
			
			for (pugi::xml_node object = layer.child("object"); object; object = object.next_sibling("object"))
			{
				//const char* name = object.attribute("name").value();
				const char* typeString = object.attribute("type").value();
				//std::cout << name << '\n';

				SpawnableEntity* pCurrEnt = &m_spawnableEntities[m_numSpawnableEntities];
				const u32 entType = Hash(typeString);
				pCurrEnt->type = entType;
				
				const int x = atoi(object.attribute("x").value())/tileSizeScaleDiv;
				const int y = atoi(object.attribute("y").value())/tileSizeScaleDiv;
				
				const int width = atoi(object.attribute("width").value())/tileSizeScaleDiv;
				const int height = atoi(object.attribute("height").value())/tileSizeScaleDiv;
				
				pCurrEnt->position.x = x+(f32)(width/2);
				pCurrEnt->position.y = y+(f32)(height/2);
				pCurrEnt->position.z = 0.0f;
				
				pCurrEnt->scale.x = width;
				pCurrEnt->scale.y = height;

				++m_numSpawnableEntities;
			}
		}
	}
	else
	{
		COREDEBUG_PrintDebugMessage("Failed to load level file.  FILE NOT FOUND!");
	}
	
	return true;
}
