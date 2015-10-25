#ifndef OURGAME_SDL_AUDIO
#define OURGAME_SDL_AUDIO

#ifdef _WIN32
#include "SDL_mixer.h"
#else
#include "SDL/SDL_mixer.h"
#endif

bool Audio_Init();
Mix_Chunk* Audio_LoadWav(const char *file);
bool Audio_PlayWav(Mix_Chunk *sound);
bool Audio_LoadPresetWavFiles();

void Audio_ShutDown();

void Audio_PlayCanonFire(); // AUDIO_CANONFIRE
void Audio_PlayEngineSound(); // AUDIO_ENGINESOUND
void Audio_PlayExplosionSmall(); // AUDIO_EXPLOSIONSMALL
void Audio_PlayExplosionBig(); // AUDIO_EXPLOSIONBIG


bool Audio_IsPlayingEngineSound();

Mix_Music* Audio_LoadMp3File(const char *file);
bool Audio_LoadPresetMp3Files();

void Audio_PlayMusic(int musicIndex = 0, int playCount = -1);
void Audio_StopMusic();


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

#endif
