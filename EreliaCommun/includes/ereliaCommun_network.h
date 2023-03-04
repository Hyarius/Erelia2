#pragma once

#include "jgl2.h"

#define SERVER_PORT 26500
#define ACCOUNT_PORT 24500
#define WORLD_PORT 25500

enum class ServerMessage
{
	WorldMessage = 42,
	WorldAwnser = 84,
	AccountMessage
};

#define ACCOUNT_ID 1
#define WORLD_ID 2

using Connection = jgl::Network::Connection<ServerMessage>;
using Message = jgl::Network::Message<ServerMessage>;
using Server = jgl::Network::Server<ServerMessage>;
using Client = jgl::Network::Client<ServerMessage>;

using Node = jgl::Network::Node<ServerMessage>;

using NodeHandler = jgl::Abstract::Network::NodeHandler<ServerMessage>;
using SinglethreadNodeHandler = jgl::Network::SinglethreadNodeHandler<ServerMessage>;
using MultithreadNodeHandler = jgl::Network::MultithreadNodeHandler<ServerMessage>;
using OnlineNodeHandler = jgl::Network::OnlineNodeHandler<ServerMessage>;

using NodeManager = jgl::Widget::NodeManager<ServerMessage>;

using ServerManager = jgl::Widget::ServerManager<ServerMessage>;
using ClientManager = jgl::Widget::ClientManager<ServerMessage>;