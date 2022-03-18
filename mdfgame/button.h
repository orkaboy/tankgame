#pragma once

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <string>

#include "menuoptions.h"

namespace MDF {

class Button
{
public:
	Button(int x, int y, MenuOption ret, std::string txt, TTF_Font *font);

	void Draw(SDL_Renderer *screen) const;

	SDL_Rect Pos() const { return mPos; }
	MenuOption Ret() const { return mRet; }
	int Over() const { return mOver; }
	void SetOver(int over) { mOver = over; }

private:
	SDL_Rect mPos;
	std::string mText;
	MenuOption mRet;
	int mOver;
	
	TTF_Font *mFont;
};

}
