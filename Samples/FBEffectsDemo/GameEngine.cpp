
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
  _graphic->GetSceneManager()->RemoveScene(_sceneGUI);
  delete _sceneGUI;
  delete _scene3d;
  delete _lightingMaterial;
}

void GameEngine::CreateWindow(Nc::Graphic::Window *win)
{
  // create the window
  win->Create("FBEffects", Vector2ui(800,600), Window::Titlebar | Window::Closeable | Window::Resizeable, "Nc:Image:icone.png", 3);
}

void GameEngine::LoadContent()
{
  // add the window Input to the Inputs of the engine
  AddInput(_graphic->GetWindow()->GetInput());

  // create the scene
  _scene3d = new SceneGraph();
  _graphic->GetSceneManager()->AddScene(_scene3d);

  // create the fb effect
  _fbEffect = new FrameBufferEffect();
  _scene3d->AddChild(_fbEffect);

  // create the camera
  _camera = new StandardCamera3d(_graphic->GetWindow());
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
  _camera->AddChild(BasicMeshCreator::Axis(1));
  _fbEffect->AddChild(_camera);

  // creation du lightingEffect
  _lightingMaterial = new DefaultLightingMaterial();
  LightingEffect *leffect = new LightingEffect(_lightingMaterial);
  _fbEffect->AddChild(leffect);

  // loading de la scene
  Graphic::Object *scene = _sceneNodeFormatManager.Load("Nc:Mesh:scene1/scene1.dae")->As<Graphic::Object>();
  leffect->AddChild(scene);

  // creation d'une light
  _light = new Light(Color(0.5f, 0.5f, 0.5f), 20);
  _light->Matrix.Translation(0, 0, 3);
  leffect->Lights().push_back(_light);
  _camera->AddChild(_light);

  // creation de la gui avec le fps widget
  _sceneGUI = new GUI::SceneGraph();
  _sceneGUI->AddChild(new Camera2d());
  _sceneGUI->AddChild(new GUI::FPSWidget());
  _graphic->GetSceneManager()->AddScene(_sceneGUI);

    // no need to active/disable the context at each loop
//    _pattern.Disable(Nc::Engine::HasAContext);
}

void GameEngine::Update(float runningTime)
{
  _scene3d->UpdateNode(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
  if (event.Type == System::Event::Resized)
    _camera->Resized(event);
  MainEngine::ManageWindowEvent(event);
}

void    GameEngine::KeyboardEvent(System::Event &event)
{
  _camera->KeyboardEvent(event);

  if (event.Type == System::Event::KeyPressed)
    {
      if (event.Key.Code == System::Key::Escape)
	  Quit();
      if (event.Key.Code == System::Key::Space)
	_fbEffect->TriggerPostProcessActivation();
      if (event.Key.Code == System::Key::F2)        // bump mapping
	_lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::BumpMapping);
#ifdef _DEBUG
      else if (event.Key.Code == System::Key::F1)        // draw les normal
	_lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::DisplayNormal);
#endif
    }
}

void    GameEngine::MouseButtonEvent(System::Event &event)
{
  _camera->MouseButtonEvent(event);
}

void    GameEngine::MouseMotionEvent(System::Event &event)
{
  _camera->MouseMotionEvent(event);
}
