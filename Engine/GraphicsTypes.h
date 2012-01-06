#ifndef __GRAPHICS_TYPES
#define __GRAPHICS_TYPES

/*
 *  GraphicsTypes.h
 *  CoreEngine3D
 *
 *  Created by Jody McAdams on 8/31/10.
 *  Copyright 2010 GameJelly Online. All rights reserved.
 *
 */

#ifdef PLATFORM_OSX
#include <OpenGL/OpenGL.h>
#endif

#ifdef PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#include "MaterialDeclarations.h"
#include "MathTypes.h"
#include "RenderLayer.h"
#include "RenderFlags.h"
#include "ViewFlags.h"
#include "PowerVR/PVRTModelPOD.h"
#include "CoreObject.h"

#define MAX_SHARED_CONST_UNIFORM_VALUES 4
#define MAX_SHARED_UNIFORM_VALUES 8
#define MAX_UNIQUE_UNIFORM_VALUES 8
#define MAX_LOCALDATA_SLOTS_FOR_RENDERABLE 4

#define MAX_TRAIL_PARTICLES_PER_TRAIL 32

#define DEBUG_GRAPHICS

typedef vec3 color3f;
typedef vec4 color4f;

extern const color3f color3f_black;
extern const color4f color4f_yellow;
extern const color4f color4f_red;

extern const u32 PVRTEX_PIXELTYPE;			// pixel type is always in the last 16bits of the flags

/*!***************************************************************************
 Describes the header of a PVR header-texture
 *****************************************************************************/
typedef struct
{
	u32 dwHeaderSize;			/*!< size of the structure */
	u32 dwHeight;				/*!< height of surface to be created */
	u32 dwWidth;				/*!< width of input surface */
	u32 dwMipMapCount;			/*!< number of mip-map levels requested */
	u32 dwpfFlags;				/*!< pixel format flags */
	u32 dwTextureDataSize;		/*!< Total size in bytes */
	u32 dwBitCount;			/*!< number of bits per pixel  */
	u32 dwRBitMask;			/*!< mask for red bit */
	u32 dwGBitMask;			/*!< mask for green bits */
	u32 dwBBitMask;			/*!< mask for blue bits */
	u32 dwAlphaBitMask;		/*!< mask for alpha channel */
	u32 dwPVR;					/*!< magic number identifying pvr file */
	u32 dwNumSurfs;			/*!< the number of surfaces present in the pvr */
} PVR_Texture_Header;

// Attributes used for shaders
typedef enum {
	ATTRIB_VERTEX,
	ATTRIB_VERTEX2,
	ATTRIB_NORMAL,
	ATTRIB_NORMAL2,
	ATTRIB_COLOR,
	ATTRIB_TEXCOORD,
	ATTRIB_TEXCOORD2,
    ATTRIB_TANGENT,
    ATTRIB_BINORMAL,
	ATTRIB_BONEINDEX,
	ATTRIB_BONEWEIGHT,
	NUM_ATTRIBUTES
} Attributes;

typedef enum {
	ATTRIBFLAG_VERTEX = 1 << 0,
	ATTRIBFLAG_VERTEX2 = 1 << 1,
	ATTRIBFLAG_NORMAL = 1 << 2,
	ATTRIBFLAG_NORMAL2 = 1 << 3,
	ATTRIBFLAG_COLOR = 1 << 4,
	ATTRIBFLAG_TEXCOORD = 1 << 6,
	ATTRIBFLAG_TEXCOORD2 = 1 << 7,
    ATTRIBFLAG_TANGENT = 1 << 8,
    ATTRIBFLAG_BINORMAL = 1 << 9,
	ATTRIBFLAG_BONEINDEX = 1 << 10,
	ATTRIBFLAG_BONEWEIGHT = 1 << 11,
	NUM_ATTRIBUTEFLAGS
} AttributeFlags;

