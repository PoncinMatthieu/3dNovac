

#ifndef LIGHTDEMO_GAMEENGINE_H_
#define LIGHTDEMO_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/GUI/GUI.h>

namespace LightDemo
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
        public:
            NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, LightDemo::GameEngine);

        public:
            GameEngine(Nc::Engine::Manager *manager);
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

            Nc::Graphic::SceneGraph			        *_scene;
            Nc::GUI::SceneGraph			            *_sceneGUI;
            Nc::Graphic::StandardCamera3d           *_camera;
            Nc::Graphic::SceneNodeFormatManager     _sceneNodeFormatManager;

            Nc::Graphic::DefaultLightingMaterial    *_lightingMaterial;
            Nc::Graphic::Light                      *_light;
    };
}

#endif
