#include "ereliaServer.h"

class MainApplication : public jgl::Abstract::Widget::NoGraphics 
{
private:
	NodeManager<2>* _nodeManager;

	jgl::Bool _onUpdate()
	{
		return (false);
	}

public:
	MainApplication() : jgl::Abstract::Widget::NoGraphics("MainApplication")
	{
		_nodeManager = addChildren<NodeManager<2>>(SERVER_PORT);
		_nodeManager->activate();

		_nodeManager->setNode(new OnlineNodeHandler("World node", "127.0.0.1", WORLD_PORT), WORLD_ID);
		_nodeManager->setNode(new OnlineNodeHandler("Account node", "127.0.0.1", ACCOUNT_PORT), ACCOUNT_ID);
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