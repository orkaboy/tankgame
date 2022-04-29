#pragma once

#include "SDL_mixer.h"

namespace MDF {
namespace Audio {

bool Init();
Mix_Chunk* LoadWav(const char *file);
bool PlayWav(Mix_Chunk *sound);
bool LoadPresetWavFiles();

void ShutDown();

void PlayCanonFire(); // AUDIO_CANONFIRE
void PlayEngineSound(); // AUDIO_ENGINESOUND
void PlayExplosionSmall(); // AUDIO_EXPLOSIONSMALL
void PlayExplosionBig(); // AUDIO_EXPLOSIONBIG


bool IsPlayingEngineSound();

Mix_Music* LoadMp3File(const char *file);
bool LoadPresetMp3Files();

void PlayMusic(int musicIndex = 0, int playCount = -1);
void StopMusic();

}
}

// WARNING
// Very unsafe, be sure to have AUDIO_BUFFERSIZE last
// Do or die...
enum
{
	AUDIO_CANONFIRE = 0,
	AUDIO_ENGINESOUND,
	AUDIO_EXPLOSIONSMALL,
	AUDIO_EXPLOSIONBIG,
	
	// Do NOT put anything after this, or die...
	AUDIO_BUFFERSIZE
};

// WARNING
// Very unsafe, be sure to have MUSIC_BUFFERSIZE last
// Do or die...
enum
{
	MUSIC_MUSIC_1 = 0,
	
	// Do NOT put anything after this, or die...
	MUSIC_BUFFERSIZE
};
