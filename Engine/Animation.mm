//
//  untitled.m
//  CoreEngine3D
//
//  Created by sam on 10/26/10.
//  Copyright 2010 GameJelly Online. All rights reserved.
//

//DISCLAIMER:  THIS HORRIBLE FILE IS NOT JODY'S FAULT

#import "Animation.h"
#ifdef PLATFORM_OSX
#include <OpenGL/OpenGL.h>
#endif
#import "GraphicsTypes.h"
#import "matrix.h"

#include <fstream>
#include <iostream>

const vec2  tcp = {0,0};
const vec2  tcd = {1,1};


@implementation AnimationSystem2D


int topleftofcurrentframe = 0;
RenderableObject3D obj;


Animationvert2 avert[4];
Animationcoord2 acord[4];


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
	std::cout << "\nAttach Point Array Index " << returnIndex << '\n';
	
	for(int headerIDX=0; headerIDX<pCurrArray->numAttachPointLocationHeaders; ++headerIDX)
	{
		AnimationAttachPointLocations* pAttachLocationsHeader = &pCurrArray->pAttachPointLocationHeaders[headerIDX];
		std::cout << "Attach Point Name: " << pAttachLocationsHeader->attachPointName << '\n';
		
		for(int pointsIDX=0; pointsIDX<pAttachLocationsHeader->numAttachPointLocations; ++pointsIDX)
		{
			vec2* pVec = &pAttachLocationsHeader->pAttachPointLocations[pointsIDX];
			std::cout << "Frame " << pointsIDX << ": <" << pVec->x << ',' << pVec->y << ">\n";
		}
	}
	
	std::cout << '\n';
	
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


//Copies the attach position X,Y (-1 to 1) into the pOut_vec 
/*-(BOOL) GetAnimationAttachPointByIndex:(s32) animationInfoIndex:(s32)attachPointIndex:(vec2*)pOut_vec
{
	AnimationInfo* pAnimInfo = &m_animationInfos[animationInfoIndex];
	
	if(pAnimInfo->currentClip == NULL || pAnimInfo->currentClip->attachHeaderIndex == -1)
	{
		return NO;
	}
	
	AnimationAttachPointArray* pAttachPointArray = &pAnimInfo->m_pAttachPointHeaderArrays[pAnimInfo->currentClip->attachHeaderIndex];
	AnimationAttachPointLocations* pLocations = &pAttachPointArray->pAttachPointLocationHeaders[attachPointIndex];
	
	const s32 currFrame = pAnimInfo->currentClip->currentFrame;
	if(currFrame < pLocations->numAttachPointLocations)
	{
		vec2* pVec = &pLocations->pAttachPointLocations[currFrame];
		CopyVec2(pOut_vec, pVec);
		printf("Attach point location for %s is <%f,%f>\n",pLocations->attachPointName,pVec->x,pVec->y);
		
		return YES;
	}
	
	return NO;
}*/

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
	
	const s32 currFrame = pAnimInfo->currentClip->currentFrame;
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
	for (s32 x=0; x<m_highestAnimationIndex+1; ++x) 
	{
		[m_animationInfos[x].clips release];
		m_animationInfos[x].clips = nil;
		
		for(id key in m_animationInfos[x].clipDictionary) {
			NSValue* value = [m_animationInfos[x].clipDictionary objectForKey:key];
			free(value.pointerValue);
		}
		
		[m_animationInfos[x].clipDictionary release];
		m_animationInfos[x].clipDictionary = nil;
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
	m_animationInfos[newid].startOfPlay = TRUE;
	m_animationInfos[newid].endTime = 999999999999999999.0;
	m_animationInfos[newid].m_numAttachPointHeaderArrays = 0;

	return newid;
}


-(void) LinkAnimationPlayer:(RenderableObject3D*)_r3d: (s32)animationID
{
	
	m_animationInfos[animationID].r3d = _r3d;
	_r3d->uniqueUniformValues[0] = (f32*)&m_animationInfos[animationID].wwlo3;
}


