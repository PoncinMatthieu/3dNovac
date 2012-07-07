
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace Nc;

void CreateEngines(Engine::Manager &manager, Graphic::Window &window)
{
  // create the graphic engine, by using the CreateWindow member function of our game engine
  Graphic::Engine *graphic = new Graphic::Engine(&window, &manager);
  // limit its frame rate to 60 frame by second, and add the engine to the manager
//  graphic->LimitFrameRate(60);
  manager.AddEngine(graphic);
  LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << std::endl;

  // create our game engine
  SimpleParticleEffect::GameEngine *game = new SimpleParticleEffect::GameEngine(&window, &manager);
  // limit its frame rate to 60 frame by second to avoid to take 100% of the CPU, and add the engine to the manager
  game->LimitFrameRate(60);
  manager.AddEngine(game);
  LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << std::endl;

  LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << std::endl;
}

int main()
{
    Graphic::WindowStyle    style = Graphic::Window::Titlebar | Graphic::Window::Closeable | Graphic::Window::Resizeable;
    Vector2ui               winSize(800, 600);

    try
    {
        // create the game engine with the config file name
        Engine::Manager game("3dNovac.conf");
        Graphic::Window window("Simple Particle Effect", winSize, style, "Nc:Image:icone.png", 3);

        // create the engines
        CreateEngines(game, window);

        // start the engines and wait it
        game.Start();
        game.Wait();
    }
    catch (const std::exception &e)
    {
        LOG << "Fatal error: " << e.what() << std::endl;
    }
    return 0;
}