typedef enum
{
	MGLPT_ARGB_4444 = 0x00,
	MGLPT_ARGB_1555,
	MGLPT_RGB_565,
	MGLPT_RGB_555,
	MGLPT_RGB_888,
	MGLPT_ARGB_8888,
	MGLPT_ARGB_8332,
	MGLPT_I_8,
	MGLPT_AI_88,
	MGLPT_1_BPP,
	MGLPT_VY1UY0,
	MGLPT_Y1VY0U,
	MGLPT_PVRTC2,
	MGLPT_PVRTC4,
	MGLPT_PVRTC2_2,
	MGLPT_PVRTC2_4,
	
	OGL_RGBA_4444= 0x10,
	OGL_RGBA_5551,
	OGL_RGBA_8888,
	OGL_RGB_565,
	OGL_RGB_555,
	OGL_RGB_888,
	OGL_I_8,
	OGL_AI_88,
	OGL_PVRTC2,
	OGL_PVRTC4,
	
	// OGL_BGRA_8888 extension
	OGL_BGRA_8888,
	
	D3D_DXT1 = 0x20,
	D3D_DXT2,
	D3D_DXT3,
	D3D_DXT4,
	D3D_DXT5,
	
	D3D_RGB_332,
	D3D_AI_44,
	D3D_LVU_655,
	D3D_XLVU_8888,
	D3D_QWVU_8888,
	
	//10 bits per channel
	D3D_ABGR_2101010,
	D3D_ARGB_2101010,
	D3D_AWVU_2101010,
	
	//16 bits per channel
	D3D_GR_1616,
	D3D_VU_1616,
	D3D_ABGR_16161616,
	
	//HDR formats
	D3D_R16F,
	D3D_GR_1616F,
	D3D_ABGR_16161616F,
	
	//32 bits per channel
	D3D_R32F,
	D3D_GR_3232F,
	D3D_ABGR_32323232F,
	
	// Ericsson
	ETC_RGB_4BPP,
	ETC_RGBA_EXPLICIT,
	ETC_RGBA_INTERPOLATED,
	
	// DX10
	
	
	ePT_DX10_R32G32B32A32_FLOAT= 0x50,
	ePT_DX10_R32G32B32A32_UINT ,
	ePT_DX10_R32G32B32A32_SINT,
	
	ePT_DX10_R32G32B32_FLOAT,
	ePT_DX10_R32G32B32_UINT,
	ePT_DX10_R32G32B32_SINT,
	
	ePT_DX10_R16G16B16A16_FLOAT ,
	ePT_DX10_R16G16B16A16_UNORM,
	ePT_DX10_R16G16B16A16_UINT ,
	ePT_DX10_R16G16B16A16_SNORM ,
	ePT_DX10_R16G16B16A16_SINT ,
	
	ePT_DX10_R32G32_FLOAT ,
	ePT_DX10_R32G32_UINT ,
	ePT_DX10_R32G32_SINT ,
	
	ePT_DX10_R10G10B10A2_UNORM ,
	ePT_DX10_R10G10B10A2_UINT ,
	
	ePT_DX10_R11G11B10_FLOAT ,
	
	ePT_DX10_R8G8B8A8_UNORM ,
	ePT_DX10_R8G8B8A8_UNORM_SRGB ,
	ePT_DX10_R8G8B8A8_UINT ,
	ePT_DX10_R8G8B8A8_SNORM ,
	ePT_DX10_R8G8B8A8_SINT ,
	
	ePT_DX10_R16G16_FLOAT ,
	ePT_DX10_R16G16_UNORM ,
	ePT_DX10_R16G16_UINT ,
	ePT_DX10_R16G16_SNORM ,
	ePT_DX10_R16G16_SINT ,
	
	ePT_DX10_R32_FLOAT ,
	ePT_DX10_R32_UINT ,
	ePT_DX10_R32_SINT ,
	
	ePT_DX10_R8G8_UNORM ,
	ePT_DX10_R8G8_UINT ,
	ePT_DX10_R8G8_SNORM ,
	ePT_DX10_R8G8_SINT ,
	
	ePT_DX10_R16_FLOAT ,
	ePT_DX10_R16_UNORM ,
	ePT_DX10_R16_UINT ,
	ePT_DX10_R16_SNORM ,
	ePT_DX10_R16_SINT ,
	
	ePT_DX10_R8_UNORM,
	ePT_DX10_R8_UINT,
	ePT_DX10_R8_SNORM,
	ePT_DX10_R8_SINT,
	
	ePT_DX10_A8_UNORM,
	ePT_DX10_R1_UNORM,
	ePT_DX10_R9G9B9E5_SHAREDEXP,
	ePT_DX10_R8G8_B8G8_UNORM,
	ePT_DX10_G8R8_G8B8_UNORM,
	
	ePT_DX10_BC1_UNORM,
	ePT_DX10_BC1_UNORM_SRGB,
	
	ePT_DX10_BC2_UNORM,
	ePT_DX10_BC2_UNORM_SRGB,
	
	ePT_DX10_BC3_UNORM,
	ePT_DX10_BC3_UNORM_SRGB,
	
	ePT_DX10_BC4_UNORM,
	ePT_DX10_BC4_SNORM,
	
	ePT_DX10_BC5_UNORM,
	ePT_DX10_BC5_SNORM,
	
	//ePT_DX10_B5G6R5_UNORM,			// defined but obsolete - won't actually load in DX10
	//ePT_DX10_B5G5R5A1_UNORM,
	//ePT_DX10_B8G8R8A8_UNORM,
	//ePT_DX10_B8G8R8X8_UNORM,
	
	// OpenVG
	
	/* RGB{A,X} channel ordering */
	ePT_VG_sRGBX_8888  = 0x90,
	ePT_VG_sRGBA_8888,
	ePT_VG_sRGBA_8888_PRE,
	ePT_VG_sRGB_565,
	ePT_VG_sRGBA_5551,
	ePT_VG_sRGBA_4444,
	ePT_VG_sL_8,
	ePT_VG_lRGBX_8888,
	ePT_VG_lRGBA_8888,
	ePT_VG_lRGBA_8888_PRE,
	ePT_VG_lL_8,
	ePT_VG_A_8,
	ePT_VG_BW_1,
	
	/* {A,X}RGB channel ordering */
	ePT_VG_sXRGB_8888,
	ePT_VG_sARGB_8888,
	ePT_VG_sARGB_8888_PRE,
	ePT_VG_sARGB_1555,
	ePT_VG_sARGB_4444,
	ePT_VG_lXRGB_8888,
	ePT_VG_lARGB_8888,
	ePT_VG_lARGB_8888_PRE,
	
	/* BGR{A,X} channel ordering */
	ePT_VG_sBGRX_8888,
	ePT_VG_sBGRA_8888,
	ePT_VG_sBGRA_8888_PRE,
	ePT_VG_sBGR_565,
	ePT_VG_sBGRA_5551,
	ePT_VG_sBGRA_4444,
	ePT_VG_lBGRX_8888,
	ePT_VG_lBGRA_8888,
	ePT_VG_lBGRA_8888_PRE,
	
	/* {A,X}BGR channel ordering */
	ePT_VG_sXBGR_8888,
	ePT_VG_sABGR_8888 ,
	ePT_VG_sABGR_8888_PRE,
	ePT_VG_sABGR_1555,
	ePT_VG_sABGR_4444,
	ePT_VG_lXBGR_8888,
	ePT_VG_lABGR_8888,
	ePT_VG_lABGR_8888_PRE,
	
	// max cap for iterating
	END_OF_PIXEL_TYPES,
	
	MGLPT_NOTYPE = 0xff
	
} PixelType;

