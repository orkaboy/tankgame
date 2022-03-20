#include "animation.h"

#include "resources.h"
#include "graphics.h"
#include <fstream>

#include <fmt/core.h>

namespace MDF {

static std::map<std::string, Animation*>	animations;

Animation * Animation::LoadAnimation(const char *filepath) {
	std::ifstream file;
	file.open(filepath, std::ios::in);
	
	if(!file) {
		fmt::print("Couldn't load animation {}\n", filepath);
		return NULL;
	}
	
	Animation *anim = new Animation;
	
	int numFrames;
	file >> numFrames;
	
	for(unsigned int i = 0; i < numFrames; i++) {
		std::string imgPath;
		float timer;
		file >> imgPath >> timer;
		anim->frames.emplace_back(Graphics::getImage(imgPath));
		anim->frameTimer.emplace_back(timer);
	}
	
	return anim;
}

void Animation::LoadAnimations(void) {
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::ANIMATION);
	
	for (auto it : res)
		animations[it.first] = LoadAnimation(it.second.c_str());
}

void Animation::UnloadAnimations(void) {
	for(auto anim_pair : animations) {
		auto anim = anim_pair.second;
		if(anim) {
			delete[] anim;
		}
	}
}

Animation * Animation::GetAnimation(std::string id) {
	return animations[id];
}

SDL_Texture * Animation::GetFrame(unsigned int frame) {
    SDL_Texture *ret = NULL;

	if(frame < frames.size())
		ret = frames[frame];
	
	return ret;
}

void Animation::UpdateAnimation(unsigned int &frame, float &timer, float dt) {
	if(frames.empty()) return;
	
	if(frame >= frames.size())
		frame = 0;
	timer += dt;
	if(timer >= frameTimer[frame]) {
		timer -= frameTimer[frame];
		frame++;
		frame = frame % frames.size();
	}
}

}
