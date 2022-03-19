#pragma once

#include "utility.h"
#include "planet.h"
#include "player.h"
#include "tank.h"
#include "effects.h"
#include "camera.h"

#include <vector>

namespace MDF {

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;
constexpr int SCREEN_BPP = 32;
constexpr int FPS = 30;

// Uncomment to set the server listener port
//#define NET_SERVER_PORT 1337
//#define NET_SERVER_MAX_PLAYERS 4
//#define NET_SERVER_PACKET_SIZE 4096

class World
{
public:
	/* init the game */
	World(std::string findpath);
	~World();

	void SpawnAmmo(float dt);

	static World GetWorld(std::string id);

	std::vector<MDF::Planet*> planets;
	std::vector<MDF::Tank*> tanks;
	MDF::Player* player;
	std::vector<Projectile*> projectiles;
	std::vector<MDF::Effect*> effects;
	
	MDF::Camera camera;
	
	vec2	size;
	float timeSinceLastAmmo;
};

// //Game now exists only for cultural and historical reasons ;)
// typedef struct Game
// {
//   /* player one, player has tank */
//   Player *player1;
//   /* player two  player has tank */
//   Player *player2;
//   /* one planet to begin with */
//   Planet *planet;
// } Game;


}
