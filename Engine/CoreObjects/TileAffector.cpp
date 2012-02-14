//
//  TileAffector.cpp
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "TileAffector.h"

#include "Game.h"

#include "matrix.h"
#include "OpenGLRenderer.h"
#include "CoreObject_Manager.h"
#include "CoreObjectFactories.h"

#include "Hash.h"


bool TileAffector::SpawnInit(void* pSpawnStruct)
{
	SpawnableEntity* pSpawnableEnt = (SpawnableEntity*)pSpawnStruct;
	if(pSpawnableEnt == NULL)
	{
		return false;
	}
	
	m_tileIndex_X = pSpawnableEnt->tileIndexX;
	m_tileIndex_Y = pSpawnableEnt->tileIndexY;
	
	for (pugi::xml_node property = pSpawnableEnt->pProperties.child("property"); property; property = property.next_sibling("property"))
	{
		
	}
	
	return true;
}


void TileAffector::ProcessMessage(u32 message)	//Pass in a hash value
{
	if(message == Hash("On"))
	{
		GAME->ToggleTileVisibility(LevelLayer_Main1,m_tileIndex_X,m_tileIndex_Y,true);
	}
	else if(message == Hash("Off"))
	{
		GAME->ToggleTileVisibility(LevelLayer_Main1,m_tileIndex_X,m_tileIndex_Y,false);
	}
}


void TileAffector::Uninit()
{
	
}

