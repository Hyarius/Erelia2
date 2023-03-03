#pragma once

#include "jgl2.h"

#include "ereliaCommun_network.h"

class Node
{
private:

	virtual Message _treatMessage(Message& p_msg) = 0;

public:
	Message treatMessage(Message& p_msg)
	{
		Message awnser = _treatMessage(p_msg);

		awnser.header.emiterID = p_msg.header.emiterID;
		for (jgl::Size_t i = 0; i < MESSAGE_FREE_SPACE; i++)
			awnser.header.sparedSpace[i] = p_msg.header.sparedSpace[i];

		return (awnser);
	}
};

namespace Abstract
{
	class NodeHandler
	{
	protected:
		jgl::LockedQueue<Message> _awnserReady;

	public:
		jgl::LockedQueue<Message>& awnserReady() { return (_awnserReady); }
		virtual void emitMessage(Message& p_msg) = 0;
	};
}

class SinglethreadNodeHandler : public Abstract::NodeHandler
{
private:
	Node* _node;

public:
	SinglethreadNodeHandler() : Abstract::NodeHandler(),
		_node(nullptr)
	{

	}

	void setNode(Node* p_node)
	{
		_node = p_node;
	}

	void emitMessage(Message& p_msg)
	{
		_awnserReady.push_back(_node->treatMessage(p_msg));
	}
};

class MultithreadNodeHandler : public Abstract::NodeHandler
{
private:
	Node* _node;
	jgl::WorkerPool _workerPool;

	std::function<void(Message& p_msg)> _emitMessageLambda = [&](Message& p_msg) {
		_awnserReady.push_back(_node->treatMessage(p_msg));
	};

public:
	MultithreadNodeHandler(std::string p_nodeName, jgl::Size_t p_nbThread = 1) : Abstract::NodeHandler(),
		_node(nullptr),
		_workerPool(p_nodeName, p_nbThread)
	{

	}

	void setNode(Node* p_node)
	{
		_node = p_node;
	}

	void emitMessage(Message& p_msg)
	{
		_workerPool.addJob(_emitMessageLambda, p_msg);
	}
};

class OnlineNodeHandler : public Abstract::NodeHandler
{
private:
	ClientManager* _nodeManager;
	Client* _nodeClient;

public:
	OnlineNodeHandler(std::string p_nodeName, std::string p_address, jgl::Int p_serverPort) : Abstract::NodeHandler()
	{
		_nodeClient = new Client();
		_nodeClient->connect(p_address, p_serverPort);
		_nodeClient->setUnknowMessageActivityFunction([&](Message& p_msg) {
				jgl::cout << "Adding awnser from node " << std::endl << p_msg << std::endl;
				_awnserReady.push_back(p_msg);
			});

		_nodeManager = jgl::Abstract::Application::Core::instance()->addRootWidget<ClientManager>(p_nodeName + " handler");
		_nodeManager->setClient(_nodeClient);
		_nodeManager->activate();
	}

	void emitMessage(Message& p_msg)
	{
		jgl::cout << "Emiting a message to the node" << std::endl << p_msg << std::endl;
		_nodeClient->send(p_msg);
	}
};

template <jgl::Size_t C_NB_NODE>
class NodeManager : public jgl::Abstract::Widget::NoGraphics
{
private:
	Server* _inputServer;
	ServerManager* _inputServerManager;
	Abstract::NodeHandler* _nodeHandler[C_NB_NODE + 1];

	jgl::Bool _onUpdate()
	{
		for (jgl::Size_t i = 0; i < C_NB_NODE + 1; i++)
		{
			if (_nodeHandler[i] != nullptr)
			{
				auto& tmpAwnserArray = _nodeHandler[i]->awnserReady();
				while (tmpAwnserArray.empty() == false)
				{
					Message awnser = tmpAwnserArray.pop_front();
					jgl::cout << "Reading awnser " << std::endl << awnser << std::endl;

					jgl::Long clientID = awnser.header.sparedSpace[CONNECTION_BYTE];
					jgl::cout << "Sending message back to : " << clientID << std::endl;
					Connection* connection = _inputServer->connection(clientID);
					connection->send(awnser);

					jgl::cout << " ---- SENDING BACK AWNSER TO CLIENT ---- " << std::endl << std::endl << std::endl;
				}
			}
		}

		return (false);
	}

public:
	NodeManager(jgl::Size_t p_serverPort) : jgl::Abstract::Widget::NoGraphics("NodeManager")
	{
		_inputServer = new Server(p_serverPort);
		_inputServer->setUnknowMessageActivityFunction([&](Connection* p_connection, Message& p_msg) {
				jgl::cout << "Central server received a new message from " << p_connection->id() << std::endl;
				jgl::Size_t nodeId = p_msg.header.sparedSpace[NODE_ID_BYTE];
				p_msg.header.emiterID = p_connection->id();
				p_msg.header.sparedSpace[CONNECTION_BYTE] = p_connection->id();

				jgl::cout << "Retrieving connection : " << _inputServer->connection(p_connection->id()) << std::endl;
				
				_nodeHandler[nodeId]->emitMessage(p_msg);
			});
		_inputServer->start();

		_inputServerManager = jgl::Abstract::Application::Core::instance()->addRootWidget<ServerManager>("Central node manager");
		_inputServerManager->setServer(_inputServer);
		_inputServerManager->activate();
	}

	void setNode(Abstract::NodeHandler* p_node, jgl::Size_t p_nodeID)
	{
		_nodeHandler[p_nodeID] = p_node;
	}
};