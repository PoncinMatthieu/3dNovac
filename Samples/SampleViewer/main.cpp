
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#ifdef SYSTEM_WINDOWS
    #include <Nc/Graphics/Window/WWindow.h>
#else
    #include <Nc/Graphics/Window/XWindow.h>
#endif
#include "GameEngine.h"

using namespace std;
using namespace Nc;

void CreateEngines(Nc::Engine::Manager *manager, Graphic::Window *win)
{
    Graphic::Engine *graphic = new Graphic::Engine(win, manager);
    graphic->LimitFrameRate(60);
    manager->AddEngine(graphic);
    LOG << "Creation of " << *graphic << "\t\t\t\tDONE" << endl;

    SampleViewer::GameEngine *game = new SampleViewer::GameEngine(win, manager);
    game->LimitFrameRate(30);
    manager->AddEngine(game);
    LOG << "Creation of " << *game << "\t\t\t\t\tDONE" << endl;
    LOG << "-----------GameManager-ENGINES-CREATION-SUCCESS--------------" << endl;
}

int main()
{
    try
    {
        Engine::Manager GUITest("3dNovac.conf");

        unsigned long   pattern = Graphic::Window::Titlebar | Graphic::Window::Closeable | Graphic::Window::Resizeable;
        Vector2ui       winSize(1360, 768);
        #ifdef SYSTEM_WINDOWS
        Graphic::WWindow win("Sample Viewer", winSize, pattern, "Nc:Image:icone.png", 3);
        #else
        Graphic::XWindow win("Sample Viewer", winSize, pattern, "Nc:Image:icone.png", 3);
        #endif

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
