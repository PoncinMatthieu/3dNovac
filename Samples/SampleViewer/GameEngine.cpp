
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;

GameEngine::GameEngine(Graphic::Engine *graphic, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(graphic, manager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
  _graphic->GetSceneManager()->RemoveScene(_scene);
  delete _scene;
}

void GameEngine::CreateWindow(Window *win)
{
    unsigned long   pattern = Window::Titlebar | Window::Closeable | Window::Resizeable;
    Vector2ui       winSize(1360, 768);

    win->Create("Sample Viewer", winSize, pattern, "Nc:Image:icone.png", 3);
}

void GameEngine::LoadContent()
{
    AddInput(_graphic->GetWindow()->GetInput());

    // creation de la scene
    _scene = new GUI::SceneGraph();
    _graphic->GetSceneManager()->AddScene(_scene);
    _scene->AddChild(new Camera2d());

    // GUI :
    _menu = new MainMenu(_scene);
    //_scene->AddChild(new GUI::FPSWidget());
    _console = new GUI::Console();
    //_scene->AddChild(_console);

    // no need to active/disable the context at each loop
    //_pattern.Disable(Nc::Engine::HasAContext);
}

void GameEngine::Update(float runningTime)
{
    _scene->UpdateNode(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
    bool send = true;
    if (event.Type == System::Event::KeyPressed)
    {
        if (event.Type == System::Event::KeyPressed && event.Key.Code == System::Key::Escape)
            Quit();
        else if (event.Key.Code == System::Key::Tilde) // Tilde pour afficher/cacher la console
        {
            if (!_console->Focus())
            {
                _scene->Focus(_console);
                send = false;
            }
            else
                _scene->Unfocus(_console);
        }
    }
    // send les evenements au gameManager (celui ci les dispatch a la GUI et au fonction Keybord/MouseEvent)
    if (send)
        MainEngine::ManageWindowEvent(event);
    _scene->ManageWindowEvent(event);
}

void GameEngine::KeyboardEvent(System::Event &)
{
}

void GameEngine::MouseButtonEvent(System::Event &)
{
}

void GameEngine::MouseMotionEvent(System::Event &)
{
}
