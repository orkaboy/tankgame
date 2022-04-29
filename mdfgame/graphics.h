#pragma once

#include <string>

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

#include "world.h"
#include "button.h"
#include "menu.h"

namespace MDF {
namespace Graphics {

void BeginScene();
void EndScene();

void DrawScene(World* world);
void ApplySurface(SDL_Texture *source, int x, int y, float scaling = 1, float angle = 0);
bool Init();
/* func for loading fonts */
TTF_Font * LoadFont( const std::string &s, int fontsize );
void LoadFonts(void);

void DrawString(std::string str, int x, int y);

/* set tank color */
void Planet_SetImage(Planet *planet, std::string id);
void Tank_SetImages(Tank *tank, TankColors col);
void Menu_Draw(Menu* menu);
SDL_Texture * getImage(std::string id);
TTF_Font * getFont(std::string id);

SDL_Renderer* ReturnRenderer();
SDL_Window* ReturnScreen();
SDL_Texture* ReturnBg();
SDL_Texture* ReturnCursor();

}

}
