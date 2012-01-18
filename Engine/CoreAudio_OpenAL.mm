//
//  CoreAudio_OpenAL.cpp
//  CoreEngine3D(iOS)
//
//  Created by Jody McAdams on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CoreAudio_OpenAL.h"
#include "MathUtil.h"

#include "stddef.h" //for NULL -_-
#include <cstdio>

CoreAudioOpenAL* OPENALAUDIO = NULL;



#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
#include <OpenAL/al.h>
#endif

#if defined (PLATFORM_WIN)
#include "al.h"
#endif

#if defined (PLATFORM_OSX) || (defined PLATFORM_IOS)
#include <Foundation/Foundation.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#endif


bool CoreAudioOpenAL::Init()
{
	OPENALAUDIO = this;
	
	m_maxSoundDistance = 1.0f;

	// Initialization
	m_device = alcOpenDevice(NULL);
	
	if (m_device)
	{
		m_context = alcCreateContext(m_device,NULL);
		CheckForOpenALError();
		
		alcMakeContextCurrent(m_context);
		CheckForOpenALError();
		
		return true;
	}
	
	return false;
}

void CoreAudioOpenAL::SetMaxSoundDistance(f32 maxDistance)
{
	m_maxSoundDistance = maxDistance;
}


void CoreAudioOpenAL::CleanUp()
{
	// destroy the context
	alcDestroyContext(m_context);
	
	// close the device
	alcCloseDevice(m_device);
}

const char* CoreAudioOpenAL::GetPathToFile(const char* filename)
{
#if defined PLATFORM_OSX || defined PLATFORM_IOS
	NSString* fileString = [NSString stringWithCString:filename encoding:NSUTF8StringEncoding];
	NSString *fullPath = [[NSBundle mainBundle] pathForResource:[fileString lastPathComponent] ofType:nil inDirectory:[fileString stringByDeletingLastPathComponent]];
	
	return [fullPath UTF8String];
#else
	return filename;
#endif
}

#if defined (PLATFORM_WIN)
bool LoadSoundDataFromFile_WIN(const char* filename, CoreAudioFileInfo* pOut_AudioFileInfo)
{
	return false;
}
#endif

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
bool CoreAudioOpenAL::LoadSoundDataFromFile_APPLE(const char* filename, CoreAudioFileInfo* pOut_AudioFileInfo)
{
	CFStringRef filenameStr = CFStringCreateWithCString( NULL, GetPathToFile(filename), kCFStringEncodingUTF8 );
    CFURLRef url = CFURLCreateWithFileSystemPath( NULL, filenameStr, kCFURLPOSIXPathStyle, false );
    CFRelease( filenameStr );
	
    AudioFileID audioFile;
    OSStatus error = AudioFileOpenURL( url, kAudioFileReadPermission, kAudioFileWAVEType, &audioFile );
    CFRelease( url );
	
    if ( error != noErr )
    {
        fprintf( stderr, "Error opening audio file. %d\n", error );
        return false;
    }
	
    AudioStreamBasicDescription basicDescription;
    UInt32 propertySize = sizeof(basicDescription);
    error = AudioFileGetProperty( audioFile, kAudioFilePropertyDataFormat, &propertySize, &basicDescription );
	
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading audio file basic description. %d\n", error );
        AudioFileClose( audioFile );
        return false;
    }
	
    if ( basicDescription.mFormatID != kAudioFormatLinearPCM )
    {
        // Need PCM for Open AL. WAVs are (I believe) by definition PCM, so this check isn't necessary. It's just here
        // in case I ever use this with another audio format.
        fprintf( stderr, "Audio file is not linear-PCM. %d\n", basicDescription.mFormatID );
        AudioFileClose( audioFile );
        return false;
    }
	
    UInt64 audioDataByteCount = 0;
    propertySize = sizeof(audioDataByteCount);
    error = AudioFileGetProperty( audioFile, kAudioFilePropertyAudioDataByteCount, &propertySize, &audioDataByteCount );
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading audio file byte count. %d\n", error );
        AudioFileClose( audioFile );
        return false;
    }
	
    Float64 estimatedDuration = 0;
    propertySize = sizeof(estimatedDuration);
    error = AudioFileGetProperty( audioFile, kAudioFilePropertyEstimatedDuration, &propertySize, &estimatedDuration );
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading estimated duration of audio file. %d\n", error );
        AudioFileClose( audioFile );
        return false;
    }
	
    ALenum alFormat = 0;
	
    if ( basicDescription.mChannelsPerFrame == 1 )
    {
        if ( basicDescription.mBitsPerChannel == 8 )
            alFormat = AL_FORMAT_MONO8;
			else if ( basicDescription.mBitsPerChannel == 16 )
				alFormat = AL_FORMAT_MONO16;
				else
				{
					alFormat = AL_FORMAT_MONO16;
					fprintf( stderr, "Expected 8 or 16 bits for the mono channel but got %d\n", basicDescription.mBitsPerChannel );
					AudioFileClose( audioFile );
					return false;
				}
		
    }
    else if ( basicDescription.mChannelsPerFrame == 2 )
    {
        if ( basicDescription.mBitsPerChannel == 8 )
            alFormat = AL_FORMAT_STEREO8;
			else if ( basicDescription.mBitsPerChannel == 16 )
				alFormat = AL_FORMAT_STEREO16;
				else
				{
					fprintf( stderr, "Expected 8 or 16 bits per channel but got %d\n", basicDescription.mBitsPerChannel );
					AudioFileClose( audioFile );
					return false;
				}
    }
    else
    {
        fprintf( stderr, "Expected 1 or 2 channels in audio file but got %d\n", basicDescription.mChannelsPerFrame );
        AudioFileClose( audioFile );
        return false;
    }
	
    UInt32 numBytesToRead = audioDataByteCount;
    void* buffer = malloc( numBytesToRead );
	
    if ( buffer == NULL )
    {
        fprintf( stderr, "Error allocating buffer for audio data of size %u\n", numBytesToRead );
        return false;
    }
	
    error = AudioFileReadBytes( audioFile, false, 0, &numBytesToRead, buffer );
    AudioFileClose( audioFile );
	
    if ( error != noErr )
    {
        fprintf( stderr, "Error reading audio bytes. %d\n", error );
        free(buffer);
        return false;
    }
	
    if ( numBytesToRead != audioDataByteCount )
    {
        fprintf( stderr, "Tried to read %lld bytes from the audio file but only got %d bytes\n", audioDataByteCount, numBytesToRead );
        free(buffer);
        return false;
    }
	
    pOut_AudioFileInfo->sampleRate = basicDescription.mSampleRate;
    pOut_AudioFileInfo->dataSize = audioDataByteCount;
    pOut_AudioFileInfo->format = alFormat;
    pOut_AudioFileInfo->data = (u8*)buffer;
    //*estimatedDurationOut = estimatedDuration;
	
    return true;
}
#endif

