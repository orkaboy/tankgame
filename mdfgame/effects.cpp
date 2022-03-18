#include "effects.h"

#include "graphics.h"

namespace MDF {

Effect::Effect(EffectType type, vec2 _pos)
{
	/* Default Init values */
	scale = 1;
	anim = NULL;
	rot = 0;
	lifetime = 0;
	type = type;
	pos = _pos;
	curFrame = 0;
	frameTimer = 0;
	
	/* Do special init based on type */
	switch(type)
	{
		case EffectType::EXPLOSION:
			break;
		case EffectType::CRUSHED:
			lifetime = 1;
			scale = 0.5;
			anim = GetAnimation("Crushed");
			break;
		default:
			break;
	}
}

void Effect::Update(std::vector<Effect*> &effects, float dt)
{
	for(unsigned int i = 0; i < effects.size(); i++)
	{
		Effect *e = effects[i];
		
		e->lifetime -= dt;
		if(e->lifetime <= 0)
		{
			delete e;
			effects.erase(effects.begin() + i);
			i--;
		}
		else
		{
			UpdateAnimation(e->anim, e->curFrame, e->frameTimer, dt);
			
			switch(e->type)
			{
				case EffectType::EXPLOSION:
					break;
				case EffectType::CRUSHED:
					e->scale *= 1.05;
					break;
				default: break;
			}
		}
	}
}

void Effect::Draw(std::vector<Effect*> &effects, vec2 offset)
{
	for(auto e : effects)
	{
        SDL_Texture *img = GetFrame(e->anim, e->curFrame);
		
		if(img != NULL)
            Graphics_ApplySurface(img, e->pos.x - offset.x, e->pos.y - offset.y, e->scale, e->rot);
	}
}

}
