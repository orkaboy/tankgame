#ifndef NET_TRANSLATIONS_H
#define NET_TRANSLATIONS_H

extern long net_nbotemp;
extern ID2MTable id2mem;
extern M2IDTable mem2id;

#define READ32T(fieldandvalue, type) fieldandvalue = (type)SDLNet_Read32(&(fieldandvalue))
#define READ32(fieldandvalue) fieldandvalue = SDLNet_Read32(&(fieldandvalue))
#define WRITE32(fieldandvalue) SDLNet_Write32(fieldandvalue, &fieldandvalue)

#define READF(fieldandvalue) net_nbotemp = SDLNet_Read32(&(fieldandvalue)); \
	fieldandvalue = *(float*)&net_nbotemp
#define WRITEF(fieldandvalue) SDLNet_Write32(*((long*)&fieldandvalue), &fieldandvalue)

#define READ16T(fieldandvalue, type) fieldandvalue = (type)SDLNet_Read16(&(fieldandvalue))
#define READ16(fieldandvalue) fieldandvalue = SDLNet_Read16(&(fieldandvalue))
#define WRITE16(fieldandvalue) SDLNet_Write16(fieldandvalue, &fieldandvalue)

inline void ContentIndex_Write(ContentIndex* index)
{
	WRITE16(index->planet_offset);
	WRITE16(index->projectile_offset);
	WRITE16(index->tank_offset);
}

inline void ContentIndex_Read(ContentIndex* index)
{
	READ16(index->planet_offset);
	READ16(index->projectile_offset);
	READ16(index->tank_offset);
}

inline void _ServerInfo_Write(_ServerInfo* info)
{
	WRITE32(info->server_packetsz);
	WRITE32(info->num_players);
	WRITE32(info->max_players);
}

inline void _ServerInfo_Read(_ServerInfo* info)
{
	READ32(info->server_packetsz);
	READ32(info->num_players);
	READ32(info->max_players);
}

inline void MessageHeader_Read(MessageHeader* header)
{
	READ32T(header->type, MessageType);
	READ32T(header->content, MessageContent);
	READ32(header->len);
	READ32(header->iid);
	READ32(header->timestamp);
}

inline void MessageHeader_Write(MessageHeader* header)
{
	WRITE32(header->content);
	WRITE32(header->len);
	WRITE32(header->type);
	WRITE32(header->iid);
	WRITE32(header->timestamp);
}

typedef struct Net_Projectile
{
	int id;
	float x, y;
	float vx, vy;
	int player_id;
	float r;
	unsigned int type;
	float lifetime;
} Net_Projectile;

inline void Net_Projectile_Write(Net_Projectile *proj)
{
	WRITE32(proj->id);
	WRITEF(proj->x);
	WRITEF(proj->y);
	WRITEF(proj->vx);
	WRITEF(proj->vy);
	WRITE32(proj->player_id);
	WRITEF(proj->r);
	WRITE32(proj->type);
	WRITEF(proj->lifetime);
}

inline void Net_Projectile_Read(Net_Projectile *proj)
{
	READ32(proj->id);
	READF(proj->x);
	READF(proj->y);
	READF(proj->vx);
	READF(proj->vy);
	READ32(proj->player_id);
	READF(proj->r);
	READ32(proj->type);
	READF(proj->lifetime);
}

inline void Projectile2Net ( const Projectile* proj, Net_Projectile* nproj )
{
	nproj->lifetime = proj->LifeTime;
	nproj->x = proj->pos.x;
	nproj->y = proj->pos.y;
	nproj->vx = proj->vel.x;
	nproj->vy = proj->vel.y;
	nproj->type = (unsigned int)proj->type;
	nproj->r = proj->radius;
}

inline void Net2Projectile ( const Net_Projectile* nproj, Projectile* proj )
{
	proj->LifeTime = nproj->lifetime;
	proj->pos = vec2(nproj->x, nproj->y);
	proj->vel = vec2(nproj->vx, nproj->vy);
	proj->type = (ProjectileType)nproj->type;
	proj->radius = nproj->r;
}

