#ifndef BUTTON_H
#define BUTTON_H

#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

#include <string>

using namespace std;

typedef struct Button
{
	SDL_Rect pos;
	string text;
	int ret;
	int over;
	
	TTF_Font *font;
} Button;

Button* Button_Init(int x, int y, int ret, string txt, TTF_Font *font);

void Button_Draw(SDL_Renderer *screen, Button *button);

void Menu_GetMouse(int &x, int &y);

#endif
