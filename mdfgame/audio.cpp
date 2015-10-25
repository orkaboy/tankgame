#include "audio.h"
#include "resources.h"

Mix_Chunk *soundBuffer[AUDIO_BUFFERSIZE];
Mix_Music *musicBuffer[MUSIC_BUFFERSIZE];

bool Audio_Init()
{
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
//	int audio_buffers = 4096;
	int audio_buffers = 512;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		fprintf(stderr, "Unable to initialize audio: %s\n", Mix_GetError());
		return false;
	}
	
	Mix_AllocateChannels(300); 
	return true;
}

Mix_Chunk* Audio_LoadWav(const char *file)
{
	Mix_Chunk *sound = NULL;
	sound = Mix_LoadWAV(file);
	if(sound == NULL)
		fprintf(stderr, "Unable to load WAV file: %s\n\t%s\n\tThey are case sensitive!\n", file, Mix_GetError());

	return sound;
}

Mix_Music* Audio_LoadMp3File(const char *file)
{
	Mix_Music *music = NULL;
	music = Mix_LoadMUS(file);
	
	if(music == NULL)
		fprintf(stderr, "Unable to load MP3 file: %s\n\t%s\n\tThey are case sensitive!\n", file, Mix_GetError());

	return music;
}

bool Audio_IsPlayingEngineSound()
{
	return Mix_Playing(0) != 0 ? true : false;
}

bool Audio_LoadPresetWavFiles()
{
	soundBuffer[AUDIO_CANONFIRE] = Audio_LoadWav(Resources_GetFileName(RT_SOUND, "CannonFire").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_CANONFIRE], 30); // volum between 0 and 128
	soundBuffer[AUDIO_ENGINESOUND] = Audio_LoadWav(Resources_GetFileName(RT_SOUND, "Engine").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_ENGINESOUND], 30);
	soundBuffer[AUDIO_EXPLOSIONSMALL] = Audio_LoadWav(Resources_GetFileName(RT_SOUND, "ExplosionSmall").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_EXPLOSIONSMALL], 30);
	soundBuffer[AUDIO_EXPLOSIONBIG] = Audio_LoadWav(Resources_GetFileName(RT_SOUND, "ExplosionBig").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_EXPLOSIONBIG], 30);
	
	// this reserves channel 0 for engine sound..
	Mix_ReserveChannels(1);
	
	return true;
}

bool Audio_LoadPresetMp3Files()
{
	musicBuffer[0] = Audio_LoadMp3File(Resources_GetFileName(RT_SOUND, "GravitankMusic").c_str());
	return true;
}

bool Audio_PlayWav(Mix_Chunk *sound)
{
	int channel;
	channel = Mix_PlayChannel(-1, sound, 0);
	if(channel == -1) 
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());

	// if channel == -1, return false, else return true
	return channel == -1 ? false : true;
}

bool Audio_PlayWavOnSpecificChannel(Mix_Chunk *sound, int channel)
{
	channel = Mix_PlayChannel(channel, sound, 0);
	if(channel == -1) 
		fprintf(stderr, "Unable to play WAV file: %s\n", Mix_GetError());

	// if channel == -1, return false, else return true
	return channel == -1 ? false : true;
}

void Audio_PlayMusic(int musicIndex, int playCount)
{
	if(musicIndex > MUSIC_BUFFERSIZE)
	{
		fprintf(stderr, "Trying to play music outside of music buffer, aborting play attempt 0\n");
		return;
	}
	Mix_VolumeMusic(125); 
	if(Mix_PlayMusic(musicBuffer[musicIndex], playCount) == -1)
		 printf("Mix_PlayMusic: %s\n", Mix_GetError());
}

void Audio_StopMusic()
{
	Mix_HaltMusic();
}

void Audio_PlayCanonFire()
{
	Audio_PlayWav(soundBuffer[AUDIO_CANONFIRE]);
}

void Audio_PlayEngineSound()
{
	Audio_PlayWavOnSpecificChannel(soundBuffer[AUDIO_ENGINESOUND], 0);
}

void Audio_PlayExplosionSmall()
{
	Audio_PlayWav(soundBuffer[AUDIO_EXPLOSIONSMALL]);
}

void Audio_PlayExplosionBig()
{
	Audio_PlayWav(soundBuffer[AUDIO_EXPLOSIONBIG]);
}

void Audio_ShutDown()
{
	int i;
	for(i = 0; i < AUDIO_BUFFERSIZE; i++)
		Mix_FreeChunk(soundBuffer[i]); 
	
	for(i = 0; i < MUSIC_BUFFERSIZE; i++)
		Mix_FreeMusic(musicBuffer[i]);
}
