
#include <Nc/Core/Engine/Manager.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace GUIDemo;

GameEngine::GameEngine(Graphic::Engine *graphic, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(graphic, manager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
  _graphic->GetSceneManager()->RemoveScene(_sceneGUI);
  delete _sceneGUI;
}

void GameEngine::CreateWindow(Window *win)
{
  bool            fullscreen = false;
  unsigned long   pattern = Window::Titlebar | Window::Closeable | Window::Resizeable;
  Vector2ui       winSize(800, 600);

  if (fullscreen)
    {
      pattern |= Window::Fullscreen;
      winSize = Vector2i(1680, 1050);
    }
  win->Create("GUI Tests", winSize, pattern, "Nc:Image:icone.png", 3);
}

void GameEngine::LoadContent()
{
  AddInput(_graphic->GetWindow()->GetInput());

  // creation de la scene
  _sceneGUI = new GUI::SceneGraph(_graphic->GetWindow());
  _graphic->GetSceneManager()->AddScene(_sceneGUI);
  _sceneGUI->AddChild(new Camera2d(_graphic->GetWindow()));

  // GUI :
  _menu = new MainMenu(_sceneGUI);
  _sceneGUI->AddChild(new GUI::FPSWidget());
  _console = new GUI::Console(_graphic->GetWindow());
  _sceneGUI->AddChild(_console);


    // no need to active/disable the context at each loop
    _pattern.Disable(Nc::Engine::HasAContext);
}

void GameEngine::Update(float)
{
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
  bool send = true;
  if (event.type == System::Event::KeyPressed)
    {
      if (event.type == System::Event::KeyPressed && event.key.Code == System::Key::Escape)
	Quit();
      else if (event.key.Code == System::Key::Tilde) // Tilde pour afficher/cacher la console
	{
	  if (!_console->Focus())
	    {
	      _sceneGUI->Focus(_console);
	      send = false;
	    }
	  else
	    _sceneGUI->Unfocus(_console);
	}
    }
  // send les evenements au gameManager (celui ci les dispatch a la GUI et au fonction Keybord/MouseEvent)
  if (send)
    MainEngine::ManageWindowEvent(event);
  _sceneGUI->ManageWindowEvent(event);
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
