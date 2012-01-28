//
//  CoreAudio_OpenAL.h
//  CoreEngine3D(iOS)
//
//  Created by Jody McAdams on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#ifndef CoreEngine3D_iOS__CoreAudio_OpenAL_h
#define CoreEngine3D_iOS__CoreAudio_OpenAL_h

class CoreAudioOpenAL;
extern CoreAudioOpenAL* OPENALAUDIO;

#include "MathTypes.h"
#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
#include <OpenAL/alc.h>
#endif

#if defined (PLATFORM_WIN)
#include <alc.h>
#endif

struct CoreAudioFileInfo
{
	u8* data;
	s32 dataSize;
	u32 format;
	s32 sampleRate;
};

struct ItemSoundDescription
{
    const char*		soundFileName;
    u32			soundBufferID;
};

class CoreAudioOpenAL
{
public:
	bool Init();
	void CleanUp();
	bool CreateSoundBufferFromFile(const char* filename, u32* pSoundBufferID);
	u32 CreateSoundSourceFromBuffer(u32 bufferID);
	void DeleteSoundBuffer(u32* soundBufferID);
	void DeleteSoundSource(u32* soundSourceID);
	bool PlaySoundSource(u32 soundSourceID, f32 volume, f32 pitch, bool isLooping);
	void PauseSoundSource(u32 soundSourceID);
	void StopSoundSource(u32 soundSourceID);
	void RewindSoundSource(u32 soundSourceID);
	void SetSoundSourceIsLooping(u32 soundSourceID, bool isLooping);
	void SetSoundSourcePitch(u32 soundSourceID, f32 pitch);
	void SetSoundSourceVolume(u32 soundSourceID, f32 volume);
	void SetSoundSourcePosition(u32 soundSourceID, const vec3* pPosition);
	void SetSoundSourceDirection(u32 soundSourceID, const vec3* pDirection);
	void SetSoundSourceVelocity(u32 soundSourceID, const vec3* pVelocity);
	bool GetSourceIsStopped(u32 soundSourceID);
	void SetListenerPosition(const vec3* pPosition);
	void SetListenerVelocity(const vec3* pVelocity);
	void SetListenerOrientation(const vec3* pAt, const vec3* pUp);
	void SetListenerVolume(f32 volume);
	void SetMaxSoundDistance(f32 maxDistance);
	
private:
#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
	bool LoadSoundDataFromFile_APPLE(const char* filename, CoreAudioFileInfo* pOut_AudioFileInfo);
#endif

	bool CheckForOpenALError();
#if defined(PLATFORM_WIN)
	bool CheckForALUTError();
#endif
	const char* GetPathToFile(const char* filename);
	ALCcontext* m_context;
	ALCdevice* m_device;
	f32 m_maxSoundDistance;
};

#endif
