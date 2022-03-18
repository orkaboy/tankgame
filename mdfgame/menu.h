#pragma once

#include "SDL2/SDL.h"

#include <vector>
#include "button.h"
#include "menuoptions.h"

typedef struct Menu
{
    SDL_Texture *bg;
    SDL_Texture *cursor;
	std::vector <MDF::Button*> list;
} Menu;

Menu* Menu_Init(SDL_Texture* bg, SDL_Texture* cursor);

void Menu_AddButton(Menu* menu,int x, int y, MenuOption ret, std::string text, TTF_Font *font);

MenuOption Menu_CheckButton(Menu* menu, int mx, int my);

MenuOption TheMenu ( bool* quit );
