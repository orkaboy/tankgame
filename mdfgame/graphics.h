#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>

#ifdef _WIN32
#include "SDL.h"
#include "SDL_ttf.h"
#else
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"
#endif

#include "world.h"
#include "button.h"
#include "menu.h"

void Graphics_BeginScene();
void Graphics_EndScene();

void Graphics_DrawScene(World &world);
void Graphics_ApplySurface(SDL_Surface *source, SDL_Surface *destination, int x, int y, float scaling = 1, float angle = 0);
bool Graphics_Init();
/* funk for load fonts */
TTF_Font * Graphics_LoadFont( const string &s, int fontsize );

SDL_Surface* Graphics_GetSurface(std::string& id);
void Graphics_DrawString(std::string str, int x, int y);
/* set tank color */
void Planet_SetImage(Planet *planet, string id);
void Tank_SetImages(Tank *tank, TankColors col);
void LoadFont(void);
void Menu_Draw(Menu* menu);
SDL_Surface * getImage(string id);
TTF_Font * getFont(string id);

SDL_Surface* ReturnScreen();
SDL_Surface* ReturnBg();
SDL_Surface* ReturnCursor();

#endif

