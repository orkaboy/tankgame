#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#include "net_util.h"

void StartClient();

// Client msgs
void SendConnectionRequest();
void SendHandshake();
void SendServerRefresh();

// Client events
void ServerAdRecieved(MessageHeader* header, void* data, IPaddress& ip);
void ServerClosingRecieved(MessageHeader* header, void* data);
void ConnectionRefusedRecieved(MessageHeader* header, void* data);
void ConnectionAcceptedRecieved(MessageHeader* header, void* data);
void HandshakeRequestRecieved(MessageHeader* header, void* data);
void SyncWorldRecieved(MessageHeader* header, void* data);
void SyncPlayersRecieved(MessageHeader* header, void* data);

#endif
