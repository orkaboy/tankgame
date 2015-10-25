#ifndef MENU_H
#define MENU_H

#include "SDL2/SDL.h"

#include <vector>
#include "button.h"

typedef struct Menu
{
    SDL_Texture *bg;
    SDL_Texture *cursor;
	vector <Button*> list;
}Menu;

Menu* Menu_Init(SDL_Texture* bg, SDL_Texture* cursor);

void Menu_AddButton(Menu* menu,int x, int y,int ret, string text, TTF_Font *font);

int Menu_CheckButton(Menu* menu, int mx, int my);

#endif
