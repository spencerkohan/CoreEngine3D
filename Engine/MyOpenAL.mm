//
//  MyOpenAL.m
//

#import "MyOpenAL.h"
#import "MyOpenALSupport.h"
#include "MathTypes.h"

//#define     

//UInt64 aARC4RANDOM_MAX =  0x100000000;
unsigned int ARC4RANDOM_MAX =  (unsigned int)4294967295ULL;

struct soundData * lsound;

@implementation MyOpenAL

@synthesize bufferStorageArray;
@synthesize soundDictionary;
@synthesize bufferDictionary;

static MyOpenAL *sharedMyOpenAL = nil;

- (void) shutdownMyOpenAL {
	@synchronized(self) {
        if (sharedMyOpenAL != nil) {
			[self dealloc]; // assignment not done here
        }
    }
}

+ (MyOpenAL*)sharedMyOpenAL {
	
    @synchronized(self) {
        if (sharedMyOpenAL == nil)
		{
            [[self alloc] init]; // assignment not done here
        }
    }
    return sharedMyOpenAL;
}


+ (id)allocWithZone:(NSZone *)zone
{
	//NSLog(@"wtf is allocWithZone and why is it getting called");
	//Answer: http://www.omnigroup.com/mailman/archive/macosx-dev/2001-July/029527.html
	//interesting
    @synchronized(self) {
        if (sharedMyOpenAL == nil) {
            sharedMyOpenAL = [super allocWithZone:zone];
            return sharedMyOpenAL;  // assignment and return on first allocation
        }
    }
    return nil; //on subsequent allocation attempts return nil
}

- (id)copyWithZone:(NSZone *)zone
{
    return self;
}

- (id)retain
{
    return self;
}

- (unsigned)retainCount
{
	
	return UINT_MAX;  //denotes an object that cannot be released
}


- (id)autorelease
{
    return self;
}


// start up openAL
-(bool) initOpenAL
{
	// Initialization
	mDevice = alcOpenDevice(NULL); // select the "preferred device"
	if (mDevice)
	{
		ALog(@"does this happen more than once");
		// use the device to make a context
		mContext=alcCreateContext(mDevice,NULL);
		// set my context to the currently active one
		alcMakeContextCurrent(mContext);
		
		return true;
	}
	return false;
}

NSMutableArray * sourceArray1; // array of sources

-(id) init 
{
	//MARK;
	if ((self = [super init]) ) 
	{
		//ALog(@"this should run only once");
		AudioSessionInitialize(NULL,NULL,NULL,NULL);
		if ([self initOpenAL]) 
		{
			
			self.bufferStorageArray = [[[NSMutableArray alloc]init]autorelease];
			self.soundDictionary =   [[[NSMutableDictionary alloc]init]autorelease];
			self.bufferDictionary = [[[NSMutableDictionary alloc]init]autorelease];
			sourceArray1=			[[NSMutableArray alloc]init];
			
		}
		return self;
	}
	[self release];
	return nil;
}

-(void) dealloc {
	
	MARK;
	// delete the sources
	for (NSNumber * sourceNumber in [soundDictionary allValues]) {
		NSUInteger sourceID = [sourceNumber unsignedIntegerValue];
		alDeleteSources(1, &sourceID);
	}
	
	self.soundDictionary=nil;
	
	// delete the buffers
	for (NSNumber * bufferNumber in bufferStorageArray) {
		NSUInteger bufferID = [bufferNumber unsignedIntegerValue];
		alDeleteBuffers(1, &bufferID);
	}
	self.bufferStorageArray=nil;
	
	// destroy the context
	alcDestroyContext(mContext);
	// close the device
	alcCloseDevice(mDevice);
	
	[super dealloc];
}

// the main method: grab the sound ID from the library
// and start the source playing
- (void)playSoundWithKey:(NSString*)soundKey
{
//	MARK;
	NSNumber * numVal = [soundDictionary objectForKey:soundKey];
	if (numVal == nil) return;
	NSUInteger sourceID = [numVal unsignedIntValue];
	alSourcePlay(sourceID);
}

- (void)stopSoundWithKey:(NSString*)soundKey
{
	MARK;
	NSNumber * numVal = [soundDictionary objectForKey:soundKey];
	if (numVal == nil) return;
	NSUInteger sourceID = [numVal unsignedIntValue];
	alSourceStop(sourceID);
}

