/*
 *  RenderFlags.c
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "RenderFlags.h"

const char* g_RenderFlagNames[NUM_RENDERFLAGS] =
{
	"RenderFlag_CastShadow",
	"RenderFlag_ReceiveShadow",
	"RenderFlag_MarkedForRemoval",
	"RenderFlag_Visible",
	"RenderFlag_UseOrthographicProjection_NDC",
	"RenderFlag_DisableCulling",
	"RenderFlag_AlphaBlended",
    "RenderFlag_DisableDepthTest",
	"RenderFlag_DisableDepthWrite",
	"RenderFlag_DisableDepthRead",
	"RenderFlag_Initialized",
	"RenderFlag_IgnoreViewMatrix",
	"RenderFlag_AdditiveBlending",
	"RenderFlag_InverseCulling",
	"RenderFlag_RenderFrontsAndBacks",	//Not implemented
	"RenderFlag_EnableAlphaTest",
	"RenderFlag_HasOutline",
    "RenderFlag_UseOrthographicProjection_Points",
	"RenderFlag_NonPremultipliedAlpha",
	"RenderFlag_DisableBlending",
	"RenderFlag_EnableDepthTest",
	"RenderFlag_EnableDepthRead",
	"RenderFlag_EnableDepthWrite",
};
