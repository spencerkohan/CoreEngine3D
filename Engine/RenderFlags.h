#ifndef __RENDERFLAGS_H
#define __RENDERFLAGS_H

#include "MathTypes.h"

/*
 *  RenderFlags.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

typedef enum
{
	RenderFlag_CastShadow = 1<<0,
	RenderFlag_ReceiveShadow = 1<<1,
	RenderFlag_Visible = 1<<2,
	RenderFlag_UseOrthographicProjection_NDC = 1<<3,
	RenderFlag_DisableCulling = 1<<4,
	RenderFlag_AlphaBlended = 1<<5,
    RenderFlag_DisableDepthTest = 1<<6,
	RenderFlag_DisableDepthWrite = 1<<7,
	RenderFlag_DisableDepthRead = 1<<8,
	RenderFlag_Initialized = 1<<9,
	RenderFlag_IgnoreViewMatrix = 1<<10,
	RenderFlag_AdditiveBlending = 1<<11,
	RenderFlag_InverseCulling = 1<<12,
	RenderFlag_RenderFrontsAndBacks = 1<<13,	//Not implemented
	RenderFlag_EnableAlphaTest = 1<<14,
	RenderFlag_HasOutline = 1<<15,
    RenderFlag_UseOrthographicProjection_Points = 1<<16,
	RenderFlag_NonPremultipliedAlpha = 1 << 17,
	RenderFlag_DisableBlending = 1<<18,
	RenderFlag_EnableDepthTest = 1<<19,
	RenderFlag_EnableDepthRead = 1<<20,
	RenderFlag_EnableDepthWrite = 1<<21,
} RenderFlags;

#define NUM_RENDERFLAGS 23
extern const char* g_RenderFlagNames[NUM_RENDERFLAGS];

const u32 RenderFlagDefaults_2DTexture_NoAlpha = RenderFlag_IgnoreViewMatrix|RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_DisableBlending|RenderFlag_DisableDepthTest;
const u32 RenderFlagDefaults_2DTexture_AlphaBlended = RenderFlag_IgnoreViewMatrix|RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_DisableDepthTest;
const u32 RenderFlagDefaults_2DTexture_AlphaBlended_UseView = RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_DisableDepthTest;
const u32 RenderFlagDefaults_2DTexture_NoAlpha_UseView = RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_DisableBlending|RenderFlag_DisableDepthTest;

const u32 RenderFlagDefaults_2DTexture_PreMultipliedAlphaBlended_UseView = RenderFlag_UseOrthographicProjection_Points|RenderFlag_DisableCulling|RenderFlag_AlphaBlended|RenderFlag_DisableDepthTest;

const u32 RenderFlagDefaults_2DTexture_Fullscreen = RenderFlag_IgnoreViewMatrix|RenderFlag_UseOrthographicProjection_NDC|RenderFlag_DisableCulling|RenderFlag_DisableBlending|RenderFlag_DisableDepthTest;
const u32 RenderFlagDefaults_3DModel = RenderFlag_DisableBlending|RenderFlag_EnableDepthTest|RenderFlag_EnableDepthRead|RenderFlag_EnableDepthWrite;
const u32 RenderFlagDefaults_3DModelWithAlpha = RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_EnableDepthTest|RenderFlag_EnableDepthRead|RenderFlag_EnableDepthWrite;
const u32 RenderFlagDefaults_CardWithAlpha = RenderFlag_DisableCulling|RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_DisableDepthTest;
const u32 RenderFlagDefaults_CardWithAlpha_DepthWrite = RenderFlag_DisableCulling|RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_EnableDepthTest|RenderFlag_EnableDepthWrite|RenderFlag_DisableDepthRead;
const u32 RenderFlagDefaults_3DCard_NoAlpha = RenderFlag_AlphaBlended|RenderFlag_NonPremultipliedAlpha|RenderFlag_EnableDepthTest|RenderFlag_EnableDepthRead|RenderFlag_EnableDepthWrite|RenderFlag_DisableCulling;


#endif
