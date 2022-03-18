#pragma once

#include "animation.h"
#include "utility.h"

#include <vector>

namespace MDF {

enum class EffectType
{
	EXPLOSION,
	CRUSHED,
};

class Effect
{
public:
	/* Functions */
	Effect(EffectType type, vec2 pos);

	static void Update(std::vector<Effect*> &effects, float dt);
	static void Draw(std::vector<Effect*> &effects, vec2 offset);
private:
	EffectType type;
	
	Animation *anim;
	unsigned int curFrame;
	float frameTimer;
	
	float lifetime;
	float scale;
	float rot;
	vec2 pos;
};

}
