
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;

int main()
{
    Graphic::WindowStyle    style = Graphic::IWindow::Titlebar | Graphic::IWindow::Closeable | Graphic::IWindow::Resizeable;
    Vector2ui               winSize(1360, 768);

    try
    {
        Engine::Manager manager("3dNovac.conf");
        Graphic::Window window("Sample Viewer", winSize, style, "Nc:Image:icone.png", 3);

		Graphic::Engine *graphic = new Graphic::Engine(&window, &manager);
		graphic->LimitFrameRate(60);
		manager.AddEngine(graphic);

		SampleViewer::GameEngine *game = new SampleViewer::GameEngine(&window, &manager);
		game->LimitFrameRate(30);
		manager.AddEngine(game);

		manager.Start();
        manager.Wait();
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Fatal error: " << e.what() << std::endl;
    }
    Utils::Logger::DeleteInstance();
    return 0;
}
