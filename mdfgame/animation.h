#ifndef ANIMATION_H
#define ANIMATION_H

#ifdef _WIN32
#include "SDL.h"
#else
#include "SDL/SDL.h"
#endif

#include <string>
using namespace std;

typedef struct Animation
{
	SDL_Surface **frames;
	float *frameTimer;
	unsigned int numFrames;
} Animation;

void LoadAnimations(void);
void UnloadAnimations(void);

/* Returns a previously loaded animation */
Animation * GetAnimation(string id);

/* Return the surface at frame, if there is one */
SDL_Surface * GetFrame(Animation *anim, unsigned int frame);

/* Updates frame and timer */
void UpdateAnimation(Animation *anim, unsigned int &frame, float &timer, float dt);

#endif
