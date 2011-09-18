
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;

void CreateEngines(Nc::Engine::Manager *manager)
{
    Nc::Graphic::Engine *graphic = new Nc::Graphic::Engine(manager, (Nc::Graphic::Engine::CreateWindowFunc)&GameEngine::CreateWindow);
    graphic->LimitFrameRate(60);
    manager->AddEngine("Graphic", graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    GameEngine *game = new GameEngine(graphic, manager);
    game->LimitFrameRate(60);
    manager->AddEngine("Main", game);
    LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << endl;
}

int main()
{
  Nc::Engine::Manager GUITest("3dNovac.conf");
  CreateEngines(&GUITest);
  GUITest.Start();
  GUITest.Wait();
  return 0;
}
