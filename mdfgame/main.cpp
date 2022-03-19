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

#include <fmt/core.h>

bool gquit = false;

class Game {
public:
	Game(std::string resource_path) {
		MDF::Resource::Init(resource_path);
		Graphics_Init();
		Audio_Init();
		MDF::Animation::LoadAnimations();
		LoadWeapons();

		Audio_LoadPresetWavFiles();
		Audio_LoadPresetMp3Files();
	}

	~Game() {
		MDF::Animation::UnloadAnimations();
		Audio_ShutDown();

		IMG_Quit();
		SDL_Quit();
	}

	void Run() {
		//TODO Clean this horrid mess

		while(!quit)
		{
			HandleMenu();

			if(!quit)
			{
				World world = World_GetWorld("WorldOne");

				auto player = new MDF::Player;

				InitializeInput(world, player);

				//world.players.push_back(player); // DEPRECATED! THERE SHALL BE NO HARDCODING OF INDICES!
				world.player = player;

				player->SetCol(MDF::TANK_NEUTRAL);
				//player2->SetCol(MDF::TANK_YELLOW);

				MDF::Tank::Spawn(world, player, world.planets[0]);
				Tank_SetImages(player->GetTank(), player->Col());
				//Tank_Spawn(world, player2, world.planets[4]);
				//Tank_SetImages(player2->GetTank(), player2->Col());

				GameLoop(world);
			}
		}
	}

private:

	static void KeyDown ( SDL_Scancode key, World& world )
	{
		switch ( key )
		{
		case SDL_SCANCODE_ESCAPE:
			gquit = true;
			break;
		default: break;
		}
	}

	void InitializeInput(World& world, MDF::Player* player) {
		Input_Init(world);

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
	}

	void GameLoop(World& world) {
		// Time variables
		float timestamp = 0.0f;
		const float dt = (float)(1.0 / FPS);

		float currentTime = (float)SDL_GetTicks();
		float accumulator = 0.0f;

		Audio_PlayMusic(0, -1);

		gquit = false;
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
				MDF::Physics::Process(world, dt);
				/* Animtions */
				MDF::Effect::Update(world.effects, dt);
			}

			/* Graphics */
			Graphics_BeginScene();

			Graphics_DrawScene(world);

			//TODO Draw HUD

			Graphics_EndScene();
		}

		World_DeInit(world);
	}

	void HandleMenu() {
		auto chosen = MDF::TheMenu(&quit);
		
		switch(chosen)
		{
			case MenuOption::HostGame:
				fmt::print("Host game\n");
			break;
			case MenuOption::JoinGame:
				fmt::print("Join game\n");
			break;
			case MenuOption::Settings:
				fmt::print("Settings\n");
			break;
			case MenuOption::QuitGame:
				quit = true;
			break;
		}
	}

	bool quit{false};
};


#include <unistd.h>

int main(int argc, char **argv)
{
	//srand((unsigned)time(NULL));

	std::string resource_path;

	int c;
	while ((c = getopt(argc, argv, "r:")) != -1) {
		switch(c) {
		case 'r': {
			resource_path = optarg;
			fmt::print("Setting resource file path: '{}'\n", resource_path);
			break;
		}
		default:
			break;
		}
	}

	Game game(resource_path);

	game.Run();

	return 0;
}

