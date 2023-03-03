#include "ereliaClient.h"

class MainApplication : public jgl::Abstract::Widget::Core
{
private:
	Client* _client;
	ClientManager* _clientManager;

	jgl::Bool _onUpdate()
	{
		if (jgl::Application::Graphical::instance()->keyboard()->getKey(jgl::Keyboard::A) == jgl::InputStatus::Released)
		{
			Message tmp(ServerMessage::WorldMessage);
			tmp.header.sparedSpace[0] = WORLD_ID;
			tmp.header.sparedSpace[1] = 0;
			tmp.header.sparedSpace[2] = 0;
			tmp.header.sparedSpace[3] = 0;

			_client->send(tmp);
		}
		if (jgl::Application::Graphical::instance()->keyboard()->getKey(jgl::Keyboard::Z) == jgl::InputStatus::Released)
		{
			Message tmp(ServerMessage::AccountMessage);
			tmp.header.sparedSpace[0] = ACCOUNT_ID;
			tmp.header.sparedSpace[1] = 0;
			tmp.header.sparedSpace[2] = 0;
			tmp.header.sparedSpace[3] = 0;

			_client->send(tmp);
		}
		return (false);
	}

	void _onRender()
	{

	}

	void _onGeometryChange()
	{

	}

public:
	MainApplication() : jgl::Abstract::Widget::Core("MainApplication")
	{
		_client = new Client();
		_client->setUnknowMessageActivityFunction([&](Message& p_msg) {

			std::string message = p_msg.pull<std::string>();

			jgl::cout << "Message : " << message << std::endl;
			});
		_client->connect("127.0.0.1", SERVER_PORT);

		_clientManager = jgl::Abstract::Application::Core::instance()->addRootWidget<ClientManager>("ClientManager");
		_clientManager->setClient(_client);
		_clientManager->activate();
	}
};

int main(int argc, char** argv)
{
	jgl::cout << "---=== CLIENT CONSOLE ===---" << std::endl;
	jgl::Application::Graphical app = jgl::Application::Graphical("Erelia", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.setDefaultFont(new jgl::Font("resources/font/Sono-Regular.ttf"));

	MainApplication* mainApp = app.addRootWidget<MainApplication>();
	mainApp->activate();

	return (app.run());
}