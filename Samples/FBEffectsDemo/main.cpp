
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Nc::Engine::Manager &manager)
{
  // creation du graphicEngine
  Graphic::Engine *graphicEngine = new Graphic::Engine(&manager, (Graphic::Engine::CreateWindowFunc)&GameEngine::CreateWindow);
  //graphicEngine->LimitFrameRate(60);
  manager.AddEngine("Graphic", graphicEngine);
  LOG << "Creation of the " << *graphicEngine << "\t\tDONE" << std::endl;

  // creation du gameEngine
  GameEngine *gameEngine = new GameEngine(&manager, graphicEngine);
  gameEngine->LimitFrameRate(60);
  manager.AddEngine("Main", gameEngine);
  LOG << "Creation of the " << *gameEngine << "\t\tDONE" << std::endl;
}

int main()
{
  Nc::Engine::Manager game("3dNovac.conf");

  CreateEngines(game);

  game.Start();
  game.Wait();
  return 0;
}
