#ifndef BUTTON_H
#define BUTTON_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <string>

#include "menuoptions.h"

typedef struct Button
{
	SDL_Rect pos;
	std::string text;
	MenuOption ret;
	int over;
	
	TTF_Font *font;
} Button;

Button* Button_Init(int x, int y, MenuOption ret, std::string txt, TTF_Font *font);

void Button_Draw(SDL_Renderer *screen, Button *button);

void Menu_GetMouse(int &x, int &y);

#endif