bool CoreAudioOpenAL::CheckForOpenALError()
{
	ALenum error = alGetError();
	
	switch(error)
	{
		case AL_INVALID_NAME:
		{
			printf("Invalid Name paramater passed to AL call.\n");
			
			return true;
		}
		case AL_INVALID_ENUM:
		{
			printf("Invalid parameter passed to AL call.\n");
			
			return true;
		}
		case AL_INVALID_VALUE:
		{
			printf("Invalid enum parameter value.\n");
			
			return true;
		}                          
		case AL_INVALID_OPERATION:
		{
			printf("Illegal call.\n");
			
			return true;
		}                      
		case AL_OUT_OF_MEMORY:
		{
			printf("Out of memory!\n");
			
			return true;
		}
		case AL_NO_ERROR:
		{
			return false;
		}
		default:
		{
			return true;
		}
	}
}

void CoreAudioOpenAL::SetListenerPosition(const vec3* pPosition)
{
	alListenerfv(AL_POSITION,(const ALfloat*)pPosition);
	CheckForOpenALError();
}


void CoreAudioOpenAL::SetListenerVelocity(const vec3* pVelocity)
{
	alListenerfv(AL_VELOCITY,(const ALfloat*)pVelocity);
	CheckForOpenALError();
}


void CoreAudioOpenAL::SetListenerOrientation(const vec3* pAt, const vec3* pUp)
{
	f32 orientVec[6];
	CopyVec3((vec3*)&orientVec[0],pAt);
	CopyVec3((vec3*)&orientVec[3],pUp);
	
	alListenerfv(AL_ORIENTATION,(const ALfloat*)orientVec);
	CheckForOpenALError();
}


void CoreAudioOpenAL::SetListenerVolume(f32 volume)
{
	alListenerf(AL_GAIN,volume);
	CheckForOpenALError();
}


void CoreAudioOpenAL::PlaySoundSource(u32 soundSourceID, f32 volume, f32 pitch, bool isLooping)
{
	SetSoundSourceIsLooping(soundSourceID, isLooping);
	SetSoundSourcePitch(soundSourceID, pitch);
	SetSoundSourceVolume(soundSourceID, volume);
	
	alSourcePlay(soundSourceID);
	CheckForOpenALError();
}


