
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;

void CreateEngines(Engine::Manager *manager, Graphic::Window &window)
{
    Graphic::Engine *graphic = new Graphic::Engine(&window, manager);
    graphic->LimitFrameRate(60);
    manager->AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    GUIDemo::GameEngine *game = new GUIDemo::GameEngine(&window, manager);
    game->LimitFrameRate(60);
    manager->AddEngine(game);
    LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << endl;
}

int main()
{
    Graphic::WindowStyle    style = Graphic::Window::Titlebar | Graphic::Window::Closeable | Graphic::Window::Resizeable;
    Vector2ui               winSize(800, 600);

	try
	{
        Engine::Manager GUITest("3dNovac.conf");
        Graphic::Window window("GUI Tests", winSize, style, "Nc:Image:logo/logoNcTransparent.png", 3);

        CreateEngines(&GUITest, window);
        GUITest.Start();
        GUITest.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
    return 0;
}