//Vertex Formats (V = Vertex, N = Normal, T = Texture Coordinates, C = Vertex Colors
typedef enum {
	VertexFormat_VNT,
    VertexFormat_VTNT,
    VertexFormat_VTBNT,
	VertexFormat_VVNNTT,
	VertexFormat_VNTC,
	VertexFormat_VNC,
	VertexFormat_VC,
	VertexFormat_VT,
	VertexFormat_VN,
	VertexFormat_V,
	VertexFormat_POD_Skinned,
} VertexFormat;

typedef struct
{
	u32 drawMethod;
	u8* vertexData;
	GLushort* indexData;
	u32 numVerts;
	u32 sizeOfVertexData;
	u32 sizeOfIndexData;
    GLuint vertexArrayObjectID; //Gets filled in at init
	GLuint indexBufferID; //Gets filled in at init
}PrimitiveData;

typedef struct
{
	Attributes attribute;
	u16 type;
	u8 size;
	u8 offset;
}AttributeData;

typedef struct
{
	u32 attribFlags;
	AttributeData* attributeArray;
	u32 numAttributes;
	u32 stride;
	const char* modelName;
	PrimitiveData* primitiveArray;
	u32 numPrimitives;
	s32 modelID;
}ModelData;

typedef enum {
    TextureFormatGray,
    TextureFormatGrayAlpha,
    TextureFormatRgb,
    TextureFormatRgba,
    TextureFormatPvrtcRgb2,
    TextureFormatPvrtcRgba2,
    TextureFormatPvrtcRgb4,
    TextureFormatPvrtcRgba4,
	TextureFormat565,
    TextureFormat5551,
} TextureFormat;

