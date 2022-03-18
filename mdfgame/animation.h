#pragma once

#include "SDL2/SDL.h"

#include <string>

typedef struct Animation
{
    SDL_Texture **frames;
	float *frameTimer;
	unsigned int numFrames;
} Animation;

void LoadAnimations(void);
void UnloadAnimations(void);

/* Returns a previously loaded animation */
Animation * GetAnimation(std::string id);

/* Return the surface at frame, if there is one */
SDL_Texture * GetFrame(Animation *anim, unsigned int frame);

/* Updates frame and timer */
void UpdateAnimation(Animation *anim, unsigned int &frame, float &timer, float dt);
