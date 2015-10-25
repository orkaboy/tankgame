#include "net_server.h"
#include "net_util.h"
#include "net_translations.h"

void SendAssignPlayer(unsigned int iid, NetID player_id)
{
	MessageHeader header;
	NetID* ptr = (NetID*)PrepareHeader(&header, MT_ASSIGN_PLAYER, MC_4BYTE_SETS, sizeof(NetID));
	*ptr = player_id;
	WRITE32(*ptr);

	SendMessage(clients[iid], &header);
}

void SendSyncPlayers()
{
	MessageHeader header;

	if ( clients.size() == 0 )
		return;

	unsigned int size = sizeof(Net_Player) * players.size();

	char* ptr = (char*)PrepareHeader(&header, MT_SYNC_PLAYERS, MC_PLAYERS, size);

    for (unsigned int i = 0; i < players.size(); i++ )
	{
		Player* player = players[i];
		Net_Player nplayer;
		Player2Net(player, &nplayer);

		nplayer.id = mem2id[player];
		nplayer.tank_id = mem2id[player->tank];

		Net_Player_Write(&nplayer);
		memcpy(ptr, &nplayer, sizeof(Net_Player));
		ptr += sizeof(Net_Player);
	}

	for ( std::map<unsigned int, int>::iterator it = clients.begin(); it != clients.end(); it++ )
		SendMessage(it->second, &header);
}

void SendSyncWorld()
{
	MessageHeader header;

	if ( clients.size() == 0 )
		return;

	unsigned int size = current_world->planets.size() * sizeof(Net_Planet) + 
		current_world->tanks.size() * sizeof(Net_Tank) + 
		current_world->projectiles.size() * sizeof(Net_Projectile) +
		sizeof(ContentIndex);

	if ( size + sizeof(MessageHeader) <= current_packetsz )
	{
		unsigned short offset = 0;
		char* dataptr = (char*)PrepareHeader(&header, MT_SYNC_WORLD, MC_WORLD, size);
		
		ContentIndex* index = (ContentIndex*)dataptr;
		dataptr += sizeof(ContentIndex);
		index->planet_offset = offset;
		// Fill planets !
		for ( unsigned int i = 0; i < current_world->planets.size(); i++ )
		{
			
			Net_Planet nplanet;
			Planet* planet = current_world->planets[i];
			Planet2Net(planet, &nplanet);
			nplanet.id = mem2id[planet];

			Net_Planet_Write(&nplanet);
			memcpy(dataptr, &nplanet, sizeof(Net_Planet));

			dataptr += sizeof(Net_Planet);
			offset += sizeof(Net_Planet);
		}
		index->tank_offset = offset;
		// Fill tanks !
		for ( unsigned int i = 0; i < current_world->tanks.size(); i++ )
		{
			
			Net_Tank ntank;
			Tank* tank = (Tank*)current_world->tanks[i];
			Tank2Net(tank, &ntank);

			ntank.id = mem2id[tank];
			ntank.planet_id = mem2id[tank->planet];
			ntank.player_id = player_id;
			
			Net_Tank_Write(&ntank);
			memcpy(dataptr, &ntank, sizeof(Net_Tank));

			dataptr += sizeof(Net_Tank);
			offset += sizeof(Net_Tank);
		}
		index->projectile_offset = offset;
		// Fill projectiles !
		for ( unsigned int i = 0; i < current_world->projectiles.size(); i++ )
		{
			Net_Projectile nproj;
			Projectile* proj = current_world->projectiles[i];
			Projectile2Net(proj, &nproj);
			nproj.id = mem2id[proj];
			nproj.player_id = mem2id[proj->player];

			Net_Projectile_Write(&nproj);
			memcpy(dataptr, &nproj, sizeof(Net_Projectile));

			dataptr += sizeof(Net_Projectile);
			offset += sizeof(Net_Projectile);
		}

		ContentIndex_Write(index);
		packet->len = size;
		for ( std::map<unsigned int, int>::iterator it = clients.begin(); it != clients.end(); it++ )
			SendMessage(it->second, &header);
	}
}

