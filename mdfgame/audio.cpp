#include "audio.h"
#include "resources.h"

#include <array>

#include <iostream>
#include <fmt/ostream.h>

std::array<Mix_Chunk*, AUDIO_BUFFERSIZE> soundBuffer;
std::array<Mix_Music*, MUSIC_BUFFERSIZE> musicBuffer;

bool Audio_Init()
{
	int audio_rate = 22050;
	Uint16 audio_format = AUDIO_S16SYS;
	int audio_channels = 2;
//	int audio_buffers = 4096;
	int audio_buffers = 512;

	if(Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) != 0)
	{
		fmt::print(std::cerr, "Unable to initialize audio: {}\n", Mix_GetError());
		return false;
	}
	
	Mix_AllocateChannels(300); 
	return true;
}

Mix_Chunk* Audio_LoadWav(const char *file)
{
	Mix_Chunk *sound = Mix_LoadWAV(file);
	if(sound == NULL)
		fmt::print(std::cerr, "Unable to load WAV file: {}\n\t{}\n\tThey are case sensitive!\n", file, Mix_GetError());

	return sound;
}

Mix_Music* Audio_LoadMp3File(const char *file)
{
	Mix_Music *music = Mix_LoadMUS(file);
	if(music == NULL)
		fmt::print(std::cerr, "Unable to load MP3 file: {}\n\t{}\n\tThey are case sensitive!\n", file, Mix_GetError());

	return music;
}

bool Audio_IsPlayingEngineSound()
{
	return Mix_Playing(0) != 0 ? true : false;
}

bool Audio_LoadPresetWavFiles()
{
	soundBuffer[AUDIO_CANONFIRE] = Audio_LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "CannonFire").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_CANONFIRE], 30); // volum between 0 and 128
	soundBuffer[AUDIO_ENGINESOUND] = Audio_LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "Engine").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_ENGINESOUND], 30);
	soundBuffer[AUDIO_EXPLOSIONSMALL] = Audio_LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "ExplosionSmall").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_EXPLOSIONSMALL], 30);
	soundBuffer[AUDIO_EXPLOSIONBIG] = Audio_LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "ExplosionBig").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_EXPLOSIONBIG], 30);
	
	// this reserves channel 0 for engine sound..
	Mix_ReserveChannels(1);
	
	return true;
}

bool Audio_LoadPresetMp3Files()
{
	musicBuffer[0] = Audio_LoadMp3File(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "GravitankMusic").c_str());
	return true;
}

bool Audio_PlayWav(Mix_Chunk *sound)
{
	int channel = Mix_PlayChannel(-1, sound, 0);
	if(channel == -1) 
		fmt::print(std::cerr, "Unable to play WAV file: {}\n", Mix_GetError());

	// if channel == -1, return false, else return true
	return channel == -1 ? false : true;
}

bool Audio_PlayWavOnSpecificChannel(Mix_Chunk *sound, int channel)
{
	channel = Mix_PlayChannel(channel, sound, 0);
	if(channel == -1) 
		fmt::print(std::cerr, "Unable to play WAV file: {}\n", Mix_GetError());

	// if channel == -1, return false, else return true
	return channel == -1 ? false : true;
}

void Audio_PlayMusic(int musicIndex, int playCount)
{
	if(musicIndex > MUSIC_BUFFERSIZE)
	{
		fmt::print(std::cerr, "Trying to play music outside of music buffer, aborting play attempt 0\n");
		return;
	}
	Mix_VolumeMusic(125); 
	if(Mix_PlayMusic(musicBuffer[musicIndex], playCount) == -1)
		fmt::print("Mix_PlayMusic: {}\n", Mix_GetError());
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
	for(auto sound : soundBuffer)
		Mix_FreeChunk(sound); 
	
	for(auto music : musicBuffer)
		Mix_FreeMusic(music);
}
