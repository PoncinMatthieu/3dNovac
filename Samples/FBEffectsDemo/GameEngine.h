
#ifndef MAIN_ENGINE_H_
#define MAIN_ENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/GUI/GUI.h>
#include "FrameBufferEffect.h"

class GameEngine : public Nc::Contrib::GameEngine
{
 public:
  GameEngine(Nc::Engine::Manager *manager, Nc::Graphic::Engine *graphic);
  ~GameEngine();

  /** To create the window (called by the graphic engine) */
  void CreateWindow(Nc::Graphic::Window *win);

 private:
  virtual void LoadContent();
  virtual void ReleaseContent();

  virtual void Update(float runningTime);

  // manage window inputs
  virtual void	ManageWindowEvent(Nc::System::Event &event);
  virtual void	KeyboardEvent(Nc::System::Event &event);
  virtual void	MouseButtonEvent(Nc::System::Event &event);
  virtual void	MouseMotionEvent(Nc::System::Event &event);

 private:
  Nc::Graphic::SceneGraph		*_scene3d;	///< our scene 3d, displayed by the graphic engine
  Nc::GUI::SceneGraph			*_sceneGUI;
  Nc::Graphic::StandardCamera3d		*_camera;	///< our camera used by our scene. We use it to resize and update it with the window inputs

  Nc::Graphic::SceneNodeFormatManager	_sceneNodeFormatManager;

  Nc::Graphic::DefaultLightingMaterial	*_lightingMaterial;
  Nc::Graphic::Light			*_light;

  Nc::Graphic::FrameBufferEffect	*_fbEffect;
};

#endif
