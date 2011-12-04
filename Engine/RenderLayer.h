#ifndef __RENDERLAYER_H
#define __RENDERLAYER_H

/*
 *  RenderLayer.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 12/27/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

typedef enum
{
    RenderLayer_Background,
	RenderLayer_BeforeNormal,
	RenderLayer_Normal,
	RenderLayer_AfterNormal,
	RenderLayer_AlphaBlended1,
	RenderLayer_AlphaBlended2,
	RenderLayer_UI,
	NumRenderLayers,
}RenderLayer;

extern const char* g_RenderLayerNames[NumRenderLayers];

#endif
