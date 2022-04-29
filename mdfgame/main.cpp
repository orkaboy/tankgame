/* MAIN FILE FOR WTF GAME!!! */

#include "world.h"
#include "graphics.h"
#include "physics.h"
#include "input.h"
#include "audio.h"
#include "resources.h"
#include "weapon.h"
#include "gamestate.h"
#include "game.h"

#include <ctime>

#include <vector>

#include <fmt/core.h>

bool gquit = false;

namespace MDF {

class TankGame {
public:
	TankGame(std::string resource_path) {
		Resource::Init(resource_path);
		Graphics::Init();
		Audio::Init();
		Animation::LoadAnimations();
		Weapon::LoadWeapons();

		Audio::LoadPresetWavFiles();
		Audio::LoadPresetMp3Files();
		
		fmt::print("Loaded resources and initialized libraries.\n");

		mStateMachine.RegisterState("MainMenu", new Menues::Main);
		mStateMachine.RegisterState("Game", new Game);
		
		/* Start game in main menu */
		mStateMachine.TransitionState("MainMenu");
		mStateMachine.Run();
	}

	~TankGame() {
		Animation::UnloadAnimations();
		Audio::ShutDown();

		IMG_Quit();
		SDL_Quit();
	}

private:
	GameSM mStateMachine;
};

}

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
			fmt::print("Unsupported commandline argument: {}\n", c);
			return -1;
		}
	}

	MDF::TankGame game(resource_path);
	return 0;
}

