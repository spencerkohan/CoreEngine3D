//
//  OpenGLRenderer.h
//  InfiniSurv
//
//  Created by Jody McAdams on 11/21/11.
//  Copyright (c) 2011 Jody McAdams. All rights reserved.
//

#ifndef InfiniSurv_OpenGLRenderer_h
#define InfiniSurv_OpenGLRenderer_h

//** INCLUDES **/
#ifdef PLATFORM_OSX
#include <OpenGL/gl.h>
#endif

#ifdef PLATFORM_IOS
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>
#endif

#include "MathUtil.h"
#include "GraphicsTypes.h"
#include "PhysicsTypes.h"
#include "MaterialDeclarations.h"
#include "ArrayUtil.h"

#include "PVRTModelPOD.h"

#define GAUSSIAN_NUMSAMPLES 6
#define MAX_TRAILS 64
#define MAX_PARTICLES 512
#define VERTS_PER_PARTICLE 6
#define VERTS_PER_TRAILPARTICLE 2
#define PARTICLE_TEXTURE_SIZE 256.0f
#define PARTICLES_PER_ROW 8.0f
#define PARTICLE_CELL_SIZE (PARTICLE_TEXTURE_SIZE/PARTICLES_PER_ROW)
#define PARTICLE_TEXCOORD_OFFSET (PARTICLE_CELL_SIZE/PARTICLE_TEXTURE_SIZE)
#define HALF_PARTICLE_TEXCOORD_OFFSET (PARTICLE_TEXCOORD_OFFSET*0.5f)
#define DEBUG_LINES_MAXVERTICES 30
#define NUM_SINCOS_BUCKETS 10
#define NUM_SINCOS_BUCKET_MAX_MULT 0.3f

#define DEBUGDRAW_MAXLINESEGMENTS 512
#define DEBUGDRAW_MAXDEBUGOBJECTS 32

#define MAX_RENDERABLE_3D_OBJECTS 2300
#define MAX_RENDERABLE_UI_OBJECTS 64

#define GAUSSIAN_STANDARDDEVIATION 0.9f

#define MAX_COMPILED_VERTEX_SHADERS 32
#define MAX_COMPILED_PIXEL_SHADERS 32

#define MAX_TEXTURED_LINES 16

/**STRUCTS**/

typedef struct
{
	f32 sinTheta;
	f32 cosTheta;
	f32 multiplier;
} SinCosBucket;


typedef struct
{
	Particle3D m_particleQueue[MAX_PARTICLES];
	ParticleData m_spriteData[MAX_PARTICLES*VERTS_PER_PARTICLE];
	s32 m_numParticles;
    s32 m_numParticlesToDraw;
	bool m_particlesNeedSorting;
} RendererParticleBucket;


typedef struct
{
    DebugDrawObjectType objectType;
    mat4f matrix;
    vec4 color;
}DebugDrawObject;


typedef struct
{
	GLuint texturedID;
	LineSegment line;
	vec4 diffuseColor;
	f32 lineWidth0;
	f32 lineWidth1;
	f32 numTextureRepeats;
}TexturedLineObject;


typedef struct
{
	vec3 position;
	vec2 texcoord;
}TexturedLineVert;

typedef struct
{
	const char* filename;
	GLuint openGLID;
}ShaderCreationStatus;

class OpenGLRenderer
{
public:
	
	//public functions
	
