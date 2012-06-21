
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;

void CreateEngines(Nc::Engine::Manager *manager)
{
    Nc::Graphic::Engine *graphic = new Nc::Graphic::Engine(SampleViewer::GameEngine::ClassName(), manager, (Nc::Graphic::Engine::CreateWindowFunc)&SampleViewer::GameEngine::CreateWindow);
    graphic->LimitFrameRate(30);
    manager->AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    SampleViewer::GameEngine *game = new SampleViewer::GameEngine(graphic, manager);
    game->LimitFrameRate(30);
    manager->AddEngine(game);
    LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << endl;
}

int main()
{
	try
	{
	  Nc::Engine::Manager GUITest("3dNovac.conf");
	  CreateEngines(&GUITest);
	  GUITest.Start();
	  GUITest.Wait();
	}
	catch (const std::exception &e)
	{
		LOG << "Fatal error: " << e.what() << std::endl;
	}
  return 0;
}
