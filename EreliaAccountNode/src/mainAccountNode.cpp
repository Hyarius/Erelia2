#include "ereliaAccountNode.h"

class MainApplication : public jgl::Abstract::Widget::NoGraphics
{
private:
	Server* _server;
	ServerManager* _serverManager;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

public:
	MainApplication() : jgl::Abstract::Widget::NoGraphics("MainApplication")
	{
		_server = new Server(ACCOUNT_PORT);
		_server->setUnknowMessageActivityFunction([&](Connection* p_connection, Message& p_msg) {
				Message awnser;

				awnser.copyHeaderData(p_msg);
				awnser << std::string("Message from accountNode");

				p_connection->send(awnser);
			});
		_server->setLoginFunction([&](Connection*p_client){jgl::cout << "New connection [" << p_client->id() << "] - The main node is connected" << std::endl;});
		_server->start();

		_serverManager = jgl::Abstract::Application::Core::instance()->addRootWidget<ServerManager>("Central node manager");
		_serverManager->setServer(_server);
		_serverManager->activate();
	}
};

int main(int argc, char** argv)
{
	jgl::cout << "---=== ACCOUNT CONSOLE ===---" << std::endl;
	jgl::Application::Console app = jgl::Application::Console();

	MainApplication* mainApp = app.addRootWidget<MainApplication>();
	mainApp->activate();

	return (app.run());
}