typedef enum{
	Uniform_Int,
	Uniform_Float,
	Uniform_Vec2,
	Uniform_Vec3,
	Uniform_Vec4,
	Uniform_Mat3x3,
    Uniform_Mat4x4,
} UniformType;

typedef enum{
	ProjMatType_Perspective,
	ProjMatType_Orthographic_NDC,
    ProjMatType_Orthographic_Points,
	NumProjMatTypes,
} ProjMatType;

typedef struct {
    TextureFormat Format;
    int BitsPerComponent;
    vec2 Size;
    int MipCount;
} TextureDescription;

typedef struct
{
	vec3 vertex1;
	vec3 vertex2;
	vec3 normal1;
	vec3 normal2;
	vec2 texCoord1;
	vec2 texCoord2;
} TexturedMorphVertexData3D;

typedef struct
{
    vec3 p0;
    vec3 p1;
} LineSegment;

//UniformValue
typedef struct
{
	u8* data;
	UniformType type;
} UniformValue;

//UniformBlob
typedef struct
{
	GLint uniform;
	UniformValue value;
	GLsizei size;
} UniformBlob;


//Material
typedef struct
{
	GLuint shaderProgram;
	GLint uniform_worldViewProjMat;
	GLint uniform_camPos;//TODO: remove
	UniformBlob uniforms_shared[MAX_SHARED_UNIFORM_VALUES];
    UniformBlob uniforms_shared_const[MAX_SHARED_CONST_UNIFORM_VALUES];
	GLint uniforms_unique[MAX_UNIQUE_UNIFORM_VALUES];
	GLsizei uniforms_unique_sizes[MAX_UNIQUE_UNIFORM_VALUES];
	UniformType uniforms_unique_types[MAX_UNIQUE_UNIFORM_VALUES];
	int numUniforms_shared;
    int numUniforms_shared_const;
	int numUniforms_unique;
	GLuint* texture0;
	GLuint* texture1;
	GLuint* texture2;
	VertexFormat requiredVertexFormat;	//TODO: use this
	const char* materialName;
} Material;

typedef enum{
	PPDrawArea_FullScreen,
	PPDrawArea_TopLeft,
	PPDrawArea_TopRight,
	PPDrawArea_BottomLeft,
	PPDrawArea_BottomRight,
}PostProcessDrawArea;

//FadeState
typedef enum
{
	FadeState_FadeOut,
    FadeState_WaitingForFadeIn,
	FadeState_FadeIn,
	FadeState_Idle,
}FadeState;


//RenderTarget	
typedef struct
{
	GLuint texture;
	GLuint width;
	GLuint height;
	GLuint FBO;
} RenderTarget;


