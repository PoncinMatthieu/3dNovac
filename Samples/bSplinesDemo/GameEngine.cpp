
#include <Nc/Core/Engine/Manager.h>
#include <Nc/GUI/Widget/FPSWidget.h>
#include <Nc/GUI/Widget/Console.h>

#include "GameEngine.h"

using namespace Nc;
using namespace Nc::Graphic;
using namespace bSplinesDemo;

GameEngine::GameEngine(Nc::Graphic::Window *window, Nc::Engine::Manager *manager)
  : Contrib::GameEngine(window, manager)
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::ReleaseContent()
{
    delete _scene2d;
}
/*
void GameEngine::CreateWindow(Nc::Graphic::Window *win)
{
    // create the window
    win->Create("bSpline 2d", Vector2ui(800,600), Window::Titlebar | Window::Closeable | Window::Resizeable, "Nc:Image:icone.png", 3);
    SetWindow(win);
}
*/
void GameEngine::LoadContent()
{
  _scene2d = new GUI::SceneGraph(_window);
  _scene2d->AddChild(new Camera2d(_window));
  _window->SceneManager()->AddScene(_scene2d);

  _current = 0;
  CreateSplines1();  // splines et nurbs 1
  CreateSplines2();  // splines et nurbs 2
}

void	GameEngine::ManageWindowEvent(Nc::System::Event &event)
{
    if (event.type == System::Event::Resized)
    {
        LOG << "Resized: " << event.size.width << "\t" << event.size.height << std::endl;
    }


  Nc::Engine::MainEngine::ManageWindowEvent(event);
  _scene2d->ManageWindowEvent(event);
}

void GameEngine::KeyboardEvent(System::Event &event)
{
    if (event.type == System::Event::KeyPressed)
    {
        if (event.type == System::Event::KeyPressed && event.key.code == System::Key::Escape)
            Quit();
        else if (event.key.code == System::Key::Num1)
        {
            _current = 0;
            _spline1->Enable(true);
            _spline2->Enable(false);
        }
        else if (event.key.code == System::Key::Num2)
        {
            _current = 0;
            _spline2->Enable(true);
            _spline1->Enable(false);
        }
        else if (event.key.code == System::Key::Up)
        {
            if (_spline1->Enabled())
            {
                _spline1->Vertex(_current)[1] += 0.01f;
                _spline1->Update();
            }
            else
            {
                _spline2->Vertex(_current)[1] += 0.02f;
                _spline2->Update();
            }
        }
        else if (event.key.code == System::Key::Down)
        {
            if (_spline1->Enabled())
            {
                _spline1->Vertex(_current)[1] -= 0.01f;
                _spline1->Update();
            }
            else
            {
                _spline2->Vertex(_current)[1] -= 0.02f;
                _spline2->Update();
            }
        }
        else if (event.key.code == System::Key::Right)
        {
            if (_spline1->Enabled())
            {
                _spline1->Vertex(_current)[0] += 0.1f;
                _spline1->Update();
            }
            else
            {
                _spline2->Vertex(_current)[0] += 0.1f;
                _spline2->Update();
            }
        }
        else if (event.key.code == System::Key::Left)
        {
            if (_spline1->Enabled())
            {
                _spline1->Vertex(_current)[0] -= 0.1f;
                _spline1->Update();
            }
            else
            {
                _spline2->Vertex(_current)[0] -= 0.1f;
                _spline2->Update();
            }
        }
        else if (event.key.code == System::Key::Space)
        {
            if (_spline1->Enabled())
                _current = (_current + 1) % _spline1->NbPoint();
            else
                _current = (_current + 1) % _spline2->NbPoint();
        }
    }
}

void GameEngine::CreateSplines1()
{
    Array<Vector2f> controlPoints(5);
    controlPoints[0].Init(0, 0);
    controlPoints[1].Init(1.f, 0.15f);
    controlPoints[2].Init(2.f, 0.14f);
    controlPoints[3].Init(3.f, 0.15f);
    controlPoints[4].Init(4.f, 0.f);

    Array<int> weight(5);
    weight[0] = 1;
    weight[1] = 10;
    weight[2] = 10;
    weight[3] = 10;
    weight[4] = 1;

    Array<int>  knots(9);
    knots[0] = 0;
    knots[1] = 0;
    knots[2] = 0;
    knots[3] = 0;
    knots[4] = 2;
    knots[5] = 4;
    knots[6] = 4;
    knots[7] = 4;
    knots[8] = 4;

    // matrice de transformation de la courbe, pour l'affichage
    TMatrix     m;
    m.Scale(170, 3000, 0);
    m.AddTranslation(10, 10, 0);

    _spline1 = new bSplines(controlPoints, weight, m);
    _scene2d->AddChild(_spline1);
}

void GameEngine::CreateSplines2()
{
    Array<Vector2f> controlPoints(6);
    controlPoints[0].Init(1, 0);
    controlPoints[1].Init(0.87f, 0.5f);
    controlPoints[2].Init(0.71f, 0.71f);
    controlPoints[3].Init(0.5f, 0.87f);
    controlPoints[4].Init(0.f, 0.90f);
    controlPoints[5].Init(-1, 0);

    Array<int> weight(6);
    weight[0] = 1;
    weight[1] = 5;
    weight[2] = 1;
    weight[3] = 1;
    weight[4] = 5;
    weight[5] = 1;

    Array<float>  knots(10);
    knots[0] = 0;
    knots[1] = 0;
    knots[2] = 0;
    knots[3] = 0;
    knots[4] = 0.5;
    knots[5] = 0.5;
    knots[6] = 1;
    knots[7] = 1;
    knots[8] = 1;
    knots[9] = 1;

    // matrice de transformation de la courbe, pour l'affichage
    TMatrix     m;
    m.Scale(170, 300, 0);
    m.AddTranslation(200, 10, 0);

    _spline2 = new bSplines(controlPoints, weight, m);
    _spline2->Enable(false);
    _scene2d->AddChild(_spline2);
}
