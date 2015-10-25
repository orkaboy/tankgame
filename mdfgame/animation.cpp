#include "animation.h"

#include "resources.h"
#include "graphics.h"
#include <fstream>
#include <iostream>
using namespace std;

static map<string, Animation*>	animations;

Animation * LoadAnimation(const char *filepath)
{
	ifstream file;
	file.open(filepath, ios::in);
	
	if(!file)
	{
		cout << "Couldn't load animation " << filepath << endl;
		return NULL;
	}
	
	Animation *anim = new Animation;
	
	file >> anim->numFrames;
	
	anim->frames = new SDL_Surface*[anim->numFrames];
	anim->frameTimer = new float[anim->numFrames];
	
	for(unsigned int i = 0; i < anim->numFrames; i++)
	{
		string imgPath;
		file >> imgPath;
		anim->frames[i] = getImage(imgPath);
		file >> anim->frameTimer[i];
	}
	
	return anim;
}

void LoadAnimations(void)
{
	ResourceMap res = Resources_GetOfType(RT_ANIMATION);
	
	for ( ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		animations[it->first] = LoadAnimation(it->second.c_str());
}

void UnloadAnimations(void)
{
	for(map<string, Animation*>::iterator it = animations.begin(); it != animations.end(); it++)
	{
		Animation *anim = it->second;
		if(anim)
		{
			delete[] anim->frameTimer;
			delete[] anim->frames;
			delete[] anim;
		}
	}
}

Animation * GetAnimation(string id)
{
	return animations[id];
}

SDL_Surface * GetFrame(Animation *anim, unsigned int frame)
{
	if(anim == NULL) return NULL;
	SDL_Surface *ret = NULL;
	
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
