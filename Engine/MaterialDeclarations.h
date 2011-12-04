#ifndef __MATERIALDECLARATIONS_H
#define __MATERIALDECLARATIONS_H
/*
 *  MaterialDeclarations.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 9/3/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

typedef enum
{
	MT_AgentsTest,
	MT_Terrain,
	MT_Rope,
    MT_TextureOnlySimple,
    MT_TextureOnlyDiscard,
    MT_TextureAndDiffuseColor,
    MT_TextureAndDiffuseColorDiscard,
	MT_TextureAndFogColorDiscard,
    MT_TextureAndDiffuseColorWithTexcoordOffsetDiscard,
	MT_BlockColor,
	MT_FontTest,
	MT_Animation_NewMethod3D,
	MT_TextureOnlyWithTexcoordOffset,
    MT_TextureOnlyWithTexcoordOffsetDiscard,
	MT_3DBackground,
	MT_Water,
	MT_BasicPointSprite,
	MT_PointSpriteColorShine,
	MT_EnergyBar,
	MT_CellShading,
	MT_ScrollingTexture_H,
    MT_VertColors,
    MT_VertWithColorInput,
    MT_VertWithColorInputAndTexture,
    MT_TextureWithColor,
	MT_WiggleUsingTexcoordV,
	PPMT_Add,
	PPMT_BlendUsingTexture,
	PPMT_Copy,
	PPMT_FinalProcessing,
	PPMT_Invert,
	PPMT_GaussianBlurH,
	PPMT_GaussianBlurV,
	PPMT_Multiply,
	PPMT_Subtract,
	PPMT_Trippin,
	PPMT_PureColor,
	NumRenderMaterials,			//Number of materials
}RenderMaterial;

#define RENDERMATERIAL_INVALID 999999

extern const char* g_MaterialNames[NumRenderMaterials]; //Defined in the C file

#endif
