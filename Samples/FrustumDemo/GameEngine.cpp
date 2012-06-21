
#include <Nc/Core/Graph/DotGraph.h>
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include <Nc/Graphics/Object/SkyBox.h>
#include <Nc/Graphics/Effect/DefaultLightingMaterial.h>
#include <Nc/Contrib/Contrib.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace FrustumDemo;

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
  _graphic->GetSceneManager()->RemoveScene(_scene3d);
  delete _sceneGUI;
  delete _scene3d;
}

void GameEngine::CreateWindow(Window *win)
{
    bool            fullscreen = false;
    unsigned long   pattern = Window::Titlebar | Window::Closeable | Window::Resizeable;
    Vector2ui       winSize(1600, 600);

    if (fullscreen)
    {
        pattern |= Window::Fullscreen;
        winSize = Vector2i(1680, 1050);
    }
    win->Create("Frustum Demo", winSize, pattern, "Nc:Image:icone.png", 3);
}

void GameEngine::LoadContent()
{
  AddInput(_graphic->GetWindow()->GetInput());

    // creation de la scene
    _scene3d = new SceneGraph();

    // creation de la camera
    _camera = new StandardCamera3d(_graphic->GetWindow(), (_graphic->GetWindow()->Width() / 2.f) / _graphic->GetWindow()->Height(), 0.5, 40);
    _camera->SetViewport(0, 0, _graphic->GetWindow()->Width() / 2.f, _graphic->GetWindow()->Height());
    _scene3d->AddChild(_camera);

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

    // creation de l'octree et des cubes
    _entity = new Entity();
    Generate(_entity, _sceneNodeFormatManager.Load("Nc:Mesh:cube/cube.dae")->As<Graphic::Object>());
    _scene3d->AddChild(_entity);

    // creation de la 2eme camera avec un autre viewport
    _camera2 = static_cast<StandardCamera3d*>(_camera->Clone());
    _camera2->SetProjection((_graphic->GetWindow()->Width() / 2.f) / _graphic->GetWindow()->Height(), 0.1f, 1000, 70);
    _camera2->SetViewport(_graphic->GetWindow()->Width() / 2.f, 0, _graphic->GetWindow()->Width() / 2.f, _graphic->GetWindow()->Height());
    _camera2->InibitMovement();
    _scene3d->AddChild(_camera2);
    // creation d'une troisieme camera pour fake la vue de l'octree
    _camera3 = new StandardCamera3d(_graphic->GetWindow(), (_graphic->GetWindow()->Width() / 2.f) / _graphic->GetWindow()->Height(), 0.5, 40);
    _camera3->InibitMovement();
    _camera3->SetFixState(false);
    _camera3->DrawFrustum(true);
    _scene3d->AddChild(_camera3);
    _scene3d->AddChild(_entity); // ajout d'octree
    _graphic->GetSceneManager()->AddScene(_scene3d);

    // creation de la gui avec le fps widget
    _sceneGUI = new GUI::SceneGraph(_graphic->GetWindow());
    _sceneGUI->AddChild(new Camera2d(_graphic->GetWindow()));
    _sceneGUI->AddChild(new GUI::FPSWidget());
    _graphic->GetSceneManager()->AddScene(_sceneGUI);

    _pattern.Disable(Nc::Engine::HasAContext);

    // no need to active/disable the context at each loop
    _pattern.Disable(Nc::Engine::HasAContext);
}

void GameEngine::Update(float runningTime)
{
    _scene3d->UpdateNode(runningTime);

    // colle la camera3 sur la premiere
    _camera3->Eye(_camera->Eye());
    _camera3->Center(_camera->Center());
    _camera3->Up(_camera->Up());

    ActiveContext();
    _camera3->UpdateViewFrustum();
    DisableContext();
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
    bool send = true;
    if (event.type == System::Event::Resized)
      {
          Window *win = _graphic->GetWindow();
        _camera->SetViewport(0, 0, win->Width() / 2.f, win->Height());
        _camera2->SetViewport(win->Width() / 2.f, 0, win->Width() / 2.f, win->Height());
        _camera3->SetViewport(0, 0, win->Width() / 2.f, win->Height());
      }
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
      if (event.key.code == System::Key::Space)
	{
	  Octree *oct = _entity->Data->As<Octree>();
	  if (oct->DrawOutlines() == false)
	    oct->DrawOutlines(true);
	  else
	    oct->DrawOutlines(false);
	}
    }
}

void GameEngine::MouseButtonEvent(System::Event &event)
{
  _camera->MouseButtonEvent(event);
  _camera2->MouseButtonEvent(event);
  _camera3->MouseButtonEvent(event);
}

void GameEngine::MouseMotionEvent(System::Event &event)
{
    WindowInput *input = static_cast<WindowInput*>(event.emitter);

  if (_camera->InViewport(input->MousePositionInGLCoord()))
    _camera->UninibitMovement();
  else
    _camera->InibitMovement();

  if (_camera2->InViewport(input->MousePositionInGLCoord()))
    _camera2->UninibitMovement();
  else
    _camera2->InibitMovement();

  _camera->MouseMotionEvent(event);
  _camera2->MouseMotionEvent(event);
  _camera3->MouseMotionEvent(event);
}

void GameEngine::Generate(Entity *node, Graphic::Object *obj)
{
  Vector2f	size(40, 40);
  Octree	*oct = new Octree(Box3f(Vector3f(0,0,0),Vector3f(50,50,50)));
  Box3f		box;

  node->Data = oct;
  std::list<std::pair<Box3f,ISceneNode*> > listToInsert;
  for (unsigned int i = 0; i < size[0]; ++i)
    {
      for (unsigned int j = 0; j < size[1]; ++j)
        {
          Graphic::Object *newObj = obj->Clone()->As<Graphic::Object>();
          newObj->Matrix.Translation(i,j,0);
          newObj->GetReelBox(box);
          listToInsert.push_front(std::pair<Box3f,ISceneNode*>(box, newObj));
        }
    }
  obj->Matrix.Translation(42,42,42);
  obj->GetReelBox(box);
  listToInsert.push_front(std::pair<Box3f,ISceneNode*>(box, obj));

  oct->Insert(listToInsert, 16);
  oct->DrawOutlines(true);
}
