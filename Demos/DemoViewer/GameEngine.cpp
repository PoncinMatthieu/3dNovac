
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace DemoViewer;

GameEngine::GameEngine(Nc::Graphic::IWindow *window, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(window, manager)
{
    _demoFactory = new DemoFactory(manager);

    AddNewCmd(GUI::Event::Toggled,          (Nc::Engine::CmdFunction)&GameEngine::StartDemoCmd);
    AddNewCmd(GUI::Event::ItemChanged,      (Nc::Engine::CmdFunction)&GameEngine::DemoSelectedCmd);
    AddNewCmd(GUI::Event::ReturnPressed,    (Nc::Engine::CmdFunction)&GameEngine::ConsolePushCommandCmd);
}

GameEngine::~GameEngine()
{
	delete _demoFactory;
}

void GameEngine::ReleaseContent()
{
    delete _scene;
	delete _menu;
}

void GameEngine::LoadContent()
{
	// creation de la scene
    _scene = new GUI::SceneGraph(_window);
    _window->SceneManager()->AddScene(_scene);
    _scene->AddChild(new Camera2d(_window));

    // GUI :
    _menu = new MainMenu(_scene);

    for (std::list<std::string>::const_iterator it = _demoFactory->DemoNames().begin(); it != _demoFactory->DemoNames().end(); ++it)
        _menu->AddDemo(*it);
    _menu->DemoSelected();
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

void GameEngine::DemoSelectedCmd(Nc::Engine::IEvent *)
{
    _menu->DemoSelected();
}

void GameEngine::StartDemoCmd(Nc::Engine::IEvent *e)
{
    if (!_menu->Demo())
        throw Utils::Exception("StartDemoCmd", "No demo selected.");

    LOG << "Start demo: " << *_menu->Demo() << std::endl;

    // close the previous sub window
    _menu->CloseDemoWindow();

    // delete the previous demo game engine
    if (!_currentDemo.empty())
        _manager->RemoveEngine(_currentDemo, true);

    // create the widget sub window
    GUI::SubWindow *w = _menu->CreateDemoWindow(_window);

    // create the new demo game engine
    Contrib::GameEngine *engine = _demoFactory->CreateDemo(w->GetSubWindow(), *_menu->Demo());
    if (engine == NULL)
        throw Utils::Exception("StartDemoCmd", "Failed to create the new GameEngine.");

    // init the demo game engine
    engine->LimitFrameRate(60);
    _manager->AddEngine(engine);

    // start the demo game engine
    _currentDemo = engine->ResolvedClassName();
    engine->Start();

	// wait the loading and enable the sub window for rendering
	_manager->WaitEnginesLoading();
	w->EnableSubWindow();
}

void    GameEngine::ConsolePushCommandCmd(Nc::Engine::IEvent *e)
{
    if (e->GetData<Nc::GUI::Widget*>() == _menu->GetConsole())
        _menu->ExecConsoleCommand();
}



