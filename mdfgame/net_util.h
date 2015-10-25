#ifndef NET_UTIL_H
#define NET_UTIL_H

#include <cassert>
#include <cstring>
#include <ctime>
#include <map>
#include <climits>
#include <iostream>

#include <tr1/unordered_map>
#include "SDL2/SDL_net.h"

#include "player.h"
#include "tank.h"
#include "projectile.h"
#include "world.h"

#include "net.h"

#ifndef NET_CLIENT_PORT
#define NET_CLIENT_PORT 1338
#endif

#ifndef NET_SERVER_PORT
#define NET_SERVER_PORT 1337
#endif
#ifndef NET_SERVER_MAX_PLAYERS
#define NET_SERVER_MAX_PLAYERS 4
#endif
#ifndef NET_SERVER_PACKET_SIZE
#define NET_SERVER_PACKET_SIZE 2048
#endif

#define NET_FRAMES_PER_SYNC 4

enum MessageContent
{
	MC_STRINGS,
	MC_RAWDATA,
	MC_PROJECTILES,
	MC_TANKS,
	MC_PLANETS,
	MC_PLAYERS,
	MC_SERVER_INFO,
	MC_4BYTE_SETS,
	MC_2BYTE_SETS,
	MC_WORLD,
	MC_EMPTY
};

enum MessageType
{
	MC_NONE = 1024,
	
	MT_SERVER_AD,
	MT_SERVER_REFRESH,

	MT_HANDSHAKE,
	MT_HANDSHAKE_REQUEST,

	MT_SERVER_CLOSING,
	MT_CLIENT_CONNECT,
	MT_CONNECTION_ACCEPTED,
	MT_CONNECTION_REFUSED,

	MT_ASSIGN_PLAYER,

	MT_SYNC_PLAYERS,
	MT_SYNC_WORLD
};

typedef struct MessageHeader
{
	MessageType type;
	MessageContent content;
	unsigned int iid;
	unsigned int len;
	unsigned int timestamp;
} MessageHeader;

typedef struct _ServerInfo
{
	char name[1024];
	unsigned int server_packetsz;
	unsigned int num_players;
	unsigned int max_players;
	unsigned int iid;
} _ServerInfo;

typedef struct ContentIndex
{
	unsigned short tank_offset, planet_offset, projectile_offset;
} ContentIndex;

typedef int NetID;
typedef std::map<unsigned int, std::pair<IPaddress, _ServerInfo> > ServerInfoMap;
typedef std::tr1::unordered_map<void*, NetID> M2IDTable;
typedef std::tr1::unordered_map<NetID, void*> ID2MTable;

//-------------------------- Global variables -----------------------------
extern const std::string default_server_name;
extern Player* local_player;
extern World* current_world;

extern const char* handshake;

extern UDPsocket sock;
extern int serverchan;
extern UDPpacket* packet;

extern bool serverMode;
extern std::vector<Player*> players;
extern std::map<unsigned int, int> clients;
extern std::map<unsigned int, int> pending_clients;
extern unsigned int current_packetsz;
extern unsigned int instance_id;
extern ServerInfoMap servers;
extern M2IDTable mem2id;
extern ID2MTable id2mem;
extern NetID player_id;
//---------------------------------------------------------------------------

NetID NextEntityID();
void* PrepareHeader(MessageHeader* header, MessageType type, MessageContent content, unsigned int datalen);
void ParseHeader(void* data, MessageHeader& header);
void SetPacketSize(unsigned int size);
void SendMessage ( unsigned int channel, MessageHeader* header );
void AcceptClient(unsigned int instance_id, IPaddress* address);
void RemoveClient(unsigned int instance_id);
IPaddress ResolveHost ( std::string host, unsigned short port );
void SpawnOrSync(NetID id, EntityType type, void* entity);

// Channel mgmt
void ActivateBroadcast(unsigned int chan);
//static void SwitchPort(unsigned int chan, unsigned short newport);
unsigned int BindAddress(IPaddress& ip) ;
void RebindChannel(unsigned int chan, IPaddress& ip);
void UnbindChannel ( unsigned int channel );

void RegisterEntity(void* entity);
void UnRegisterEntity(void* entity);

#endif