void SendServerAd(unsigned int iid)
{	
	MessageHeader header;
	void* dataptr = PrepareHeader(&header, MT_SERVER_AD, MC_SERVER_INFO, sizeof(_ServerInfo));

	_ServerInfo info;
	info.server_packetsz = current_packetsz;
	info.max_players = NET_SERVER_MAX_PLAYERS;
	info.num_players = players.size();
	strcpy(info.name, default_server_name.c_str());
	_ServerInfo_Write(&info);

	memcpy(dataptr, &info, sizeof(_ServerInfo));
	packet->len = sizeof(MessageHeader) + sizeof(_ServerInfo);
	
	SendMessage(clients[iid], &header);
}

void SendHandshakeRequest(unsigned int iid)
{
	MessageHeader header;
	Uint32* ptr = (Uint32*)PrepareHeader(&header, MT_HANDSHAKE_REQUEST, MC_4BYTE_SETS, 4);

	*ptr = NET_CLIENT_PORT;
	WRITE32(*ptr);

	packet->len = sizeof(Uint32);

	SendMessage(clients[iid], &header);

	//SwitchPort(clients[iid], NET_CLIENT_PORT); // TODO: If the client is to switch ports, the server has to be ready listening on the same port
}

void SendConnectionRefused(unsigned int iid)
{
	MessageHeader header;
	PrepareHeader(&header, MT_CONNECTION_REFUSED, MC_EMPTY, 0);
	SendMessage(clients[iid], &header);
}

void SendConnectionAccepted(unsigned int iid)
{
	MessageHeader header;
	PrepareHeader(&header, MT_CONNECTION_ACCEPTED, MC_EMPTY, 0);
	SendMessage(clients[iid], &header);
}

void SendServerClosing(unsigned int iid)
{
	MessageHeader header;
	PrepareHeader(&header, MT_SERVER_CLOSING, MC_EMPTY, 0);
	SendMessage(clients[iid], &header);
}

// --------------------------------- Events -----------------------------------
void ServerRefreshRecieved(MessageHeader* header, void* dataptr, IPaddress& ip)
{
	AcceptClient(header->iid, &ip);
	SendServerAd(header->iid);
	RemoveClient(header->iid);
}

void HandshakeRecieved(MessageHeader* header, void* dataptr)
{
	// dataptr should be pointing at the handshake string
	if ( header->content == MC_STRINGS )
	{
		if ( strcmp(handshake, (const char*)dataptr) == 0 )
		{
			if ( clients.find(header->iid) == clients.end() )
				clients[header->iid] = pending_clients[header->iid];

			pending_clients.erase(header->iid);

			std::cout << "Client [" << header->iid << "] authenticated with handshake:" << (char*)dataptr << std::endl;
			SendConnectionAccepted(header->iid);

			Player* newplayer = Player_Init();
			newplayer->col = (TankColors)((unsigned int)TANK_NEUTRAL + players.size());
			Tank_Spawn(*current_world, newplayer, current_world->planets[rand() % current_world->planets.size()]);
			RegisterEntity(newplayer);
			RegisterEntity(newplayer->tank);
			players.push_back(newplayer);

			SendSyncPlayers();
			SendAssignPlayer(header->iid, mem2id[newplayer]);
		}
		else
		{
			std::cout << "Client [" << header->iid << "] sent an invalid handshake, refusing connection." << std::endl;
			RemoveClient(header->iid);
			SendConnectionRefused(header->iid);
		}
	}
}

void ClientConnectionRequestRecieved(MessageHeader* header, void* dataptr)
{
	if ( clients.find(header->iid) == clients.end() )
	{
		unsigned int clientchan = SDLNet_UDP_Bind(sock, -1, &packet->address);
		pending_clients[header->iid] = clientchan;
	}

	SendHandshakeRequest(header->iid);
}

void StartServer()
{
	if ( sock )
		SDLNet_UDP_Close(sock);
	
	sock = SDLNet_UDP_Open(NET_SERVER_PORT);
	assert(sock && "Could not open the server port");
	
	players.push_back(current_world->player);
	RegisterEntity(current_world->player);
	player_id = mem2id[current_world->player];

	std::cout << "Server [" << instance_id << "] started on port " << NET_SERVER_PORT << std::endl;	
}