typedef struct Net_Tank
{
	int id;
	int player_id;
	float angular;
	float turret;
	int weapon;
	float hitPoints;
	int planet_id;
	char dying, firing;
	/* Padding */
	char align[2];
} Net_Tank;

inline void Net_Tank_Write(Net_Tank *tank)
{
	WRITEF(tank->angular);
	WRITEF(tank->turret);
	WRITE32(tank->weapon);
	WRITEF(tank->hitPoints);
	WRITE32(tank->id);
	WRITE32(tank->planet_id);
	WRITE32(tank->player_id);
}

inline void Net_Tank_Read(Net_Tank *tank)
{
	READF(tank->angular);
	READF(tank->turret);
	READ32(tank->weapon);
	READF(tank->hitPoints);
	READ32(tank->id);
	READ32(tank->planet_id);
	READ32(tank->player_id);
}

inline void Tank2Net ( const Tank* tank, Net_Tank* ntank )
{
	ntank->dying = (char)tank->dying;
	ntank->firing = (char)tank->firing;
	ntank->angular = tank->angular_position;
	ntank->turret = tank->turret_angle;
	ntank->hitPoints = tank->hitPoints;
	ntank->weapon = tank->weapon;
}

inline void Net2Tank ( const Net_Tank* ntank, Tank* tank )
{
	tank->angular_position = ntank->angular;
	tank->dying = ntank->dying == 1;
	tank->firing = ntank->firing == 1;
	tank->hitPoints = ntank->hitPoints;
	tank->weapon = ntank->weapon;
	tank->turret_angle = ntank->turret;
}

typedef struct Net_Planet
{
	float x, y;
	float vx, vy;
	float radius;
	float rot;
	float rotvel;
	int id;
} Net_Planet;

inline void Net_Planet_Write(Net_Planet *planet)
{
	WRITEF(planet->x);
	WRITEF(planet->y);
	WRITEF(planet->vx);
	WRITEF(planet->vy);
	WRITEF(planet->radius);
	WRITEF(planet->rot);
	WRITEF(planet->rotvel);
	WRITE32(planet->id);
}

inline void Net_Planet_Read(Net_Planet *planet)
{
	READF(planet->x);
	READF(planet->y);
	READF(planet->vx);
	READF(planet->vy);
	READF(planet->radius);
	READF(planet->rot);
	READF(planet->rotvel);
	READ32(planet->id);
}

inline void Planet2Net ( const Planet* planet, Net_Planet* nplanet )
{
	nplanet->radius = planet->radius;
	nplanet->rot = planet->rot;
	nplanet->rotvel = planet->rotvel;
	nplanet->x = planet->pos.x;
	nplanet->y = planet->pos.y;
	nplanet->vx = planet->vel.x;
	nplanet->vy = planet->vel.y;
}

inline void Net2Planet ( const Net_Planet* nplanet, Planet* planet )
{
	planet->radius = nplanet->radius;
	planet->pos.x = nplanet->x;
	planet->pos.y = nplanet->y;
	planet->rot = nplanet->rot;
	planet->rotvel = nplanet->rotvel;
	planet->vel.x = nplanet->vx;
	planet->vel.y = nplanet->vy;
}

typedef struct Net_Player
{
	int id;
	int points;
	int kills;
	int deaths;
	unsigned int col;
	int tank_id;
} Net_Player;

inline void Net_Player_Write(Net_Player *player)
{
	WRITE32(player->id);
	WRITE32(player->points);
	WRITE32(player->kills);
	WRITE32(player->deaths);
	WRITE32(player->col);
	WRITE32(player->tank_id);
}

inline void Net_Player_Read(Net_Player *player)
{
	READ32(player->id);
	READ32(player->points);
	READ32(player->kills);
	READ32(player->deaths);
	READ32(player->col);
	READ32(player->tank_id);
}

inline void Player2Net ( const Player* player, Net_Player* nplayer )
{
	nplayer->points = player->points;
	nplayer->col = player->col;
	nplayer->kills = player->kills;
	nplayer->deaths = player->deaths;
}

inline void Net2Player ( const Net_Player* nplayer, Player* player )
{
}

#endif
