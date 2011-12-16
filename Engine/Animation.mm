//
//  untitled.m
//  CoreEngine3D
//
//  Created by sam on 10/26/10.
//  Copyright 2010 GameJelly Online. All rights reserved.
//

//DISCLAIMER:  THIS HORRIBLE FILE IS NOT JODY'S FAULT

#import "Animation.h"
#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "GraphicsTypes.h"
#import "ES2Renderer.h"
#import "matrix.h"


#include <fstream>
#include <iostream>
#include "ArrayUtil.h"

@implementation AnimationSystem2D

@synthesize animEventDelegate;

//returns a headerArrayIndex
-(s32) AddAttachPointsToAnimationPlayer:(s32) animationInfoIndex:(const char*)attachPointFile
{
	if(attachPointFile == NULL)
	{
		return -1;
	}
	
	AnimationInfo* pAnimInfo = &m_animationInfos[animationInfoIndex];
	AnimationAttachPointArray* pAttachPointArray = &pAnimInfo->m_pAttachPointHeaderArrays[pAnimInfo->m_numAttachPointHeaderArrays];
	
	pAttachPointArray->pAttachPointLocationHeaders = NULL;
	pAttachPointArray->numAttachPointLocationHeaders = 0;
	
	std::ifstream inputFileStream;
    
    //Load level from the app bundle
    NSString* nsFileName = [NSString stringWithCString:attachPointFile encoding:NSUTF8StringEncoding];
	NSString *path = [[NSBundle mainBundle] pathForResource:nsFileName ofType:@""];
	
	AttachPointFileReadState fileReadState = AttachPointFileReadState_ReadNumNames;

    inputFileStream.open([path UTF8String]);

    if(!inputFileStream.is_open())
    {
        std::cout << "ERROR: AddAttachPointsToAnimationPlayer -> File not found! " << attachPointFile << '\n';
    }
    else
    {
        //SUCCESS!  Let's read the file.
        
        std::cout << "AddAttachPointsToAnimationPlayer: Reading file: " << attachPointFile << '\n';
        
        std::string currLine;

		int attachPointHeaderIndex = 0;
		int currAttachPointIndex = 0;
		int numNames = 0;
		
        while(!inputFileStream.eof())
		{
			std::getline(inputFileStream,currLine);
			
			if(currLine.size() == 0)
			{
				break;
			}
			
			//std::cout << currLine << '\n';
			
			switch(fileReadState)
			{
				case AttachPointFileReadState_ReadNumNames:
				{
					numNames = atoi(currLine.c_str());
					
					pAttachPointArray->numAttachPointLocationHeaders = numNames;
					pAttachPointArray->pAttachPointLocationHeaders = new AnimationAttachPointLocations[numNames];
					
					fileReadState = AttachPointFileReadState_ReadName;
					
					break;
				}
				case AttachPointFileReadState_ReadName:
				{
					int commaLoc = 0;
					for(unsigned int i=0; i<currLine.size();++i)
					{
						if(currLine[i] == ',')
						{
							commaLoc = i;
							break;
						}
					}
					
					std::string numPointsStr(currLine.substr(0,commaLoc));
					
					int numPoints = atoi(numPointsStr.c_str());
					
					std::string name(currLine.substr(commaLoc+1,currLine.size()-commaLoc));
					
					//std::cout << "attach name: " << name << '\n';
					//std::cout << "num points: " << numPoints << '\n';
					
					//Get the struct to write to
					AnimationAttachPointLocations* pLocations = &pAttachPointArray->pAttachPointLocationHeaders[attachPointHeaderIndex];
					
					//Copy the name in
					strcpy(pLocations->attachPointName,name.c_str());
					
					//Set how many points there are in it
					pLocations->numAttachPointLocations = numPoints;
					
					//Allocate points
					pLocations->pAttachPointLocations = new vec2[numPoints];
					
					//Now read in the points
					currAttachPointIndex = 0;
					fileReadState = AttachPointFileReadState_ReadPoints;
					
					break;
				}
				case AttachPointFileReadState_ReadPoints:
				{
					//Get the struct to write to
					AnimationAttachPointLocations* pLocations = &pAttachPointArray->pAttachPointLocationHeaders[attachPointHeaderIndex];
					
					int commaLoc = 0;
					for(unsigned int i=0; i<currLine.size();++i)
					{
						if(currLine[i] == ',')
						{
							commaLoc = i;
							break;
						}
					}
					
					std::string x_str(currLine.substr(0,commaLoc));
					std::string y_str(currLine.substr(commaLoc+1,currLine.size()-commaLoc));
					
					vec2* pCurrVec = &pLocations->pAttachPointLocations[currAttachPointIndex];
					pCurrVec->x = atof(x_str.c_str());
					pCurrVec->y = atof(y_str.c_str());
					
					++currAttachPointIndex;
					
					if(currAttachPointIndex == pLocations->numAttachPointLocations)
					{
						++attachPointHeaderIndex;
						
						if(attachPointHeaderIndex == numNames)
						{
							fileReadState = AttachPointFileReadState_None;
						}
						else
						{
							fileReadState = AttachPointFileReadState_ReadName;
						}
					}
					
					break;
				}
				case AttachPointFileReadState_None:
				{
					break;
				}
			}
		}
		
		inputFileStream.close();
	}
	
	
	const s32 returnIndex = pAnimInfo->m_numAttachPointHeaderArrays;
	++pAnimInfo->m_numAttachPointHeaderArrays;
	
	
	//DEBUG
	
	AnimationAttachPointArray* pCurrArray = &pAnimInfo->m_pAttachPointHeaderArrays[returnIndex];
	//std::cout << "\nAttach Point Array Index " << returnIndex << '\n';
	
	for(int headerIDX=0; headerIDX<pCurrArray->numAttachPointLocationHeaders; ++headerIDX)
	{
		AnimationAttachPointLocations* pAttachLocationsHeader = &pCurrArray->pAttachPointLocationHeaders[headerIDX];
		//std::cout << "Attach Point Name: " << pAttachLocationsHeader->attachPointName << '\n';
		
		for(int pointsIDX=0; pointsIDX<pAttachLocationsHeader->numAttachPointLocations; ++pointsIDX)
		{
			//vec2* pVec = &pAttachLocationsHeader->pAttachPointLocations[pointsIDX];
			//std::cout << "Frame " << pointsIDX << ": <" << pVec->x << ',' << pVec->y << ">\n";
		}
	}
	
	//std::cout << '\n';
	
	return returnIndex;
}


