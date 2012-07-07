
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;

void CreateEngines(Nc::Engine::Manager *manager, Graphic::IWindow *win)
{
    Graphic::Engine *graphic = new Graphic::Engine(win, manager);
    graphic->LimitFrameRate(60);
    manager->AddEngine(graphic);

    SampleViewer::GameEngine *game = new SampleViewer::GameEngine(win, manager);
    game->LimitFrameRate(30);
    manager->AddEngine(game);
}

int main()
{
    Graphic::WindowStyle    style = Graphic::IWindow::Titlebar | Graphic::IWindow::Closeable | Graphic::IWindow::Resizeable;
    Vector2ui               winSize(1360, 768);

    try
    {
        Engine::Manager GUITest("3dNovac.conf");
        Graphic::Window win("Sample Viewer", winSize, style, "Nc:Image:icone.png", 3);

        CreateEngines(&GUITest, &win);
        GUITest.Start();
        GUITest.Wait();
    }
    catch (const std::exception &e)
    {
        LOG << "Fatal error: " << e.what() << std::endl;
    }
    Utils::Logger::DeleteInstance();
    return 0;
}
