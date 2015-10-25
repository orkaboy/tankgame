#ifndef BUTTON_H
#define BUTTON_H

#ifdef _WIN32
#include "SDL.h"
#include "SDL_ttf.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#endif

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

void Button_Draw(SDL_Surface *screen, Button *button);

void Menu_GetMouse(int &x, int &y);

#endif
