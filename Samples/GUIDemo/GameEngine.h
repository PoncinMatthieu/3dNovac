

#ifndef GUIDEMO_GAMEENGINE_H_
#define GUIDEMO_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/GUI/GUI.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include "MainMenu.h"

namespace GUIDemo
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
        public:
            NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, GUIDemo::GameEngine);

        public:
            enum IndexCmd
            {
                ModeSinglePlayer,
                ModeMultiplayer
            };

        public:
            GameEngine(Nc::Graphic::IWindow *window, Nc::Engine::Manager *manager);
            virtual ~GameEngine();

            virtual void    LoadContent();
            virtual void    ReleaseContent();

        protected:
            virtual void    Update(float runningTime);
            virtual void    ManageWindowEvent(Nc::System::Event &event);
            virtual void    KeyboardEvent(Nc::System::Event &event);
            virtual void    MouseButtonEvent(Nc::System::Event &event);
            virtual void    MouseMotionEvent(Nc::System::Event &event);

            Nc::GUI::SceneGraph		*_sceneGUI;
            Nc::GUI::Console		*_console;
            MainMenu                *_menu;
    };
}

#endif