//Gets the index of the attach point name in the specified headerArray
-(s32) GetAnimationAttachPointIndexByName:(s32)animationInfoIndex:(const char*)attachPointName
{
	AnimationInfo* pAnimInfo = &m_animationInfos[animationInfoIndex];
	
	if(pAnimInfo->currentClip == NULL || pAnimInfo->currentClip->attachHeaderIndex == -1)
	{
		return -1;
	}
	
	AnimationAttachPointArray* pAttachPointArray = &pAnimInfo->m_pAttachPointHeaderArrays[pAnimInfo->currentClip->attachHeaderIndex];
	
	for(int i=0; i<pAttachPointArray->numAttachPointLocationHeaders; ++i)
	{
		if(strcmp(attachPointName, pAttachPointArray->pAttachPointLocationHeaders[i].attachPointName) == 0)
		{
			return i;
		}
	}
	
	return -1;
}


-(BOOL) GetAnimationAttachPointByName:(s32) animationInfoIndex:(const char*)attachPointName:(vec2*)pOut_vec
{
	const s32 attachPointIndex = [self GetAnimationAttachPointIndexByName:animationInfoIndex:attachPointName];
	if(attachPointIndex == -1)
	{
		return NO;
	}
	
	AnimationInfo* pAnimInfo = &m_animationInfos[animationInfoIndex];
	
	if(pAnimInfo->currentClip == NULL || pAnimInfo->currentClip->attachHeaderIndex == -1)
	{
		return NO;
	}
	
	AnimationAttachPointArray* pAttachPointArray = &pAnimInfo->m_pAttachPointHeaderArrays[pAnimInfo->currentClip->attachHeaderIndex];
	AnimationAttachPointLocations* pLocations = &pAttachPointArray->pAttachPointLocationHeaders[attachPointIndex];
	
	const s32 currFrame = pAnimInfo->currentFrame;
	if(currFrame < pLocations->numAttachPointLocations)
	{
		vec2* pVec = &pLocations->pAttachPointLocations[currFrame];
		CopyVec2(pOut_vec, pVec);
		//printf("Attach point location for %s is <%f,%f>\n",pLocations->attachPointName,pVec->x,pVec->y);
		
		return YES;
	}
	
	return NO;
}


-(void) ClearAllAnimationPlayers
{
	m_lastAnimEventFrame = -1;
	
	for (s32 x=0; x<m_highestAnimationIndex+1; ++x) 
	{
		[m_animationInfos[x].clips release];
		
		for(id key in m_animationInfos[x].clipDictionary) {
			NSValue* value = [m_animationInfos[x].clipDictionary objectForKey:key];
			free(value.pointerValue);
		}
		
		[m_animationInfos[x].clipDictionary release];
	}
		
	[self Init:renderer];
}


