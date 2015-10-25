#ifndef EFFECTS_H
#define EFFECTS_H

#include "animation.h"
#include "utility.h"

#include <vector>
using namespace std;

enum EffectType
{
	FX_EXPLOSION,
	FX_CRUSHED,
};

typedef struct Effect
{
	EffectType type;
	
	Animation *anim;
	unsigned int curFrame;
	float frameTimer;
	
	float lifetime;
	float scale;
	float rot;
	vec2 pos;
} Effect;

/* Functions */
Effect * Effect_Create(EffectType type, vec2 pos);
void Effect_Delete(Effect *e);
void Effect_Update(vector<Effect*> &effects, float dt);
void Effect_Draw(vector<Effect*> &effects, SDL_Surface *screen, vec2 offset);

#endif
