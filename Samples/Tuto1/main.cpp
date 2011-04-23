
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Engine::Manager &manager)
{
  // create the graphic engine, by using the CreateWindow member function of our game engine
  Graphic::Engine *graphic = new Graphic::Engine(&manager, (Graphic::Engine::CreateWindowFunc)&GameEngine::CreateWindow);
  // limit its frame rate to 60 frame by second, and add the engine to the manager
  graphic->LimitFrameRate(60);
  manager.AddEngine("Graphic", graphic);
  LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << std::endl;

  // create our game engine
  GameEngine *game = new GameEngine(graphic, &manager);
  // limit its frame rate to 60 frame by second to avoid to take 100% of the CPU, and add the engine to the manager
  game->LimitFrameRate(60);
  manager.AddEngine("Main", game);
  LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << std::endl;

  LOG << "-----------GameManager-ENGINES-CREATION-SUCESS--------------" << std::endl;
}

int main()
{
  // create the game engine with the config file name
  Engine::Manager game("3dNovac.conf");

  // create the engines
  CreateEngines(game);

  // start the engines and wait it
  game.Start();
  game.Wait();
  return 0;
}