-(void) rewindSoundWithKey:(NSString *) soundKey {
	
	MARK;
	NSNumber * numVal = [soundDictionary objectForKey:soundKey];
	if (numVal == nil) return;
	NSUInteger sourceID = [numVal unsignedIntValue];
	alSourceRewind (sourceID);
}

-(bool) isPlayingSoundWithKey:(NSString *) soundKey {
	
	MARK;
	NSNumber * numVal = [soundDictionary objectForKey:soundKey];
	if (numVal == nil) return false;
	NSUInteger sourceID = [numVal unsignedIntValue];
	
	ALenum state;
	
    alGetSourcei(sourceID, AL_SOURCE_STATE, &state);
	
    return (state == AL_PLAYING);
}

-(bool) loadSoundBufferWithKey:(NSString *)_soundKey volume:(float) _volume pitch:(float)_pitch shift:(float)_shift File:(NSString *)_file Ext:(NSString *) _ext 
{
	if ([bufferDictionary objectForKey:_soundKey])//if this sounds was already loaded.
	{
		return true;
	}
	
	ALvoid * outData;
	ALenum  error = AL_NO_ERROR;
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
	NSBundle * bundle = [NSBundle mainBundle];
	
	
	// get some audio data from a wave file
	CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:[bundle pathForResource:_file ofType:_ext]] retain];
	
	if (!fileURL)
	{
		NSLog(@"error loading sound file, file Not found");
		//END_TIMER(@"file not found.");
		return false;
	}
	
	outData = MyGetOpenALAudioData(fileURL, &size, &format, &freq);
	
	CFRelease(fileURL);
	
	if((error = alGetError()) != AL_NO_ERROR) {
		printf("error loading sound: %x\n", error);		
	}
	
	//ALog(@"getting a free buffer from openAL.");
	NSUInteger bufferID;
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	//ALog(@"loading audio data into openAL buffer.");
	// load the awaiting data blob into the openAL buffer.
	alBufferData(bufferID,format,outData,size,freq); 
	
	struct soundData * thesound;
	if ([bufferDictionary objectForKey:_soundKey])
	{
		thesound=(struct soundData *)[[bufferDictionary objectForKey:_soundKey] unsignedIntValue];
	}
	else 
	{
		thesound=(struct soundData *)calloc(1, sizeof(struct soundData));
	}

	
	
	thesound[0].sbufferid=bufferID;
    //hack
    const f32 hackVolumeMult = 5.0f;
	thesound[0].volume=_volume*hackVolumeMult;	
	thesound[0].pitch=_pitch;
	thesound[0].shift=_shift;
	[bufferDictionary setObject:[NSNumber numberWithUnsignedInt:(unsigned int)thesound] forKey:_soundKey];
	
	for (s32 x = 0; x< MaxSourcesPerBuffer; x++)
	{
		alGenSources(1, &thesound->thesources[x]); 
	}
		 
		 
	
	// save the buffer so we can release it later
	[bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
	//[bufferDictionary setObject:[NSNumber numberWithUnsignedInt:bufferID] forKey:_soundKey];
	
	
	//END_TIMER(@"Audiofile successfully loaded.");
	return true;
	
	
}

