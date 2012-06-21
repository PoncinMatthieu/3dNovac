
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace SampleViewer;

GameEngine::GameEngine(Graphic::Engine *graphic, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(graphic, manager)
{
    _sampleFactory = new SampleFactory(graphic, manager);

    AddNewCmd(StartSample,      (Nc::Engine::CmdFunction)&GameEngine::StartSampleCmd);
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
    _scene = new GUI::SceneGraph(_graphic->GetWindow());
    _graphic->GetSceneManager()->AddScene(_scene);
    _scene->AddChild(new Camera2d(_graphic->GetWindow()));

    // GUI :
    _menu = new MainMenu(_scene);

    for (std::list<std::string>::const_iterator it = _sampleFactory->SampleNames().begin(); it != _sampleFactory->SampleNames().end(); ++it)
        _menu->AddSample(*it);


///    // no need to active/disable the context at each loop
    _pattern.Disable(Nc::Engine::HasAContext);
}

void GameEngine::Update(float runningTime)
{
    _scene->UpdateNode(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
    bool send = true;
    if (event.type == System::Event::KeyPressed)
    {
        if (event.type == System::Event::KeyPressed && event.key.code == System::Key::Escape)
            Quit();
        else if (event.key.code == System::Key::Tilde) // Tilde pour afficher/cacher la console
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

void GameEngine::StartSampleCmd(Nc::Engine::IEvent *e)
{
    if (!_menu->Sample())
        throw Utils::Exception("SampleViewer::GameEngine", "No sample selected.");

    LOG << "Start sample: " << *_menu->Sample() << std::endl;

//    Contrib::GameEngine *e = _sampleFactory->CreateSample(*_menu->Sample());

}



