#pragma once

#include <string>

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_ttf.h"

#include "world.h"
#include "button.h"
#include "menu.h"

void Graphics_BeginScene();
void Graphics_EndScene();

void Graphics_DrawScene(World &world);
void Graphics_ApplySurface(SDL_Texture *source, int x, int y, float scaling = 1, float angle = 0);
bool Graphics_Init();
/* funk for load fonts */
TTF_Font * Graphics_LoadFont( const std::string &s, int fontsize );

void Graphics_DrawString(std::string str, int x, int y);
/* set tank color */
void Planet_SetImage(Planet *planet, std::string id);
void Tank_SetImages(MDF::Tank *tank, MDF::TankColors col);
void LoadFont(void);
void Menu_Draw(MDF::Menu& menu);
SDL_Texture * getImage(std::string id);
TTF_Font * getFont(std::string id);

SDL_Renderer* ReturnScreen();
SDL_Texture* ReturnBg();
SDL_Texture* ReturnCursor();
