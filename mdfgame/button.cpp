#include "button.h"

#ifdef _WIN32
#include "SDL_gfxPrimitives.h"
#else
#include "SDL/SDL_gfxPrimitives.h"
#endif

#include "graphics.h"

Button* Button_Init(int x, int y, int ret, string txt, TTF_Font *font)
{
	Button *button = new Button;
	
	button->ret = ret;
	button->pos.x = x;
	button->pos.y = y;
	button->font = font;
	button->text = txt;
	button->over = 0;
	
	SDL_Color c = {255, 255, 255};
	SDL_Surface *text = TTF_RenderText_Solid( button->font, button->text.c_str(), c);
	
	button->pos.w = text->w + 10;
	button->pos.h = text->h + 4;
	
	SDL_FreeSurface(text);
		
	return button;
}

void Button_Draw(SDL_Surface *screen, Button *button)
{
	if(!button->over)
	{
		rectangleRGBA( screen, button->pos.x, button->pos.y, button->pos.x+button->pos.w, button->pos.y+button->pos.h, 0, 255, 0, 255 );
	}
	else
	{
		boxRGBA( screen, button->pos.x, button->pos.y, button->pos.x+button->pos.w, button->pos.y+button->pos.h, 0, 255, 0, 255 );
	}
	
	SDL_Color c = {255, 255, 255};
	SDL_Surface *txt = TTF_RenderText_Solid( button->font, button->text.c_str(), c);
	
	Graphics_ApplySurface(txt, screen, (button->pos.x) + (button->pos.w / 2), button->pos.y + (button->pos.h / 2));
	SDL_FreeSurface(txt);
}