	void Init(f32 screenWidth, f32 screenHeight);
	bool InitSceneFromPOD(RenderableScene3D* pScene, const char* fileName,  mat4f matrix4x4, u32 viewFlags);
	void CleanUp();
	bool GetFadeFinished();
	void ClearRenderables();
	void ClearOneFrameGeometry();
	void RenderLoop(u32 camViewIDX,RenderableGeometry3D** renderableObjectArray, u32 numRenderableObjects);
	void RenderEffects();
	void SetClearColor(f32 r, f32 g, f32 b);
	void SetGravityDir(const vec3* pNewGravityDir);
	void SetSortRenderablesByZ(bool sortRenderablesByZ);
	void Render(f32 timeElapsed);
	bool LoadTextureFromData(GLuint* out_textureName,const void* data,u32 texWidth, u32 texHeight, u32 format, u32 type, GLuint filterMode, GLuint wrapModeU, GLuint wrapModeV);
	bool UpdateTextureFromData(GLuint* out_textureName, const void* data, u32 texWidth, u32 texHeight, u32 format, u32 type);
	void RegisterModel(ModelData* pModelData);
	void AddRenderableObject3DToList(RenderableObject3D* pRenderable);
	void AddRenderableScene3DToList(RenderableScene3D* pRenderableScene);
	void AddParticleToQueue(Particle3D* pParticle, vec3* pPosition, vec3* pCallbackPos, ParticleBuckets particleBucket);
	void SpawnParticles(vec3* pPosition, const vec3* pColor, const ParticleSettings* particleSettings, s32 numParticles);
	void UpdateParticleQueues(f32 timeElapsed);
	void UpdateTrails(f32 timeElapsed);
	void InitRenderableObject3D(RenderableObject3D* renderableObject, ModelData* pModel, RenderMaterial materialID, GLuint* customTexture, mat4f matrix4x4, RenderLayer renderLayer, u32 viewFlags, u32 renderFlags);
	void SortRenderableGeometry3DList();
	void CreateMaterials();
	void SetViewMatrixForIndex(f32* pCameraMatrix, u32 viewIndex);
	void SetViewPos(vec3* pCamPos, u32 viewIndex);
	f32* GetCameraMatrix(u32 viewIndex);
	void StartFadeOut(f32 timeInSeconds);
	void ForceFadeIn(f32 timeInSeconds);
	void StartFadeIn();
	void StartPauseFadeIn();
	void StartPauseFadeOut(f32 finalFade, f32 timeInSeconds);
	void ResetPauseFade();
	void ClearParticles();
	void SetScreenFadeColor(vec3* screenFadeColor);
	void ForceRenderablesNeedSorting();
	void SortRenderablesWithMaterialByZ(RenderMaterial materialID);
	void SortRenderablesInLayerRangeByZ(RenderLayer layerBegin, RenderLayer layerEnd);
	void FlashScreen(const vec3* pColor, f32 timeInSeconds);
	void ShakeScreen(f32 shakeAmount,f32 shakeSpeed, f32 shakeTime);
	void LoadTexture(const char* fileName,ImageType imageType, GLuint* pGLTexture, GLuint filterMode, GLuint wrapModeU, GLuint wrapModeV, bool flipY = false);
	GFX_Trail* CreateTrail(GFX_Trail** pCallbackTrail, vec3* pInitialPos, f32 timeToLive, const GFX_TrailSettings* pTrailSettings, u32 renderFlags);
	void DRAW_DrawTexturedLine(const vec3* p0, const vec3* p1, const vec4* pDiffuseColor, GLuint texturedID, f32 lineWidth0, f32 lineWidth1, f32 numTextureRepeats);
	void DEBUGDRAW2D_DrawLineSegment(const vec3* p0, const vec3* p1, const vec4* color);
	void DEBUGDRAW2D_DrawCircle(const vec3* p0, f32 radius, const vec4* color);
	void DEBUGDRAW3D_DrawLineSegment(const vec3* p0, const vec3* p1, const vec4* color);
	void DEBUGDRAW3D_DrawLineSegment_2Color(const vec3* p0, const vec3* p1, const vec4* color1, const vec4* color2);
	void DEBUGDRAW3D_DrawCircle(mat4f matrix4x4, const vec4* color);
	void DEBUGDRAW3D_DrawCylinder(mat4f matrix4x4, const vec4* color);
	
	//public member variables
	f32 screenWidth_points;
	f32 screenHeight_points;
	f32 aspectRatio;
	bool paused;
	
private:
	
