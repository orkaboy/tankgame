#ifndef GAME_H
#define GAME_H

#include "utility.h"
#include "planet.h"
#include "player.h"
#include "tank.h"
#include "effects.h"
#include "camera.h"

#include <vector>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define SCREEN_BPP 32
#define FPS 30

// Uncomment to set the server listener port
//#define NET_SERVER_PORT 1337
//#define NET_SERVER_MAX_PLAYERS 4
//#define NET_SERVER_PACKET_SIZE 4096

typedef struct World
{
	std::vector<Planet*> planets;
	std::vector<Tank*> tanks;
	Player* player;
	std::vector<Projectile*> projectiles;
	std::vector<Effect*> effects;
	
	Camera camera;
	
	vec2	size;
	float timeSinceLastAmmo;
} World;

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

/* init the game */
World World_Init(std::string findpath);
void World_DeInit(World &world);

void World_SpawnAmmo(World &world, float dt);

World World_GetWorld(std::string id);

#endif
