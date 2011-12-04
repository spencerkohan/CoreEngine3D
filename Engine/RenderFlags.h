#ifndef __RENDERFLAGS_H
#define __RENDERFLAGS_H

/*
 *  RenderFlags.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "MathTypes.h"

typedef enum
{
	RenderFlag_CastShadow = 1<<0,
	RenderFlag_ReceiveShadow = 1<<1,
	RenderFlag_MarkedForRemoval = 1<<2,
	RenderFlag_Visible = 1<<3,
	RenderFlag_UseOrthographicProjection_NDC = 1<<4,
	RenderFlag_DisableCulling = 1<<5,
	RenderFlag_AlphaBlended = 1<<6,
    RenderFlag_DisableDepthTest = 1<<7,
	RenderFlag_DisableDepthWrite = 1<<8,
	RenderFlag_DisableDepthRead = 1<<9,
	RenderFlag_Initialized = 1<<10,
	RenderFlag_IgnoreViewMatrix = 1<<11,
	RenderFlag_AdditiveBlending = 1<<12,
	RenderFlag_InverseCulling = 1<<13,
	RenderFlag_RenderFrontsAndBacks = 1<<14,	//Not implemented
	RenderFlag_EnableAlphaTest = 1<<15,
	RenderFlag_HasOutline = 1<<16,
    RenderFlag_UseOrthographicProjection_Points = 1<<17,
	RenderFlag_NonPremultipliedAlpha = 1 << 18,
	RenderFlag_DisableBlending = 1<<19,
} RenderFlags;

const u32 RenderFlagDefaults_2DTexture_NoAlpha = RenderFlag_IgnoreViewMatrix|RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_DisableBlending|RenderFlag_DisableDepthTest;
const u32 RenderFlagDefaults_2DTexture_AlphaBlended = RenderFlag_IgnoreViewMatrix|RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_DisableDepthTest;

#define NUM_RENDERFLAGS 20

extern const char* g_RenderFlagNames[NUM_RENDERFLAGS];

#endif
