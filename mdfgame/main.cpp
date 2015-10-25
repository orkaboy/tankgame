/* MAIN FILE FOR WTF GAME!!! */

#include "world.h"
#include "graphics.h"
#include "physics.h"
#include "input.h"
#include "audio.h"
#include "resources.h"
#include "weapon.h"
#include "net.h"
#include "button.h"

#include <ctime>

#include <vector>
#include <iostream>
using namespace std;

bool quit = false;
bool gquit = false;

void KeyDown ( SDLKey key, World& world )
{
	switch ( key )
	{
	case SDLK_1:
		Net_Join(Net_GetAvailableServers()[0].iid, world);
		break;
	case SDLK_2:
		Net_Join(Net_GetAvailableServers()[1].iid, world);
		break;
	case SDLK_F1:
		Net_StartServer(world);
		break;
	case SDLK_F2:
		Net_RefreshServers();
		break;
	case SDLK_ESCAPE:
		gquit = true;
		break;
	default: break;
	}
	
}

int TheMenu ( void )
{
	SDL_Event event;
     
	SDL_Surface* screen = ReturnScreen();
	SDL_Surface* bg = ReturnBg();
	SDL_Surface* cursor = ReturnCursor();
	Menu* menu = Menu_Init(bg,cursor);
	Menu_AddButton(menu,60,100,1,"Host Game", getFont("Text"));
	Menu_AddButton(menu,290,100,2,"Join Game", getFont("Text"));
	Menu_AddButton(menu,510,100,3,"Settings", getFont("Text"));
	Menu_AddButton(menu,700,100,4,"Quit", getFont("Text"));
	
	
	bool mquit = false;
	int mx,my;
	unsigned int i;
	float timestamp = 0.0f;
	const float dt = 1.0 / FPS;
	float currentTime = SDL_GetTicks();
	float accumulator = 0.0f;

	while(!mquit)
	{
		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_QUIT)
			{
				mquit = true;
				quit = true;
			}
			
			if(event.type == SDL_KEYDOWN)
			{
				switch ( event.key.keysym.sym )
				{
				case SDLK_e:
					mquit = true;
				break;
				case SDLK_ESCAPE:
					mquit = true;
					quit = true;
				break;
				default:
				break;
				}
			}
			if( event.type == SDL_MOUSEBUTTONUP )
			{
				for(i=0; i<menu->list.size(); i++)
				{
				    if(menu->list[i]->over)
					return menu->list[i]->ret;
				}
			}
		}
		
		float newTime = (float)SDL_GetTicks();
		float deltaTime = (newTime - currentTime) / 1000.f;
		currentTime = newTime;

		accumulator += deltaTime;

		while (accumulator>=dt)
		{
			timestamp += dt;
			accumulator -= dt;
		}
		
		SDL_GetMouseState(&mx, &my);		
		Menu_CheckButton(menu,mx,my);
		Menu_Draw(menu);
		Graphics_ApplySurface(cursor, screen, mx, my,1,0);
		SDL_Flip(screen);
	}
	return 0;
}


int main(int argc, char **argv)
{
	Player *player;
	
	int chosen;

	//srand((unsigned)time(NULL));
	
	Resources_Init();
	Graphics_Init();
	Audio_Init();
	LoadAnimations();
	LoadWeapons();
	
	while(!quit)
	{
		chosen = TheMenu();
		
		switch(chosen)
		{
			case 1:
				printf("start\n");
			break;
			case 2:
				printf("LAN\n");
			break;
			case 3:
				printf("Settings\n");
			break;
			case 4:
				quit = true;
			break;
		}
		
		if(!quit)
		{
			gquit = false;
			
			World world = World_GetWorld("WorldOne");
			
			
			Input_Init(world);

			Audio_LoadPresetWavFiles();
			Audio_LoadPresetMp3Files();
			
			player = Player_Init();
			
			Input_BindKey(SDLK_a, player, PA_MOVE_BACKWARD, KEY_HOLD);
			Input_BindKey(SDLK_d, player, PA_MOVE_FORWARD, KEY_HOLD);
			//Input_BindKey(SDLK_w, player, PA_TURRET_UP, KEY_HOLD);
			//Input_BindKey(SDLK_s, player, PA_TURRET_DOWN, KEY_HOLD);
			Input_BindKey(SDLK_SPACE, player, PA_FIRE_BEGIN, KEY_DOWN);
			Input_BindKey(SDLK_SPACE, player, PA_FIRE_END, KEY_UP);
			Input_BindKey(SDLK_q, player, PA_TELEPORT, KEY_HOLD);
			Input_BindKey(SDLK_e, player, PA_CHANGE_WEP, KEY_DOWN);

			Input_BindMouseHandler(player);
			
			/*Input_BindKey(SDLK_LEFT, player2, PA_MOVE_BACKWARD, KEY_HOLD);
			Input_BindKey(SDLK_RIGHT, player2, PA_MOVE_FORWARD, KEY_HOLD);
			Input_BindKey(SDLK_UP, player2, PA_TURRET_UP, KEY_HOLD);
			Input_BindKey(SDLK_DOWN, player2, PA_TURRET_DOWN, KEY_HOLD);
			Input_BindKey(SDLK_RCTRL, player2, PA_FIRE_BEGIN, KEY_DOWN);
			Input_BindKey(SDLK_RCTRL, player2, PA_FIRE_END, KEY_UP);
			Input_BindKey(SDLK_RSHIFT, player2, PA_TELEPORT, KEY_HOLD);
			Input_BindKey(SDLK_RETURN, player2, PA_CHANGE_WEP, KEY_DOWN);*/

			Input_SetHandler(KEY_DOWN, KeyDown);
			
			// If we create a player whom is active in the world, we HAVE to register it in the network:
			Net_RegisterEntity(player);
			//world.players.push_back(player); // DEPRECATED! THERE SHALL BE NO HARDCODING OF INDICES!
			world.player = player;
			
			player->col = TANK_NEUTRAL;
			//player2->col = TANK_YELLOW;
			
			Tank_Spawn(world, player, world.planets[0]);
			Tank_SetImages(player->tank, player->col);
			//Tank_Spawn(world, player2, world.planets[4]);
			//Tank_SetImages(player2->tank, player2->col);

			Net_Init(player);
			
			// Time variables
			float timestamp = 0.0f;
			const float dt = (float)(1.0 / FPS);

			float currentTime = (float)SDL_GetTicks();
			float accumulator = 0.0f;
			
			Audio_PlayMusic(0, -1);
			
			while (!gquit)
			{
				Net_Update();

				/* Timer phase */
				float newTime = (float)SDL_GetTicks();
				float deltaTime = (newTime - currentTime) / 1000.f;
				currentTime = newTime;

				accumulator += deltaTime;

				while (accumulator>=dt)
				{
					Input_HandleEvents();

					timestamp += dt;
					accumulator -= dt;
					
					World_SpawnAmmo(world,dt);

					/* Physics */
					Physics(world, dt);
					/* Animtions */
					Effect_Update(world.effects, dt);
				}
				
				/* Graphics */
				Graphics_BeginScene();

				Graphics_DrawScene(world);
				
				std::vector<ServerInfo> servers = Net_GetAvailableServers();
				for ( unsigned int i = 0; i < servers.size(); i++ )
					Graphics_DrawString("[" + convertToStr(i + 1) + "]:" + servers[i].name, 0, 200 + (i * 30));
				
				Graphics_EndScene();
			}
			
			World_DeInit(world);
		}
	}
	
	UnloadAnimations();
	Audio_ShutDown();
	Net_Quit();
	
	return 0;
}

