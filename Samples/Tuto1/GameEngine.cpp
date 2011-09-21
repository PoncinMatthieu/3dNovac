
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include <Nc/Graphics/Object/SkyBox.h>

#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;

GameEngine::GameEngine(Nc::Graphic::Engine *graphic, Nc::Engine::Manager *manager)
  : Nc::Engine::MainEngine(manager), _graphic(graphic)
{
}

GameEngine::~GameEngine()
{
}

void	GameEngine::ReleaseContent()
{
  // delete the scene and it's childs node
  delete _scene3d;
  delete _context;
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
  _scene3d = new SceneGraph();
  _camera = new StandardCamera3d(_graphic->GetWindow());
  _scene3d->AddChild(_camera);
  _graphic->GetSceneManager()->AddScene(_scene3d);


  // chargement de la sky box
  Utils::FileName filenamesSky1[6] =
    {
      ("Nc:Image:sky/space_lf.png"),
      ("Nc:Image:sky/space_rt.png"),
      ("Nc:Image:sky/space_ft.png"),
      ("Nc:Image:sky/space_bk.png"),
      ("Nc:Image:sky/space_dn.png"),
      ("Nc:Image:sky/space_up.png")
    };
  _camera->AddChild(new SkyBox(filenamesSky1));

  // create a new repere to display at the center of the scene
  _scene3d->AddChild(BasicMeshCreator::Repere(1));

  _scene3d->AddChild(_sceneNodeFormatManager.Load("Nc:Mesh:scene1/scene1.dae")->As<Graphic::Object>());
}

void	GameEngine::Update(float runningTime)
{
  // update the scene (will update it's effects and camera)
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
