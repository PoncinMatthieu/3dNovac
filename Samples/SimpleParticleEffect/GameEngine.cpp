
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

GameEngine::GameEngine(Nc::Engine::Manager *manager)
  : Contrib::GameEngine(manager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
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
    SetWindow(win);
}

void GameEngine::LoadContent()
{
    // creation de la camera
    _camera = new StandardCamera3d(_window);
    _camera->AddChild(BasicMeshCreator::Axis(1)); // creation d'un repere

    // creation de la scene
    _scene = new SceneGraph();
    _scene->AddChild(_camera);
    _window->SceneManager()->AddScene(_scene);

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
    _sceneGUI = new GUI::SceneGraph(_window);
    _sceneGUI->AddChild(new Camera2d(_window));
    _sceneGUI->AddChild(new GUI::FPSWidget());
    _window->SceneManager()->AddScene(_sceneGUI);
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
    _sceneGUI->ManageWindowEvent(event);
}

void GameEngine::KeyboardEvent(System::Event &event)
{
    _camera->KeyboardEvent(event);

    if (event.type == System::Event::KeyPressed)
    {
        if (event.key.code == System::Key::Space)
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
