#include "effects.h"

#include "graphics.h"

Effect * Effect_Create(EffectType type, vec2 pos)
{
	Effect *newEffect = new Effect;
	
	/* Default Init values */
	newEffect->scale = 1;
	newEffect->anim = NULL;
	newEffect->rot = 0;
	newEffect->lifetime = 0;
	newEffect->type = type;
	newEffect->pos = pos;
	newEffect->curFrame = 0;
	newEffect->frameTimer = 0;
	
	/* Do special init based on type */
	switch(type)
	{
		case FX_EXPLOSION:
			break;
		case FX_CRUSHED:
			newEffect->lifetime = 1;
			newEffect->scale = 0.5;
			newEffect->anim = GetAnimation("Crushed");
			break;
		default:
			break;
	}
	
	return newEffect;
}

void Effect_Delete(Effect *e)
{
	delete e;
}

void Effect_Update(vector<Effect*> &effects, float dt)
{
	for(unsigned int i = 0; i < effects.size(); i++)
	{
		Effect *e = effects[i];
		
		e->lifetime -= dt;
		if(e->lifetime <= 0)
		{
			Effect_Delete(e);
			effects.erase(effects.begin() + i);
			i--;
		}
		else
		{
			UpdateAnimation(e->anim, e->curFrame, e->frameTimer, dt);
			
			switch(e->type)
			{
				case FX_EXPLOSION:
					break;
				case FX_CRUSHED:
					e->scale *= 1.05;
					break;
				default: break;
			}
		}
	}
}

void Effect_Draw(vector<Effect*> &effects, vec2 offset)
{
	for(unsigned int i = 0; i < effects.size(); i++)
	{
		const Effect *e = effects[i];
		
        SDL_Texture *img = GetFrame(e->anim, e->curFrame);
		
		if(img != NULL)
            Graphics_ApplySurface(img, e->pos.x - offset.x, e->pos.y - offset.y, e->scale, e->rot);
	}
}
