//
//  SpriteSheetAnimator.h
//  Crow-Regime(iOS)
//
//  Created by Jody McAdams on 12/17/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef Crow_Regime_iOS__SpriteSheetAnimator_h
#define Crow_Regime_iOS__SpriteSheetAnimator_h

#include "MathTypes.h"

#define SPRITESHEET_MAX_SHEETS 3

struct SpriteFrameInfo
{
	vec2 textureOffset;
	u32 textureHandle;
	u32 cellsPerRow;
};

struct SpriteSheet
{
	u32 cellsPerRow;
	u32 textureHandle;
	u32 totalCells;
};

class SpriteSheetAnimator
{
public:
	SpriteSheetAnimator();
	void SetSpriteSheet(u32 spriteSheetIndex, u32 textureHandle, u32 cellsPerRow);
	bool GetSpriteInfoForFrame(u32 frame, SpriteFrameInfo* pOut_SpriteFrameInfo);
private:
	SpriteSheet m_spriteSheets[SPRITESHEET_MAX_SHEETS];
};

#endif
