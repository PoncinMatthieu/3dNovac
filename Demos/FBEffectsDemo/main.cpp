
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Nc::Engine::Manager &manager, Nc::Graphic::Window &window, Graphic::SceneNodeFormatManager &sceneNodeformatManager)
{
	// creation du graphicEngine
	Graphic::Engine *graphicEngine = new Graphic::Engine(&window, &manager);
	//graphicEngine->LimitFrameRate(60);
	manager.AddEngine(graphicEngine);
	LOG << "Creation of the " << *graphicEngine << "\t\tDONE" << std::endl;

	// creation du gameEngine
	FBEffectsDemo::GameEngine *gameEngine = new FBEffectsDemo::GameEngine(&window, &sceneNodeformatManager, &manager);
	gameEngine->LimitFrameRate(60);
	manager.AddEngine(gameEngine);
	LOG << "Creation of the " << *gameEngine << "\t\tDONE" << std::endl;
}

int main()
{
    Graphic::WindowStyle    style = Graphic::Window::Titlebar | Graphic::Window::Closeable | Graphic::Window::Resizeable;
    Vector2ui               winSize(800,600);

	try
	{
		Nc::Engine::Manager game("Nc-Demos.conf");
        Graphic::Window window("FBEffects", winSize, style, "Nc:Image:logo/logoNcTransparent.png", 3);
        Nc::Graphic::SceneNodeFormatManager sceneNodeformatManager;

		CreateEngines(game, window, sceneNodeformatManager);

		game.Start();
		game.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
	return 0;
}