-(s32) CreateAnimationPlayer
{
	s32 newid = -1;
	for (int x = 0; x<MAX_ANIMATION_PLAYERS; ++x) 
	{
		if (m_animPlayers[x] == -1)
		{
			m_animPlayers[x] = x;
			newid = x;

			if (x > m_highestAnimationIndex)
			{
				m_highestAnimationIndex = x;
			}
			
			break;
		}
	}
	
	if (newid == -1)
	{
		NSLog(@"we were unable to find an AnimationPlayer for you.  Bye");
		return -1;
	}
	
	m_animationInfos[newid].drawthisframe = 1;
	
	m_animationInfos[newid].clips = [[NSMutableArray alloc] init];
	m_animationInfos[newid].clipDictionary = [[NSMutableDictionary alloc] init];
	m_animationInfos[newid].m_numAttachPointHeaderArrays = 0;

	return newid;
}


-(void) LinkAnimationPlayer:(RenderableObject3D*)_r3d: (s32)animationID
{
	
	m_animationInfos[animationID].r3d = _r3d;
	_r3d->uniqueUniformValues[0] = (f32*)&m_animationInfos[animationID].wwlo3;
}


-(void) AddClip:(AnimationClip *)_clip ToAnimation:(s32) _animation
{
	s32 startFrame = _clip->startFrame;
	s32 endFrame = _clip->endFrame;
	
	const s32 frameDiff = endFrame-startFrame;
	
	switch(_clip->playMode)
	{
		case AnimPlayMode_Forward:
		{
			if(frameDiff < 0)
			{
				NSLog(@"INSANE ERROR: AddClip->Your clip is set to forward but the frames are backward.  Clip not added...");
				return;
			}
			
			break;
		}
		case AnimPlayMode_Backward:
		{
			s32 endFrameTemp = endFrame;
			endFrame = startFrame;
			startFrame = endFrameTemp;
			
			if(frameDiff > 0)
			{
				NSLog(@"INSANE ERROR: AddClip->Your clip is set to backward but the frames are forward.  Clip not added...");
				return;
			}
			
			break;
		}
		case AnimPlayMode_PingPong:
		{
			if(frameDiff < 0)
			{
				s32 endFrameTemp = endFrame;
				endFrame = startFrame;
				startFrame = endFrameTemp;
			}
			
			break;
		}
		default:
		{
			break;
		}
	}

	AnimationClip* newclip = (AnimationClip*)calloc(sizeof(AnimationClip), sizeof(AnimationClip));
	
	newclip->attachHeaderIndex = _clip->attachHeaderIndex;
	newclip->animationName = _clip->animationName;
    newclip->nextAnimName = _clip->nextAnimName;
	newclip->startFrame = startFrame;
	newclip->endFrame = endFrame;
	newclip->framesPerRow = _clip->framesPerRow;
	newclip->sheetHeight = _clip->sheetHeight;
	newclip->sheetWidth = _clip->sheetWidth;
	newclip->textureHandle = _clip->textureHandle;
	newclip->fps = _clip->fps;
	newclip->numAnimEvents = 0;
	newclip->playMode = _clip->playMode;

	newclip->frameWidth = _clip->sheetWidth/_clip->framesPerRow;
	newclip->frameHeight = _clip->sheetHeight/_clip->framesPerColumn;
	
	[m_animationInfos[_animation].clipDictionary setObject:[NSValue valueWithPointer:newclip] forKey:_clip->animationName];
}


-(void) AddAnimEventToClip:(NSString *) clipName ForAnimation:(s32) _animation OnFrame:(s32)frame WithID:(s32)eventID
{
	if(_animation == -1)
	{
		return;
	}
	
	AnimationClip* clip = (AnimationClip*)[[m_animationInfos[_animation].clipDictionary objectForKey:clipName] pointerValue];
	if (!clip)
	{
		return;
	}
	
	const s32 numAnimEvents = clip->numAnimEvents;
	
	if(numAnimEvents < MAX_ANIMATION_EVENTS)
	{
		int eventIndex = 0;
		
		for(int i=0; i<numAnimEvents; ++i)
		{
			AnimEvent* pCurrEvent = &clip->animEvents[numAnimEvents];
			
			if(pCurrEvent->frame == frame)
			{
				eventIndex = i;
				
				break;
			}
			else if(frame > pCurrEvent->frame)
			{
				//Shift all the anim events over to make room
				for(int eventIDX=numAnimEvents; eventIDX != i; --eventIDX)
				{
					clip->animEvents[eventIDX] = clip->animEvents[eventIDX-1];
				}
				
				eventIndex = i;
				
				break;
			}
		}
		
		AnimEvent* pTheEvent = &clip->animEvents[eventIndex];
		
		pTheEvent->eventID = eventID;
		pTheEvent->frame = frame;
		
		++clip->numAnimEvents;
	}
}


