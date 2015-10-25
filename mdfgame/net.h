#ifndef NET_H
#define NET_H

#include <string>
#include <vector>

struct World;
struct Player;

#define NET_VER_MAJOR "0"
#define NET_VER_MINOR "2"

typedef struct
{
	unsigned int iid;
	std::string name;
	unsigned int num_players;
	unsigned int max_players;
} ServerInfo;

enum EntityType
{
	ET_TANK,
	ET_PLANET,
	ET_PROJECTILE,
	ET_PLAYER
};

std::vector<ServerInfo> Net_GetAvailableServers();

// Entity management

void Net_RegisterEntity(void* entity);
void Net_UnRegisterEntity(void* entity);

void Net_Init(Player* localPlayer);
void Net_RefreshServers();
void Net_Update();
void Net_ChangeMap(World& newMap);
void Net_StartServer(World& map);
void Net_Join(unsigned int instance_id, World& world);
void Net_Join(std::string hostname, World& world);
void Net_Quit();

#endif
