/*
 *  MaterialDeclarations.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 10/9/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#include "MaterialDeclarations.h"

const char* g_MaterialNames[NumRenderMaterials] = 

{
	"MT_TextureAndVertColor",
    "MT_TextureOnlySimple",
    "MT_TextureOnlyDiscard",
    "MT_TextureAndDiffuseColor",
    "MT_TextureAndDiffuseColorDiscard",
	"MT_TextureAndFogColor",
	"MT_TextureAndFogColorDiscard",
	"MT_TextureAndFogColorWithTexcoordOffset",
	"MT_TextureAndDiffuseColorWithTexcoordOffset",
    "MT_TextureAndDiffuseColorWithTexcoordOffsetDiscard",
	//"MT_TextureAndFogColorWithMipMapBlurWithTexcoordOffset",
	"MT_Animation_NewMethod3D",
	"MT_TextureOnlyWithTexcoordOffset",
    "MT_TextureOnlyWithTexcoordOffsetDiscard",
	"MT_Water",
	"MT_BasicPointSprite",
	"MT_PointSpriteColorShine",
    "MT_VertColors",
    "MT_VertWithColorInput",
    "MT_TextureWithColor",
	"MT_WiggleUsingTexcoordV",
	//"MT_SkinnedWithNormalMapping",
	//"MT_TextureWithScrollingMult",
	//"MT_ScrollingTextureWithScrollingMult",
	//"PPMT_Add",
	"PPMT_BlendUsingTexture",
	//"PPMT_Copy",
	//"PPMT_Invert",
	//"PPMT_GaussianBlurH",
	//"PPMT_GaussianBlurV",
	//"PPMT_Multiply",
	//"PPMT_Subtract",
	//"PPMT_Trippin",
	"PPMT_PureColor",
};

