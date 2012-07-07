
#ifndef BSPLINESDEMO_GAMEENGINE_H_
#define BSPLINESDEMO_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Core/Engine/MainEngine.h>
#include <Nc/GUI/SceneGraph.h>

#include "bSplines.h"

namespace bSplinesDemo
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
     public:
        NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, bSplinesDemo::GameEngine);

     public:
      GameEngine(Nc::Graphic::Window *window, Nc::Engine::Manager *manager);
      virtual ~GameEngine();

     private:
      virtual void LoadContent();
      virtual void ReleaseContent();

      virtual void Update(float)			{}

      // manage window inputs
      virtual void	ManageWindowEvent(Nc::System::Event &event);
      virtual void	KeyboardEvent(Nc::System::Event &event);
      virtual void	MouseButtonEvent(Nc::System::Event &)	{}
      virtual void	MouseMotionEvent(Nc::System::Event &)	{}

      // creation des 2 splines avec des valeurs differentes
      void CreateSplines1();
      void CreateSplines2();

     private:
        Nc::GUI::SceneGraph     *_scene2d;	///< our GUI scene 2d, displayed by the graphic engine

        bSplines			    *_spline1;
        bSplines			    *_spline2;
        unsigned int			_current;
    };
}

#endif
