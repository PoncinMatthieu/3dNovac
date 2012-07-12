
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Engine::Manager &manager, Graphic::Window &window)
{
    // creation du graphicEngine
    Graphic::Engine *graphic = new Graphic::Engine(&window, &manager);
    graphic->LimitFrameRate(60);
    manager.AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << std::endl;

    // creation du gameEngine
    bSplinesDemo::GameEngine *gameEngine = new bSplinesDemo::GameEngine(&window, &manager);
    gameEngine->LimitFrameRate(60);
    manager.AddEngine(gameEngine);
    LOG << "Creation of " << *gameEngine << "\t\t\t\t\tDONE" << std::endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCESS--------------" << std::endl;
}

int main()
{
    Graphic::WindowStyle    style = Graphic::Window::Titlebar | Graphic::Window::Closeable | Graphic::Window::Resizeable;
    Vector2ui               winSize(800, 600);

	try
	{
        Engine::Manager bsplines("3dNovac.conf");
        Graphic::Window window("bSpline 2d", winSize, style, "Nc:Image:logo/logoNcTransparent.png", 3);

        CreateEngines(bsplines, window);
        bsplines.Start();
        bsplines.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
  return 0;
}
