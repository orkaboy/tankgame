//#include "net.h"
#include "net_util.h"
#include "net_translations.h"
#include "net_server.h"
#include "net_client.h"

const std::string default_server_name = "MDF Warzone";

Player* local_player = 0;
World* current_world = 0;

const char* handshake = "1337MDF" NET_VER_MAJOR NET_VER_MINOR;

UDPsocket sock;
int serverchan = -1;
UDPpacket* packet;

//static bool hand_shaken = false; //Not used yet?
bool closing = false;
bool serverMode = false;
SDL_Thread* recieveThread = 0;
ServerInfoMap servers;
std::vector<Player*> players;
std::map<unsigned int, int> clients;
std::map<unsigned int, int> pending_clients;
unsigned int current_packetsz = NET_SERVER_PACKET_SIZE;
unsigned int instance_id = 0;

static int Recieve(void* unsed);

unsigned int frameCount = 0;

// API functions
std::vector<ServerInfo> Net_GetAvailableServers()
{
	std::vector<ServerInfo> srvs;
	for ( ServerInfoMap::iterator i = servers.begin(); i != servers.end(); i++ )
	{
		_ServerInfo info = i->second.second;
		ServerInfo realinfo;
		realinfo.iid = info.iid;
		realinfo.max_players = info.max_players;
		realinfo.num_players = info.num_players;
		realinfo.name = std::string(info.name);

		srvs.push_back(realinfo);
	}

	return srvs;
}

// State management
void Net_RegisterEntity(void* entity)
{
	RegisterEntity(entity);
}

void Net_UnRegisterEntity(void* entity)
{
	UnRegisterEntity(entity);
}

// -------------------

void Net_Join(std::string hostname, World& world)
{
	current_world = &world;
	IPaddress srv = ResolveHost(hostname, NET_SERVER_PORT);
	serverchan = SDLNet_UDP_Bind(sock, -1, &srv);

	SendConnectionRequest();
}

void Net_Join(unsigned int iid, World& world)
{
	current_world = &world;
	RebindChannel(serverchan, servers[iid].first);
	SendConnectionRequest();
}

void Net_StartServer(World& map)
{
	serverMode = true;
	Net_ChangeMap(map);
	StartServer();
}

void Net_Init(Player* localPlayer)
{
	current_world = NULL;
	local_player = localPlayer;

	SDLNet_Init();

	instance_id = time(0) % UINT_MAX;

	SetPacketSize(NET_SERVER_PACKET_SIZE);

	StartClient();
}

void Net_Sync()
{
	SendSyncWorld();
}

void Net_Update()
{
	//if ( serverMode )
	Recieve(0);

	frameCount = (frameCount + 1) % UINT_MAX;

	if ( serverMode && frameCount % NET_FRAMES_PER_SYNC == 0 )
		Net_Sync();
}

void Net_RefreshServers()
{
	ActivateBroadcast(serverchan);
	SendServerRefresh();
}

void Net_Quit()
{
	if ( serverMode )
	{
		for ( std::map<unsigned int, int>::iterator it = clients.begin(); it != clients.end(); it++ )
			SendServerClosing(it->first);
	}

	closing = true;
	SDL_WaitThread(recieveThread, 0);
	SDLNet_UDP_Close(sock);
	SDLNet_Quit();
}

void Net_ChangeMap(World& newMap)
{
	id2mem.clear();
	mem2id.clear();
	
	if ( newMap.projectiles.size() > 0 )
	{
		std::cout << "Net_ChangeMap(): Warning! new map contains active projectiles!" << std::endl;
		newMap.projectiles.clear();
	}

	for ( unsigned int i = 0; i < newMap.tanks.size(); i++ )
		RegisterEntity(newMap.tanks[i]);
	for ( unsigned int i = 0; i < newMap.planets.size(); i++ )
		RegisterEntity(newMap.planets[i]);
	for ( unsigned int i = 0; i < players.size(); i++ )
		RegisterEntity(players[i]);

	current_world = &newMap;
}
static int Recieve(void* unsed)
{
	MessageHeader header;
	while( SDLNet_UDP_Recv(sock, packet) )
	{
		Uint8* dataptr = packet->data;
		
		ParseHeader(dataptr, header);
		dataptr += sizeof(MessageHeader);

		if ( serverMode )
		{
			if ( header.iid == instance_id )
				continue;

			switch ( header.type )
			{
			case MT_HANDSHAKE:
				HandshakeRecieved(&header, dataptr);
				break;

			case MT_CLIENT_CONNECT:
				ClientConnectionRequestRecieved(&header, dataptr);
				break;

			case MT_SERVER_REFRESH:
				ServerRefreshRecieved(&header, dataptr, packet->address);
			default:
				continue;
			}
		}
		else // Client msgs
		{
			switch ( header.type )
			{
			case MT_SERVER_AD:
				ServerAdRecieved(&header, dataptr, packet->address);
				break;

			case MT_HANDSHAKE_REQUEST:
				HandshakeRequestRecieved(&header, dataptr);
				break;

			case MT_CONNECTION_ACCEPTED:
				ConnectionAcceptedRecieved(&header, dataptr);
				break;

			case MT_SERVER_CLOSING:	
				ServerClosingRecieved(&header, dataptr);
				break;

			case MT_CONNECTION_REFUSED:
				ConnectionRefusedRecieved(&header, dataptr);
				break;
			case MT_SYNC_WORLD:
				SyncWorldRecieved(&header, dataptr);
				break;
			default:
				continue;
			}
		}
	}

	return 0;
}
