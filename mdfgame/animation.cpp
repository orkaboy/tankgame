#include "animation.h"

#include "resources.h"
#include "graphics.h"
#include <fstream>

#include <fmt/core.h>

static std::map<std::string, Animation*>	animations;

Animation * LoadAnimation(const char *filepath)
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
		anim->frames[i] = getImage(imgPath);
		file >> anim->frameTimer[i];
	}
	
	return anim;
}

void LoadAnimations(void)
{
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::ANIMATION);
	
	for ( MDF::Resource::ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		animations[it->first] = LoadAnimation(it->second.c_str());
}

void UnloadAnimations(void)
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

Animation * GetAnimation(std::string id)
{
	return animations[id];
}

SDL_Texture * GetFrame(Animation *anim, unsigned int frame)
{
	if(anim == NULL) return NULL;
    SDL_Texture *ret = NULL;
	
	if(frame < anim->numFrames)
		ret = anim->frames[frame];
	
	return ret;
}

void UpdateAnimation(Animation *anim, unsigned int &frame, float &timer, float dt)
{
	if(anim == NULL) return;
	if(anim->numFrames == 0) return;
	
	if(frame >= anim->numFrames)
		frame = 0;
	timer += dt;
	if(timer >= anim->frameTimer[frame])
	{
		timer -= anim->frameTimer[frame];
		frame++;
		
		if(frame >= anim->numFrames)
			frame = 0;
	}
}
