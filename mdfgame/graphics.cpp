
#include "resources.h"
#include "graphics.h"

#ifdef _WIN32
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "SDL_gfxPrimitives.h"
#include "SDL_rotozoom.h"
#else
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_gfxPrimitives.h"
#include "SDL/SDL_rotozoom.h"
#endif

#include <string>
#include <stdio.h>
#include <map>


using namespace std;

static SDL_Surface* screen;
static SDL_Surface* background;
static map<string, SDL_Surface *> images;
static map<string, TTF_Font *> fonts;
static map<TankColors,SDL_Surface*> tankParts;
static SDL_Surface* LoadImage(const char *s);
static void	LoadImages();

SDL_Surface * getImage(string id)
{
	return images[id];
}

TTF_Font * getFont(string id)
{
	return fonts[id];
}

bool Graphics_Init(void)
{
	// initialize SDL video
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "Unable to init SDL: %s\n", SDL_GetError() );
		return 0;
	}
	// initialize TTF fonts
	if (TTF_Init() < 0 )
	{
		printf("Unable to init Fonts: %s\n", SDL_GetError() );
		return 0;
	}
	// make sure SDL cleans up before ex
	atexit(TTF_Quit);
	atexit(SDL_Quit);
	// create a new window
	screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
						SDL_HWSURFACE|SDL_DOUBLEBUF);
	if ( !screen )
	{
		printf("Unable to set %dx%d video: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		return 0;
	}
	
	SDL_ShowCursor(SDL_DISABLE);

	LoadImages();
	LoadFont();
	
	background = images["Background"];

	return 1;
}

void Graphics_ApplySurface(SDL_Surface *source, SDL_Surface *destination, int x, int y, float scaling, float angle)
{
	SDL_Rect offset;

	SDL_Surface *rotated = rotozoomSurface(source, (angle * 180) / M_PI, scaling, 0);

	offset.x = x - rotated->w / 2;
	offset.y = y - rotated->h / 2;
	SDL_BlitSurface(rotated, NULL, destination, &offset);
	SDL_FreeSurface(rotated);
//		SDL_BlitSurface(source, NULL, destination, &offset);
}

#include <iostream>
using namespace std;

void Graphics_DrawScene(World &world)
{
	int cursorX, cursorY;
	SDL_GetMouseState(&cursorX, &cursorY);
	vec2 offset = world.camera.GetCorner();
	offset.x += cursorX;
	offset.y += cursorY;
	
	world.camera.PositionCorner(world.player ? world.player->tank->pos : vec2(0, 0), offset, world.size);
	
	offset = world.camera.GetCorner();
	/** Fix bg **/
	
	Graphics_ApplySurface(background, screen, (screen->w / 2), (screen->h / 2));

	hlineRGBA(screen, -offset.x, world.size.width - offset.x, -offset.y, 255, 0, 0, 128);
	hlineRGBA(screen, -offset.x, world.size.width - offset.x, world.size.height - offset.y, 255, 0, 0, 128);
	vlineRGBA(screen, -offset.x, -offset.y + 1, (world.size.height - 1) - offset.y, 255, 0, 0, 128);
	vlineRGBA(screen, world.size.width - offset.x, -offset.y + 1, (world.size.height - 1) - offset.y, 255, 0, 0, 128);
	
	/* fix score board text */
	TTF_Font *font = fonts["Text"];
	string p1score, p2score, p1deaths, p2deaths;
	if( font == NULL )
	    printf("no font\n");
	SDL_Surface *score1 = NULL;
// 	SDL_Surface *score2 = NULL;
	
	/* set scoreboard text color */
	SDL_Color col;
	col.r = 255;
	col.g = 255;
	col.b = 255;
	
	for (unsigned int l = 0; l < world.planets.size(); l++)
	{
		SDL_Surface* img = world.planets[l]->image;

		float scaling = 2 * world.planets[l]->radius / (img->w * (1 - 0.1176f));
		Graphics_ApplySurface(img, screen, world.planets[l]->pos.x - offset.x, world.planets[l]->pos.y - offset.y, scaling, world.planets[l]->rot);
	}
	
	for (unsigned int l = 0; l < world.tanks.size(); l++)
	{
		const Tank *tank = world.tanks[l];
		Tank_Draw(tank, screen, offset);
	}
	
	/* Draw projectiles */
	Projectile_Draw(world.projectiles, screen, offset);
	
	/** Maiden, please clean this up... /orka **/

	// FÖR I HELVETE VAD HÄR SER UT!? /Jim

	if ( world.player )
	{
		
		/* life tank 1 */
		rectangleRGBA( screen, 8, 8, 162, 32, 0, 255, 0, 100 );
		boxRGBA( screen, 10, 10, 10 + ((150.0*world.player->tank->hitPoints)/MAX_HITPOINTS), 30, 255 - (255.0*world.player->tank->hitPoints)/MAX_HITPOINTS, (255.0*world.player->tank->hitPoints)/MAX_HITPOINTS, 0, 100 );
		
		/* reload teleport timer*/
		rectangleRGBA( screen, 8, 33, 162, 39, 0, 100, 0, 100);
		
		if( world.player->tank->teleportTimer > 0 )
			boxRGBA( screen, 10, 35, 10 + 150/30*(30-world.player->tank->teleportTimer),37,255,0,100,100);
		
		if( world.player->tank->teleportTimer < 0 )
			boxRGBA( screen, 10, 35, 160,37,255,0,255,100);
		
		rectangleRGBA( screen, 8, 40, 162, 45, 0, 100, 0, 100);
		/* reload gun timer */
		float delay = Weapon_GetDelay(world.player->tank->weapon);
		
		if( world.player->tank->timeSinceLastFire > 0 )
			boxRGBA( screen, 10, 42, 10 + 150/delay*(delay-world.player->tank->timeSinceLastFire),43,255,0,100,100);
		
		if( world.player->tank->timeSinceLastFire < 0 )
			boxRGBA( screen, 10, 42, 160,43,255,0,255,100);
		
		/* life tank 2 */
		//rectangleRGBA( screen, (screen->w -162), 8, (screen->w -8), 32, 0, 255, 0, 255 );
		//boxRGBA( screen, (screen->w -160), 10, (screen->w -160) + ((150.0*world.players[1]->tank->hitPoints)/MAX_HITPOINTS), 30, 255 - (255.0*world.players[1]->tank->hitPoints)/MAX_HITPOINTS, (255.0*world.players[1]->tank->hitPoints)/MAX_HITPOINTS, 0, 255 );
	}
	Effect_Draw(world.effects, screen, offset);
	
	
	if( score1 != NULL )
	{
	    Graphics_ApplySurface(score1, screen, 90, 20);
	    SDL_FreeSurface(score1);
	}
	
	/*if( score2 != NULL )
	{
	    Graphics_ApplySurface(score2, screen, screen->w -80, 20);
	}*/

	
}

