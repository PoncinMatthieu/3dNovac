
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;

void CreateEngines(Nc::Engine::Manager *manager)
{
    Nc::Graphic::Engine *graphic = new Nc::Graphic::Engine(FrustumDemo::GameEngine::ClassName(), manager, (Nc::Graphic::Engine::CreateWindowFunc)&FrustumDemo::GameEngine::CreateWindow);
//    graphic->LimitFrameRate(60);
    manager->AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    FrustumDemo::GameEngine *game = new FrustumDemo::GameEngine(graphic, manager);
    game->LimitFrameRate(60);
    manager->AddEngine(game);
    LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << endl;
}

int main()
{
	try
	{
	  Nc::Engine::Manager demo("3dNovac.conf");
	  CreateEngines(&demo);
	  demo.Start();
	  demo.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
  return 0;
}
