

#ifndef POPULOUS_GAME_ENGINE
#define POPULOUS_GAME_ENGINE

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/Graphics/SpacePartitioning/Octree.h>
#include <Nc/GUI/GUI.h>

namespace FrustumDemo
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
     public:
      GameEngine(Nc::Graphic::Engine *graphic, Nc::Engine::Manager *manager);
      virtual ~GameEngine();

      void            CreateWindow(Nc::Graphic::Window *win);
      virtual void    LoadContent();
      virtual void    ReleaseContent();

     protected:
      virtual void    Update(float runningTime);
      virtual void    ManageWindowEvent(Nc::System::Event &event);
      virtual void    KeyboardEvent(Nc::System::Event &event);
      virtual void    MouseButtonEvent(Nc::System::Event &event);
      virtual void    MouseMotionEvent(Nc::System::Event &event);

      void Generate(Nc::Graphic::Entity *node, Nc::Graphic::Object *obj);

      Nc::Graphic::SceneGraph		*_scene3d;
      Nc::GUI::SceneGraph			*_sceneGUI;
      Nc::Graphic::StandardCamera3d		*_camera;
      Nc::Graphic::StandardCamera3d		*_camera2;
      Nc::Graphic::StandardCamera3d		*_camera3;
      Nc::Graphic::SceneNodeFormatManager	_sceneNodeFormatManager;
      Nc::Graphic::Entity			*_entity;
    };
}

#endif