-(void) SetLerpTimeForAnimation:(s32) _animation: (f32)lerpValue
{
	AnimationInfo* pCurrAnimInfo = &m_animationInfos[_animation];
	pCurrAnimInfo->lerpVal = lerpValue;
}


-(void) PlayClip:(NSString *) clipName ForAnimation:(s32) _animation timesToPlay:(int)_timestoplay;
{
	if(_animation == -1)
	{
		return;
	}

	AnimationClip* pCurrClip = (AnimationClip*)[[m_animationInfos[_animation].clipDictionary objectForKey:clipName] pointerValue];
	if (!pCurrClip)
	{
		return;
	}
	
	m_lastAnimEventFrame = -1;
	
	AnimationInfo* pCurrAnimInfo = &m_animationInfos[_animation];
	
	pCurrAnimInfo->currentClip = pCurrClip;
	[renderer setCustomTexture0:m_animationInfos[_animation].r3d :pCurrClip->textureHandle];
	pCurrAnimInfo->currentFrame = m_animationInfos[_animation].currentClip->startFrame;
	pCurrAnimInfo->timesToPlay = _timestoplay;
	pCurrAnimInfo->wwlo3.x = 0;
	pCurrAnimInfo->wwlo3.y = 0;
	pCurrAnimInfo->animFinished = false;

	const s32 frameDiff = pCurrClip->endFrame-pCurrClip->startFrame;
	
	pCurrAnimInfo->lerpVal = 0.0f;
	
	pCurrAnimInfo->currentTime = 0.0f;
	pCurrAnimInfo->timesPlayed = 0;
	pCurrAnimInfo->pingPong = (frameDiff > 0)?true:false;
}


-(bool) IsAnimationDone:(s32) _animationIndex
{
	if( !m_animationInfos[ _animationIndex ].drawthisframe )
		return false;
	
	if( !m_animationInfos[ _animationIndex].animFinished )
	{
		return false;
	}
	
	return true;
}


-(bool) IsAnimationPlaying:(s32) _animationIndex:(NSString *) _animationName
{
	if(_animationIndex == -1)
	{
		return false;
	}
	
	if (![m_animationInfos[_animationIndex].clipDictionary objectForKey:_animationName])
	{
		NSLog(@"2Error in setIdleClip. Clip not found");
		return false;
	}

	AnimationClip* clip = (AnimationClip*)[[m_animationInfos[ _animationIndex ].clipDictionary objectForKey:_animationName] pointerValue];
	return m_animationInfos[ _animationIndex ].currentClip == clip;
}


-(u32) GetCurrentFrame:(s32) _animationIndex
{
	if(_animationIndex == -1)
	{
		return 0;
	}
	
	if( !m_animationInfos[_animationIndex].currentClip )
	{
		return 0;
	}
	
	return m_animationInfos[_animationIndex].currentFrame;
}