typedef struct
{
    GLuint* texture;
    f32 fatnessBegin;
    f32 fatnessEnd;
    f32 segmentLength;
    f32 timeToLivePerParticle;
    f32 alphaFadeInTime;
    f32 alphaFadeOutTime;
    vec4 colorBegin;
    vec4 colorEnd;
    s32 sinCosBucket;
    f32 sinCosEffectScaleBegin;
    f32 sinCosEffectScaleEnd;
    f32 sinCosFreq;
    f32 scrollAmountU;
    f32 scrollAmountV;
    f32 texCoordMultU;
    f32 texCoordMultV;
}GFX_TrailSettings;

typedef struct
{
    vec3 position;
    f32 timeToLiveStart;
    f32 timeToLiveCurr;
} GFX_TrailParticle;

typedef struct
{
    GFX_TrailParticle trailParticles[MAX_TRAIL_PARTICLES_PER_TRAIL+1];
    GLuint texture;
    s32 m_numTrailParticles;
    f32 segmentLengthSq;
    vec3 currPos;
    f32 timeToLive;
    vec4 diffuseColor;
    GFX_TrailSettings trailSettings;
    u32 renderFlags;
    void** pCallbackTrailPointer;
    u8 inUse;
} GFX_Trail;

typedef struct
{
	vec3 position;	//12 bytes
    u32 pad2;
	vec2 texcoord; //8 bytes
    u32 pad0;
    u32 pad1;
    vec4 color;
}GFX_TrailParticleData;


//DoubleRenderTarget
typedef struct
{
	RenderTarget* renderTargetA;
	RenderTarget* renderTargetB;
}DoubleRenderTarget;

typedef enum
{
    ImageType_PVR,
    ImageType_PNG,
	ImageType_TGA,
} ImageType;


struct RenderableMaterial
{
	RenderMaterial materialID;  //4 bytes
	u8* uniqueUniformValues[MAX_UNIQUE_UNIFORM_VALUES];    //32 bytes
	GLuint* customTexture0;   //4 bytes
    GLuint* customTexture1;   //4 bytes
	u32 flags;  //4 bytes
};

typedef struct
{
	ModelData* pModelData;
	RenderableMaterial* pMaterial;
	mat4f worldMat;   //16 bytes
} SceneMesh;

class RenderableGeometry3D: public CoreObject
{
public:
	mat4f worldMat;   //16 bytes
	ModelData* pModel;  //4 bytes
	RenderableMaterial material;
	RenderLayer renderLayer;    //4 bytes
	u32 viewFlags;  //4 bytes
	const char* debugName;  //4 bytes
	u32 sortValue;  //4 bytes
    u32 postRenderLayerSortValue; //4 bytes (optional)

	virtual void Uninit()
	{
		CoreObject::Uninit();
		
		material.flags &= ~RenderFlag_Visible;
		material.flags |= RenderFlag_MarkedForRemoval;
		
		assert(GetHandle() == INVALID_COREOBJECT_HANDLE);
	}
};


struct RenderableScene3D
{
	SceneMesh* pSceneMesh;
	u32 numSceneMeshes;
	
	ModelData* pModel;
	u32 numModels;
	
	RenderableMaterial* pMaterial;
	u32 numMaterial;
	
	u32* pTexture;
	u32 numTexture;
	
	CPVRTModelPOD* pPod;
};

class RenderableSceneObject3D: public CoreObject
{
public:
	RenderableScene3D* pScene;
	mat4f worldMat;   //16 bytes
	RenderableMaterial material;	//optional
	RenderableMaterial* pMaterialOverride;
	mat4f* pParentMat;
	bool visible;
	bool markedForRemoval;
	
	virtual void Uninit()
	{
		visible = false;
		
		CoreObject::Uninit();
		markedForRemoval = true;
	}
};

