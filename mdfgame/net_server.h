#ifndef NET_SERVER_H
#define NET_SERVER_H

#include "net_util.h"

void StartServer();

// Server msgs
void SendServerAd(unsigned int iid);
void SendHandshakeRequest(unsigned int iid);
void SendConnectionRefused(unsigned int iid);
void SendConnectionAccepted(unsigned int iid);
void SendServerClosing(unsigned int iid);
void SendSyncWorld();
void SendSyncPlayers();
void SendAssignPlayer(unsigned int iid, NetID player_id);

// Server events
void ClientConnectionRequestRecieved(MessageHeader* header, void* dataptr);
void HandshakeRecieved(MessageHeader* header, void* dataptr);
void ServerRefreshRecieved(MessageHeader* header, void* dataptr, IPaddress& ip);

#endif
