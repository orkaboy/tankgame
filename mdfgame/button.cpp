#include "button.h"

#include "SDL2/SDL2_gfxPrimitives.h"

#include "graphics.h"

namespace MDF {

Button::Button(int x, int y, MenuOption ret, std::string txt, TTF_Font *font)
{
	mRet = ret;
	mPos.x = x;
	mPos.y = y;
	mFont = font;
	mText = txt;
	mOver = 0;
	
	SDL_Color c = {255, 255, 255};
	SDL_Surface *text = TTF_RenderText_Solid( mFont, mText.c_str(), c);
	
	mPos.w = text->w + 10;
	mPos.h = text->h + 4;
	
	SDL_FreeSurface(text);
}

void Button::Draw(SDL_Renderer *screen) const
{
	if(!mOver)
	{
		rectangleRGBA( screen, mPos.x, mPos.y, mPos.x+mPos.w, mPos.y+mPos.h, 0, 255, 0, 255 );
	}
	else
	{
		boxRGBA( screen, mPos.x, mPos.y, mPos.x+mPos.w, mPos.y+mPos.h, 0, 255, 0, 255 );
	}
	
	SDL_Color c = {255, 255, 255};
    SDL_Surface *txt = TTF_RenderText_Solid( mFont, mText.c_str(), c);
    SDL_Texture *texture = SDL_CreateTextureFromSurface( screen, txt );

    Graphics_ApplySurface(texture, (mPos.x) + (mPos.w / 2), mPos.y + (mPos.h / 2));
    SDL_FreeSurface(txt);
    SDL_DestroyTexture(texture);
}

}
