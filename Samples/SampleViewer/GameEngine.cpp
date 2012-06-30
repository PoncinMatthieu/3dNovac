
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace SampleViewer;

GameEngine::GameEngine(Nc::Engine::Manager *manager)
  : Contrib::GameEngine(manager)
{
    _sampleFactory = new SampleFactory(manager);

    AddNewCmd(StartSample,      (Nc::Engine::CmdFunction)&GameEngine::StartSampleCmd);
    AddNewCmd(SampleSelected,   (Nc::Engine::CmdFunction)&GameEngine::SampleSelectedCmd);
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
    delete _scene;
	delete _menu;
}

void GameEngine::CreateWindow(Window *win)
{
    unsigned long   pattern = Window::Titlebar | Window::Closeable | Window::Resizeable;
    Vector2ui       winSize(1360, 768);

    win->Create("Sample Viewer", winSize, pattern, "Nc:Image:icone.png", 3);
    SetWindow(win);
}

void GameEngine::LoadContent()
{
    // creation de la scene
    _scene = new GUI::SceneGraph(_window);
    _window->SceneManager()->AddScene(_scene);
    _scene->AddChild(new Camera2d(_window));

    // GUI :
    _menu = new MainMenu(_scene);

    for (std::list<std::string>::const_iterator it = _sampleFactory->SampleNames().begin(); it != _sampleFactory->SampleNames().end(); ++it)
        _menu->AddSample(*it);
    _menu->SampleSelected();
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
/*
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
*/
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

void GameEngine::SampleSelectedCmd(Nc::Engine::IEvent *)
{
    _menu->SampleSelected();
}

void GameEngine::StartSampleCmd(Nc::Engine::IEvent *e)
{
    if (!_menu->Sample())
        throw Utils::Exception("StartSampleCmd", "No sample selected.");

    LOG << "Start sample: " << *_menu->Sample() << std::endl;

    // close the previous sub window
    _menu->CloseSampleWindow();

    // delete the previous sample game engine
    if (!_currentSample.empty())
        _manager->RemoveEngine(_currentSample, true);

    // create the new sample game engine
    Contrib::GameEngine *engine = _sampleFactory->CreateSample(*_menu->Sample());
    if (engine == NULL)
        throw Utils::Exception("StartSampleCmd", "Failed to create the new GameEngine.");

    // create the widget sub window
    GUI::SubWindow *w = _menu->CreateSampleWindow(_window);

    // init the sample game engine
    engine->LimitFrameRate(60);
    engine->SetWindow(w->GetSubWindow());
    _manager->AddEngine(engine);

    // start the sample game engine
    _currentSample = engine->ResolvedClassName();
    engine->Start();

	// wait the loading and enable the sub window for rendering
	_manager->WaitEnginesLoading();
	w->EnableSubWindow();
}



