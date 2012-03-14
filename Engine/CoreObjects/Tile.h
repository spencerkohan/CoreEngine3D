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

const u8 TILE_FLIPPED_HORIZONTALLY_FLAG = 0x80;
const u8 TILE_FLIPPED_VERTICALLY_FLAG   = 0x40;
const u8 TILE_FLIPPED_DIAGONALLY_FLAG   = 0x20;

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
	b2Body* pBody;	
	CoreObjectHandle hRenderable;
	vec2 texCoordOffset;
	u16 indexX;
	u16 indexY;
	u8 flags;
	u8 pad0;
	
private:
	
};


#endif
