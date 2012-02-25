//
//  Box2DDebugDraw.cpp
//  CoreEngine3D(iOS)
//
//  Created by Jody McAdams on 2/19/12.
//  Copyright (c) 2012 Jody McAdams. All rights reserved.
//

#include "Box2DDebugDraw.h"
#include "OpenGLRenderer.h"
#include "Game.h"

/// Draw a closed polygon provided in CCW order.
void Box2DDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	const vec4 colorF = {color.r,color.g,color.b,1.0f};
	const f32 pixelsPerMeter = GAME->GetPixelsPerMeter();
	
	for(u32 i=1; i<vertexCount; ++i)
	{
		const b2Vec2* pVertA = &vertices[i-1];
		const b2Vec2* pVertB = &vertices[i];
		
		const vec3 vecA = {pVertA->x*pixelsPerMeter,pVertA->y*pixelsPerMeter,0.0f};
		const vec3 vecB = {pVertB->x*pixelsPerMeter,pVertB->y*pixelsPerMeter,0.0f};
		
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &vecA, &vecB, &colorF);
	}
	
	if(vertexCount > 2)
	{
		const b2Vec2* pVertA = &vertices[0];
		const b2Vec2* pVertB = &vertices[vertexCount-1];
		
		const vec3 vecA = {pVertA->x*pixelsPerMeter,pVertA->y*pixelsPerMeter,0.0f};
		const vec3 vecB = {pVertB->x*pixelsPerMeter,pVertB->y*pixelsPerMeter,0.0f};
		
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &vecA, &vecB, &colorF);
	}
}


/// Draw a solid closed polygon provided in CCW order.
void Box2DDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	//TODO: make solid
	
	const vec4 colorF = {color.r,color.g,color.b,1.0f};
	const f32 pixelsPerMeter = GAME->GetPixelsPerMeter();
	
	for(u32 i=1; i<vertexCount; ++i)
	{
		const b2Vec2* pVertA = &vertices[i-1];
		const b2Vec2* pVertB = &vertices[i];
		
		const vec3 vecA = {pVertA->x*pixelsPerMeter,pVertA->y*pixelsPerMeter,0.0f};
		const vec3 vecB = {pVertB->x*pixelsPerMeter,pVertB->y*pixelsPerMeter,0.0f};
		
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &vecA, &vecB, &colorF);
	}
	
	if(vertexCount > 2)
	{
		const b2Vec2* pVertA = &vertices[0];
		const b2Vec2* pVertB = &vertices[vertexCount-1];
		
		const vec3 vecA = {pVertA->x*pixelsPerMeter,pVertA->y*pixelsPerMeter,0.0f};
		const vec3 vecB = {pVertB->x*pixelsPerMeter,pVertB->y*pixelsPerMeter,0.0f};
		
		GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &vecA, &vecB, &colorF);
	}
}


/// Draw a circle.
void Box2DDebugDraw::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const f32 pixelsPerMeter = GAME->GetPixelsPerMeter();
	
	const vec4 colorF = {color.r,color.g,color.b,1.0f};
	
	const vec3 circleCenter = {center.x*pixelsPerMeter,center.y*pixelsPerMeter,0.0f};
	
	GLRENDERER->DEBUGDRAW_DrawCircleXY(DebugDrawMode_2D, &circleCenter, radius*pixelsPerMeter, &colorF);
}


/// Draw a solid circle.
void Box2DDebugDraw::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	//TODO: make solid
	const f32 pixelsPerMeter = GAME->GetPixelsPerMeter();
	
	const vec4 colorF = {color.r,color.g,color.b,1.0f};
	
	const vec3 circleCenter = {center.x*pixelsPerMeter,center.y*pixelsPerMeter,0.0f};
	
	GLRENDERER->DEBUGDRAW_DrawCircleXY(DebugDrawMode_2D, &circleCenter, radius*pixelsPerMeter, &colorF);
}


/// Draw a line segment.
void Box2DDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	const f32 pixelsPerMeter = GAME->GetPixelsPerMeter();
	
	//const vec4 colorF = {color.r,color.g,color.b,1.0f};
	const vec4 colorF = {1.0f,1.0f,1.0f,1.0f};

	const vec3 vecA = {p1.x*pixelsPerMeter,p1.y*pixelsPerMeter,0.0f};
	const vec3 vecB = {p2.x*pixelsPerMeter,p2.y*pixelsPerMeter,0.0f};
	
	GLRENDERER->DEBUGDRAW_DrawLineSegment(DebugDrawMode_2D, &vecA, &vecB, &colorF);
}


/// Draw a transform. Choose your own length scale.
/// @param xf a transform.
void Box2DDebugDraw::DrawTransform(const b2Transform& xf)
{
	//const vec4 color = {color.r,color.g,color.b,1.0f};
	
	//TODO: implement
	
}
