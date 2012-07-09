

#ifndef DEMOVIEWER_GAMEENGINE_H_
#define DEMOVIEWER_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include <Nc/GUI/GUI.h>
#include "MainMenu.h"
#include "DemoFactory.h"

namespace DemoViewer
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
        public:
            NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, DemoViewer::GameEngine);

        public:
            enum IndexCmd
            {
                StartDemo = 0,
                DemoSelected
            };

        public:
            GameEngine(Nc::Graphic::IWindow *window, Nc::Engine::Manager *manager);
            virtual ~GameEngine();

            virtual void    LoadContent();
            virtual void    ReleaseContent();

            void            StartDemoCmd(Nc::Engine::IEvent *e);
            void            DemoSelectedCmd(Nc::Engine::IEvent *e);

        protected:
            virtual void    Update(float runningTime);
            virtual void    ManageWindowEvent(Nc::System::Event &event);
            virtual void    KeyboardEvent(Nc::System::Event &event);
            virtual void    MouseButtonEvent(Nc::System::Event &event);
            virtual void    MouseMotionEvent(Nc::System::Event &event);

            Nc::GUI::SceneGraph		*_scene;
            Nc::GUI::Console		*_console;
            MainMenu                *_menu;
            DemoFactory             *_demoFactory;
            std::string             _currentDemo;
    };
}

#endif
