/* MAIN FILE FOR WTF GAME!!! */

#include "world.h"
#include "graphics.h"
#include "physics.h"
#include "input.h"
#include "audio.h"
#include "resources.h"
#include "weapon.h"
#include "button.h"

#include <ctime>

#include <vector>
#include <iostream>
using namespace std;

bool gquit = false;

void KeyDown ( SDL_Scancode key, World& world )
{
	switch ( key )
    {
    case SDL_SCANCODE_ESCAPE:
		gquit = true;
		break;
	default: break;
	}
	
}


int main(int argc, char **argv)
{
    bool quit = false;
    Player *player;

	//srand((unsigned)time(NULL));
	
	Resources_Init();
	Graphics_Init();
	Audio_Init();
	LoadAnimations();
	LoadWeapons();

    Audio_LoadPresetWavFiles();
    Audio_LoadPresetMp3Files();
	
    //TODO Clean this horrid mess

	while(!quit)
	{
        int chosen = TheMenu(&quit);
		
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

			player = Player_Init();
			
            Input_BindKey(SDL_SCANCODE_A, player, PA_MOVE_BACKWARD, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_D, player, PA_MOVE_FORWARD, KEY_HOLD);
            //Input_BindKey(SDL_SCANCODE_W, player, PA_TURRET_UP, KEY_HOLD);
            //Input_BindKey(SDL_SCANCODE_S, player, PA_TURRET_DOWN, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_SPACE, player, PA_FIRE_BEGIN, KEY_DOWN);
            Input_BindKey(SDL_SCANCODE_SPACE, player, PA_FIRE_END, KEY_UP);
            Input_BindKey(SDL_SCANCODE_Q, player, PA_TELEPORT, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_E, player, PA_CHANGE_WEP, KEY_DOWN);

			Input_BindMouseHandler(player);
			
            /*Input_BindKey(SDL_SCANCODE_LEFT, player2, PA_MOVE_BACKWARD, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_RIGHT, player2, PA_MOVE_FORWARD, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_UP, player2, PA_TURRET_UP, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_DOWN, player2, PA_TURRET_DOWN, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_RCTRL, player2, PA_FIRE_BEGIN, KEY_DOWN);
            Input_BindKey(SDL_SCANCODE_RCTRL, player2, PA_FIRE_END, KEY_UP);
            Input_BindKey(SDL_SCANCODE_RSHIFT, player2, PA_TELEPORT, KEY_HOLD);
            Input_BindKey(SDL_SCANCODE_RETURN, player2, PA_CHANGE_WEP, KEY_DOWN);*/

			Input_SetHandler(KEY_DOWN, KeyDown);

			//world.players.push_back(player); // DEPRECATED! THERE SHALL BE NO HARDCODING OF INDICES!
			world.player = player;
			
			player->col = TANK_NEUTRAL;
			//player2->col = TANK_YELLOW;
			
			Tank_Spawn(world, player, world.planets[0]);
			Tank_SetImages(player->tank, player->col);
			//Tank_Spawn(world, player2, world.planets[4]);
			//Tank_SetImages(player2->tank, player2->col);

			// Time variables
			float timestamp = 0.0f;
			const float dt = (float)(1.0 / FPS);

			float currentTime = (float)SDL_GetTicks();
			float accumulator = 0.0f;
			
			Audio_PlayMusic(0, -1);
			
			while (!gquit)
            {
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

                //TODO Draw HUD

				Graphics_EndScene();
			}
			
			World_DeInit(world);
		}
    }

	UnloadAnimations();
    Audio_ShutDown();

    IMG_Quit();
    SDL_Quit();

	return 0;
}