-(void) UpdateAnimations : (f32) timeElapsed
{	
	for (s32 x=0; x<m_highestAnimationIndex+1; ++x) 
	{
		AnimationInfo* pCurrAnimInfo = &m_animationInfos[x];
		
		if(!pCurrAnimInfo->animFinished
		   && pCurrAnimInfo->drawthisframe == true
		   && pCurrAnimInfo->currentClip  )
		{
			AnimationClip* pCurrClip = m_animationInfos[x].currentClip;
			
			if(pCurrClip->playMode == AnimPlayMode_Lerp)
			{
				const f32 floatValue = Lerp((f32)pCurrClip->startFrame,(f32)pCurrClip->endFrame,pCurrAnimInfo->lerpVal);
				pCurrAnimInfo->currentFrame = (s32)floatValue;
			}
			else
			{
				pCurrAnimInfo->currentTime += timeElapsed;
				const f32 frameTime = 1.0f/pCurrClip->fps;
				if(pCurrAnimInfo->currentTime > frameTime)
				{
					pCurrAnimInfo->currentTime -=frameTime;
				}
				else
				{
					continue;
				}
				
				switch(pCurrClip->playMode)
				{
					case AnimPlayMode_Forward:
					{
						++pCurrAnimInfo->currentFrame;
						if(pCurrAnimInfo->currentFrame > pCurrClip->endFrame)
						{
							++pCurrAnimInfo->timesPlayed;
							pCurrAnimInfo->currentFrame = pCurrClip->startFrame;
						}
						
						break;
					}
					case AnimPlayMode_Backward:
					{
						--pCurrAnimInfo->currentFrame;
						if(pCurrAnimInfo->currentFrame < pCurrClip->startFrame)
						{
							++pCurrAnimInfo->timesPlayed;
							pCurrAnimInfo->currentFrame = pCurrClip->endFrame;
						}
						
						break;
					}
					case AnimPlayMode_PingPong:
					{
						if(pCurrAnimInfo->pingPong)
						{
							++pCurrAnimInfo->currentFrame;
							if(pCurrAnimInfo->currentFrame > pCurrClip->endFrame)
							{
								pCurrAnimInfo->pingPong = !pCurrAnimInfo->pingPong;
								--pCurrAnimInfo->currentFrame;
							}
						}
						else
						{
							--pCurrAnimInfo->currentFrame;
							if(pCurrAnimInfo->currentFrame < pCurrClip->startFrame)
							{
								++pCurrAnimInfo->timesPlayed;
								pCurrAnimInfo->pingPong = !pCurrAnimInfo->pingPong;
								++pCurrAnimInfo->currentFrame;
							}
						}
						
						break;
					}
					default:
					{
						break;
					}
				}
				
				
				
				
				if(pCurrAnimInfo->timesToPlay > 0
				   && pCurrAnimInfo->timesPlayed >= pCurrAnimInfo->timesToPlay)
				{	
					pCurrAnimInfo->animFinished = true;
					
					switch(pCurrClip->playMode)
					{
						case AnimPlayMode_Forward:
						{
							pCurrAnimInfo->currentFrame = pCurrClip->endFrame;
							break;
						}
						case AnimPlayMode_Backward:
						{
							pCurrAnimInfo->currentFrame = pCurrClip->startFrame;
							break;
						}
						case AnimPlayMode_PingPong:
						{
							pCurrAnimInfo->currentFrame = pCurrClip->startFrame;
							break;
						}
						default:
						{
							break;
						}
					}
					
					m_lastAnimEventFrame = -1;
					
					if (pCurrClip->nextAnimName) 
					{
						[self PlayClip:pCurrClip->nextAnimName ForAnimation:x timesToPlay:-1];
					}
					
					continue;
				}
			}
			
			//Finally, use whatever frame we have to animate
			
			GLuint rowForFrame = pCurrAnimInfo->currentFrame/pCurrClip->framesPerRow;
			
			s32 columnForCurrentFrame = pCurrAnimInfo->currentFrame-(rowForFrame*pCurrClip->framesPerRow);	
			s32 topleftofcurrentframex = (columnForCurrentFrame*pCurrClip->frameWidth);
			s32 topleftofcurrentframey = (rowForFrame*pCurrClip->frameHeight);	
			
			
			f32 cx = (f32)topleftofcurrentframex/(f32)pCurrClip->sheetWidth;
			f32 cy = (f32)topleftofcurrentframey/(f32)pCurrClip->sheetHeight;
			
			pCurrAnimInfo->wwlo3.x = cx;
			pCurrAnimInfo->wwlo3.y = cy;
			
			//Do this whenever the frame changes
			if(m_lastAnimEventFrame != pCurrAnimInfo->currentFrame)
			{
				for(s32 eventIDX=0; eventIDX < pCurrClip->numAnimEvents; ++eventIDX)
				{
					AnimEvent* pCurrEvent = &pCurrClip->animEvents[eventIDX];
					
					//JAMTODO: THIS HAS A BUG (lol)
					if(pCurrEvent->frame == pCurrAnimInfo->currentFrame )
					{
						[self.animEventDelegate AnimationSystem2D_Delegate_HandleAnimEvent:pCurrEvent->eventID];
					}
				}
				
				m_lastAnimEventFrame = pCurrAnimInfo->currentFrame;
			}
		}
	}
}


-(void)Init:(ES2Renderer*)_renderer
{
	m_highestAnimationIndex = -1;
	memset(m_animPlayers, -1, MAX_ANIMATION_PLAYERS*sizeof(s32));
	renderer = _renderer;
}


-(void) setRenderer :(ES2Renderer*)_renderer
{
	renderer = _renderer;
}


@end
