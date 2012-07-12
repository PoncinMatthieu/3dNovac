
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;

void CreateEngines(Engine::Manager *manager, Graphic::Window &window, Graphic::SceneNodeFormatManager &sceneNodeformatManager)
{
    Graphic::Engine *graphic = new Graphic::Engine(&window, manager);
//    graphic->LimitFrameRate(60);
    manager->AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    LightDemo::GameEngine *game = new LightDemo::GameEngine(&window, &sceneNodeformatManager, manager);
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
        Engine::Manager lightTest("3dNovac.conf");
        Graphic::Window window("Light Demo", winSize, style, "Nc:Image:logo/logoNcTransparent.png", 3);
        Graphic::SceneNodeFormatManager sceneNodeformatManager;

        CreateEngines(&lightTest, window, sceneNodeformatManager);
        lightTest.Start();
        lightTest.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
    return 0;
}
