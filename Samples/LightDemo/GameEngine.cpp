
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include <Nc/Graphics/Object/SkyBox.h>
#include <Nc/Graphics/Effect/DefaultLightingMaterial.h>
#include <Nc/Contrib/Contrib.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace LightDemo;

GameEngine::GameEngine(Nc::Graphic::IWindow *window, Nc::Graphic::SceneNodeFormatManager *sceneNodeFormatManager, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(window, manager), _sceneNodeFormatManager(sceneNodeFormatManager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
    delete _lightingMaterial;
    delete _scene;
}

void GameEngine::LoadContent()
{
  // creation de la camera
  _camera = new StandardCamera3d(_window);

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
  _camera->AddChild(BasicMeshCreator::Axis(1)); // creation d'un repere

  // creation de la scene
  _scene = new SceneGraph();
  _scene->AddChild(_camera);
  _window->SceneManager()->AddScene(_scene);

  // creation du default lighting Material
  _lightingMaterial = new DefaultLightingMaterial();
  LightingEffect *lightEffect = new LightingEffect(_lightingMaterial);
  _scene->AddChild(lightEffect);

  // chargement des lumieres
  _light = new Light(Color(0.5, 0.5, 0.5), 20);
  Light *light2 = new Light(Color(0.5f, 0.f, 0.f), 10);
  Light *light3 = new Light(Color(0.f, 0.5f, 0.f), 10);
  _light->Matrix.Translation(0,0,1);
  light2->Matrix.Translation(0, 0, 5);
  light3->Matrix.Translation(0, 5, 5);
  _camera->AddChild(_light);
  _scene->AddChild(light2);
  _scene->AddChild(light3);
  lightEffect->Lights().push_back(_light);
  lightEffect->Lights().push_back(light2);
  lightEffect->Lights().push_back(light3);

  lightEffect->AddChild(_sceneNodeFormatManager->Load("Nc:Mesh:scene1/scene1.dae")->As<Graphic::Object>());
}

void GameEngine::Update(float runningTime)
{
  _scene->UpdateNode(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
    bool send = true;
    if (event.type == System::Event::Resized)
        _camera->Resized(event);
    if (event.type == System::Event::KeyPressed)
    {
        if (event.type == System::Event::KeyPressed && event.key.code == System::Key::Escape)
            Quit();
    }
    // send les evenements au gameManager (celui ci les dispatch a la GUI et au fonction Keybord/MouseEvent)
    if (send)
        MainEngine::ManageWindowEvent(event);
}

void GameEngine::KeyboardEvent(System::Event &event)
{
  _camera->KeyboardEvent(event);
  if (event.type == System::Event::KeyPressed)
    {
      if (event.key.code == System::Key::F2)        // bump mapping
        _lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::BumpMapping);
#ifdef _DEBUG
      else if (event.key.code == System::Key::F1)        // draw les normal
        _lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::DisplayNormal);
#endif
    }
}

void GameEngine::MouseButtonEvent(System::Event &event)
{
  _camera->MouseButtonEvent(event);
}

void GameEngine::MouseMotionEvent(System::Event &event)
{
  _camera->MouseMotionEvent(event);
}