void CoreAudioOpenAL::PauseSoundSource(u32 soundSourceID)
{
	alSourcePause(soundSourceID);
	CheckForOpenALError();
}


void CoreAudioOpenAL::StopSoundSource(u32 soundSourceID)
{
	alSourceStop(soundSourceID);
	CheckForOpenALError();
}


void CoreAudioOpenAL::RewindSoundSource(u32 soundSourceID)
{
	alSourceRewind(soundSourceID);
	CheckForOpenALError();
}

u32 CoreAudioOpenAL::CreateSoundSourceFromBuffer(u32 bufferID)
{
	u32 soundSourceID;
	alGenSources(1,&soundSourceID);
	
	if(CheckForOpenALError())
	{
		return 0;
	}
	
	alSourcei(soundSourceID, AL_BUFFER, bufferID); 
	if(CheckForOpenALError())
	{
		return 0;
	}
	
	alSourcef(soundSourceID, AL_REFERENCE_DISTANCE, m_maxSoundDistance);
	if(CheckForOpenALError())
	{
		return 0;
	}
	
	return soundSourceID;
}

void CoreAudioOpenAL::DeleteSoundBuffer(u32* soundBufferID)
{
	alDeleteBuffers(1, soundBufferID);
	*soundBufferID = 0;
}

void CoreAudioOpenAL::DeleteSoundSource(u32* soundSourceID)
{
	alDeleteSources(1, soundSourceID);
	*soundSourceID = 0;
}

void CoreAudioOpenAL::SetSoundSourceIsLooping(u32 soundSourceID, bool isLooping)
{
	alSourcei(soundSourceID, AL_LOOPING, isLooping?AL_TRUE:AL_FALSE);
}


void CoreAudioOpenAL::SetSoundSourcePitch(u32 soundSourceID, f32 pitch)
{
	alSourcef(soundSourceID, AL_PITCH, pitch);
}

void CoreAudioOpenAL::SetSoundSourceVolume(u32 soundSourceID, f32 volume)
{
	alSourcef(soundSourceID, AL_GAIN, volume);
}

void CoreAudioOpenAL::SetSoundSourcePosition(u32 soundSourceID, const vec3* pPosition)
{
	alSourcefv(soundSourceID, AL_POSITION, (ALfloat*)pPosition);
}

void CoreAudioOpenAL::SetSoundSourceDirection(u32 soundSourceID, const vec3* pDirection)
{
	alSourcefv(soundSourceID, AL_DIRECTION, (ALfloat*)pDirection);
}

void CoreAudioOpenAL::SetSoundSourceVelocity(u32 soundSourceID, const vec3* pVelocity)
{
	alSourcefv(soundSourceID, AL_VELOCITY, (ALfloat*)pVelocity);
}

bool CoreAudioOpenAL::GetSourceIsStopped(u32 soundSourceID)
{
	s32 state;
	alGetSourcei(soundSourceID,AL_SOURCE_STATE,&state);
	
	return state == AL_STOPPED;
}

bool CoreAudioOpenAL::CreateSoundBufferFromFile(const char* filename, u32* pSoundBufferID)
{
#if defined (PLATFORM_WIN)
	return false;
#endif

	if (pSoundBufferID == NULL || *pSoundBufferID != 0)
    {       
        return false;
    }
	
	u32 soundBufferID;
	alGenBuffers(1, &soundBufferID);
	
	if(CheckForOpenALError())
	{
		return false;
	}
	
	CoreAudioFileInfo fileInfo;

#if defined (PLATFORM_IOS) || defined (PLATFORM_OSX)
	LoadSoundDataFromFile_APPLE(filename, &fileInfo);
#endif

#if defined (PLATFORM_WIN)
	//LoadSoundDataFromFile_WIN(filename, &fileInfo);
#endif

	if(CheckForOpenALError())
	{
		alDeleteBuffers(1, &soundBufferID);
		return false;
	}
	
	// Copy wav data into AL Buffer
	alBufferData(soundBufferID,fileInfo.format,fileInfo.data,fileInfo.dataSize,fileInfo.sampleRate);
	
	if(CheckForOpenALError())
	{
		alDeleteBuffers(1, &soundBufferID);
		return false;
	}
	
	delete[] fileInfo.data;

	*pSoundBufferID = soundBufferID;

	return true;
}