float mod;
-(void) addClip:(AnimationClip *)_clip ToAnimation:(s32) _animation
{
	mod=0.1f;
	
	//	[m_animationInfos[_animation].clips addObject:[NSValue valueWithPointer:_clip]];
	//[m_animationInfos[_animation].clipDictionary addObject:[NSValue valueWithPointer:_clip] forkey ];
	AnimationClip* newclip = (AnimationClip*)calloc(sizeof(AnimationClip), sizeof(AnimationClip));
	
	newclip->attachHeaderIndex = _clip->attachHeaderIndex;
	newclip->animationName = _clip->animationName;
    newclip->nextAnimName = _clip->nextAnimName;
	newclip->beginingFrame = _clip->beginingFrame;
	newclip->endingFrame = _clip->endingFrame;
	newclip->framesPerRow = _clip->framesPerRow;
	newclip->sheetHeight = _clip->sheetHeight;
	newclip->sheetWidth = _clip->sheetWidth;
	newclip->textureHandle = _clip->textureHandle;
	newclip->fps = _clip->fps;
	
	newclip->frameWidth = _clip->sheetWidth/_clip->framesPerRow;
	newclip->frameHeight = _clip->sheetHeight/_clip->framesPerColumn;
	

	if (newclip->nextAnimName == nil)
	{
		newclip->stopAtEnd = TRUE;		
	}
	else
	{
		newclip->stopAtEnd = FALSE;
	}

	
	for (s32 y = 0; y < MAX_ANIMATION_EVENTS; y++)
	{		
		newclip->cs[y]=_clip->cs[y];
		newclip->cf[y]=_clip->cf[y];
		if (_clip->cs[y])
			newclip->usesCallBacks =  YES;
	}	
	
	//NSAutoreleasePool *pool = [[NSAutoreleasePool alloc] init];
	
	[m_animationInfos[_animation].clipDictionary setObject:[NSValue valueWithPointer:newclip] forKey:_clip->animationName];
	
	//[pool drain];
}


-(void) playclip:(NSString *) _nsstring ForAnimation:(s32) _animation timesToPlay:(int)_timestoplay;
{
	if(_animation == -1)
	{
		return;
	}
	
//	NSLog(@"we will now change the current animation to %@ for animation %d", _nsstring,_animation);
	if (![m_animationInfos[_animation].clipDictionary objectForKey:_nsstring])
	{
		NSLog(@"2Error in setIdleClip. Clip not found");
		
		return;
	}
	AnimationClip* clip = (AnimationClip*)[[m_animationInfos[_animation].clipDictionary objectForKey:_nsstring] pointerValue];
	m_animationInfos[_animation].currentClip = clip;
	m_animationInfos[_animation].r3d->customTexture0 = clip->textureHandle;
	renderer->ForceRenderablesNeedSorting();
	m_animationInfos[_animation].currentClip->currentFrame = m_animationInfos[_animation].currentClip->beginingFrame;
	m_animationInfos[_animation].timesToPlay = _timestoplay;
	m_animationInfos[_animation].startOfPlay = TRUE;
	m_animationInfos[_animation].wwlo3.x = 0;
	m_animationInfos[_animation].wwlo3.y = 0;
	
	if (_timestoplay < 1) 
	{
		m_animationInfos[_animation].endTime = 9999999999999999.0;
	}
}

-(bool) IsAnimationDone:(s32) _animationIndex
{
	if( !m_animationInfos[ _animationIndex ].drawthisframe )
		return false;
	
	if( m_animationInfos[ _animationIndex ].currentClip )
		return false;
	
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
	
	return m_animationInfos[_animationIndex].currentClip->currentFrame;
}

int columnForCurrentFrame;
f32 m_pausableTime;

