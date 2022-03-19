
#include "resources.h"
#include "graphics.h"
#include "player.h"
#include "input.h"

#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL2_gfxPrimitives.h"
#include "SDL2/SDL2_rotozoom.h"

#include <string>
#include <map>

#include <fmt/core.h>

namespace MDF {

namespace Graphics {

static SDL_Window* screen;
static SDL_Surface* screenSurface;
static SDL_Renderer *renderer;
static SDL_Texture* background;
static std::map<std::string, SDL_Texture *> images;
static std::map<std::string, TTF_Font *> fonts;
static std::map<MDF::TankColors, SDL_Texture*> tankParts;

SDL_Texture * getImage(std::string id)
{
	return images[id];
}

TTF_Font * getFont(std::string id)
{
	return fonts[id];
}

static SDL_Texture* LoadImage(const char *s);
static void	LoadImages();

bool Init(void)
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
	LoadFonts();
	
	background = images["Background"];

	return 1;
}

void ApplySurface(SDL_Texture *source, int x, int y, float scaling, float angle)
{
    int w, h;
    SDL_QueryTexture(source, NULL, NULL, &w, &h);
    w *= scaling;
    h *= scaling;
    SDL_Rect renderQuad = { x-w/2, y-h/2, w, h };

    SDL_RenderCopyEx(renderer, source, NULL, &renderQuad, -(angle * 180) / M_PI, NULL, SDL_FLIP_NONE);
}

void DrawScene(MDF::World &world)
{
	auto cursor = MDF::Input::GetMousePos();
	world.camera.PositionCorner(world.player ? world.player->GetTank()->Pos() : MDF::vec2(0, 0), cursor);
	MDF::vec2 offset = world.camera.GetCorner();

	/** Fix bg **/
	
    int w, h;
    SDL_GetWindowSize(screen, &w, &h);
    ApplySurface(background, (w / 2), (h / 2));

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
        auto img = planet->Image();
        int w;
        SDL_QueryTexture(img, NULL, NULL, &w, NULL);

        float scaling = 2 * planet->Radius() / (w * (1 - 0.1176f));
        Graphics::ApplySurface(img, planet->Pos().x - offset.x, planet->Pos().y - offset.y, scaling, planet->Rot());
	}
	
	for (const auto tank : world.tanks)
	{
        tank->Draw(offset);
	}
	
	/* Draw projectiles */
    Projectile::Draw(world.projectiles, renderer, offset);

	if ( world.player )
	{
        world.player->DrawHud(renderer, vec2(8, 8));
	}
    MDF::Effect::Draw(world.effects, offset);
}

void BeginScene()
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
}

void EndScene()
{
    SDL_RenderPresent(renderer);
}

void DrawString(std::string str, int x, int y)
{
	TTF_Font *font = fonts["Text"];
	SDL_Color c = {255, 255, 255};
	SDL_Surface *txt = TTF_RenderText_Solid( font, str.c_str(), c);
    SDL_Texture *texture = SDL_CreateTextureFromSurface( renderer, txt );
	
    ApplySurface(texture, x + txt->w / 2, y + txt->h / 2);
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
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::IMAGE);
	
	for ( MDF::Resource::ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		images[it->first] = LoadImage(it->second.c_str());
	
	/* Temporary Tank Loading code */
	tankParts[MDF::TANK_NEUTRAL] = images["TankNeutral"];
	tankParts[MDF::TANK_BLUE] = images["TankBlue"];
	tankParts[MDF::TANK_GREEN] = images["TankGreen"];
	tankParts[MDF::TANK_RED] = images["TankRed"];
	tankParts[MDF::TANK_YELLOW] = images["TankYellow"];
}

void LoadFonts()
{
	MDF::Resource::ResourceMap res = MDF::Resource::GetOfType(MDF::Resource::Type::FONT);
	
	for ( MDF::Resource::ResourceMap::iterator it = res.begin(); it != res.end(); it++ )
		fonts[it->first] = Graphics::LoadFont(it->second.c_str(),32);    
}

TTF_Font * LoadFont(const std::string &s, int fontsize)
{
	return TTF_OpenFont( s.c_str(), fontsize );
}

/* set tank color */
void Tank_SetImages(MDF::Tank *tank, MDF::TankColors col)
{
	tank->SetTankBody(tankParts[col]);
}

void Planet_SetImage(MDF::Planet *planet, std::string id)
{
	if( (planet->SetImage(images[id]) ) == NULL)
		fmt::print("Couldn't load image {} from 'images'\n", id);
}


/* MENU STUFFIES ###############################################*/

void Menu_Draw(MDF::Menu* menu)
{
    int w, h;
    SDL_GetWindowSize(screen, &w, &h);
    Graphics::ApplySurface(menu->BG(), w / 2, h / 2);
    
    for(auto button : menu->Buttons())
        button->Draw(renderer);
}

SDL_Renderer* ReturnRenderer()
{
    return renderer;
}

SDL_Window* ReturnScreen() {
	return screen;
}


SDL_Texture* ReturnBg()
{
	return background;
}

SDL_Texture* ReturnCursor()
{	
	return images["Cursor"];
}

} // namespace Graphics
} // namespace MDF
