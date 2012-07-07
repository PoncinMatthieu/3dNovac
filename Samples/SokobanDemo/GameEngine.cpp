
#include <Nc/Graphics/Scene/SceneGraph.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/Graphics/Object/SkyBox.h>
#include <Nc/Graphics/Effect/DefaultLightingMaterial.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>

#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace SokobanDemo;

GameEngine::GameEngine(Nc::Graphic::IWindow *window, Nc::Graphic::SceneNodeFormatManager *sceneNodeFormatManager, Nc::Engine::Manager *manager)
  : Nc::Contrib::GameEngine(window, manager), _sceneNodeFormatManager(sceneNodeFormatManager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
    delete _scene3d;
    delete _lightingMaterial;
}

void GameEngine::LoadContent()
{
    // create the camera, scene and set it to the SceneGraphManager of the Graphic engine
    _camera = new StandardCamera3d(_window);
    _scene3d = new SceneGraph();
    _scene3d->AddChild(_camera);
    _window->SceneManager()->AddScene(_scene3d);

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
    _map = new Map("Nc:Ressources:Sokoban.map", *_sceneNodeFormatManager);
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
    _scene3d->AddChild(BasicMeshCreator::Axis(1, true, _map->Center()));
    _scene3d->AddChild(BasicMeshCreator::Axis(1, true, true));
}

void GameEngine::Update(float runningTime)
{
    // update the scene (will update it's effect and camera)
    _scene3d->UpdateNode(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
    // resize the camera if needed
    if (event.type == System::Event::Resized)
        _camera->Resized(event);
    MainEngine::ManageWindowEvent(event);
}

void    GameEngine::KeyboardEvent(System::Event &event)
{
    // send the events to the camera
    _camera->KeyboardEvent(event);

    if (event.type == System::Event::KeyPressed)
    {
        if (event.key.code == System::Key::Escape)
            Quit();
        if (event.key.code == System::Key::Up || event.key.code == System::Key::Down ||
            event.key.code == System::Key::Right || event.key.code == System::Key::Left)
            MovePlayer(event);
        if (event.key.code == System::Key::F2)        // bump mapping
            _lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::BumpMapping);
        #ifdef _DEBUG
        else if (event.key.code == System::Key::F1)        // draw les normal
            _lightingMaterial->Pattern().Trigger(DefaultLightingMaterial::DisplayNormal);
        #endif
        if (event.key.code == System::Key::PageUp)
            _light->Matrix.AddTranslation(0,0,1);
        if (event.key.code == System::Key::PageDown)
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

void    GameEngine::MovePlayer(Nc::System::Event &event)
{
    // get the camera orientation (X or Y) to know how to manage arrow keys
    Vector3f lookAt = _camera->Center() - _camera->Eye();
    if (Math::Abs(lookAt[0]) > Math::Abs(lookAt[1]))
    {
        if (event.key.code == System::Key::Up)
            event.key.code = (lookAt[0] >= 0) ? System::Key::Right : System::Key::Left;
        else if (event.key.code == System::Key::Down)
            event.key.code = (lookAt[0] >= 0) ? System::Key::Left : System::Key::Right;
        else if (event.key.code == System::Key::Right)
            event.key.code = (lookAt[0] >= 0) ? System::Key::Down : System::Key::Up;
        else if (event.key.code == System::Key::Left)
            event.key.code = (lookAt[0] >= 0) ? System::Key::Up : System::Key::Down;
    }
    else
    {
        if (event.key.code == System::Key::Up)
            event.key.code = (lookAt[1] >= 0) ? System::Key::Up : System::Key::Down;
        else if (event.key.code == System::Key::Down)
            event.key.code = (lookAt[1] >= 0) ? System::Key::Down : System::Key::Up;
        else if (event.key.code == System::Key::Right)
            event.key.code = (lookAt[1] >= 0) ? System::Key::Right : System::Key::Left;
        else if (event.key.code == System::Key::Left)
            event.key.code = (lookAt[1] >= 0) ? System::Key::Left : System::Key::Right;
    }

    if (event.key.code == System::Key::Up)
        _map->MovePlayerUp();
    else if (event.key.code == System::Key::Down)
        _map->MovePlayerDown();
    else if (event.key.code == System::Key::Right)
        _map->MovePlayerRight();
    else if (event.key.code == System::Key::Left)
        _map->MovePlayerLeft();
}
