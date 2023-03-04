#include "ereliaServer.h"

class MainApplication : public jgl::Abstract::Widget::NoGraphics 
{
private:
	NodeManager* _nodeManager;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

public:
	MainApplication() : jgl::Abstract::Widget::NoGraphics("MainApplication")
	{
		_nodeManager = addChildren<NodeManager>(SERVER_PORT);
		_nodeManager->activate();

		_nodeManager->setNode(new OnlineNodeHandler(WORLD_ID, "World node", "127.0.0.1", WORLD_PORT));
		_nodeManager->setNode(new OnlineNodeHandler(ACCOUNT_ID, "Account node", "127.0.0.1", ACCOUNT_PORT));
	}
};

int main(int argc, char** argv)
{
	jgl::cout << "---=== SERVER CONSOLE ===---" << std::endl;
	jgl::Application::Console app = jgl::Application::Console();

	MainApplication* mainApp = app.addRootWidget<MainApplication>();
	mainApp->activate();

	return (app.run());
}