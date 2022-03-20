#pragma once

#include "SDL2/SDL.h"

#include <string>
#include <vector>

namespace MDF {

class Animation
{
public:
	static void LoadAnimations(void);
	static void UnloadAnimations(void);

	/* Returns a previously loaded animation */
	static auto GetAnimation(std::string id) -> Animation*;

	/* Return the surface at frame, if there is one */
	SDL_Texture * GetFrame(unsigned int frame);

	/* Updates frame and timer */
	void UpdateAnimation(unsigned int &frame, float &timer, float dt);

private:
	static Animation* LoadAnimation(const char *filepath);

	std::vector<SDL_Texture*> frames;
	std::vector<float> frameTimer;
};

}