-(void) StopAllSourcesForBuffer:(NSString *)_buffer
{
	lsound=(struct soundData *)[[bufferDictionary objectForKey:_buffer] unsignedIntValue];
	if(!lsound)
	{
		NSLog(@"Problem in the sound system. Sound not found?");
		return;	
	}
	for (int x=0; x< MaxSourcesPerBuffer; x++) 
	{
		alSourceStop(lsound->thesources[x]);		
	}
	
}
//
-(bool) PlaySoundBufferAtLocation:(NSString *)_buffer Location:(const vec3*)pSoundPos PitchAdd:(f32)pitchAdd Looping:(BOOL)loops
{
	lsound = nil;
	
	//ALog(@"getting a free source from openAL.");
	NSUInteger sourceID;


	lsound=(struct soundData *)[[bufferDictionary objectForKey:_buffer] unsignedIntValue];
	if(!lsound)
	{
		NSLog(@"Problem in the sound system. Sound not found? did you load the sound?");
		return FALSE;	
	}
	sourceID = lsound->thesources[lsound->nextSourceToUse];
	
	lsound->nextSourceToUse++;
	if (lsound->nextSourceToUse>= MaxSourcesPerBuffer) 
	{
		lsound->nextSourceToUse = 0;
	}
	
	alSourcei(sourceID, AL_BUFFER, lsound[0].sbufferid);

	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, pitchAdd + lsound[0].pitch + (((double)arc4random() / ARC4RANDOM_MAX)*2-1 )*  lsound[0].shift);

	alSourcef(sourceID, AL_GAIN, lsound[0].volume);

	alSourcefv(sourceID, AL_POSITION, (ALfloat*)pSoundPos);
	
	
	if (loops) alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	
	
	alSourcePlay(sourceID);
	
	// store this for future use (Jody Note: what?)
	//	[soundDictionary setObject:[NSNumber numberWithUnsignedInt:sourceID] forKey:_soundKey];	
	//	NSLog(@"here is the contents of the sound dictionary: %@",soundDictionary);
	//	ALog(@"free %i bytes of temporary allocated memory.", size);
	// clean up the buffer
	//	if (outData)
	//	{
	//		free(outData);
	//		outData = NULL;
	//	}
	
	//END_TIMER(@"Audiofile successfully loaded.");
	return true;
	
}

-(bool) loadSoundWithKey:(NSString *)_soundKey File:(NSString *)_file Ext:(NSString *) _ext Loop:(bool)loops{
	
	//START_TIMER;
	
	NSNumber * numVal = [soundDictionary objectForKey:_soundKey];//looks to see if that sound key has already been loaded.
	if (numVal != nil)
	{
		//	NSLog(@"That sound key was already loaded once, I won't allow you to do it again. It seems like a waste of cpu.");
		//	return true;
	}//if it has, it just returns true so it does not reload it.
	
	ALvoid * outData;
	ALenum  error = AL_NO_ERROR;
	ALenum  format;
	ALsizei size;
	ALsizei freq;
	
	NSBundle * bundle = [NSBundle mainBundle];
	
	
	// get some audio data from a wave file
	CFURLRef fileURL = (CFURLRef)[[NSURL fileURLWithPath:[bundle pathForResource:_file ofType:_ext]] retain];
	
	if (!fileURL)
	{
		//END_TIMER(@"file not found.");
		return false;
	}
	
	outData = MyGetOpenALAudioData(fileURL, &size, &format, &freq);
	
	CFRelease(fileURL);
	
	if((error = alGetError()) != AL_NO_ERROR) {
		printf("error loading sound: %x\n", error);
		exit(1);
	}
	
	//	ALog(@"getting a free buffer from openAL.");
	NSUInteger bufferID;
	// grab a buffer ID from openAL
	alGenBuffers(1, &bufferID);
	
	//	ALog(@"loading audio data into openAL buffer.");
	// load the awaiting data blob into the openAL buffer.
	alBufferData(bufferID,format,outData,size,freq); 
	
	// save the buffer so we can release it later
	[bufferStorageArray addObject:[NSNumber numberWithUnsignedInteger:bufferID]];
	
	//	ALog(@"getting a free source from openAL.");
	NSUInteger sourceID;
	// grab a source ID from openAL
	alGenSources(1, &sourceID); 
	
	//	ALog(@"attatching the buffer to the source and setting up preferences");
	// attach the buffer to the source
	alSourcei(sourceID, AL_BUFFER, bufferID);
	// set some basic source prefs
	alSourcef(sourceID, AL_PITCH, 1.0f);
	alSourcef(sourceID, AL_GAIN, 1.0f);
	if (loops) alSourcei(sourceID, AL_LOOPING, AL_TRUE);
	
	// store this for future use
	[soundDictionary setObject:[NSNumber numberWithUnsignedInt:sourceID] forKey:_soundKey];	
	//	NSLog(@"here is the contents of the sound dictionary: %@",soundDictionary);
	//	ALog(@"free %i bytes of temporary allocated memory.", size);
	// clean up the buffer
	if (outData)
	{
		free(outData);
		outData = NULL;
	}
	
	//END_TIMER(@"Audiofile successfully loaded.");
	return true;
	
		
}

-(void)setListenerVolume :(float) _volume
{		
	alListenerf(AL_GAIN, _volume);
}
@end
