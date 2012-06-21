
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Nc::Engine::Manager &manager)
{
	// creation du graphicEngine
	Graphic::Engine *graphicEngine = new Graphic::Engine(FBEffectsDemo::GameEngine::ClassName(), &manager, (Graphic::Engine::CreateWindowFunc)&FBEffectsDemo::GameEngine::CreateWindow);
	//graphicEngine->LimitFrameRate(60);
	manager.AddEngine(graphicEngine);
	LOG << "Creation of the " << *graphicEngine << "\t\tDONE" << std::endl;

	// creation du gameEngine
	FBEffectsDemo::GameEngine *gameEngine = new FBEffectsDemo::GameEngine(&manager, graphicEngine);
	gameEngine->LimitFrameRate(60);
	manager.AddEngine(gameEngine);
	LOG << "Creation of the " << *gameEngine << "\t\tDONE" << std::endl;
}

int main()
{
	try
	{
		Nc::Engine::Manager game("3dNovac.conf");

		CreateEngines(game);

		game.Start();
		game.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
	return 0;
}
