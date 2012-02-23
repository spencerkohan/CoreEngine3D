//
//  SpriteSheetAnimator.cpp
//  Crow-Regime(iOS)
//
//  Created by Jody McAdams on 12/17/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#include "SpriteSheetAnimator.h"
#include "stddef.h" //for NULL -_-

SpriteSheetAnimator::SpriteSheetAnimator()
{
	
}

bool SpriteSheetAnimator::GetSpriteInfoForFrame(u32 frame, SpriteFrameInfo* pOut_SpriteFrameInfo)
{
	SpriteSheet* pSheet = NULL;
	
	u32 frameCount = 0;
	u32 prevFrameCount = 0;
	for(u32 i=0; i<SPRITESHEET_MAX_SHEETS; ++i)
	{
		SpriteSheet* pCurrSheet = &m_spriteSheets[i];
		
		frameCount += pCurrSheet->totalCells;
		
		if(frame < frameCount)
		{
			pSheet = pCurrSheet;
			break;
		}
		
		prevFrameCount = frameCount;
	}
	
	if(pSheet == NULL)
	{
		return false;
	}
	
	int adjustedFrame = frame-prevFrameCount;
	
	const u32 cellsPerRow = pSheet->cellsPerRow;
	
	u32 cellX = adjustedFrame%cellsPerRow;
	u32 cellY = adjustedFrame/cellsPerRow;
	
	const f32 uIncrement = 1.0f/(f32)cellsPerRow;
	const f32 vIncrement = 1.0f/(f32)cellsPerRow;
	
	const f32 halfTexel = 0.5f/pSheet->textureSize;
	
	pOut_SpriteFrameInfo->pTextureHandle = pSheet->pTextureHandle;
	pOut_SpriteFrameInfo->cellsPerRow = cellsPerRow;
	pOut_SpriteFrameInfo->textureOffset.x = uIncrement*(f32)cellX;
	pOut_SpriteFrameInfo->textureOffset.y = vIncrement*(f32)cellY+halfTexel;
	
	return true;
}

void SpriteSheetAnimator::SetSpriteSheet(u32 spriteSheetIndex, u32* pTextureHandle, f32 textureSize, u32 cellsPerRow)
{
	if(spriteSheetIndex > SPRITESHEET_MAX_SHEETS-1)
	{
		return;
	}
	
	SpriteSheet* pSheet = &m_spriteSheets[spriteSheetIndex];
	pSheet->textureSize = textureSize;
	pSheet->cellsPerRow = cellsPerRow;
	pSheet->pTextureHandle = pTextureHandle;
	pSheet->totalCells = cellsPerRow*cellsPerRow;
}