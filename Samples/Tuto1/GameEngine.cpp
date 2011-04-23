
#include <Nc/Graphics/Scene/Basic3dSceneGraph.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>

#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;

GameEngine::GameEngine(Nc::Graphic::Engine *graphic, Nc::Engine::Manager *manager)
  : Nc::Engine::MainEngine(manager), _graphic(graphic)
{
}

GameEngine::~GameEngine()
{
  // delete the objects of the scene, and delete it
  _scene3d->Clear(true);
  delete _scene3d;
}

void	GameEngine::CreateWindow(Window *win)
{
  // create the window
  win->Create("Tuto 1", Vector2i(800, 600), Window::Titlebar | Window::Closeable | Window::Resizeable, "Nc:Image:icone.png", 3);

  // add the window Input to the Inputs of the engine
  AddInput(win->GetInput());
}

void	GameEngine::LoadContent()
{
  // create the camera, scene and set it to the SceneGraphManager of the Graphic engine
  _camera = new StandardCamera3d(_graphic->GetWindow());
  _scene3d = new Basic3dSceneGraph(_camera);
  _graphic->GetSceneManager().Set3dSceneGraph(_scene3d);

  // create a new repere to display at the center of the scene
  _scene3d->AddObject(BasicMeshCreator::Repere(1));
}

void	GameEngine::Update(float runningTime)
{
  // update the scene (will update it's effect and camera)
  _scene3d->Update(runningTime);
}

void	GameEngine::ManageWindowEvent(System::Event &event)
{
  // resize the camera if needed
  if (event.Type == System::Event::Resized)
    _camera->Resized(event);
  MainEngine::ManageWindowEvent(event);
}

void	GameEngine::KeyboardEvent(System::Event &event)
{
  // send the events to the camera
  _camera->KeyboardEvent(event);
}

void	GameEngine::MouseButtonEvent(System::Event &event)
{
  // send the events to the camera
  _camera->MouseButtonEvent(event);
}

void	GameEngine::MouseMotionEvent(System::Event &event)
{
  // send the events to the camera
  _camera->MouseMotionEvent(event);
}
