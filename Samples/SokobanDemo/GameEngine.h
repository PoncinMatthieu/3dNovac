
#ifndef SOKOBANDEMO_GAMEENGINE_H_
#define SOKOBANDEMO_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include "Map.h"

namespace SokobanDemo
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
        public:
            NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, SokobanDemo::GameEngine);

     public:
      GameEngine(Nc::Graphic::Window *window, Nc::Graphic::SceneNodeFormatManager *sceneNodeFormatManager, Nc::Engine::Manager *manager);
      ~GameEngine();

     private:
      virtual void LoadContent();
      virtual void ReleaseContent();

      virtual void Update(float runningTime);

      // manage window inputs
      virtual void	ManageWindowEvent(Nc::System::Event &event);
      virtual void	KeyboardEvent(Nc::System::Event &event);
      virtual void	MouseButtonEvent(Nc::System::Event &event);
      virtual void	MouseMotionEvent(Nc::System::Event &event);

      void          MovePlayer(Nc::System::Event &event);

     private:
      Nc::Graphic::SceneGraph		        *_scene3d;	///< our scene 3d, displayed by the graphic engine
      Nc::Graphic::StandardCamera3d		    *_camera;	///< our camera used by our scene. We use it to resize and update it with the window inputs

      Nc::Graphic::SceneNodeFormatManager	*_sceneNodeFormatManager;

      Map					                *_map;
      Nc::Graphic::DefaultLightingMaterial	*_lightingMaterial;
      Nc::Graphic::Light			        *_light;
    };
}

#endif
