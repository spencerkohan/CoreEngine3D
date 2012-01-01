//
//  CoreAudio_OpenAL.h
//  CoreEngine3D(iOS)
//
//  Created by Jody McAdams on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_iOS__CoreAudio_OpenAL_h
#define CoreEngine3D_iOS__CoreAudio_OpenAL_h

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#endif

#include "MathTypes.h"

struct CoreAudioFileInfo
{
	u8* data;
	s32 dataSize;
	u32 format;
	s32 sampleRate;
};

class CoreAudioOpenAL
{
public:
	bool Init();
	void CleanUp();
	u32 CreateSoundBufferFromFile(const char* filename);
	u32 CreateSoundSourceFromBuffer(u32 bufferID);
	void DeleteBuffer(u32 soundBufferID);
	void DeleteSoundSource(u32 soundSourceID);
	void SetSoundSourceIsLooping(u32 soundSourceID, bool isLooping);
	void SetSoundSourcePitch(u32 soundSourceID, f32 pitch);
	void SetSoundSourceVolume(u32 soundSourceID, f32 volume);
	void SetSoundSourcePosition(u32 soundSourceID, const vec3* pPosition);
	void SetSoundSourceDirection(u32 soundSourceID, const vec3* pDirection);
	void SetSoundSourceVelocity(u32 soundSourceID, const vec3* pVelocity);
	
private:
	bool LoadSoundDataFromFile_APPLE(const char* filename, CoreAudioFileInfo* pOut_AudioFileInfo);
	bool CheckForOpenALError();
	
	ALCcontext* m_context;
	ALCdevice* m_device;
};

#endif
