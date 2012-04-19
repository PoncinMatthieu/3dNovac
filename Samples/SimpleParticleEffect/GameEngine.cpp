
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include <Nc/Graphics/Object/SkyBox.h>
#include <Nc/Graphics/Effect/DefaultLightingMaterial.h>
#include <Nc/Contrib/Contrib.h>
#include "GameEngine.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace SimpleParticleEffect;

GameEngine::GameEngine(Graphic::Engine *graphic, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(graphic, manager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
    _graphic->GetSceneManager()->RemoveScene(_scene);
    _graphic->GetSceneManager()->RemoveScene(_sceneGUI);
    delete _scene;
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
    win->Create("Simple Particle Effect", winSize, pattern, "Nc:Image:icone.png", 3);
}

void GameEngine::LoadContent()
{
    AddInput(_graphic->GetWindow()->GetInput());

    // creation de la camera
    _camera = new StandardCamera3d(_graphic->GetWindow());
    _camera->AddChild(BasicMeshCreator::Axis(1)); // creation d'un repere

    // creation de la scene
    _scene = new SceneGraph();
    _scene->AddChild(_camera);
    _graphic->GetSceneManager()->AddScene(_scene);

    // create the particle effect
    _particleEffect = new ParticleEffect();
    _particleEffect->Start();
    _scene->AddChild(_particleEffect);

    ParticleEffect *particleEffect2 = new ParticleEffect();
    particleEffect2->Matrix.AddTranslation(5,5,5);
    particleEffect2->Start();
    _scene->AddChild(particleEffect2);

    particleEffect2 = new ParticleEffect();
    particleEffect2->Matrix.AddTranslation(-3,3,3);
    particleEffect2->Start();
    _scene->AddChild(particleEffect2);

    // create the GUI
    CreateGUI();

    // no need to active/disable the context at each loop
    _pattern.Disable(Nc::Engine::HasAContext);
}

void GameEngine::CreateGUI()
{
    // creation de la gui avec le fps widget
    _sceneGUI = new GUI::SceneGraph();
    _sceneGUI->AddChild(new Camera2d());
    _sceneGUI->AddChild(new GUI::FPSWidget());
    _graphic->GetSceneManager()->AddScene(_sceneGUI);
}

void GameEngine::Update(float runningTime)
{
    _scene->UpdateNode(runningTime);
}

void GameEngine::ManageWindowEvent(System::Event &event)
{
    bool send = true;
    if (event.Type == System::Event::Resized)
        _camera->Resized(event);
    if (event.Type == System::Event::KeyPressed)
    {
        if (event.Type == System::Event::KeyPressed && event.Key.Code == System::Key::Escape)
            Quit();
    }
    // send les evenements au gameManager (celui ci les dispatch a la GUI et au fonction Keybord/MouseEvent)
    if (send)
        MainEngine::ManageWindowEvent(event);
}

void GameEngine::KeyboardEvent(System::Event &event)
{
    _camera->KeyboardEvent(event);

    if (event.Type == System::Event::KeyPressed)
    {
        if (event.Key.Code == System::Key::Space)
        {
            if (_particleEffect->Paused())
                _particleEffect->Restart();
            else
                _particleEffect->Pause();
        }
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
