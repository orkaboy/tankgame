#include "button.h"

#include "SDL2/SDL2_gfxPrimitives.h"

#include "graphics.h"

Button* Button_Init(int x, int y, MenuOption ret, string txt, TTF_Font *font)
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

void Button_Draw(SDL_Renderer *screen, Button *button)
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
    SDL_Texture *texture = SDL_CreateTextureFromSurface( screen, txt );

    Graphics_ApplySurface(texture, (button->pos.x) + (button->pos.w / 2), button->pos.y + (button->pos.h / 2));
    SDL_FreeSurface(txt);
    SDL_DestroyTexture(texture);
}
