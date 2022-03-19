#include "animation.h"

#include "resources.h"
#include "graphics.h"
#include <fstream>

#include <fmt/core.h>

namespace MDF {

static std::map<std::string, Animation*>	animations;

Animation * Animation::LoadAnimation(const char *filepath)
{
	std::ifstream file;
	file.open(filepath, std::ios::in);
	
	if(!file)
	{
		fmt::print("Couldn't load animation {}\n", filepath);
		return NULL;
	}
	
	Animation *anim = new Animation;
	
	file >> anim->numFrames;
	
    anim->frames = new SDL_Texture*[anim->numFrames];
	anim->frameTimer = new float[anim->numFrames];
	
	for(unsigned int i = 0; i < anim->numFrames; i++)
	{
		std::string imgPath;
		file >> imgPath;
		anim->frames[i] = Graphics::getImage(imgPath);
		file >> anim->frameTimer[i];
	}
	
	return anim;
}

void Animation::LoadAnimations(void)
{
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::ANIMATION);
	
	for ( MDF::Resource::ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		animations[it->first] = LoadAnimation(it->second.c_str());
}

void Animation::UnloadAnimations(void)
{
	for(auto anim_pair : animations)
	{
		auto anim = anim_pair.second;
		if(anim)
		{
			delete[] anim->frameTimer;
			delete[] anim->frames;
			delete[] anim;
		}
	}
}

Animation * Animation::GetAnimation(std::string id)
{
	return animations[id];
}

SDL_Texture * Animation::GetFrame(unsigned int frame)
{
    SDL_Texture *ret = NULL;

	if(frame < numFrames)
		ret = frames[frame];
	
	return ret;
}

void Animation::UpdateAnimation(unsigned int &frame, float &timer, float dt)
{
	if(numFrames == 0) return;
	
	if(frame >= numFrames)
		frame = 0;
	timer += dt;
	if(timer >= frameTimer[frame])
	{
		timer -= frameTimer[frame];
		frame++;
		
		if(frame >= numFrames)
			frame = 0;
	}
}

}
