
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Nc::Engine::Manager &manager)
{
  // creation du graphicEngine
  Graphic::Engine *graphic = new Graphic::Engine(&manager, (Graphic::Engine::CreateWindowFunc)&GameEngine::CreateWindow);
  graphic->LimitFrameRate(60);
  manager.AddEngine("Graphic", graphic);
  LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << std::endl;

  // creation du gameEngine
  GameEngine *gameEngine = new GameEngine(&manager, graphic);
  gameEngine->LimitFrameRate(60);
  manager.AddEngine("Main", gameEngine);
  LOG << "Creation of " << *gameEngine << "\t\t\t\t\tDONE" << std::endl;
  LOG << "-----------GameManager-ENGINES-CREATION-SUCESS--------------" << std::endl;
}

int main()
{
	try
	{
	  Nc::Engine::Manager bsplines("3dNovac.conf");

	  CreateEngines(bsplines);

	  bsplines.Start();
	  bsplines.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
  return 0;
}
