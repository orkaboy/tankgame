#include "net_client.h"
#include "net_util.h"
#include "net_translations.h"

#include "net.h"

#include <cstring>

void SendServerRefresh()
{
	std::cout << "Looking for servers..." << std::endl;
	MessageHeader header;
	PrepareHeader(&header, MT_SERVER_REFRESH, MC_EMPTY, 0);
	SendMessage(serverchan, &header);
}

void SendConnectionRequest()
{
	MessageHeader header;
	PrepareHeader(&header, MT_CLIENT_CONNECT, MC_EMPTY, 0);
	SendMessage(serverchan, &header);
}

void SendHandshake()
{
	MessageHeader header;
	char* dataptr = (char*)PrepareHeader(&header, MT_HANDSHAKE, MC_STRINGS, strlen(handshake) + 1);
	strcpy(dataptr, handshake);
	SendMessage(serverchan, &header);
}

// --------------------------- Events -------------------------------
void ServerAdRecieved(MessageHeader* header, void* data, IPaddress& ip)
{
	_ServerInfo info;
	if ( header->len >= sizeof(_ServerInfo) )
	{
		memcpy(&info, data, sizeof(_ServerInfo));
		_ServerInfo_Read(&info);

		info.iid = header->iid;

		ServerInfoMap::iterator it = servers.find(header->iid);
		
		if ( it == servers.end() )
			servers[header->iid] = std::pair<IPaddress, _ServerInfo>(ip, info);
	}
}

void ConnectionAcceptedRecieved(MessageHeader* header, void* data)
{
	std::cout << "Successfully connected to server [" << servers[header->iid].second.name << "] !!" << std::endl;

	current_world->player = NULL;
	current_world->tanks.clear();
	current_world->planets.clear();
	current_world->projectiles.clear();
}

void HandshakeRequestRecieved(MessageHeader* header, void* data)
{
	//SwitchPort(ReadUInt32(dataptr)); // See SendHandshakeRequest for problem desc.
	SendHandshake();
}

void ConnectionRefusedRecieved(MessageHeader* header, void* data)
{
	std::cout << "Connection refused from server [" << servers[header->iid].second.name << "]" << std::endl;
	UnbindChannel(serverchan);
}

void ServerClosingRecieved(MessageHeader* header, void* data)
{
	std::cout << "Server is shutting down. Disconnecting" << std::endl;
	UnbindChannel(serverchan);
}

void AssignPlayerRecieved(MessageHeader* header, void* data)
{
	if ( header->content != MC_4BYTE_SETS )
		return;

	int pid = *(int*)data;
	READ32(pid);
	player_id = pid;
	
	current_world->player = (Player*)id2mem[player_id];
}

void SyncPlayersRecieved(MessageHeader* header, void* data)
{
	if ( header->content != MC_PLAYERS )
		return;
}

void SyncWorldRecieved(MessageHeader* header, void* data)
{
	if ( header->content != MC_WORLD )
		return;

	char* start = (char*)data + sizeof(ContentIndex), *ptr;
	ContentIndex index = *((ContentIndex*)data);
	ContentIndex_Read(&index);

	// Planets
	ptr = start + index.planet_offset;
	for ( int i = 0; ptr < start + index.projectile_offset; i++ )
	{
		Net_Planet nplanet;
		memcpy(&nplanet, ptr, sizeof(Net_Planet));
		Net_Planet_Read(&nplanet);
		SpawnOrSync(nplanet.id, ET_PLANET, &nplanet);
		Planet* planet = (Planet*)id2mem[nplanet.id];
		ptr += sizeof(Net_Planet);
	}

	// Tanks
	ptr = start + index.tank_offset;
	for ( int i = 0; ptr < start + index.planet_offset; i++ )
	{
		Net_Tank ntank;
		memcpy(&ntank, ptr, sizeof(Net_Tank));
		Net_Tank_Read(&ntank);
		SpawnOrSync(ntank.id, ET_TANK, &ntank);
		Tank* tank = (Tank*)id2mem[ntank.id];		
		tank->planet = (Planet*)id2mem[ntank.planet_id];
		ptr += sizeof(Net_Tank);
	}

	// Projectiles
	/*ptr = start + index.projectile_offset;
	for ( int i = 0; ptr < start + header->len; i++ )
	{
		Net_Projectile nproj;
		memcpy(&nproj, ptr, sizeof(Net_Projectile));
		Net_Projectile_Read(&nproj);
		SpawnOrSync(nproj.id, ET_PROJECTILE, &nproj);
		Projectile* proj = (Projectile*)id2mem[nproj.id];
		proj->player = (Player*)id2mem[nproj.player_id];
		ptr += sizeof(Net_Projectile);
	}*/
}

void StartClient()
{
	if ( sock ) 
		SDLNet_UDP_Close(sock);
	
	sock = SDLNet_UDP_Open(0);

	IPaddress ip;
	SDLNet_ResolveHost(&ip, "255.255.255.255", NET_SERVER_PORT);
	serverchan = BindAddress(ip);

	assert(sock && "Could not open the server port");

	//recieveThread = SDL_CreateThread(Recieve, 0);
}
