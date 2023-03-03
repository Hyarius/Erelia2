#pragma once

#include "jgl2.h"

#define SERVER_PORT 26500
#define ACCOUNT_PORT 24500
#define WORLD_PORT 25500

enum class ServerMessage
{
	WorldMessage,
	AccountMessage
};

#define ACCOUNT_ID 1
#define WORLD_ID 2

#define NODE_ID_BYTE 0
#define CONNECTION_BYTE 1

using Connection = jgl::Network::Connection<ServerMessage>;
using Message = jgl::Network::Message<ServerMessage>;
using Server = jgl::Network::Server<ServerMessage>;
using Client = jgl::Network::Client<ServerMessage>;
using ServerManager = jgl::Widget::ServerManager<ServerMessage>;
using ClientManager = jgl::Widget::ClientManager<ServerMessage>;