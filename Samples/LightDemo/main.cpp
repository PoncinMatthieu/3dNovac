
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;

void CreateEngines(Nc::Engine::Manager *manager)
{
    Nc::Graphic::Engine *graphic = new Nc::Graphic::Engine(LightDemo::GameEngine::ClassName(), manager, (Nc::Graphic::Engine::CreateWindowFunc)&LightDemo::GameEngine::CreateWindow);
//    graphic->LimitFrameRate(60);
    manager->AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    LightDemo::GameEngine *game = new LightDemo::GameEngine(manager);
    game->LimitFrameRate(60);
    manager->AddEngine(game);
    LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << endl;
}

int main()
{
	try
	{
	  Nc::Engine::Manager lightTest("3dNovac.conf");
	  CreateEngines(&lightTest);
	  lightTest.Start();
	  lightTest.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
  return 0;
}