void Graphics_BeginScene()
{
	
}

void Graphics_EndScene()
{
	SDL_Flip(screen);
}

void Graphics_DrawString(std::string str, int x, int y)
{
	TTF_Font *font = fonts["Text"];
	SDL_Color c = {255, 255, 255};
	SDL_Surface *txt = TTF_RenderText_Solid( font, str.c_str(), c);
	
	Graphics_ApplySurface(txt, screen, x + txt->w / 2, y + txt->h / 2);
	SDL_FreeSurface(txt);
}

// static functions
SDL_Surface* LoadImage(const char *s)
{
	SDL_Surface *loaded = IMG_Load( s );
	SDL_Surface *ret = SDL_DisplayFormatAlpha( loaded );
	SDL_FreeSurface(loaded);
	return ret;
}

#include <iostream>

void LoadImages()
{
	ResourceMap res = Resources_GetOfType(RT_IMAGE);
	
	for ( ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		images[it->first] = LoadImage(it->second.c_str());
	
	/* Temporary Tank Loading code */
	tankParts[TANK_NEUTRAL] = images["TankNeutral"];
	tankParts[TANK_BLUE] = images["TankBlue"];
	tankParts[TANK_GREEN] = images["TankGreen"];
	tankParts[TANK_RED] = images["TankRed"];
	tankParts[TANK_YELLOW] = images["TankYellow"];
}

void LoadFont()
{
	ResourceMap res = Resources_GetOfType(RT_FONT);
	
	for ( ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		fonts[it->first] = Graphics_LoadFont(it->second.c_str(),32);    
}

/* set tank color */
void Tank_SetImages(Tank *tank, TankColors col)
{
	tank->tankBody = tankParts[col];
}

void Planet_SetImage(Planet *planet, string id)
{
	if( (planet->image = images[id] ) == NULL)
		printf("Couldn't load image %s from \"images\"\n", id.c_str());
}


TTF_Font * Graphics_LoadFont(const string &s, int fontsize)
{
        return TTF_OpenFont( s.c_str(), fontsize );
}

/* MENU STUFFIES ###############################################*/

void Menu_Draw(Menu* menu)
{
    unsigned int i;
    
    Graphics_ApplySurface(menu->bg, screen, screen->w / 2, screen->h / 2);
    
    for(i=0; i<menu->list.size(); i++)
	Button_Draw(screen,menu->list[i]);
}

SDL_Surface* ReturnScreen()
{
	return screen;
}

SDL_Surface* ReturnBg()
{
	return background;
}

SDL_Surface* ReturnCursor()
{	
	return LoadImage("data/images/Cursor.png");
}
