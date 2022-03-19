#include "audio.h"
#include "resources.h"

#include <array>

#include <iostream>
#include <fmt/ostream.h>

namespace MDF {
namespace Audio {

std::array<Mix_Chunk*, AUDIO_BUFFERSIZE> soundBuffer;
std::array<Mix_Music*, MUSIC_BUFFERSIZE> musicBuffer;

bool Init()
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

Mix_Chunk* LoadWav(const char *file)
{
	Mix_Chunk *sound = Mix_LoadWAV(file);
	if(sound == NULL)
		fmt::print(std::cerr, "Unable to load WAV file: {}\n\t{}\n\tThey are case sensitive!\n", file, Mix_GetError());

	return sound;
}

Mix_Music* LoadMp3File(const char *file)
{
	Mix_Music *music = Mix_LoadMUS(file);
	if(music == NULL)
		fmt::print(std::cerr, "Unable to load MP3 file: {}\n\t{}\n\tThey are case sensitive!\n", file, Mix_GetError());

	return music;
}

bool IsPlayingEngineSound()
{
	return Mix_Playing(0) != 0 ? true : false;
}

bool LoadPresetWavFiles()
{
	soundBuffer[AUDIO_CANONFIRE] = LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "CannonFire").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_CANONFIRE], 30); // volum between 0 and 128
	soundBuffer[AUDIO_ENGINESOUND] = LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "Engine").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_ENGINESOUND], 30);
	soundBuffer[AUDIO_EXPLOSIONSMALL] = LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "ExplosionSmall").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_EXPLOSIONSMALL], 30);
	soundBuffer[AUDIO_EXPLOSIONBIG] = LoadWav(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "ExplosionBig").c_str());
	Mix_VolumeChunk(soundBuffer[AUDIO_EXPLOSIONBIG], 30);
	
	// this reserves channel 0 for engine sound..
	Mix_ReserveChannels(1);
	
	return true;
}

bool LoadPresetMp3Files()
{
	musicBuffer[0] = LoadMp3File(MDF::Resource::GetFileName(MDF::Resource::Type::SOUND, "GravitankMusic").c_str());
	return true;
}

bool PlayWav(Mix_Chunk *sound)
{
	int channel = Mix_PlayChannel(-1, sound, 0);
	if(channel == -1) 
		fmt::print(std::cerr, "Unable to play WAV file: {}\n", Mix_GetError());

	// if channel == -1, return false, else return true
	return channel == -1 ? false : true;
}

bool PlayWavOnSpecificChannel(Mix_Chunk *sound, int channel)
{
	channel = Mix_PlayChannel(channel, sound, 0);
	if(channel == -1) 
		fmt::print(std::cerr, "Unable to play WAV file: {}\n", Mix_GetError());

	// if channel == -1, return false, else return true
	return channel == -1 ? false : true;
}

void PlayMusic(int musicIndex, int playCount)
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

void StopMusic()
{
	Mix_HaltMusic();
}

void PlayCanonFire()
{
	PlayWav(soundBuffer[AUDIO_CANONFIRE]);
}

void PlayEngineSound()
{
	PlayWavOnSpecificChannel(soundBuffer[AUDIO_ENGINESOUND], 0);
}

void PlayExplosionSmall()
{
	PlayWav(soundBuffer[AUDIO_EXPLOSIONSMALL]);
}

void PlayExplosionBig()
{
	PlayWav(soundBuffer[AUDIO_EXPLOSIONBIG]);
}

void ShutDown()
{
	for(auto sound : soundBuffer)
		Mix_FreeChunk(sound); 
	
	for(auto music : musicBuffer)
		Mix_FreeMusic(music);
}

}
}