	//private functions
	CPVRTModelPOD* LoadPOD(const char* fileName);
	void SetMaterial(RenderMaterial material);
	void PostProcess(RenderMaterial ppMaterial, RenderTarget* renderTarget, PostProcessDrawArea drawArea, GLuint* texture0, GLuint* texture1, GLuint* texture2);
	void PrintOpenGLError(const char* callerName);
	bool CreateRenderTarget(RenderTarget* renderTarget, GLuint FBO, GLuint width, GLuint height);
	bool CreateShaderProgram(s32 vertexShaderIndex, s32 pixelShaderIndex, AttributeFlags attribs, GLuint* out_resultProgram);
	void SetRenderTarget(RenderTarget* renderTarget);
	void UploadWorldViewProjMatrix(const f32* pWorldMat); //only call after setMaterial
	void UploadProjMatrix(const f32* pProjMat); //only call after setMaterial
	void UploadWorldProjMatrix(const f32* pWorldMat); //only call after setMaterial
	void UploadSharedUniforms();
	void UploadUniqueUniforms(f32* const * pFloatArray);
	void SetTexture(const GLuint* pTexture,GLuint textureUnit);
	
	f32 ComputeGaussianValue(f32 x, f32 stdDevSq);
	void ComputeGaussianWeights(f32* out_pWeights, s32 numWeights, f32 standardDeviationSquared);
	void EnableAttributes(const ModelData* pModelData);
	void BindVertexArrayObject(const PrimitiveData* pPrimitive);
	void BindIndexData(const PrimitiveData* pPrimitive);
	void AddUniform_Unique(RenderMaterial renderMaterial, const char* nameOfUniformInShader,UniformType uniformType, u32 amount);
	void AddUniform_Shared(RenderMaterial renderMaterial, const char* nameOfUniformInShader, UniformType uniformType, f32* pData, u32 amount);
	void AddUniform_Shared_Const(RenderMaterial renderMaterial, const char* nameOfUniformInShader, UniformType uniformType, f32* pData, u32 amount);
	void SortParticleQueues();
	bool CompileShader(GLuint *shader, GLenum type, GLsizei count, const char* filename);
	bool LinkProgram(GLuint prog);
	GLint ValidateProgram(GLuint prog);
	const char* GetPathToFile(const char* filename);
	
	bool CreatePixelShader(s32 pixelShaderIndex);
	bool CreateVertexShader(s32 vertexShaderIndex);
	s32 AddVertexShaderToList(const char* filename);
	s32 AddPixelShaderToList(const char* filename);
	