-(void) UpdateAnimations : (f32) elapsedTime
{	
	m_pausableTime += elapsedTime;

	for (s32 x=0; x<m_highestAnimationIndex+1; ++x) 
	{
		if(m_animationInfos[x].drawthisframe == true && m_animationInfos[x].currentClip  )
		{
			if (m_animationInfos[x].startOfPlay)
			{
				m_animationInfos[x].startOfPlay = FALSE;
				m_animationInfos[x].theTimeOffset = m_pausableTime;
				m_animationInfos[x].localTime = m_pausableTime-m_animationInfos[x].theTimeOffset; //cancels out to zero?
				if(m_animationInfos[x].currentClip->usesCallBacks)
				{
					for (s32 y = 0; y < MAX_ANIMATION_EVENTS; y++)
					{
						m_animationInfos[x].currentClip->cb_Done[y] =  0;						
					}	
				}
				
				m_animationInfos[x].beginningTime = m_animationInfos[x].localTime; //zero?
		
				if(m_animationInfos[x].timesToPlay > 0)
				{
					m_animationInfos[x].endTime = m_animationInfos[x].beginningTime+(m_animationInfos[x].currentClip->endingFrame+1-m_animationInfos[x].currentClip->beginingFrame)*(1.0f/m_animationInfos[x].currentClip->fps)*m_animationInfos[x].timesToPlay;
				}
				else 
				{
					m_animationInfos[x].endTime = 9999999999999999.0;
				}
				
			}
			
			m_animationInfos[x].localTime = m_pausableTime-m_animationInfos[x].theTimeOffset;
		
			
			GLuint rowForFrame = 0;
			if(m_animationInfos[x].currentClip->currentFrame)
				rowForFrame = m_animationInfos[x].currentClip->currentFrame/m_animationInfos[x].currentClip->framesPerRow;
			else
				rowForFrame = 0;
			
			columnForCurrentFrame = m_animationInfos[x].currentClip->currentFrame-(rowForFrame*m_animationInfos[x].currentClip->framesPerRow);	
			int topleftofcurrentframex = (columnForCurrentFrame*m_animationInfos[x].currentClip->frameWidth);
			int topleftofcurrentframey = (rowForFrame*m_animationInfos[x].currentClip->frameHeight);	
			
			
			f32 cx = (float)topleftofcurrentframex/(float)m_animationInfos[x].currentClip->sheetWidth;
			f32 cy = (float)topleftofcurrentframey/(float)m_animationInfos[x].currentClip->sheetHeight;
			
			m_animationInfos[x].wwlo3.x = cx;
			m_animationInfos[x].wwlo3.y = cy;
			

			
			/*if(m_animationInfos[x].currentClip->usesCallBacks)
			{
				for (s32 y = 0; y < MAX_ANIMATION_EVENTS; y++)
				{
					if(m_animationInfos[x].currentClip->cs[y])
						if (m_animationInfos[x].currentClip->currentFrame >= m_animationInfos[x].currentClip->cf[y] && m_animationInfos[x].currentClip->cb_Done[y]==0)
						{
							m_animationInfos[x].currentClip->cb_Done[y] =  1;
							[game performSelector:m_animationInfos[x].currentClip->cs[y]];
						}
				}	
			}*/
			
			double	floatTime = m_animationInfos[x].localTime;			
			if(floatTime >= m_animationInfos[x].endTime )
			{	
				void * hm = m_animationInfos[x].currentClip;

				/*if(m_animationInfos[x].currentClip->usesCallBacks)//wrap around check
				{
					for (s32 y = 0; y < MAX_ANIMATION_EVENTS; y++)
					{
						if(m_animationInfos[x].currentClip->cs[y])
						{
							if (m_animationInfos[x].currentClip->cb_Done[y]==0)
							{
								NSLog(@"wrap around selector caller");
								m_animationInfos[x].currentClip->cb_Done[y] =  1;
								[game performSelector:m_animationInfos[x].currentClip->cs[y]];
							}
						}
							
					}	
				}*/
					
				if (m_animationInfos[x].currentClip == hm) //a selector that was just called might have changed the current animation to something different from what we think we are working with.
				{
					if (m_animationInfos[x].currentClip->stopAtEnd) 
					{
						m_animationInfos[x].currentClip = nil;
					}
					else 
					{
						[self playclip:m_animationInfos[x].currentClip->nextAnimName ForAnimation:x timesToPlay:-1];
					}
				}

				continue;
			}
			
			
			float frameInterval = 1.0f/m_animationInfos[x].currentClip->fps;
			int TimeSlot = floatTime/frameInterval;
			
			//frames in clip?
			int fis = m_animationInfos[x].currentClip->endingFrame+1- m_animationInfos[x].currentClip->beginingFrame;
			
			int cframe = (TimeSlot%fis)+m_animationInfos[x].currentClip->beginingFrame;
			
			
			m_animationInfos[x].currentClip->currentFrame = cframe;
			
			
			if(m_animationInfos[x].currentClip->currentFrame > m_animationInfos[x].currentClip->endingFrame)
			{
				/*if(m_animationInfos[x].currentClip->usesCallBacks)//wrap around check.
				{
					for (s32 y = 0; y < MAX_ANIMATION_EVENTS; y++)
					{
						if(m_animationInfos[x].currentClip->cs[y])
							if (m_animationInfos[x].currentClip->currentFrame >= m_animationInfos[x].currentClip->cf[y] && m_animationInfos[x].currentClip->cb_Done[y]==0)
							{
								m_animationInfos[x].currentClip->cb_Done[y] =  1;
								[game performSelector:m_animationInfos[x].currentClip->cs[y]];
							}
					}	
				}*/
				
				m_animationInfos[x].currentClip->currentFrame = m_animationInfos[x].currentClip->beginingFrame;
				
				if(m_animationInfos[x].currentClip->usesCallBacks)
				{
					for (s32 y = 0; y < MAX_ANIMATION_EVENTS; y++)
					{
						m_animationInfos[x].currentClip->cb_Done[y] =  0;								
						
					}	
				}				
			}
		}
	}
}


-(void)Init:(OpenGLRenderer*)_renderer
{
	m_highestAnimationIndex = -1;
	memset(m_animPlayers, -1, MAX_ANIMATION_PLAYERS*sizeof(s32));
	[self setRenderer:_renderer];
}


-(void) setRenderer :(OpenGLRenderer*)_renderer
{
	renderer = _renderer;
}


@end
