//
//  Til.h
//  Game(iOS)
//
//  Created by Jody McAdams on 12/27/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef __Tile_h
#define __Tile_h

#include "../MathTypes.h"
#include "../GraphicsTypes.h"

#include "../CoreGameObject.h"
#include <Box2D/Box2D.h>

struct TileSetDescription
{
	//Data
	char* name;
	char* textureFileName;
	u32 loadedTextureID;
	u32 firstTileIndex;
	u32 textureSizeX;
	u32 textureSizeY;
	u32 tileSizeX;
	u32 tileSizeY;
	
	//Useful calculations
	u32 numTextureTilesX;
	u32 numTextureTilesY;
	f32 halfTexelOffset;
	ModelData* pModelData;
};

class Tile: public CoreGameObject
{
public:
	virtual b2Body* Box2D_GetBody(){return pBody;}
	
	s32 tileID;
	TileSetDescription* pDesc;
	CoreObjectHandle hRenderable;
	vec2 texCoordOffset;
	s32 indexX;
	s32 indexY;
	
	b2Body* pBody;
private:
	
};


#endif
