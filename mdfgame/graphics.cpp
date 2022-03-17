
#include "resources.h"
#include "graphics.h"

#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"

#include <string>
#include <map>

#include <fmt/core.h>

static SDL_Window* screen;
static SDL_Surface* screenSurface;
static SDL_Renderer *renderer;
static SDL_Texture* background;
static std::map<std::string, SDL_Texture *> images;
static std::map<std::string, TTF_Font *> fonts;
static std::map<TankColors,SDL_Texture*> tankParts;
static SDL_Texture* LoadImage(const char *s);
static void	LoadImages();

SDL_Texture * getImage(std::string id)
{
	return images[id];
}

TTF_Font * getFont(std::string id)
{
	return fonts[id];
}

bool Graphics_Init(void)
{
	// initialize SDL video
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fmt::print( "Unable to init SDL: {}\n", SDL_GetError() );
		return 0;
	}
	// initialize TTF fonts
	if (TTF_Init() < 0 )
	{
		fmt::print("Unable to init Fonts: {}\n", SDL_GetError() );
		return 0;
	}
	// make sure SDL cleans up before ex
	atexit(TTF_Quit);
	atexit(SDL_Quit);
	// create a new window
    screen = SDL_CreateWindow("Tank game",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              0,
                              0,
                              SDL_WINDOW_FULLSCREEN_DESKTOP);
	if ( !screen )
	{
		fmt::print("Unable to set {}x{} video: {}\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		return 0;
	}

    int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if(!(IMG_Init(imgFlags) & imgFlags))
    {
        fmt::print("Unable to initialize SDL_Image: {}\n", IMG_GetError());
        return 0;
    }

    screenSurface = SDL_GetWindowSurface(screen);

    renderer = SDL_CreateRenderer(screen, -1, 0);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");  // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

	SDL_ShowCursor(SDL_DISABLE);

	LoadImages();
	LoadFont();
	
	background = images["Background"];

	return 1;
}

void Graphics_ApplySurface(SDL_Texture *source, int x, int y, float scaling, float angle)
{
    int w, h;
    SDL_QueryTexture(source, NULL, NULL, &w, &h);
    w *= scaling;
    h *= scaling;
    SDL_Rect renderQuad = { x-w/2, y-h/2, w, h };

    SDL_RenderCopyEx(renderer, source, NULL, &renderQuad, -(angle * 180) / M_PI, NULL, SDL_FLIP_NONE);
}

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
	
    int w, h;
    SDL_GetWindowSize(screen, &w, &h);
    Graphics_ApplySurface(background, (w / 2), (h / 2));

    hlineRGBA(renderer, -offset.x, world.size.width - offset.x, -offset.y, 255, 0, 0, 128);
    hlineRGBA(renderer, -offset.x, world.size.width - offset.x, world.size.height - offset.y, 255, 0, 0, 128);
    vlineRGBA(renderer, -offset.x, -offset.y + 1, (world.size.height - 1) - offset.y, 255, 0, 0, 128);
    vlineRGBA(renderer, world.size.width - offset.x, -offset.y + 1, (world.size.height - 1) - offset.y, 255, 0, 0, 128);
	
	/* fix score board text */
	TTF_Font *font = fonts["Text"];
	std::string p1score, p2score, p1deaths, p2deaths;
	if( font == NULL )
        fmt::print("no font\n");

	for (auto planet : world.planets)
	{
        SDL_Texture* img = planet->image;
        int w;
        SDL_QueryTexture(img, NULL, NULL, &w, NULL);

        float scaling = 2 * planet->radius / (w * (1 - 0.1176f));
        Graphics_ApplySurface(img, planet->pos.x - offset.x, planet->pos.y - offset.y, scaling, planet->rot);
	}
	
	for (const auto tank : world.tanks)
	{
        Tank_Draw(tank, offset);
	}
	
	/* Draw projectiles */
    Projectile_Draw(world.projectiles, renderer, offset);

	if ( world.player )
	{
        Player_DrawHud(renderer, world.player, 8, 8);
	}
    Effect_Draw(world.effects, offset);
}

void Graphics_BeginScene()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void Graphics_EndScene()
{
    SDL_RenderPresent(renderer);
}

void Graphics_DrawString(std::string str, int x, int y)
{
	TTF_Font *font = fonts["Text"];
	SDL_Color c = {255, 255, 255};
	SDL_Surface *txt = TTF_RenderText_Solid( font, str.c_str(), c);
    SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, txt );
	
    Graphics_ApplySurface(texture, x + txt->w / 2, y + txt->h / 2);
	SDL_FreeSurface(txt);
    SDL_DestroyTexture(texture);
}

// static functions
SDL_Texture* LoadImage(const char *s)
{
    SDL_Texture* newTexture = NULL;

	SDL_Surface *loaded = IMG_Load( s );
    if(loaded)
    {
        newTexture = SDL_CreateTextureFromSurface(renderer, loaded);
        SDL_FreeSurface(loaded);
    }
    return newTexture;
}

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

void Planet_SetImage(Planet *planet, std::string id)
{
	if( (planet->image = images[id] ) == NULL)
		fmt::print("Couldn't load image {} from 'images'\n", id);
}


TTF_Font * Graphics_LoadFont(const std::string &s, int fontsize)
{
        return TTF_OpenFont( s.c_str(), fontsize );
}

/* MENU STUFFIES ###############################################*/

void Menu_Draw(Menu* menu)
{
    int w, h;
    SDL_GetWindowSize(screen, &w, &h);
    Graphics_ApplySurface(menu->bg, w / 2, h / 2);
    
    for(auto m : menu->list)
        Button_Draw(renderer, m);
}

SDL_Renderer* ReturnScreen()
{
    return renderer;
}

SDL_Texture* ReturnBg()
{
	return background;
}

SDL_Texture* ReturnCursor()
{	
	return images["Cursor"];
}
