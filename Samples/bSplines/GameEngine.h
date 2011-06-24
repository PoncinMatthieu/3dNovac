
#ifndef BSPLINE_GAME_ENGINE
#define BSPLINE_GAME_ENGINE

#include <Nc/Graphics/Engine.h>
#include <Nc/Core/Engine/MainEngine.h>
#include <Nc/GUI/SceneGraph.h>

#include "bSplines.h"

class GameEngine : public Nc::Engine::MainEngine
{
 public:
  GameEngine(Nc::Engine::Manager *manager, Nc::Graphic::Engine *graphic);
  virtual ~GameEngine();

  /** To create the window (called by the graphic engine) */
  void CreateWindow(Nc::Graphic::Window *win);

 private:
  virtual void LoadContent();

  virtual void Update(float)			{}

  // manage the OpenGL context
  virtual void CreateContext()			{_context = _graphic->CreateSharedContext();}
  virtual void ActiveContext()			{_context->Active();}
  virtual void DisableContext()			{_context->Disable();}

  // manage window inputs
  virtual void	ManageWindowEvent(Nc::System::Event &event);
  virtual void	KeyboardEvent(Nc::System::Event &event);
  virtual void	MouseButtonEvent(Nc::System::Event &)	{}
  virtual void	MouseMotionEvent(Nc::System::Event &)	{}

  // creation des 2 splines avec des valeurs differentes
  void CreateSplines1();
  void CreateSplines2();

 private:
  Nc::Graphic::Engine		*_graphic;	///< the graphic engine
  Nc::Graphic::GLContext	*_context;	///< the OpenGL context, needed to use the graphic engine

  Nc::GUI::SceneGraph		*_scene2d;	///< our GUI scene 2d, displayed by the graphic engine

  bSplines			*_spline1;
  bSplines			*_spline2;
  unsigned int			_current;
};

#endif
