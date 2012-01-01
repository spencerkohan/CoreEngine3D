//
//  CoreAudio_OpenAL.cpp
//  CoreEngine3D(iOS)
//
//  Created by Jody McAdams on 12/31/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "CoreAudio_OpenAL.h"
#include "stddef.h" //for NULL -_-

#if defined (PLATFORM_OSX) || (defined PLATFORM_IOS)
#include <Foundation/Foundation.h>
#endif

bool CoreAudioOpenAL::Init()
{
	// Initialization
	m_device = alcOpenDevice(NULL);
	
	if (m_device)
	{
		m_context = alcCreateContext(m_device,NULL);
		
		alcMakeContextCurrent(m_context);
		
		return true;
	}
	
	return false;
}


void CoreAudioOpenAL::CleanUp()
{
	// destroy the context
	alcDestroyContext(m_context);
	
	// close the device
	alcCloseDevice(m_device);
}


bool CoreAudioOpenAL::LoadSoundDataFromFile_APPLE(const char* filename, CoreAudioFileInfo* pOut_AudioFileInfo)
{
	//NOTE: this function is horrible
	
	NSString* fileString = [NSString stringWithCString:filename encoding:NSUTF8StringEncoding];
	
	// get some audio data from a wave file
	CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:[[NSBundle mainBundle] pathForResource:fileString ofType:@""]] retain];
	
	if (!fileURL)
	{
		//NSLog(@"error loading sound file, file Not found");
		//END_TIMER(@"file not found.");
		return false;
	}
	
	UInt32		dataSize;
	OSStatus err = noErr;
	SInt64 theFileLengthInFrames = 0;
	AudioStreamBasicDescription theFileFormat;
	UInt32 thePropertySize = sizeof(theFileFormat);
	ExtAudioFileRef extRef = NULL;
	u8* theData = NULL;
	AudioStreamBasicDescription theOutputFormat;
	
	// Open a file with ExtAudioFileOpen()
	err = ExtAudioFileOpenURL(fileURL, &extRef);
	if(err)
	{
		//printf("MyGetOpenALAudioData: ExtAudioFileOpenURL FAILED, Error = %ld\n", err); 
		goto Exit;
	}
	
	// Get the audio data format
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	if(err)
	{
		printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileDataFormat) FAILED, Error = %ld\n", err); goto Exit;
	}
	if (theFileFormat.mChannelsPerFrame > 2)
	{
		printf("MyGetOpenALAudioData - Unsupported Format, channel count is greater than stereo\n");
		goto Exit;
	}
	
	// Set the client format to 16 bit signed integer (native-endian) data
	// Maintain the channel count and sample rate of the original source format
	theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;
	
	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;
	
	// Set the desired client (output) data format
	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	if(err) { printf("MyGetOpenALAudioData: ExtAudioFileSetProperty(kExtAudioFileProperty_ClientDataFormat) FAILED, Error = %ld\n", err); goto Exit; }
	
	// Get the total frame count
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	if(err) { printf("MyGetOpenALAudioData: ExtAudioFileGetProperty(kExtAudioFileProperty_FileLengthFrames) FAILED, Error = %ld\n", err); goto Exit; }
	
	// Read all the data into memory
	dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;
	theData = (u8*)malloc(dataSize);
	if (theData)
	{
		AudioBufferList		theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = theData;
		
		// Read the data into an AudioBufferList
		err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
		if(err == noErr)
		{
			// success
			pOut_AudioFileInfo->dataSize = (ALsizei)dataSize;
			pOut_AudioFileInfo->format = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
			pOut_AudioFileInfo->sampleRate = (ALsizei)theOutputFormat.mSampleRate;
		}
		else
		{
			// failure
			free (theData);
			theData = NULL; // make sure to return NULL
			printf("MyGetOpenALAudioData: ExtAudioFileRead FAILED, Error = %ld\n", err); goto Exit;
		}
	}
	
Exit:
	// Dispose the ExtAudioFileRef, it is no longer needed
	if (extRef) ExtAudioFileDispose(extRef);
	
	pOut_AudioFileInfo->data = theData;
	
	CFRelease(fileURL);
	
	return true;
}

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
	
	return soundSourceID;
}

void CoreAudioOpenAL::DeleteBuffer(u32 soundBufferID)
{
	alDeleteBuffers(1, &soundBufferID);
}

void CoreAudioOpenAL::DeleteSoundSource(u32 soundSourceID)
{
	alDeleteSources(1, &soundSourceID);
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

u32 CoreAudioOpenAL::CreateSoundBufferFromFile(const char* filename)
{
	u32 soundBufferID;
	alGenBuffers(1, &soundBufferID);
	
	if(CheckForOpenALError())
	{
		return 0;
	}
	
	CoreAudioFileInfo fileInfo;
	LoadSoundDataFromFile_APPLE(filename, &fileInfo);

	if(CheckForOpenALError())
	{
		alDeleteBuffers(1, &soundBufferID);
		return 0;
	}
	
	// Copy wav data into AL Buffer
	alBufferData(soundBufferID,fileInfo.format,fileInfo.data,fileInfo.dataSize,fileInfo.sampleRate);
	
	if(CheckForOpenALError())
	{
		alDeleteBuffers(1, &soundBufferID);
		return 0;
	}

	return soundBufferID;
}
