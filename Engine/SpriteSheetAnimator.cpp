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
	
	int frameCount = 0;
	int prevFrameCount = 0;
	for(int i=0; i<SPRITESHEET_MAX_SHEETS; ++i)
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
	
	pOut_SpriteFrameInfo->textureHandle = *pSheet->pTextureHandle;
	pOut_SpriteFrameInfo->cellsPerRow = cellsPerRow;
	pOut_SpriteFrameInfo->textureOffset.x = uIncrement*(f32)cellX;
	pOut_SpriteFrameInfo->textureOffset.y = vIncrement*(f32)cellY;
	
	return true;
}

void SpriteSheetAnimator::SetSpriteSheet(u32 spriteSheetIndex, u32* pTextureHandle, u32 cellsPerRow)
{
	if(spriteSheetIndex > SPRITESHEET_MAX_SHEETS-1)
	{
		return;
	}
	
	SpriteSheet* pSheet = &m_spriteSheets[spriteSheetIndex];
	pSheet->cellsPerRow = cellsPerRow;
	pSheet->pTextureHandle = pTextureHandle;
	pSheet->totalCells = cellsPerRow*cellsPerRow;
}