#include "net_util.h"

#include "net_translations.h"

long net_nbotemp = 0;
NetID nextID = 0;

M2IDTable mem2id;
ID2MTable id2mem;
NetID player_id;

NetID NextEntityID()
{
	return ++nextID;
}

void RegisterEntity(void* entity)
{
	mem2id[entity] = NextEntityID();
	id2mem[mem2id[entity]] = entity;
}

void UnRegisterEntity(void* entity)
{
	id2mem.erase(mem2id[entity]);
	mem2id.erase(entity);
}

void SpawnOrSync(NetID id, EntityType type, void* entity)
{
	Net_Tank* ntank;
	Net_Projectile* nproj;
	Net_Planet* nplanet;

	Tank* tank;
	Projectile* proj;
	Planet* planet;
	Player* player;
	ID2MTable::iterator it = id2mem.find(id);

	if ( it == id2mem.end() )
	{
		switch ( type )
		{
		case ET_TANK:
			id2mem[id] = tank = new Tank;
			current_world->tanks.push_back(tank);
			break;
		case ET_PROJECTILE:
			id2mem[id] = proj = new Projectile;
			current_world->projectiles.push_back(proj);
			break;
		case ET_PLANET:
			id2mem[id] = new Planet;
			current_world->planets.push_back(planet);
			break;
		case ET_PLAYER:
			id2mem[id] = player = new Player;
			players.push_back(player);
			break;
		}
		it = id2mem.find(id);

		assert(it != id2mem.end() && "Entity creation or mapping failed!!");
	}

	switch ( type )
	{
	case  ET_TANK:
		ntank = (Net_Tank*)entity;
		tank = (Tank*)it->second;
		Net2Tank(ntank, tank);
		break;
	case ET_PROJECTILE:
		nproj = (Net_Projectile*)entity;
		proj = (Projectile*)it->second;
		Net2Projectile(nproj, proj);
		break;
	case ET_PLANET:
		nplanet = (Net_Planet*)entity;
		planet = (Planet*)it->second;
		Net2Planet(nplanet, planet);
		break;
	case ET_PLAYER:
		Net2Player((Net_Player*)entity, (Player*)it->second);
		break;
	}
}

void UnbindChannel ( unsigned int channel )
{
	SDLNet_UDP_Unbind(sock, channel);
}

unsigned int BindAddress ( IPaddress& ip )
{
	return SDLNet_UDP_Bind(sock, -1, &ip);
}

void RebindChannel ( unsigned int chan, IPaddress& ip )
{
	UnbindChannel(chan);
	SDLNet_UDP_Bind(sock, chan, &ip);
}
/*
void SwitchPort ( unsigned int chan, unsigned short newport )
{
	IPaddress ad = *SDLNet_UDP_GetPeerAddress(sock, chan);

	ad.port = newport;
	WRITE16(ad.port);

	RebindChannel(chan, ad);
}
*/

void ActivateBroadcast(unsigned int chan)
{
	IPaddress ad = *SDLNet_UDP_GetPeerAddress(sock, chan);
	ad.host = 0xffffffff;
	RebindChannel(chan, ad);
}

IPaddress ResolveHost ( std::string host, unsigned short port )
{
	IPaddress ret;
	SDLNet_ResolveHost(&ret, host.c_str(), port);

	return ret;
}

void SendMessage ( unsigned int channel, MessageHeader* header )
{
	header->timestamp = time(0) % UINT_MAX;
	memcpy(packet->data, header, sizeof(MessageHeader));
	MessageHeader_Write((MessageHeader*)packet->data);

	packet->len = sizeof(MessageHeader) + header->len;
	int ok = SDLNet_UDP_Send(sock, channel, packet);

	assert(ok > 0 && "Net: Could'nt send packet!");
}

void* PrepareHeader(MessageHeader* header, MessageType type, MessageContent content, unsigned int datalen)
{
	header->type = type;
	header->content = content;
	header->len = datalen;
	header->iid = instance_id;

	return ((char*)packet->data) + sizeof(MessageHeader);
}

void ParseHeader(void* data, MessageHeader& header)
{
	memcpy(&header, data, sizeof(MessageHeader));
	MessageHeader_Read(&header);
}

void AcceptClient(unsigned int iid, IPaddress* address)
{
	IPaddress ip  = *address;
	unsigned int clientchan = SDLNet_UDP_Bind(sock, -1, &ip);
	clients[iid] = clientchan;
}

void RemoveClient(unsigned int iid)
{
	SDLNet_UDP_Unbind(sock, clients[iid]);

	if ( clients.find(iid) != clients.end() )
		clients.erase(clients.find(iid));
}

void SetPacketSize ( unsigned int size )
{
	current_packetsz = size;
	if ( !packet )
		packet = SDLNet_AllocPacket(size);
	else
		SDLNet_ResizePacket(packet, size);

	memset(packet->data, 0, packet->maxlen);
	packet->len = 0;
}