	//private member variables
	ShaderCreationStatus m_vertexShaders[MAX_COMPILED_VERTEX_SHADERS];
	ShaderCreationStatus m_pixelShaders[MAX_COMPILED_PIXEL_SHADERS];
	s32 m_numVertexShaders;
	s32 m_numPixelShaders;
	GLint backingWidth;
	GLint backingHeight;
	RenderTarget m_screenTarget;
	vec3 m_camPos[NumCameraViews];
	// The OpenGL names for the framebuffer and renderbuffer used to render to this view
	GLuint viewFrameBuffer, colorRenderbuffer, depthRenderbuffer, msaaFramebuffer,msaaRenderBuffer,msaaDepthBuffer;
	GLuint m_postProcessDefaultVertShader;
	f32 m_gaussianWeights[GAUSSIAN_NUMSAMPLES];
	f32 m_accumulatedTime;
	f32* m_view[NumCameraViews];
	mat4f m_projMats[NumProjMatTypes];
	mat4f m_viewProjMats[NumCameraViews][NumProjMatTypes];
	ProjMatType m_currProjMatType;
	vec2 m_gaussianViewportSize;
	bool firstRun;
	bool m_readyToGo;
	f32 m_totalFadeTime;
    f32 m_totalFlashTime;
    f32 m_currFlashTime;
	f32 m_currFadeTime;
	f32 m_currFadeValue;
    f32 m_finalFade;
	FadeState m_fadeState;
    FadeState m_flashState;
    bool m_requestedFadeIn;
	RenderMaterial m_lastUsedMaterial;
	vec4 m_fadeColor;
	vec3 m_fadeToScreenColor;
	u32 m_currViewIndex;
	u32 m_numRenderableGeom3Ds;
	u32 m_numRenderableUIObjects;
	bool m_renderableObject3DsNeedSorting;
	bool m_renderableUINeedSorting;
	u32 m_glBufferCount;
	u32 m_glTotalBufferSize;
	u32 m_glTotalTextureSize;
	u32 m_accumulatedFrames;
	s32 m_numModels;
    u32 m_maxNumRenderables;
    u32 m_maxNumTrails;
    u32 m_maxNumParticles;
    u32 m_numCellshadedRenderables;
	GLuint m_currTexture;
    vec3 m_flashColor;
    GLuint m_currTextureInTextureUnit[5];
    GLuint m_currTextureUnit;
    SinCosBucket m_sinCosBuckets[NUM_SINCOS_BUCKETS];
    RenderableGeometry3D* m_renderableGeometry3DList[MAX_RENDERABLE_3D_OBJECTS];
	RenderableGeometry3D* m_renderableUIList[MAX_RENDERABLE_UI_OBJECTS];
    RendererParticleBucket m_particleBuckets[NumParticleBuckets];
    GLuint trailShaderUniform_accumulatedTime;
    GLuint trailShaderUniform_scrollAmountU;
    GLuint trailShaderUniform_scrollAmountV;
    GFX_Trail m_trails[MAX_TRAILS];
    PointData m_debugLinePoints3D[DEBUGDRAW_MAXLINESEGMENTS*2];
    PointData m_debugLinePoints2D[DEBUGDRAW_MAXLINESEGMENTS*2];
    s32 m_numDebugLinePoints2D;
    s32 m_numDebugLinePoints3D;
	s32 m_numDebugLinePoints2D_saved;
    s32 m_numDebugLinePoints3D_saved;
    TexturedLineObject m_texturedLineObjects[MAX_TEXTURED_LINES];
	s32 m_numTexturedLines;
	s32 m_numTexturedLines_saved;
	TexturedLineVert m_texturedLineVerts[4];
    GFX_Trail* m_renderableTrails[MAX_TRAILS];
    u32 m_numRenderableTrails;
    GFX_TrailParticleData m_trailParticleRenderablePoints[MAX_TRAIL_PARTICLES_PER_TRAIL*VERTS_PER_TRAILPARTICLE];
    u32 m_renderStateFlags;
    f32 m_screenShakerT_X;
    f32 m_screenShakerT_Y;
    f32 m_screenShakerSpeed_X;
    f32 m_screenShakerSpeed_Y;
    f32 m_currScreenShakeAmount;
    f32 m_screenShakeTimer;
    f32 m_screenShakeStartTime;
    f32 m_screenShakeSpeed;
    vec3 m_gravityDir;
    bool m_supportsFeaturesFromiOS4;
    bool m_supportsMultisampling;
    FadeState m_pauseFadeState;
	f32 m_pauseTotalFadeTime;
    f32 m_pauseFadeTimeAtStart;
	f32 m_pauseCurrFadeTime;
    f32 m_pauseFinalFade;
    bool m_pauseRequestedFadeIn;
    bool m_sortRenderablesByZ;
    vec3 m_clearColor;
    DebugDrawObject m_debugDrawObjects[DEBUGDRAW_MAXDEBUGOBJECTS];
    u32 m_numDebugDrawObjects;
	u32 m_numDebugDrawObjects_saved;
	char* m_pArtPath;
	f32 m_identityMat[16];
};

//Helpers
char* FileToCharArray(const char* filename);
bool LoadPngImage(const char *name, int &outWidth, int &outHeight, bool &outHasAlpha, GLubyte **outData, bool flipY);
s32 RenderableGeometry3DCompare_SortByZ(const void* lhs, const void* rhs);
s32 RenderableGeometry3DCompare_SortByNegativeZ(const void* lhs, const void* rhs);
s32 RenderableGeometry3DCompare_SortValue(const void* lhs, const void* rhs);
void PackFloat16(float myFloat, float* out_x, float* out_y);
float UnpackFloat16(float x, float y);
void DoubleRenderTarget_Flip(DoubleRenderTarget* doubleRenderTarget);
void Draw_FSQuad();
void Draw_TLQuad();
void Draw_TRQuad();
void Draw_BLQuad();
void Draw_BRQuad();
void Draw_Matrix();

#endif
