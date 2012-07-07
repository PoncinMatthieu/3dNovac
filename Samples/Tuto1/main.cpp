
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace Nc;

int main()
{
    Graphic::WindowStyle    style = Graphic::Window::Titlebar | Graphic::Window::Closeable | Graphic::Window::Resizeable;
    Vector2ui               winSize(800, 600);

    // create the game engine with the config file name.
    Engine::Manager manager("3dNovac.conf");

    // create the window.
    Graphic::Window window("Tuto 1", winSize, style, "Nc:Image:icone.png", 3);

    // create the graphic engine, limit it's frame rate to 60 frame by second to avoid to take 100% of the CPU
    // and add the engine to the manager.
    Graphic::Engine *graphic = new Graphic::Engine(&window, &manager);
    graphic->LimitFrameRate(60);
    manager.AddEngine(graphic);

    // create our game engine, limit its frame rate, and add the engine to the manager.
    GameEngine *game = new GameEngine(&window, &manager);
    game->LimitFrameRate(60);
    manager.AddEngine(game);

    // start the engines and wait for them to terminate.
    manager.Start();
    manager.Wait();
    return 0;
}