struct AnimatedPOD
{
	void Update(f32 timeElapsed);
	f32 currFrame;
	f32 frameRate;
	RenderableScene3D* pScene;
	CPVRTModelPOD* pPod;
	f32 worldMat[16];   //16 bytes
};

typedef enum
{
	ParticleFlag_FlipU = 1 << 0,
	ParticleFlag_FlipV = 1 << 1,
	ParticleFlag_AlignToVel_Start = 1 << 2,
	ParticleFlag_AlignToVel_All = 1 << 3,
	ParticleFlag_FlipScalePercent = 1 << 4,
	ParticleFlag_FlipRotation = 1 << 5,
	ParticleFlag_FlipVelOnY = 1 << 6,
	ParticleFlag_CancelVelocity = 1 << 7,
	ParticleFlag_OverridePosition = 1 << 8,
	ParticleFlag_MultWidthByAlpha = 1 << 9,
	ParticleFlag_DivWidthByAlpha = 1 << 10,
}ParticleFlags;

typedef struct
{
    vec3 position; //12 bytes
	//12
    f32 timeToLive;	//4 bytes
	f32 size_min; //4 bytes
	f32 size_max; //4 bytes
	s32 sinCosBucket; //4 bytes
	f32 gravity;	//4 bytes
    //20
	vec3 velocity;	//12 bytes
	//12
    f32 alphaInTime;	//4 bytes
	f32 alphaOutTime;	//4 bytes
	f32 totalTTL;	//4 bytes
	vec2 texcoordOffset; //8 bytes
	//20
    vec3 color;	//12 bytes
	//12
    f32 overallAlpha; //4 bytes
	f32 alignmentSin;//4 bytes
	f32 alignmentCos;//4 bytes
	u32 particleFlags; //4 bytes
	f32 flipRotMult; //4 bytes
	//20
    vec3 positionOffset; //12 bytes
	//12
    vec3* pPositionOverride; //4 bytes
	f32 timeForCallbackCounter; //4 bytes
	u32* pCallbackCounter;	//4 bytes
    //12
    vec3* pCallbackPos;
    //8
    //Grand total: 128
} Particle3D;

typedef struct
{
	vec3 position;	//12 bytes
    u32 pad2;
	vec2 texcoord; //8 bytes
    u32 pad0;
    u32 pad1;
	vec4 color; //16 bytes  
}ParticleData;

typedef struct
{
	vec3 position;	//12 bytes
    u32 pad2;   //4 bytes
	vec4 color; //16 bytes  
}PointData;	//Grand total: 32 bytes

typedef enum
{
	ParticleBucket_ColorShine,
    ParticleBucket_Additive,
	ParticleBucket_Normal,
	NumParticleBuckets,
} ParticleBuckets;

typedef struct
{
	s32 atlasCell_X;
	s32 atlasCell_Y;
	s32 atlas_numCells;
	f32 alphaInTime_Min;
	f32 alphaInTime_Max;
	f32 alphaOutTime_Min;
	f32 alphaOutTime_Max;
	f32 overallAlpha;
	f32 velocityAngle_Min;
	f32 velocityAngle_Max;
	f32 gravity_Min;
	f32 gravity_Max;
	f32 timeToLive_Min;
	f32 timeToLive_Max;
	f32 speed_Min;
	f32 speed_Max;
	f32 size_Min;
	f32 size_Max;
	f32 scalePercent;
	f32 positionOffsetAmount;
	s32 sinCosBucket_Min;
	s32 sinCosBucket_Max;
	u32 particleFlags;
	ParticleBuckets particleBucket;
}ParticleSettings;

struct MaterialSettings
{
	GLuint			textureFilterMode;
	GLuint			wrapModeU;
	GLuint			wrapModeV;
    RenderMaterial	renderMaterial;
    u32				renderFlags;
	bool			flipY;
};

struct ItemArtDescription
{
    const char*		textureFileName;
    ImageType		imageType;
    GLuint			textureHandle;
	const MaterialSettings*	materialSettings;
    ModelData*		pModelData;
};

#endif
