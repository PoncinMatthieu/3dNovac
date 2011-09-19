
#include <Nc/Graphics/Scene/SceneGraph.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/Graphics/Object/SkyBox.h>
#include <Nc/Graphics/Effect/DefaultLightingMaterial.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>

#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;

GameEngine::GameEngine(Nc::Engine::Manager *manager, Graphic::Engine *graphic)
  : Nc::Contrib::GameEngine(graphic, manager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
  _graphic->GetSceneManager()->RemoveScene(_scene3d);
  delete _scene3d;
  delete _lightingMaterial;
}

void GameEngine::CreateWindow(Nc::Graphic::Window *win)
{
  // create the window
  win->Create("Sokoban", Vector2ui(800,600), Window::Titlebar | Window::Closeable | Window::Resizeable, "Nc:Image:icone.png", 3);
}

void GameEngine::LoadContent()
{
  // add the window Input to the Inputs of the engine
  AddInput(_graphic->GetWindow()->GetInput());

  // create the camera, scene and set it to the SceneGraphManager of the Graphic engine
  _camera = new StandardCamera3d(_graphic->GetWindow());
  _scene3d = new SceneGraph();
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

  // creation du lightingEffect
  _lightingMaterial = new DefaultLightingMaterial();
  LightingEffect *leffect = new LightingEffect(_lightingMaterial);
  _scene3d->AddChild(leffect);

  // creation de la map
  _map = new Map("Nc:Root:Sokoban.map", _sceneNodeFormatManager);
  leffect->AddChild(_map);

  // centre la camera
  _camera->Center(_map->Center());
  _camera->MajEye();

  // creation d'une light
  _light = new Light(Color(0.5f, 0.5f, 0.5f), 20);
  _light->Matrix.Translation(0, 0, 3);
  leffect->Lights().push_back(_light);
  _camera->AddChild(_light);

  // create a new repere to display at the center of the scene
  _scene3d->AddChild(BasicMeshCreator::Repere(1, _map->Center()));
}

void GameEngine::Update(float runningTime)
{
  // update the scene (will update it's effect and camera)
  _scene3d->Update(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
  // resize the camera if needed
  if (event.Type == System::Event::Resized)
    _camera->Resized(event);
  MainEngine::ManageWindowEvent(event);
}

void    GameEngine::KeyboardEvent(System::Event &event)
{
  // send the events to the camera
  _camera->KeyboardEvent(event);

  if (event.Type == System::Event::KeyPressed)
    {
      if (event.Key.Code == System::Key::Escape)
	  Quit();
      if (event.Key.Code == System::Key::Up)
	_map->MovePlayerUp();
      if (event.Key.Code == System::Key::Down)
	_map->MovePlayerDown();
      if (event.Key.Code == System::Key::Right)
	_map->MovePlayerRight();
      if (event.Key.Code == System::Key::Left)
	_map->MovePlayerLeft();
      if (event.Key.Code == System::Key::F2)        // bump mapping
	_lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::BumpMapping);
#ifdef _DEBUG
      else if (event.Key.Code == System::Key::F1)        // draw les normal
	_lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::DisplayNormal);
#endif
      if (event.Key.Code == System::Key::PageUp)
	_light->Matrix.AddTranslation(0,0,1);
      if (event.Key.Code == System::Key::PageDown)
	_light->Matrix.AddTranslation(0,0,-1);
    }
}

void    GameEngine::MouseButtonEvent(System::Event &event)
{
  // send the events to the camera
  _camera->MouseButtonEvent(event);
}

void    GameEngine::MouseMotionEvent(System::Event &event)
{
  // send the events to the camera
  _camera->MouseMotionEvent(event);
}
